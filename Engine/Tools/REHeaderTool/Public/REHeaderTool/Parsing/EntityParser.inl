/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

inline bool EntityParser::shouldParseCurrentEntity() const noexcept
{
	ParsingContext const& context = getContext();

	return context.parentContext != nullptr && context.parentContext->shouldParseAllNested;
}

inline ParsingContext& EntityParser::getContext() noexcept
{
	//Can't retrieve the context if there is none.
	assert(!contextsStack.empty());

	return contextsStack.top();
}

inline ParsingContext const& EntityParser::getContext() const noexcept
{
	//Can't retrieve the context if there is none.
	assert(!contextsStack.empty());

	return contextsStack.top();
}

inline void EntityParser::popContext() noexcept
{
	assert(!contextsStack.empty());

	contextsStack.pop();
}