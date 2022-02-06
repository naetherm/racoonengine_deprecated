/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <clang-c/Index.h>

#include "REHeaderTool/InfoStructures/FunctionInfo.h"
#include "REHeaderTool/Misc/EAccessSpecifier.h"

namespace REHeaderTool
{
	class MethodInfo final : public FunctionInfo
	{
		public:
			static constexpr EEntityType	nestedEntityTypes = EEntityType::Undefined;

			/** Access of this method in its outer struct/class. */
			EAccessSpecifier				accessSpecifier;

			/** Is this method defaulted or not. */
			bool							isDefault		: 1;

			/** Is this method virtual or not. */
			bool							isVirtual		: 1;

			/** Is this method virtual pure or not. */
			bool							isPureVirtual	: 1;

			/** Is this method overriden or not. */
			bool							isOverride		: 1;

			/** Is this method final or not. */
			bool							isFinal			: 1;

			/** Is this method const or not. */
			bool							isConst			: 1;

			MethodInfo(CXCursor const&			cursor,
					   std::vector<Property>&&	properties)	noexcept;
	};
}