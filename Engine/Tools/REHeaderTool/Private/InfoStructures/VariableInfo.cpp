#include "REHeaderTool/InfoStructures/VariableInfo.h"

#include <cassert>

using namespace REHeaderTool;

VariableInfo::VariableInfo(CXCursor const& cursor, std::vector<Property>&& properties, EEntityType entityType) noexcept:
	EntityInfo(cursor, std::forward<std::vector<Property>>(properties), entityType),
	isStatic{false},
	type(cursor)
{
}

VariableInfo::VariableInfo(CXCursor const& cursor, std::vector<Property>&& properties) noexcept:
	VariableInfo(cursor, std::forward<std::vector<Property>>(properties), EEntityType::Variable)
{
	assert(cursor.kind == CXCursorKind::CXCursor_VarDecl);

	isStatic = clang_getCursorLinkage(cursor) == CXLinkage_Internal;
}