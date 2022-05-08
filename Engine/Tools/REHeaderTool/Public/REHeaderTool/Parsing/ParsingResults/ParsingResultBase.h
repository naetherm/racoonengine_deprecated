/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <vector>
#include <iterator>	//std::make_move_iterator

#include "REHeaderTool/Parsing/ParsingError.h"

namespace REHeaderTool
{
	class ParsingResultBase
	{
		public:
			/** List of errors that happened during parsing */
			std::vector<ParsingError>	errors;

			/**
			*	@brief	Append the errors of the provided result to this result.
			*			Errors contained in the provided result are then cleared.
			*
			*	@param result ParsingResult to append.
			*/
			void appendResultErrors(ParsingResultBase& result) noexcept;
	};
}