#include "REHeaderTool/Parsing/MethodParser.h"

#include <assert.h>

#include "REHeaderTool/Parsing/ParsingSettings.h"
#include "REHeaderTool/Parsing/PropertyParser.h"
#include "REHeaderTool/Misc/Helpers.h"
#include "REHeaderTool/Misc/DisableWarningMacros.h"

using namespace REHeaderTool;

CXChildVisitResult MethodParser::parse(CXCursor const& methodCursor, ParsingContext const& parentContext, MethodParsingResult& out_result) noexcept
{
	//Make sure the cursor is compatible for the method parser
	assert(methodCursor.kind == CXCursorKind::CXCursor_CXXMethod);	// /!\ might have to add CXCursor_FunctionDecl and CXCursor_FunctionTemplate

	//Init context
	ParsingContext& context = pushContext(methodCursor, parentContext, out_result);

	if (!clang_visitChildren(methodCursor, &MethodParser::parseNestedEntity, this) && context.shouldCheckProperties)
	{
		//If we reach this point, the cursor had no child (no annotation)
		//Check if the parent has the shouldParseAllNested flag set
		if (shouldParseCurrentEntity())
		{
			getParsingResult()->parsedMethod.emplace(methodCursor, std::vector<Property>());
		}
	}

	popContext();

	DISABLE_WARNING_PUSH
	DISABLE_WARNING_UNSCOPED_ENUM

	return (parentContext.parsingSettings->shouldAbortParsingOnFirstError && !out_result.errors.empty()) ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;

	DISABLE_WARNING_POP
}

CXChildVisitResult MethodParser::parseNestedEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	MethodParser*	parser	= reinterpret_cast<MethodParser*>(clientData);
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
			parser->getParsingResult()->parsedMethod.emplace(context.rootCursor, std::vector<Property>());
		}
		else
		{
			//Set parsed method in result if it is valid
			return parser->setParsedEntity(cursor);
		}
	}

	switch (clang_getCursorKind(cursor))
	{
		case CXCursorKind::CXCursor_CXXFinalAttr:
			if (parser->getParsingResult()->parsedMethod.has_value())
			{
				parser->getParsingResult()->parsedMethod->isFinal = true;
			}
			break;

		case CXCursorKind::CXCursor_CXXOverrideAttr:
			if (parser->getParsingResult()->parsedMethod.has_value())
			{
				parser->getParsingResult()->parsedMethod->isOverride = true;
			}
			break;

		case CXCursorKind::CXCursor_ParmDecl:
			if (parser->getParsingResult()->parsedMethod.has_value())
			{
				parser->getParsingResult()->parsedMethod->parameters.emplace_back(FunctionParamInfo{TypeInfo(clang_getCursorType(cursor)), Helpers::getString(clang_getCursorDisplayName(cursor))});
			}
			break;

		default:
			//std::cout << "Unknown method sub cursor: " << Helpers::getString(clang_getCursorKindSpelling(clang_getCursorKind(cursor))) << std::endl;
			break;
	}

	return CXChildVisitResult::CXChildVisit_Recurse;
}

CXChildVisitResult MethodParser::setParsedEntity(CXCursor const& annotationCursor) noexcept
{
	MethodParsingResult*	result	= getParsingResult();
	ParsingContext&			context	= getContext();

	if (opt::optional<std::vector<Property>> properties = getProperties(annotationCursor))
	{
		//Set the parsed entity in the result & initialize its information from the method cursor
		result->parsedMethod.emplace(context.rootCursor, std::move(*properties));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else if (!context.propertyParser->getParsingErrorDescription().empty())
	{
		//Fatal parsing error occured
		result->errors.emplace_back(ParsingError(context.propertyParser->getParsingErrorDescription(), clang_getCursorLocation(context.rootCursor)));
	}

	return CXChildVisitResult::CXChildVisit_Break;
}

opt::optional<std::vector<Property>> MethodParser::getProperties(CXCursor const& cursor) noexcept
{
	ParsingContext& context	= getContext();

	context.propertyParser->clean();

	return (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr) ?
				context.propertyParser->getMethodProperties(Helpers::getString(clang_getCursorSpelling(cursor))) :
				opt::nullopt;
}

ParsingContext& MethodParser::pushContext(CXCursor const& methodCursor, ParsingContext const&	parentContext, MethodParsingResult& out_result) noexcept
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