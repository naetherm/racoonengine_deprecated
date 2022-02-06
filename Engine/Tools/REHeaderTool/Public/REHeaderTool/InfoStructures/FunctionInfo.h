/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <vector>

#include <clang-c/Index.h>

#include "REHeaderTool/InfoStructures/EntityInfo.h"
#include "REHeaderTool/InfoStructures/TypeInfo.h"
#include "REHeaderTool/InfoStructures/FunctionParamInfo.h"

namespace REHeaderTool
{
	class FunctionInfo : public EntityInfo
	{
		private:
			/**
			*	@brief Get the name of this function.
			*	
			*	@return Return the name of this function.
			*/
			std::string getName() const noexcept;

		protected:
			FunctionInfo(CXCursor const&			cursor,
						 std::vector<Property>&&	properties,
						 EEntityType				entityType)		noexcept;

		public:
			static constexpr EEntityType	nestedEntityTypes = EEntityType::Undefined;

			/** Prototype of this function as a string. */
			std::string						prototype;

			/** Return type of this function. */
			TypeInfo						returnType;

			/** Parameters of this function. */
			std::vector<FunctionParamInfo>	parameters;

			/** Is this function inline or not. */
			bool isInline	: 1;

			/** Is this function static or not. */
			bool isStatic	: 1;

			FunctionInfo(CXCursor const&			cursor,
						 std::vector<Property>&&	properties)	noexcept;

			/**
			*	@brief Get the prototype of this function.
			*	
			*	@param removeQualifiers	Should all function qualifiers be removed from the prototype?
			*	@param removeSpaces		Should all spaces be removed from the prototype?
			*	
			*	@return The prototype of the function.
			*/
			std::string getPrototype(bool removeQualifiers	= false,
									 bool removeSpaces		= false)	const noexcept;

			/**
			*	@brief	Get the parameter types of the function separated by a coma.
			*			If the function takes no parameter, an empty string is returned.
			*	
			*	@return The parameter types of the function.
			*/
			std::string getParameterTypes()								const noexcept;
	};
}