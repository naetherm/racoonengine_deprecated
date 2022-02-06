#include "REHeaderTool/Parsing/EntityParser.h"

#include <algorithm>

#include "REHeaderTool/Properties/NativeProperties.h"

using namespace REHeaderTool;

void EntityParser::updateShouldParseAllNested(EntityInfo const& parsingEntity) noexcept
{
	getContext().shouldParseAllNested = std::find_if(parsingEntity.properties.cbegin(), parsingEntity.properties.cend(),
													 [](Property const& prop) { return prop.name == NativeProperties::parseAllNestedProperty; })
												!= parsingEntity.properties.cend();
}