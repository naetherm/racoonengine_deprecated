/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <clang-c/Index.h>

#include "REHeaderTool/InfoStructures/StructClassInfo.h"
#include "REHeaderTool/Misc/EAccessSpecifier.h"

namespace REHeaderTool
{
	class NestedStructClassInfo : public StructClassInfo
	{
		public:
			static constexpr EEntityType	nestedEntityTypes = EEntityType::Class | EEntityType::Struct | EEntityType::Enum | EEntityType::Method | EEntityType::Field;

			/** Access specifier of this nested struct/class. */
			EAccessSpecifier				accessSpecifier;

			NestedStructClassInfo(StructClassInfo&& base,
								  EAccessSpecifier	accessSpecifier)	noexcept;
	};
}