#include "REHeaderTool/InfoStructures/TemplateParamInfo.h"

#include <cassert>

#include "REHeaderTool/InfoStructures/TypeInfo.h"
#include "REHeaderTool/Misc/Helpers.h"

using namespace REHeaderTool;

TemplateParamInfo::TemplateParamInfo(CXCursor cursor) noexcept:
	kind{getTemplateParamKind(cursor.kind)},
	type{std::make_unique<TypeInfo>(cursor)},
	name(Helpers::getString(clang_getCursorDisplayName(cursor)))
{
}

ETemplateParameterKind TemplateParamInfo::getTemplateParamKind(CXCursorKind cursorKind) noexcept
{
	switch (cursorKind)
	{
		case CXCursorKind::CXCursor_TemplateTypeParameter:
			return ETemplateParameterKind::TypeTemplateParameter;

		case CXCursorKind::CXCursor_NonTypeTemplateParameter:
			return ETemplateParameterKind::NonTypeTemplateParameter;

		case CXCursorKind::CXCursor_TemplateTemplateParameter:
			return ETemplateParameterKind::TemplateTemplateParameter;

		default:
			return ETemplateParameterKind::Undefined;
	}
}