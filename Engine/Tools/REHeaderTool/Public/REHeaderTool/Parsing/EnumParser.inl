/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

inline EnumParsingResult* EnumParser::getParsingResult() noexcept
{
	return reinterpret_cast<EnumParsingResult*>(getContext().parsingResult);
}