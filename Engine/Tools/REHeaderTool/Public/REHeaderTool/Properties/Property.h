/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <vector>

namespace REHeaderTool
{
	struct Property
	{
		/** Name of this property. */
		std::string					name;

		/** Collection of all arguments of this property. */
		std::vector<std::string>	arguments;
	};
}