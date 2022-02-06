/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include "REHeaderTool/Parsing/ParsingResults/ParsingResultBase.h"
#include "REHeaderTool/InfoStructures/MethodInfo.h"
#include "REHeaderTool/Misc/Optional.h"

namespace REHeaderTool
{
	struct MethodParsingResult : public ParsingResultBase
	{
		public:
			/** Information on the parsed method. */
			opt::optional<MethodInfo>	parsedMethod	= opt::nullopt;
	};
}