/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <vector>
#include <memory>
#include <cassert>

#include <clang-c/Index.h>

#include "REHeaderTool/InfoStructures/EntityInfo.h"
#include "REHeaderTool/InfoStructures/EnumInfo.h"
#include "REHeaderTool/InfoStructures/FieldInfo.h"
#include "REHeaderTool/InfoStructures/MethodInfo.h"
#include "REHeaderTool/InfoStructures/NestedEnumInfo.h"
#include "REHeaderTool/Misc/EAccessSpecifier.h"

namespace REHeaderTool
{
	//Forward declaration
	class NestedStructClassInfo;

	/**
	*	This class defines common data / behaviors shared by class and structs.
	*/
	class StructClassInfo : public EntityInfo
	{
		public:
			struct ParentInfo
			{
				/** Inheritance access of this parent. */
				EAccessSpecifier	inheritanceAccess;

				/** Type of the parent. */
				TypeInfo			type;

				ParentInfo(EAccessSpecifier access, TypeInfo&& parentType) noexcept;
			};

			static constexpr EEntityType						nestedEntityTypes = EEntityType::Class | EEntityType::Struct | EEntityType::Enum | EEntityType::Method | EEntityType::Field;

			struct ClassQualifiers
			{
				/** Is the class final qualified of not. */
				bool isFinal		: 1;
			}													qualifiers;

			/** Whether this StructClassInfo represents a forward declaration or not. */
			bool												isForwardDeclaration;

			/** More detailed information on this class. */
			TypeInfo											type;

			/** List of all parent classes of this class. */
			std::vector<ParentInfo>								parents;

			/** List of all nested classes contained in this class. */
			std::vector<std::shared_ptr<NestedStructClassInfo>>	nestedClasses;

			/** List of all nested structs contained in this class. */
			std::vector<std::shared_ptr<NestedStructClassInfo>>	nestedStructs;

			/** List of all nested enums contained in this class. */
			std::vector<NestedEnumInfo>							nestedEnums;

			/** List of all fields contained in this class. */
			std::vector<FieldInfo>								fields;

			/** List of all methods contained in this class. */
			std::vector<MethodInfo>								methods;

			StructClassInfo()												noexcept;
			StructClassInfo(CXCursor const&			cursor,
							std::vector<Property>&&	properties,
							bool					isForwardDeclaration)	noexcept;

			/**
			*	@brief Get the kind of a struct class info from a clang cursor.
			* 
			*	@param cursor The target cursor.
			* 
			*	@return The kind of the target cursor.
			*/
			static CXCursorKind	getCursorKind(CXCursor cursor)	noexcept;

			/**
			*	@brief Call a visitor function on a struct/class and each nested entity of the provided type(s).
			* 
			*	@param entityMask	All types of entities the visitor function should be called on.
			*	@param visitor		Function to call on entities.
			*/
			template <typename Functor, typename = std::enable_if_t<std::is_invocable_v<Functor, EntityInfo const&>>>
			void		foreachEntityOfType(EEntityType entityMask, Functor visitor)	const	noexcept;

			/**
			*	@brief Return true if this entity is a class, false if it is a struct.
			* 
			*	@return true if this entity is a class, false if it is a struct.
			*/
			inline bool	isClass()														const	noexcept;

			/**
			*	@brief Refresh the outerEntity field of all nested entities. Internal use only.
			*/
			void		refreshOuterEntity()													noexcept;
	};

	#include "REHeaderTool/InfoStructures/StructClassInfo.inl"
}