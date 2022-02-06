/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <clang-c/Index.h>

#include "REHeaderTool/InfoStructures/VariableInfo.h"
#include "REHeaderTool/Misc/EAccessSpecifier.h"

namespace REHeaderTool
{
	class FieldInfo final : public VariableInfo
	{
		public:
			static constexpr EEntityType	nestedEntityTypes = EEntityType::Undefined;

			/** Is this field mutable qualified? */
			bool							isMutable : 1;

			/** Access of this field in its outer struct/class. */
			EAccessSpecifier				accessSpecifier;

			/** Memory offset in bytes. */
			int64_t							memoryOffset;

			FieldInfo(CXCursor const&			cursor,
					  std::vector<Property>&&	propertyGroup)	noexcept;
	};
}