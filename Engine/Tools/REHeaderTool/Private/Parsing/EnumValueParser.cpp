#include "REHeaderTool/Parsing/EnumValueParser.h"

#include "REHeaderTool/Parsing/ParsingSettings.h"
#include "REHeaderTool/Parsing/PropertyParser.h"
#include "REHeaderTool/Misc/DisableWarningMacros.h"
#include "REHeaderTool/Misc/Helpers.h"

using namespace REHeaderTool;

CXChildVisitResult EnumValueParser::parse(CXCursor const& enumValueCursor, ParsingContext const& parentContext, EnumValueParsingResult& out_result) noexcept
{
	//Make sure the cursor is compatible for the enum value parser
	assert(enumValueCursor.kind == CXCursorKind::CXCursor_EnumConstantDecl);

	//Init context
	ParsingContext& context = pushContext(enumValueCursor, parentContext, out_result);

	if (!clang_visitChildren(enumValueCursor, &EnumValueParser::parseNestedEntity, this) && context.shouldCheckProperties)
	{
		//If we reach this point, the cursor had no child (no annotation)
		//Check if the parent has the shouldParseAllNested flag set
		if (shouldParseCurrentEntity())
		{
			getParsingResult()->parsedEnumValue.emplace(enumValueCursor, std::vector<Property>());
		}
	}

	popContext();

	DISABLE_WARNING_PUSH
	DISABLE_WARNING_UNSCOPED_ENUM

	return (parentContext.parsingSettings->shouldAbortParsingOnFirstError && !out_result.errors.empty()) ? CXChildVisitResult::CXChildVisit_Break : CXChildVisitResult::CXChildVisit_Continue;

	DISABLE_WARNING_POP
}

CXChildVisitResult EnumValueParser::parseNestedEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	EnumValueParser*	parser	= reinterpret_cast<EnumValueParser*>(clientData);
	ParsingContext&		context = parser->getContext();

	if (context.shouldCheckProperties)
	{
		context.shouldCheckProperties = false;

		if (parser->shouldParseCurrentEntity() && cursor.kind != CXCursorKind::CXCursor_AnnotateAttr)
		{
			//Make it valid right away so init the result
			parser->getParsingResult()->parsedEnumValue.emplace(context.rootCursor, std::vector<Property>());
		}
		else
		{
			//Set parsed enum value in result if it is valid
			return parser->setParsedEntity(cursor);
		}
	}

	//There's nothing more to parse in enum values, break right away
	return CXChildVisitResult::CXChildVisit_Break;
}

opt::optional<std::vector<Property>> EnumValueParser::getProperties(CXCursor const& cursor) noexcept
{
	ParsingContext& context = getContext();

	context.propertyParser->clean();

	if (clang_getCursorKind(cursor) == CXCursorKind::CXCursor_AnnotateAttr)
	{
		return context.propertyParser->getEnumValueProperties(Helpers::getString(clang_getCursorSpelling(cursor)));
	}

	return opt::nullopt;
}

CXChildVisitResult EnumValueParser::setParsedEntity(CXCursor const& annotationCursor) noexcept
{
	ParsingContext& context = getContext();

	if (opt::optional<std::vector<Property>> properties = getProperties(annotationCursor))
	{
		//Set the parsing entity in the result and update the shouldParseAllNested flag in the context
		getParsingResult()->parsedEnumValue.emplace(context.rootCursor, std::move(*properties));

		return CXChildVisitResult::CXChildVisit_Recurse;
	}
	else
	{
		if (!context.propertyParser->getParsingErrorDescription().empty())
		{
			context.parsingResult->errors.emplace_back(ParsingError(context.propertyParser->getParsingErrorDescription(), clang_getCursorLocation(annotationCursor)));
		}

		return CXChildVisitResult::CXChildVisit_Break;
	}
}

void EnumValueParser::setProperties(CXCursor const& annotationCursor) noexcept
{
	ParsingContext& context = getContext();

	if (opt::optional<std::vector<Property>> properties = getProperties(annotationCursor))
	{
		getParsingResult()->parsedEnumValue->properties = std::move(*properties);
	}
	else if (!context.propertyParser->getParsingErrorDescription().empty())
	{
		//Fatal parsing error occured
		context.parsingResult->errors.emplace_back(context.propertyParser->getParsingErrorDescription(), clang_getCursorLocation(annotationCursor));
	}
}

bool EnumValueParser::shouldParseCurrentEntity() noexcept
{
	return getContext().parsingSettings->shouldParseAllEnumValues || EntityParser::shouldParseCurrentEntity();
}

ParsingContext& EnumValueParser::pushContext(CXCursor const& enumValueCursor, ParsingContext const& parentContext, EnumValueParsingResult& out_result) noexcept
{
	ParsingContext newContext;

	newContext.parentContext			= &parentContext;
	newContext.rootCursor				= enumValueCursor;
	newContext.shouldCheckProperties	= true;
	newContext.propertyParser			= parentContext.propertyParser;
	newContext.parsingSettings			= parentContext.parsingSettings;
	newContext.parsingResult			= &out_result;

	contextsStack.push(std::move(newContext));

	return contextsStack.top();
}