/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>

#include "REHeaderTool/InfoStructures/TypeInfo.h"

namespace REHeaderTool
{
	struct FunctionParamInfo
	{
		public:
			/** Parameter type */
			TypeInfo	type;

			/** Parameter name. Can be empty if no name. */
			std::string	name;
	};
}