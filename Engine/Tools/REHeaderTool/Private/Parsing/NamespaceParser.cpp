#include "REHeaderTool/Parsing/NamespaceParser.h"

#include <cassert>

#include "REHeaderTool/Parsing/ParsingSettings.h"
#include "REHeaderTool/Parsing/PropertyParser.h"
#include "REHeaderTool/Misc/Helpers.h"
#include "REHeaderTool/Misc/DisableWarningMacros.h"

using namespace REHeaderTool;

CXChildVisitResult NamespaceParser::parse(CXCursor const& namespaceCursor, ParsingContext const& parentContext, NamespaceParsingResult& out_result) noexcept
{
	//Make sure the cursor is compatible for the namespace parser
	assert(namespaceCursor.kind == CXCursorKind::CXCursor_Namespace);

	//Init context
	ParsingContext& context = pushContext(namespaceCursor, parentContext, out_result);

	if (!clang_visitChildren(namespaceCursor, &NamespaceParser::parseNestedEntity, this) && context.shouldCheckProperties)
	{
		//If we reach this point, the cursor had no child (no annotation)
		//Check if the parent has the shouldParseAllNested flag set
		if (shouldParseCurrentEntity())
		{
			getParsingResult()->parsedNamespace.emplace(namespaceCursor, std::vector<Property>());
		}
	}

	popContext();

	DISABLE_WARNING_PUSH
	DISABLE_WARNING_UNSCOPED_ENUM

	return (parentContext.parsingSettings->shouldAbortParsingOnFirstError && !out_result.errors.empty()) ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;

	DISABLE_WARNING_POP
}

CXChildVisitResult NamespaceParser::parseNestedEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	NamespaceParser*	parser	= reinterpret_cast<NamespaceParser*>(clientData);
	ParsingContext&		context = parser->getContext();

	if (context.shouldCheckProperties)
	{
		context.shouldCheckProperties = false;

		if (parser->shouldParseCurrentEntity() && cursor.kind != CXCursorKind::CXCursor_AnnotateAttr)
		{
			//Make it valid right away so init the result
			parser->getParsingResult()->parsedNamespace.emplace(context.rootCursor, std::vector<Property>());
		}
		else
		{
			return parser->setParsedEntity(cursor);
		}
	}

	CXChildVisitResult visitResult = CXChildVisitResult::CXChildVisit_Continue;

	switch (cursor.kind)
	{
		case CXCursorKind::CXCursor_Namespace:
			parser->addNamespaceResult(parser->parseNamespace(cursor, visitResult));
			break;

		case CXCursorKind::CXCursor_StructDecl:
			[[fallthrough]];
		case CXCursorKind::CXCursor_ClassDecl:
			parser->addClassResult(parser->parseClass(cursor, visitResult));
			break;

		case CXCursorKind::CXCursor_ClassTemplate:
			parser->addClassResult(parser->parseClass(cursor, visitResult));
			break;

		case CXCursorKind::CXCursor_EnumDecl:
			parser->addEnumResult(parser->parseEnum(cursor, visitResult));
			break;

		case CXCursorKind::CXCursor_VarDecl:
			parser->addVariableResult(parser->parseVariable(cursor, visitResult));
			break;

		case CXCursorKind::CXCursor_FunctionDecl:
			parser->addFunctionResult(parser->parseFunction(cursor, visitResult));
			break;

		default:
			break;
	}

	return visitResult;
}

NamespaceParsingResult NamespaceParser::parseNamespace(CXCursor const& namespaceCursor, CXChildVisitResult& out_visitResult) noexcept
{
	NamespaceParsingResult namespaceResult;
	
	out_visitResult	= NamespaceParser::parse(namespaceCursor, getContext(), namespaceResult);

	return namespaceResult;
}

FunctionParsingResult NamespaceParser::parseFunction(CXCursor const& functionCursor, CXChildVisitResult& out_visitResult) noexcept
{
	FunctionParsingResult functionResult;

	out_visitResult = _functionParser.parse(functionCursor, getContext(), functionResult);

	return functionResult;
}

VariableParsingResult NamespaceParser::parseVariable(CXCursor const& variableCursor, CXChildVisitResult& out_visitResult) noexcept
{
	VariableParsingResult variableResult;

	out_visitResult = _variableParser.parse(variableCursor, getContext(), variableResult);

	return variableResult;
}

