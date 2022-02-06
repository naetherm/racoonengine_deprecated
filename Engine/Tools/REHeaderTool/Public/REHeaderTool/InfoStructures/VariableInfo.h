/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <clang-c/Index.h>

#include "REHeaderTool/InfoStructures/EntityInfo.h"
#include "REHeaderTool/InfoStructures/TypeInfo.h"

namespace REHeaderTool
{
	class VariableInfo : public EntityInfo
	{
		protected:
			VariableInfo(CXCursor const&			cursor,
						 std::vector<Property>&&	properties,
						 EEntityType				entityType)		noexcept;

		public:
			/** Is this variable static or not. */
			bool isStatic : 1;

			/** Type of this variable. */
			TypeInfo			type;

			VariableInfo(CXCursor const&			cursor,
						 std::vector<Property>&&	properties)	noexcept;
	};
}