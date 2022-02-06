/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

template <typename Functor, typename>
void NamespaceInfo::foreachEntityOfType(EEntityType entityMask, Functor visitor) const noexcept
{
	assert(entityType == EEntityType::Namespace);

	//Call visitor on this namespace if mask matches
	if (entityMask && entityType)
	{
		visitor(*this);
	}

	//Propagate call on nested entities
	if (entityMask && NamespaceInfo::nestedEntityTypes)	//EEntityType::Namespace is already included in NamespaceInfo::nestedEntityTypes
	{
		for (NamespaceInfo const& namespace_ : namespaces)
		{
			namespace_.foreachEntityOfType(entityMask, visitor);
		}
	}

	if (entityMask && StructClassInfo::nestedEntityTypes)	//EEntityType::Class and EEntityType::Struct are already included in StructClassInfo::nestedEntityTypes
	{
		for (StructClassInfo const& class_ : classes)
		{
			class_.foreachEntityOfType(entityMask, visitor);
		}

		for (StructClassInfo const& struct_ : structs)
		{
			struct_.foreachEntityOfType(entityMask, visitor);
		}
	}

	if ((entityMask && EEntityType::Enum) || (entityMask && EnumInfo::nestedEntityTypes))
	{
		for (EnumInfo const& enum_ : enums)
		{
			enum_.foreachEntityOfType(entityMask, visitor);
		}
	}

	if (entityMask && EEntityType::Function)
	{
		for (FunctionInfo const& function : functions)
		{
			visitor(function);
		}
	}

	if (entityMask && EEntityType::Variable)
	{
		for (VariableInfo const& variable : variables)
		{
			visitor(variable);
		}
	}
}