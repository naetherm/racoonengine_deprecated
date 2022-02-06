#include "REHeaderTool/Parsing/VariableParser.h"

#include <assert.h>

#include "REHeaderTool/Parsing/PropertyParser.h"
#include "REHeaderTool/Misc/Helpers.h"
#include "REHeaderTool/Misc/DisableWarningMacros.h"

using namespace REHeaderTool;

CXChildVisitResult VariableParser::parse(CXCursor const& variableCursor, ParsingContext const& parentContext, VariableParsingResult& out_result) noexcept
{
	//Make sure the cursor is compatible for the variable parser
	assert(variableCursor.kind == CXCursorKind::CXCursor_VarDecl);

	//Init context
	ParsingContext& context = pushContext(variableCursor, parentContext, out_result);

	if (!clang_visitChildren(variableCursor, &VariableParser::parseNestedEntity, this) && context.shouldCheckProperties)
	{
		//If we reach this point, the cursor had no child (no annotation)
		//Check if the parent has the shouldParseAllNested flag set
		if (shouldParseCurrentEntity())
		{
			getParsingResult()->parsedVariable.emplace(variableCursor, std::vector<Property>());
		}
	}

	popContext();

	DISABLE_WARNING_PUSH
	DISABLE_WARNING_UNSCOPED_ENUM

	return (parentContext.parsingSettings->shouldAbortParsingOnFirstError && !out_result.errors.empty()) ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;

	DISABLE_WARNING_POP
}

CXChildVisitResult VariableParser::parseNestedEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	VariableParser*	parser	= reinterpret_cast<VariableParser*>(clientData);
	ParsingContext&	context = parser->getContext();

	if (context.shouldCheckProperties)
	{
		if (cursor.kind == CXCursorKind::CXCursor_DLLImport ||
			cursor.kind == CXCursorKind::CXCursor_DLLExport)
		{
			return CXChildVisitResult::CXChildVisit_Continue;
		}

		context.shouldCheckProperties = false;

		if (parser->shouldParseCurrentEntity() && cursor.kind != CXCursorKind::CXCursor_AnnotateAttr)
		{
			//Make it valid right away so init the result
			parser->getParsingResult()->parsedVariable.emplace(context.rootCursor, std::vector<Property>());
		}
		else
		{
			//Set parsed variable in result if it is valid
			return parser->setParsedEntity(cursor);
		}
	}

	return CXChildVisitResult::CXChildVisit_Continue;
}

CXChildVisitResult VariableParser::setParsedEntity(CXCursor const& annotationCursor) noexcept
{
	VariableParsingResult*	result	= getParsingResult();
	ParsingContext&			context	= getContext();

	if (opt::optional<std::vector<Property>> properties = getProperties(annotationCursor))
	{
		//Set the parsed entity in the result & initialize its information from the method cursor
		result->parsedVariable.emplace(context.rootCursor, std::move(*properties));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else if (!context.propertyParser->getParsingErrorDescription().empty())
	{
		//Fatal parsing error occured
		result->errors.emplace_back(ParsingError(context.propertyParser->getParsingErrorDescription(), clang_getCursorLocation(context.rootCursor)));
	}

	return CXChildVisitResult::CXChildVisit_Break;
}

opt::optional<std::vector<Property>> VariableParser::getProperties(CXCursor const& cursor) noexcept
{
	ParsingContext& context	= getContext();

	context.propertyParser->clean();

	return (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr) ?
		context.propertyParser->getVariableProperties(Helpers::getString(clang_getCursorSpelling(cursor))) :
		opt::nullopt;
}

bool VariableParser::shouldParseCurrentEntity() noexcept
{
	return getContext().parsingSettings->shouldParseAllVariables || EntityParser::shouldParseCurrentEntity();
}

ParsingContext& VariableParser::pushContext(CXCursor const& methodCursor, ParsingContext const&	parentContext, VariableParsingResult& out_result) noexcept
{
	ParsingContext newContext;

	newContext.parentContext			= &parentContext;
	newContext.rootCursor				= methodCursor;
	newContext.shouldCheckProperties	= true;
	newContext.propertyParser			= parentContext.propertyParser;
	newContext.parsingSettings			= parentContext.parsingSettings;
	newContext.parsingResult			= &out_result;

	contextsStack.push(std::move(newContext));

	return contextsStack.top();
}