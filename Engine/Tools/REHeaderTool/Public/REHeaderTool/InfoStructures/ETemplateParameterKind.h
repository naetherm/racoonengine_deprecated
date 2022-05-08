/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <inttypes.h>

namespace REHeaderTool
{
	/**
	*	@brief Defines the kind of a template parameter.
	*/
	enum class ETemplateParameterKind : int8_t
	{
		Undefined,

		/**
		*	The template parameter is a non-type template parameter, like:
		*	template <int Value> or template <auto Value>
		*/
		NonTypeTemplateParameter,

		/**
		*	The template parameter is a type template parameter, like:
		*	template <typename T>
		*/
		TypeTemplateParameter,

		/**
		*	The template parameter is a template template parameter, like:
		*	template <template <typename> typename T>
		*/
		TemplateTemplateParameter
	};
}