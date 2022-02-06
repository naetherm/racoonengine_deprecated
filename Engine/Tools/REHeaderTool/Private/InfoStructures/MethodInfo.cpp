#include "REHeaderTool/InfoStructures/MethodInfo.h"

#include <algorithm>
#include <cassert>

#include "REHeaderTool/Misc/Helpers.h"

using namespace REHeaderTool;

MethodInfo::MethodInfo(CXCursor const& cursor, std::vector<Property>&& properties) noexcept:
	FunctionInfo(cursor, std::forward<std::vector<Property>>(properties), EEntityType::Method),
	accessSpecifier{EAccessSpecifier::Invalid},
	isDefault{clang_CXXMethod_isDefaulted(cursor) != 0u},
	isVirtual{clang_CXXMethod_isVirtual(cursor) != 0u},
	isPureVirtual{clang_CXXMethod_isPureVirtual(cursor) != 0u},
	isOverride{false},
	isFinal{false},
	isConst{clang_CXXMethod_isConst(cursor) != 0u}
{
	assert(cursor.kind == CXCursorKind::CXCursor_CXXMethod);

	isStatic = clang_CXXMethod_isStatic(cursor) != 0u;
}