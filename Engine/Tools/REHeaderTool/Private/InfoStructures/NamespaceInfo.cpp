#include "REHeaderTool/InfoStructures/NamespaceInfo.h"

using namespace REHeaderTool;

NamespaceInfo::NamespaceInfo(CXCursor const& cursor, std::vector<Property>&& properties) noexcept:
	EntityInfo(cursor, std::forward<std::vector<Property>>(properties), EEntityType::Namespace)
{
}

void NamespaceInfo::refreshOuterEntity() noexcept
{
	for (NamespaceInfo& namespaceInfo : namespaces)
	{
		namespaceInfo.refreshOuterEntity();
		namespaceInfo.outerEntity = this;
	}

	for (StructClassInfo& structInfo : structs)
	{
		structInfo.refreshOuterEntity();
		structInfo.outerEntity = this;
	}

	for (StructClassInfo& classInfo : classes)
	{
		classInfo.refreshOuterEntity();
		classInfo.outerEntity = this;
	}

	for (EnumInfo& enumInfo : enums)
	{
		enumInfo.refreshOuterEntity();
		enumInfo.outerEntity = this;
	}

	for (FunctionInfo& functionInfo : functions)
	{
		functionInfo.outerEntity = this;
	}

	for (VariableInfo& variableInfo : variables)
	{
		variableInfo.outerEntity = this;
	}
}