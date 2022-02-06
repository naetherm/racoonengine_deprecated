/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <clang-c/Index.h>

#include "REHeaderTool/Parsing/EntityParser.h"
#include "REHeaderTool/Parsing/ParsingResults/FunctionParsingResult.h"
#include "REHeaderTool/Misc/Optional.h"

namespace REHeaderTool
{
	class FunctionParser : public EntityParser
	{
		private:
			/**
			*	@brief This method is called at each node (cursor) of the parsing.
			*
			*	@param cursor		AST cursor to the entity to parse.
			*	@param parentCursor	Parent of the current cursor.
			*	@param clientData	Pointer to a data provided by the client. Must contain a FunctionParser*.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			static CXChildVisitResult				parseNestedEntity(CXCursor		cursor,
																	  CXCursor		parentCursor,
																	  CXClientData	clientData)				noexcept;

			/**
			*	@brief Retrieve the properties from the provided cursor if possible.
			*
			*	@param cursor Property cursor we retrieve the information from.
			*
			*	@return A filled PropertyGroup if valid, else nullopt.
			*/
			opt::optional<std::vector<Property>>	getProperties(CXCursor const& cursor)					noexcept;

			/**
			*	@brief Set the parsed function if it is a valid one.
			*
			*	@param annotationCursor The cursor used to check function validity.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult						setParsedEntity(CXCursor const& annotationCursor)		noexcept;

			/**
			*	@brief	Check whether the current function should be parsed or not.
			*			Shadow the parent implementation to add the "shouldParseAllFunctions" flag check.
			* 
			*	@return true if the current function should be parsed, else false.
			*/
			bool									shouldParseCurrentEntity()								noexcept;

			/**
			*	@brief Push a new clean context to prepare function parsing.
			*
			*	@param methodCursor		Root cursor of the function to parse.
			*	@param parentContext	Context the new context will inherit from.
			*	@param out_result		Result to fill during parsing.
			*
			*	@return The new context.
			*/
			ParsingContext&							pushContext(CXCursor const&			methodCursor,
																ParsingContext const&	parentContext,
																FunctionParsingResult&	out_result)			noexcept;

			/**
			*	@brief Helper to get the ParsingResult contained in the context as a MethodParsingResult.
			*
			*	@return The cast MethodParsingResult.
			*/
			inline FunctionParsingResult*			getParsingResult()										noexcept;

		public:
			/**
			*	@brief Parse the function starting at the provided AST cursor.
			*
			*	@param functionCursor	AST cursor to the function to parse.
			*	@param parentContext	Context the new context will inherit from.
			*	@param out_result		Result filled while parsing the function.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult	parse(CXCursor const&			functionCursor,
									  ParsingContext const&		parentContext,
									  FunctionParsingResult&	out_result)			noexcept;
	};

	#include "REHeaderTool/Parsing/FunctionParser.inl"
}