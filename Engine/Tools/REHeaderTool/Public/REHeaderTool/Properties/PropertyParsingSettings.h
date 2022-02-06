/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>

#include "REHeaderTool/Misc/Settings.h"

namespace REHeaderTool
{
	class PropertyParsingSettings : public Settings
	{
		public:
			/** Char used to separate two properties. */
			char			propertySeparator		= ',';

			/** Char used to separate two sub properties. */
			char			argumentSeparator		= ',';

			/** Chars used to respectively start and close a group of sub properties. */
			char			argumentEnclosers[2]	= { '(', ')' };

			/** Macro to use to attach properties to a namespace. */
			std::string		namespaceMacroName		= "NAMESPACE";

			/** Macro to use to attach properties to a class. */
			std::string		classMacroName			= "CLASS";

			/** Macro to use to attach properties to a struct. */
			std::string		structMacroName			= "STRUCT";

			/** Macro to use to attach properties to a variable (non-member). */
			std::string		variableMacroName		= "VARIABLE";

			/** Macro to use to attach properties to a field. */
			std::string		fieldMacroName			= "FIELD";

			/** Macro to use to attach properties to a function. */
			std::string		functionMacroName		= "FUNCTION";

			/** Macro to use to attach properties to a method. */
			std::string		methodMacroName			= "METHOD";

			/** Macro to use to attach properties to an enum. */
			std::string		enumMacroName			= "ENUM";

			/** Macro to use to attach properties to an enum value. */
			std::string		enumValueMacroName		= "ENUMVALUE";

			/**
			*	@brief Load all settings from the provided toml data.
			* 
			*	@param tomlData	Data loaded from a toml file.
			*	@param logger	Optional logger used to issue loading logs. Can be nullptr.
			* 
			*	@return true if no error occured during loading, else false.
			*/
			virtual bool loadSettingsValues(toml::value const&	tomlData,
											ILogger*			logger)		noexcept override;
	};
}