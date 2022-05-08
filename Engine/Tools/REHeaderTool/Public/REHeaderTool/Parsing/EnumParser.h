/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <clang-c/Index.h>

#include "REHeaderTool/Parsing/EntityParser.h"
#include "REHeaderTool/Parsing/EnumValueParser.h"
#include "REHeaderTool/Parsing/ParsingResults/EnumParsingResult.h"

namespace REHeaderTool
{
	class EnumParser final : public EntityParser
	{
		private:
			/** Parser used to parse enum values. */
			EnumValueParser	_enumValueParser;

			/**
			*	@brief This method is called at each node (cursor) of the parsing.
			*
			*	@param cursor		Current cursor to parse.
			*	@param parentCursor	Parent of the current cursor.
			*	@param clientData	Pointer to a data provided by the client. Must contain an EnumParser*.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			static CXChildVisitResult				parseNestedEntity(CXCursor		cursor,
																	  CXCursor		parentCursor,
																	  CXClientData	clientData)			noexcept;

			/**
			*	@brief Retrieve the properties from the provided cursor if possible.
			*
			*	@param cursor Property cursor we retrieve information from.
			*
			*	@return A filled list of properties if valid, else nullopt.
			*/
			opt::optional<std::vector<Property>>	getProperties(CXCursor const& cursor)				noexcept;

			/**
			*	@brief Set the parsed enum into the context result if it is a valid one.
			*
			*	@param annotationCursor The cursor used to check enum validity.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult						setParsedEntity(CXCursor const& annotationCursor)	noexcept;

			/**
			*	@brief Add the provided enum value result to the current enum context result.
			*
			*	@param result ClassParsingResult to add.
			*/
			void									addEnumValueResult(EnumValueParsingResult&& result)	noexcept;

			/**
			*	@brief	Check whether the current enum should be parsed or not.
			*			Shadow the parent implementation to add the "shouldParseAllEnums" flag check.
			* 
			*	@return true if the current enum should be parsed, else false.
			*/
			bool									shouldParseCurrentEntity()							noexcept;

			/**
			*	@brief Push a new clean context to prepare enum parsing.
			*
			*	@param enumCursor		Root cursor of the enum to parse.
			*	@param parentContext	Context the new context will inherit from.
			*	@param out_result		Result to fill during parsing.
			*
			*	@return The new context.
			*/
			ParsingContext&							pushContext(CXCursor const&			enumCursor,
																ParsingContext const&	parentContext,
																EnumParsingResult&		out_result)		noexcept;

			/**
			*	@brief Helper to get the ParsingResult contained in the context as a ClassParsingResult.
			*
			*	@return The cast ClassParsingResult.
			*/
			inline EnumParsingResult*				getParsingResult()									noexcept;

		protected:
			/**
			*	@brief Parse an enum value.
			*
			*	@param enumValueCursor	AST cursor to the enum value to parse.
			*	@param out_visitResult	An enum which indicates how to choose the next cursor to parse in the AST.
			*
			*	@return A structure containing information about the parsed enum value.
			*/
			EnumValueParsingResult	parseEnumValue(CXCursor const&		enumValueCursor,
												   CXChildVisitResult&	out_visitResult)	noexcept;

		public:
			/**
			*	@brief Parse the enum starting at the provided AST cursor.
			*
			*	@param enumCursor		AST cursor to the enum to parse.
			*	@param parentContext	Context the new context will inherit from.
			*	@param out_result		Result filled while parsing the enum.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult	parse(CXCursor const&			enumCursor,
									  ParsingContext const&		parentContext,
									  EnumParsingResult&		out_result)		noexcept;
	};

	#include "REHeaderTool/Parsing/EnumParser.inl"
}