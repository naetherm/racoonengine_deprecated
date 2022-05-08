/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include "REHeaderTool/InfoStructures/EntityInfo.h"
#include "REHeaderTool/Properties/PropertyParsingSettings.h"
#include "REHeaderTool/Misc/Optional.h"

namespace REHeaderTool
{
	class PropertyParser
	{
		private:
			/** Parsing settings to use. */
			PropertyParsingSettings const*			_propertyParsingSettings	= nullptr;

			/** Last parsing error which occured when parsing from this parser. */
			std::string								_parsingErrorDescription	= "";

			/** Collection of last parsed split properties. */
			std::vector<std::vector<std::string>>	_splitProps;

			/** Chars to take into consideration when parsing a property. */
			std::string								_relevantCharsForPropParsing;

			/** Chars to take into consideration when parsing property arguments. */
			std::string								_relevantCharsForPropArgsParsing;

			/**
			*	@brief	Split properties and fill _splitProps on success.
			*			On failure, _parsingErrorDescription is updated.
			*
			*	@param propertiesString String containing the properties to split.
			*
			*	@return true on a successful split, else false.
			*/
			bool									splitProperties(std::string&& propertiesString)							noexcept;

			/**
			*	@brief Remove all starting space characters.
			*
			*	@param toCleanString The string to clean.
			*/
			void									removeStartSpaces(std::string& toCleanString)					const	noexcept;

			/**
			*	@brief Remove all trailing space characters.
			*
			*	@param toCleanString The string to clean.
			*/
			void									removeTrailSpaces(std::string& toCleanString)					const	noexcept;

			/**
			*	@brief	Search the next property.
			*			inout_parsingProps will be updated to the current parsing state and out_isParsingArgument is updated consequently.
			*
			*	@param inout_parsingProps		The string we are looking the next prop in.
			*	@param out_isParsingArgument	Updated by this function call to indicate either the processed prop has following arguments or not.
			*
			*	@return true & add the property to _splitProps (new vector) on success, else return false.
			*/
			bool									lookForNextProperty(std::string&	inout_parsingProps,
																		bool&			out_isParsingArgument)				noexcept;

			/**
			*	@brief	Search the next property argument.
			*			inout_parsingProps will be updated to the current parsing state and out_isParsingArgument is updated consequently.
			*
			*	@param inout_parsingProps		The string we are looking the next prop in.
			*	@param out_isParsingArgument	Filled by this function call to indicate either the processed prop has following arguments or not.
			*
			*	@return true & add the argument to _splitProps (last added vector) on success, else return false.
			*/
			bool									lookForNextPropertyArgument(std::string&	inout_parsingProps,
																				bool&			out_isParsingArgument)		noexcept;

			/**
			*	@brief	Create a list of properties based on the parsed properties
			*	
			*	@param splitProps The collection of all parsed props / arguments.
			*
			*	@return A list of properties.
			*/
			std::vector<Property>					fillProperties(std::vector<std::vector<std::string>>& splitProps)		noexcept;

			/**
			*	@brief Add a property to the given property group using the provided data.
			*
			*	@param propertyAsVector	Vector of properties. Element 0 is the property name, next elements are associated arguments.
			*	@param out_properties	List of properties to update.
			*/
			void									addProperty(std::vector<std::string>&	propertyAsVector,
																std::vector<Property>&		out_properties)					noexcept;

			/**
			*	@brief Retrieve properties from a string if possible.
			*
			*	@param annotateMessage	The raw string contained in the __attribute__(annotate()) preprocessor.
			*	@param annotationId		The annotation the annotate message should begin with to be considered as valid.
			*	@param entityType		The type of the attached entity.
			*
			*	@return A valid optional object if all properties were valid, else an empty optional.
			*			On failure, _parsingErrorDescription is updated.
			*/
			opt::optional<std::vector<Property>>	getProperties(std::string&&			annotateMessage,
																  std::string const&	annotationId)						noexcept;

