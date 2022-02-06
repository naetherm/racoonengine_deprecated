/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <inttypes.h>
#include <type_traits>

#include "REHeaderTool/Misc/DisableWarningMacros.h"

DISABLE_WARNING_PUSH
DISABLE_WARNING_UNSCOPED_ENUM

namespace REHeaderTool
{
	enum ETypeDescriptor : uint16_t
	{
		/** Default uninitialized value for this enum */
		Undefined	= 0,

		/** Keywords */
		Const		= 1 << 0,	
		Restrict	= 1 << 1,
		Volatile	= 1 << 2,

		/** Ptr/Ref/Array */
		Ptr			= 1 << 3,
		LRef		= 1 << 4,
		RRef		= 1 << 5,
		CArray		= 1 << 6,
		Value		= 1 << 7	//Means non-ptr simple value, ex: int
	};

	inline ETypeDescriptor operator|(ETypeDescriptor e1, ETypeDescriptor e2) noexcept
	{
		using UnderlyingType = std::underlying_type_t<ETypeDescriptor>;

		return static_cast<ETypeDescriptor>(static_cast<UnderlyingType>(e1) | static_cast<UnderlyingType>(e2));
	}

	inline ETypeDescriptor operator&(ETypeDescriptor e1, ETypeDescriptor e2) noexcept
	{
		using UnderlyingType = std::underlying_type_t<ETypeDescriptor>;

		return static_cast<ETypeDescriptor>(static_cast<UnderlyingType>(e1) & static_cast<UnderlyingType>(e2));
	}

	struct TypePart
	{
		public:
			/** Padding to align memory. */
			uint16_t			padding;

			/** Descriptor of this part of the type. */
			ETypeDescriptor	descriptor;

			/** If descriptor is a CArray, additionalData is size. Might contain other information in other cases. */
			uint32_t			additionalData;
	};

	static_assert(sizeof(TypePart) == sizeof(uint64_t));
}

DISABLE_WARNING_POP