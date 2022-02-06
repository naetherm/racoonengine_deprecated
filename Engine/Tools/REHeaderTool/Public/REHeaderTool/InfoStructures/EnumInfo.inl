/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

template <typename Functor, typename>
void EnumInfo::foreachEntityOfType(EEntityType entityMask, Functor visitor) const noexcept
{
	assert(entityType == EEntityType::Enum);

	//Call visitor on this enum if mask matches
	if (entityMask && entityType)
	{
		visitor(*this);
	}

	//Propagate call on nested entities
	if (entityMask && EEntityType::EnumValue)
	{
		for (EnumValueInfo const& enumValue : enumValues)
		{
			visitor(enumValue);
		}
	}
}