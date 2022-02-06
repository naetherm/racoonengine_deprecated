/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

template <typename Functor, typename>
void StructClassInfo::foreachEntityOfType(EEntityType entityMask, Functor visitor) const noexcept
{
	assert(entityType == EEntityType::Class || entityType == EEntityType::Struct);

	//Call visitor on this struct/class if mask matches
	if (entityMask && entityType)
	{
		visitor(*this);
	}

	//Propagate call on nested entities
	if (entityMask && StructClassInfo::nestedEntityTypes)	//EEntityType::Class and EEntityType::Struct are already included in StructClassInfo::nestedEntityTypes
	{
		for (std::shared_ptr<NestedStructClassInfo> const& struct_ : nestedStructs)
		{
			//Need to use reinterpret_cast here because NestedStructClassInfo is only forward declared
			reinterpret_cast<StructClassInfo*>(struct_.get())->foreachEntityOfType(entityMask, visitor);
		}

		for (std::shared_ptr<NestedStructClassInfo> const& class_ : nestedClasses)	
		{
			//Need to use reinterpret_cast here because NestedStructClassInfo is only forward declared
			reinterpret_cast<StructClassInfo*>(class_.get())->foreachEntityOfType(entityMask, visitor);
		}
	}

	if ((entityMask && EEntityType::Enum) || (entityMask && EnumInfo::nestedEntityTypes))
	{
		for (EnumInfo const& enum_ : nestedEnums)
		{
			enum_.foreachEntityOfType(entityMask, visitor);
		}
	}

	if (entityMask && EEntityType::Field)
	{
		for (MethodInfo const& method : methods)
		{
			visitor(method);
		}
	}

	if (entityMask && EEntityType::Method)
	{
		for (MethodInfo const& method : methods)
		{
			visitor(method);
		}
	}
}

inline bool StructClassInfo::isClass() const noexcept
{
	return entityType == EEntityType::Class;
}