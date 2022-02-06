#include "REHeaderTool/InfoStructures/EnumInfo.h"

using namespace REHeaderTool;

EnumInfo::EnumInfo(CXCursor const& cursor, std::vector<Property>&& properties) noexcept:
	EntityInfo(cursor, std::forward<std::vector<Property>>(properties), EEntityType::Enum),
	type{clang_getCursorType(cursor)},
	underlyingType{clang_getEnumDeclIntegerType(cursor)}
{
}

void EnumInfo::refreshOuterEntity() noexcept
{
	for (EnumValueInfo& enumValue : enumValues)
	{
		enumValue.outerEntity = this;
	}
}