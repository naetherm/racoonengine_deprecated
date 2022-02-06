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

#include "REHeaderTool/InfoStructures/StructClassInfo.h"
#include "REHeaderTool/InfoStructures/EnumInfo.h"
#include "REHeaderTool/InfoStructures/FunctionInfo.h"
#include "REHeaderTool/InfoStructures/VariableInfo.h"

namespace REHeaderTool
{
	class NamespaceInfo : public EntityInfo
	{
		public:
			static constexpr EEntityType	nestedEntityTypes = EEntityType::Namespace | EEntityType::Variable | EEntityType::Function | StructClassInfo::nestedEntityTypes;

			/** Nested namespaces. */
			std::vector<NamespaceInfo>		namespaces;

			/** Nested structs. */
			std::vector<StructClassInfo>	structs;

			/** Nested classes. */
			std::vector<StructClassInfo>	classes;

			/** Nested enums. */
			std::vector<EnumInfo>			enums;

			/** Nested functions. */
			std::vector<FunctionInfo>		functions;

			/** Nested variables. */
			std::vector<VariableInfo>		variables;

			NamespaceInfo(CXCursor const&			cursor,
						  std::vector<Property>&&	properties)	noexcept;

			/**
			*	@brief Call a visitor function on a namespace and each nested entity of the provided type(s).
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

	#include "REHeaderTool/InfoStructures/NamespaceInfo.inl"
}