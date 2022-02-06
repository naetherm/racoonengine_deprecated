/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <unordered_set>
#include <string>

#include "REHeaderTool/Properties/PropertyParsingSettings.h"
#include "REHeaderTool/Misc/Settings.h"
#include "REHeaderTool/Misc/Filesystem.h"

namespace REHeaderTool
{
	class ParsingSettings : public Settings
	{
		private:
			/** Section name used in the toml file for FileGenerator settings. */
			static constexpr char const*			_tomlSectionName				= "ParsingSettings";

			/**
			*	Include directories of the project which must be known for accurate parsing.
			*	It basically corresponds to the list of paths specified with the -I argument
			*	when compiling.
			*/
			std::unordered_set<fs::path, PathHash>	_projectIncludeDirectories;

			/**
			*	Name of the compiler used to compile the header files being parsed.
			*	This is used to make sure the parser recognizes the included headers.
			*	As for now, supported values are "clang", "gcc" and "msvc".
			*/
			std::string								_compilerExeName				= "";

			/** Variables used to build compilation command line. */
			std::string								_REHeaderToolParsingMacro			= "-D" + parsingMacro;
			std::vector<std::string>				_projectIncludeDirs;

			std::string								_namespacePropertyMacro;
			std::string								_classPropertyMacro;
			std::string								_structPropertyMacro;
			std::string								_variablePropertyMacro;
			std::string								_fieldPropertyMacro;
			std::string								_functionPropertyMacro;
			std::string								_methodPropertyMacro;
			std::string								_enumPropertyMacro;
			std::string								_enumValuePropertyMacro;

			std::vector<char const*>				_compilationArguments;

			/**
			*	@brief Refresh all internal compilation macros to pass to the compiler.
			* 
			*	@param logger Optional logger used to issue logs in case of error. Can be nullptr.
			*/
			void	refreshBuildCommandStrings(ILogger* logger)								noexcept;

			/**
			*	@brief Make a list of all arguments to pass to the compilation command and store it in _compilationArguments.
			* 
			*	@param logger Optional logger used to issue logs in case of error. Can be nullptr.
			*/
			void	refreshCompilationArguments(ILogger* logger)							noexcept;

			/**
			*	@brief Load all shouldParse[EntityType] settings from toml.
			*
			*	@param parsingSettings	Toml content.
			*	@param logger			Optional logger used to issue loading logs. Can be nullptr.
			*/
			void	loadShouldParseAllEntities(toml::value const&	parsingSettings,
											   ILogger*				logger)					noexcept;

			/**
			*	@brief Load the shouldLogDiagnostic setting from toml.
			*
			*	@param parsingSettings	Toml content.
			*	@param logger			Optional logger used to issue loading logs. Can be nullptr.
			*/
			void	loadShouldLogDiagnostic(toml::value const&	parsingSettings,
											ILogger*			logger)						noexcept;

			/**
			*	@brief Load the shouldAbortParsingOnFirstError setting from toml.
			*
			*	@param parsingSettings	Toml content.
			*	@param logger			Optional logger used to issue loading logs. Can be nullptr.
			*/
			void	loadShouldAbortParsingOnFirstError(toml::value const&	parsingSettings,
													   ILogger*				logger)			noexcept;

			/**
			*	@brief	Load the _compilerExeName setting from toml.
			*
			*	@param parsingSettings	Toml content.
			*	@param logger			Optional logger used to issue loading logs. Can be nullptr.
			*/
			void	loadCompilerExeName(toml::value const&	parsingSettings,
										ILogger*			logger)							noexcept;

			/**
			*	@brief	Load the _projectIncludeDirectories setting from toml.
			*			Loaded directories completely replace previous _projectIncludeDirectories if any.
			*
			*	@param parsingSettings	Toml content.
			*	@param logger			Optional logger used to issue loading logs. Can be nullptr.
			*/
			void	loadProjectIncludeDirectories(toml::value const&	parsingSettings,
												  ILogger*				logger)				noexcept;

