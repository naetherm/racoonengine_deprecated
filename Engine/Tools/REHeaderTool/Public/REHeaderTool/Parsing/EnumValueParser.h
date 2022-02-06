/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <clang-c/Index.h>

#include "REHeaderTool/Parsing/EntityParser.h"
#include "REHeaderTool/Parsing/ParsingResults/EnumValueParsingResult.h"

namespace REHeaderTool
{
	class EnumValueParser final : public EntityParser
	{
		private:
			/**
			*	@brief This method is called at each node (cursor) of the parsing.
			*
			*	@param cursor		Current cursor to parse.
			*	@param parentCursor	Parent of the current cursor.
			*	@param clientData	Pointer to a data provided by the client. Must contain an EnumValueParser*.
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
			*	@brief Set the parsed enum value into the context result if it is a valid one.
			*
			*	@param annotationCursor The cursor used to check enum validity.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult						setParsedEntity(CXCursor const& annotationCursor)	noexcept;

			/**
			*	@brief Fill the enum value result properties.
			*
			*	@param annotationCursor The AST cursor to the enum value.
			*/
			void									setProperties(CXCursor const& annotationCursor)		noexcept;

			/**
			*	@brief	Check whether the current enum value should be parsed or not.
			*			Shadow the parent implementation to add the "shouldParseAllEnumValues" flag check.
			* 
			*	@return true if the current enum value should be parsed, else false.
			*/
			bool									shouldParseCurrentEntity()							noexcept;

			/**
			*	@brief Push a new clean context to prepare enum value parsing.
			*
			*	@param enumValueCursor	Root cursor of the enum value to parse.
			*	@param parentContext	Context the new context will inherit from.
			*	@param out_result		Result to fill during parsing.
			* 
			*	@return The new context.
			*/
			ParsingContext&							pushContext(CXCursor const&			enumValueCursor,
																ParsingContext const&	parentContext,
																EnumValueParsingResult&	out_result)		noexcept;

			/**
			*	@brief Helper to get the ParsingResult contained in the context as a ClassParsingResult.
			*
			*	@return The cast ClassParsingResult.
			*/
			inline EnumValueParsingResult*			getParsingResult()									noexcept;

		public:
			/**
			*	@brief Parse the enum value starting at the provided AST cursor.
			*
			*	@param enumValueCursor	AST cursor to the enum value to parse.
			*	@param parentContext	Context the new context will inherit from.
			*	@param out_result		Result filled while parsing the enum value.
			*
			*	@return An enum which indicates how to choose the next cursor to parse in the AST.
			*/
			CXChildVisitResult	parse(CXCursor const&			enumValueCursor,
									  ParsingContext const&		parentContext,
									  EnumValueParsingResult&	out_result)		noexcept;
	};

	#include "REHeaderTool/Parsing/EnumValueParser.inl"
}