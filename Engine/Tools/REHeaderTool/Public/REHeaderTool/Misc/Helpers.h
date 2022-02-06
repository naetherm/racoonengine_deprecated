/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>

#include <clang-c/Index.h>
#include <REHeaderTool/InfoStructures/EEntityType.h>

namespace REHeaderTool
{
	class Helpers
	{
		public:
			Helpers()	= delete;
			~Helpers()	= delete;

    /**
    *	@brief Convert a kodgen::EEntityType to the corresponding integer value in REHeaderTool::Entity::EKind
    *
    *	@param entityType The enum value to convert.
    *
    *	@return The converted value.
    */
    constexpr static uint16_t convertToEntityKind(REHeaderTool::EEntityType entityType) noexcept;

			/**
			*	@brief Convert a clang string to a std::string.
			*	
			*	@param clangString The clang string.
			*	
			*	@return The converted string.
			*/
			static std::string			getString(CXString&& clangString)		noexcept;

			/**
			*	@brief Get debug info about the provided cursor as string.
			*	
			*	@param cursor The cursor to debug.
			*	
			*	@return A string containing debug info.
			*/
			static std::string			getDebugInfo(CXCursor const& cursor)	noexcept;

			/**
			*	@brief Convert a boolean to a string value.
			*	
			*	@param value The boolean to convert.
			*	
			*	@return "true" if the boolean is true, else "false".
			*/
			static inline std::string	toString(bool value)					noexcept;
	};

}


#include "REHeaderTool/Misc/Helpers.inl"