/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <vector>

#include <clang-c/Index.h>

#include "REHeaderTool/InfoStructures/EntityInfo.h"

namespace REHeaderTool
{
	class EnumValueInfo final : public EntityInfo
	{
		public:
			static constexpr EEntityType	nestedEntityTypes	= EEntityType::Undefined;

			/** Default integer value for this enum value. */
			int64_t							value				= 0;

			EnumValueInfo()										= default;
			EnumValueInfo(CXCursor const&			cursor,
						  std::vector<Property>&&	properties)	noexcept;
	};
}