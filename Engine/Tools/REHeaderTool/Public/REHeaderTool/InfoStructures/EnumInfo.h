/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <vector>
#include <cassert>

#include <clang-c/Index.h>

#include "REHeaderTool/InfoStructures/TypeInfo.h"
#include "REHeaderTool/InfoStructures/EntityInfo.h"
#include "REHeaderTool/InfoStructures/EnumValueInfo.h"

namespace REHeaderTool
{
	class EnumInfo : public EntityInfo
	{
		public:
			static constexpr EEntityType	nestedEntityTypes = EEntityType::EnumValue;

			/** Detailed information on the enum. */
			TypeInfo						type;

			/** Detailed information on the underlying enum type. */
			TypeInfo						underlyingType;

			/** List of all values contained in the enum. */
			std::vector<EnumValueInfo>		enumValues;

			EnumInfo()										= default;
			EnumInfo(CXCursor const&			cursor,
					 std::vector<Property>&&	properties)	noexcept;

			/**
			*	@brief Call a visitor function on an enum and each nested entity of the provided type(s).
			* 
			*	@param entityMask	All types of entities the visitor function should be called on.
			*	@param visitor		Function to call on entities.
			*/
			template <typename Functor, typename = std::enable_if_t<std::is_invocable_v<Functor, EntityInfo const&>>>
			void	foreachEntityOfType(EEntityType entityMask, Functor visitor)	const	noexcept;

			/**
			*	@brief Refresh the outerEntity field of all nested entities. Internal use only.
			*/
			void	refreshOuterEntity()													noexcept;
	};

	#include "REHeaderTool/InfoStructures/EnumInfo.inl"
}