		protected:
			virtual bool loadSettingsValues(toml::value const&	tomlData,
											ILogger*			logger)		noexcept override;

		public:
			/** Macro defined internally when REHeaderTool processes a translation unit. */
			static inline std::string const			parsingMacro					= "KODGEN_PARSING";

			/** Settings used when parsing C++ entities. */
			PropertyParsingSettings					propertyParsingSettings;

			/** If set to true, will parse all namespaces, whether they are annotated or not. */
			bool									shouldParseAllNamespaces		= false;

			/** If set to true, will parse all classes, whether they are annotated or not. */
			bool									shouldParseAllClasses			= false;

			/** If set to true, will parse all structs, whether they are annotated or not. */
			bool									shouldParseAllStructs			= false;

			/** If set to true, will parse all variables, whether they are annotated or not. */
			bool									shouldParseAllVariables			= false;

			/** If set to true, will parse all fields, whether they are annotated or not. */
			bool									shouldParseAllFields			= false;

			/** If set to true, will parse all functions, whether they are annotated or not. */
			bool									shouldParseAllFunctions			= false;

			/** If set to true, will parse all methods, whether they are annotated or not. */
			bool									shouldParseAllMethods			= false;

			/** If set to true, will parse all enums, whether they are annotated or not. */
			bool									shouldParseAllEnums				= false;

			/** If set to true, will parse all enum values, whether they are annotated or not. */
			bool									shouldParseAllEnumValues		= true;

			/**
			*	Should parsing be aborted when an error is encountered or not.
			*	If set to false, errors will be collected for the whole parsing.
			*/
			bool									shouldAbortParsingOnFirstError	= true;

			/**
			*	Should the diagnostic of a translation unit parsing be logged ?
			*	Setting this to true might give a hint to the user for potential errors.
			*	However errors can sometimes be misleading (generated by clang itself), so use with caution.
			*/
			bool									shouldLogDiagnostic				= false;

			virtual ~ParsingSettings() = default;

			/**
			*	@brief	Initialize the build command forwarded to libclang to parse C++ files using
			*			the current settings.
			*			This method is called internally by the FileGenerator.
			* 
			*	@param logger Optional logger used to issue logs in case of error. Can be nullptr.
			*/
			void	init(ILogger* logger)																				noexcept;

			/**
			*	@brief	Add a project include directory to the parsing settings.
			*			If the provided path is invalid of if the path was already a project include directory, do nothing.
			*	
			*	@param directoryPath Path to the include directory.
			*
			*	@return true if the provided path was added to the project include directories, else false.
			*/
			bool	addProjectIncludeDirectory(fs::path const& directoryPath)											noexcept;

			/**
			*	@brief	Remove a project directory from the parsing settings.
			*
			*	@param directoryPath Path to the include directory.
			*/
			void	removeProjectIncludeDirectory(fs::path const& directoryPath)										noexcept;

			/**
			*	@brief Clear the list of project include directories.
			*/
			void	clearProjectIncludeDirectories()																	noexcept;

			/**
			*	@brief Getter for _projectIncludeDirectories field.
			*	
			*	@return _projectIncludeDirectories;
			*/
			std::unordered_set<fs::path, PathHash> const&	getProjectIncludeDirectories()						const	noexcept;

			/**
			*	@brief Getter for _compilerExeName field.
			*	
			*	@return _compilerExeName;
			*/
			std::string const&								getCompilerExeName()								const	noexcept;

			/**
			*	@brief Getter for _compilationArguments.
			* 
			*	@return _compilationArguments field.
			*/
			std::vector<char const*> const&					getCompilationArguments()							const	noexcept;

			/**
			*	@brief	Setter for _compilerExeName field.
			*			This will also check that the compiler is indeed available on the running computer.
			*			If the compiler is not available on the running computer, the field is not set.
			*			As for now, supported values are "clang++", "g++" and "msvc".
			*	
			*	@return true if the compiler is valid on the running computer, else false.
			*/
			bool											setCompilerExeName(std::string const& compilerExeName)		noexcept;
	};
}