bool NamespaceParser::shouldParseCurrentEntity() noexcept
{
	return getContext().parsingSettings->shouldParseAllNamespaces || EntityParser::shouldParseCurrentEntity();
}

ParsingContext& NamespaceParser::pushContext(CXCursor const& namespaceCursor, ParsingContext const& parentContext, NamespaceParsingResult& out_result) noexcept
{
	//Add a new context to the contexts stack
	ParsingContext newContext;

	newContext.parentContext			= &parentContext;
	newContext.rootCursor				= namespaceCursor;
	newContext.shouldCheckProperties	= true;
	newContext.propertyParser			= parentContext.propertyParser;
	newContext.parsingSettings			= parentContext.parsingSettings;
	newContext.structClassTree			= parentContext.structClassTree;
	newContext.parsingResult			= &out_result;

	contextsStack.push(std::move(newContext));

	return contextsStack.top();
}

CXChildVisitResult NamespaceParser::setParsedEntity(CXCursor const& annotationCursor) noexcept
{
	if (opt::optional<std::vector<Property>> properties = getProperties(annotationCursor))
	{
		ParsingContext& context = getContext();

		//Set the parsing entity in the result and update the shouldParseAllNested flag in the context
		updateShouldParseAllNested(getParsingResult()->parsedNamespace.emplace(context.rootCursor, std::move(*properties)));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (!getContext().propertyParser->getParsingErrorDescription().empty())
		{
			getContext().parsingResult->errors.emplace_back(ParsingError(getContext().propertyParser->getParsingErrorDescription(), clang_getCursorLocation(annotationCursor)));
		}

		return CXChildVisitResult::CXChildVisit_Break;
	}
}

opt::optional<std::vector<Property>> NamespaceParser::getProperties(CXCursor const& cursor) noexcept
{
	ParsingContext& context = getContext();

	context.propertyParser->clean();

	return (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr) ?
		context.propertyParser->getNamespaceProperties(Helpers::getString(clang_getCursorSpelling(cursor))) :
		opt::nullopt;
}

void NamespaceParser::addNamespaceResult(NamespaceParsingResult&& result) noexcept
{
	if (result.parsedNamespace.has_value() && getParsingResult()->parsedNamespace.has_value())
	{
		getParsingResult()->parsedNamespace->namespaces.emplace_back(std::move(result.parsedNamespace).value());
	}

	getParsingResult()->appendResultErrors(result);
}

void NamespaceParser::addClassResult(ClassParsingResult&& result) noexcept
{
	if (result.parsedClass.has_value() && getParsingResult()->parsedNamespace.has_value())
	{
		switch (result.parsedClass->entityType)
		{
			case EEntityType::Struct:
				getParsingResult()->parsedNamespace->structs.emplace_back(std::move(result.parsedClass).value());
				break;

			case EEntityType::Class:
				getParsingResult()->parsedNamespace->classes.emplace_back(std::move(result.parsedClass).value());
				break;

			default:
				assert(false);	//Should never reach this line
				break;
		}
	}

	getParsingResult()->appendResultErrors(result);
}

void NamespaceParser::addEnumResult(EnumParsingResult&& result) noexcept
{
	if (result.parsedEnum.has_value() && getParsingResult()->parsedNamespace.has_value())
	{
		getParsingResult()->parsedNamespace->enums.emplace_back(std::move(result.parsedEnum).value());
	}

	getParsingResult()->appendResultErrors(result);
}

void NamespaceParser::addFunctionResult(FunctionParsingResult&& result) noexcept
{
	if (result.parsedFunction.has_value() && getParsingResult()->parsedNamespace.has_value())
	{
		getParsingResult()->parsedNamespace->functions.emplace_back(std::move(result.parsedFunction).value());
	}

	getParsingResult()->appendResultErrors(result);
}

void NamespaceParser::addVariableResult(VariableParsingResult&& result) noexcept
{
	if (result.parsedVariable.has_value() && getParsingResult()->parsedNamespace.has_value())
	{
		getParsingResult()->parsedNamespace->variables.emplace_back(std::move(result.parsedVariable).value());
	}

	getParsingResult()->appendResultErrors(result);
}