		public:
			/**
			*	@brief Called by the parser to internally setup some acceleration data
			*
			*	@param propertyParsingSettings Parsing settings to be used by this parser.
			*/
			void									setup(PropertyParsingSettings const& propertyParsingSettings)	noexcept;

			/**
			*	@brief	Clear all collected data such as parsingErrors or split props. Called to have a clean state and prepare to parse new properties.
			*/
			void									clean()															noexcept;

			/**
			*	@brief Retrieve the properties from a namespace annotate attribute.
			*
			*	@param annotateMessage The string we retrieve the properties from.
			*	
			*	@return A valid optional object if all properties were valid, else an empty optional.
			*			On failure, _parsingErrorDescription is updated.
			*/
			opt::optional<std::vector<Property>>	getNamespaceProperties(std::string annotateMessage)				noexcept;
			
			/**
			*	@brief Retrieve the properties from a class annotate attribute.
			*
			*	@param annotateMessage The string we retrieve the properties from.
			*	
			*	@return A valid optional object if all properties were valid, else an empty optional.
			*			On failure, _parsingErrorDescription is updated.
			*/
			opt::optional<std::vector<Property>>	getClassProperties(std::string annotateMessage)					noexcept;
			
			/**
			*	@brief Retrieve the properties from a struct annotate attribute.
			*
			*	@param annotateMessage The string we retrieve the properties from.
			*	
			*	@return A valid optional object if all properties were valid, else an empty optional.
			*			On failure, _parsingErrorDescription is updated.
			*/
			opt::optional<std::vector<Property>>	getStructProperties(std::string annotateMessage)				noexcept;
			
			/**
			*	@brief Retrieve the properties from a variable annotate attribute.
			*
			*	@param annotateMessage The string we retrieve the properties from.
			*	
			*	@return A valid optional object if all properties were valid, else an empty optional.
			*			On failure, _parsingErrorDescription is updated.
			*/
			opt::optional<std::vector<Property>>	getVariableProperties(std::string annotateMessage)				noexcept;

			/**
			*	@brief Retrieve the properties from a field annotate attribute.
			*
			*	@param annotateMessage The string we retrieve the properties from.
			*	
			*	@return A valid optional object if all properties were valid, else an empty optional.
			*			On failure, _parsingErrorDescription is updated.
			*/
			opt::optional<std::vector<Property>>	getFieldProperties(std::string annotateMessage)					noexcept;

			/**
			*	@brief Retrieve the properties from a function annotate attribute.
			*
			*	@param The string we retrieve the properties from.
			*	
			*	@return A valid optional object if all properties were valid, else an empty optional.
			*			On failure, _parsingErrorDescription is updated.
			*/
			opt::optional<std::vector<Property>>	getFunctionProperties(std::string annotateMessage)				noexcept;

			/**
			*	@brief Retrieve the properties from a method annotate attribute.
			*
			*	@param annotateMessage The string we retrieve the properties from.
			*	
			*	@return A valid optional object if all properties were valid, else an empty optional.
			*			On failure, _parsingErrorDescription is updated.
			*/
			opt::optional<std::vector<Property>>	getMethodProperties(std::string annotateMessage)				noexcept;

			/**
			*	@brief Retrieve the properties from an enum annotate attribute.
			*
			*	@param annotateMessage The string we retrieve the properties from.
			*	
			*	@return A valid optional object if all properties were valid, else an empty optional.
			*			On failure, _parsingErrorDescription is updated.
			*/
			opt::optional<std::vector<Property>>	getEnumProperties(std::string annotateMessage)					noexcept;

			/**
			*	@brief Retrieve the properties from an enum value annotate attribute.
			*
			*	@param annotateMessage The string we retrieve the properties from.
			*	
			*	@return A valid optional object if all properties were valid, else an empty optional.
			*			On failure, _parsingErrorDescription is updated.
			*/
			opt::optional<std::vector<Property>>	getEnumValueProperties(std::string annotateMessage)				noexcept;

			/**
			*	@brief Getter for _parsingErrorDescription field.
			*	
			*	@return _parsingErrorDescription.
			*/
			std::string const&				getParsingErrorDescription()									const	noexcept;
	};
}