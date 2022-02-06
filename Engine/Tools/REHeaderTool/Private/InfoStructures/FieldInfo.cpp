#include "REHeaderTool/InfoStructures/FieldInfo.h"

#include <cassert>

using namespace REHeaderTool;

FieldInfo::FieldInfo(CXCursor const& cursor, std::vector<Property>&& properties) noexcept:
	VariableInfo(cursor, std::forward<std::vector<Property>>(properties), EEntityType::Field),
	isMutable{clang_CXXField_isMutable(cursor) != 0u},
	accessSpecifier{EAccessSpecifier::Invalid},
	memoryOffset{0}
{
	assert(cursor.kind == CXCursorKind::CXCursor_FieldDecl ||
		   cursor.kind == CXCursorKind::CXCursor_VarDecl);		//Static fields are var decl

	isStatic = cursor.kind == CXCursorKind::CXCursor_VarDecl;

	if (!isStatic)
	{
		memoryOffset = clang_Cursor_getOffsetOfField(cursor);

		assert(memoryOffset != CXTypeLayoutError::CXTypeLayoutError_Incomplete);
		assert(memoryOffset != CXTypeLayoutError::CXTypeLayoutError_InvalidFieldName);

		memoryOffset /= 8;	//From bits to bytes
	}
}