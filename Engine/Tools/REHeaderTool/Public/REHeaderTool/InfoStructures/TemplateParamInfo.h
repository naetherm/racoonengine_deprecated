/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <memory>	//std::unique_ptr

#include <clang-c/Index.h>

#include "REHeaderTool/InfoStructures/ETemplateParameterKind.h"

namespace REHeaderTool
{
	//Forward declaration
	class TypeInfo;

	class TemplateParamInfo
	{
		private:
			/**
			*	@brief Convert a clang cursor type to the corresponding ETemplateParameterKind.
			* 
			*	@return The ETemplateParameterKind corresponding to the provided clang cursor.
			*/
			static ETemplateParameterKind getTemplateParamKind(CXCursorKind cursorKind) noexcept;

		public:
			/** Kind of this template parameter. */
			ETemplateParameterKind		kind;
			
			/** Type of this template parameter. */
			std::unique_ptr<TypeInfo>	type;

			/**
			*	Name of the template parameter. In most cases, it is the same as type->getName().
			*	It is different in the case of non-type template parameters only.
			*/
			std::string					name;

			TemplateParamInfo(CXCursor cursor)	noexcept;
	};
}