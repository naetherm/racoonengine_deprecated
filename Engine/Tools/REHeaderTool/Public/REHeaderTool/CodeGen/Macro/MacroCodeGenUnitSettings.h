/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string_view>

#include "REHeaderTool/CodeGen/CodeGenUnitSettings.h"

namespace REHeaderTool
{
	//Forward declaration
	class StructClassInfo;

	class MacroCodeGenUnitSettings : public CodeGenUnitSettings
	{
		private:
			/**
			*	Tag usable in _generatedHeaderFileNamePattern and _generatedSourceFileNamePattern.
			*	All instances of this tag will be replaced by the actual target file name.
			*/
			static constexpr std::string_view const filenameTag			= "##FILENAME##";
			
			/**
			*	Tag usable in _classCodeFooterMacroPattern.
			*	All instances of this tag will be replaced by the actual class name.
			*/
			static constexpr std::string_view const classNameTag		= "##CLASSNAME##";

			/**
			*	Tag usable in _classCodeFooterMacroPattern.
			*	All instances of this tag will be replaced by the actual full class name.
			*/
			static constexpr std::string_view const classFullNameTag	= "##CLASSFULLNAME##";

			/**
			*	Pattern to use to generate header files.
			*	##FILENAME## will be replaced by the target file name.
			*/
			std::string		_generatedHeaderFileNamePattern	= "##FILENAME##.h.h";

			/**
			*	Pattern to use to generate source files.
			*	##FILENAME## will be replaced by the target file name.
			*/
			std::string		_generatedSourceFileNamePattern	= "##FILENAME##.src.h";

			/**
			*	Pattern to use to generate class footer macro.
			*	##CLASSNAME## and ##CLASSFULLNAME## will be replaced by the class name and full name respectively.
			*/
			std::string		_classFooterMacroPattern		= "##CLASSFULLNAME##_GENERATED";

			/**
			*	Pattern to use to generate header file footer macro.
			*	##FILENAME## will be replaced by the target file name.
			*/
			std::string		_headerFileFooterMacroPattern	= "File_##FILENAME##_GENERATED";

			/**
			*	Macro used to export a symbol if the generated code is injected in a dynamic library.
			*	On Windows, the macro should typically expand to __declspec(dllexport).
			*	
			*	/!\ The macro MUST evalutate to nothing when the parser is running (when KODGEN_PARSING macro is defined). /!\
			*/
			std::string		_exportSymbolMacroName			= "";

			/**
			*	Macro used to hide a symbol if the generated code is injected in a dynamic library.
			*	On Unix systems, the macro should typically expand to __attribute__((visibility("hidden"))).
			* 
			*	/!\ The macro MUST evalutate to nothing when the parser is running (when KODGEN_PARSING macro is defined). /!\
			*/
			std::string		_internalSymbolMacroName		= "";

		protected:
			/**
			*	@brief	Modifies a macro name if necessary to make it a valid C++ macro name.
			*			All invalid characters are replaced by the underscore character '_'.
			* 
			*	@param inout_macroName Name of the macro to sanitize.
			* 
			*	@return true if the macro was modified my the method, else false.
			*/
			static bool		sanitizeMacroName(std::string&	inout_macroName)						noexcept;

			/**
			*	@brief Replace all occurences of tag by replacement in the provided string.
			* 
			*	@param inout_string	String to process.
			*	@param tag			Tag to replace in the provided string.
			*	@param replacement	String all tags should be replaced by.
			*/
			static void		replaceTags(std::string&		inout_string,
										std::string_view	tag,
										std::string const&	replacement)							noexcept;

			/**
			*	@brief Load all settings from the provided toml data.
			* 
			*	@param tomlData	Data loaded from a toml file.
			*	@param logger	Optional logger used to issue loading logs. Can be nullptr.
			* 
			*	@return true if no error occured during loading, else false.
			*/
			virtual bool	loadSettingsValues(toml::value const&	tomlData,
											   ILogger*				logger)							noexcept	override;

			/**
			*	@brief Load the _generatedHeaderFileNamePattern and _generatedSourceFileNamePattern settings from toml.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadFileNamePatterns(toml::value const&	generationSettings,
												 ILogger*			logger)							noexcept;

			/**
			*	@brief Load the class footer macro pattern setting from toml.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadClassFooterMacroPattern(toml::value const&	generationSettings,
														ILogger*			logger)					noexcept;

			/**
			*	@brief Load the file footer macro pattern setting from toml.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadHeaderFileFooterMacroPattern(toml::value const&	generationSettings,
															 ILogger*			logger)				noexcept;

			/**
			*	@brief Load the _exportSymbolMacroName field from toml.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadExportSymbolMacroName(toml::value const&	generationSettings,
													  ILogger*				logger)					noexcept;

			/**
			*	@brief Load the _internalSymbolMacroName field from toml.
			*
			*	@param generationSettings	Toml content.
			*	@param logger				Optional logger used to issue loading logs. Can be nullptr.
			*/
			void			loadInternalSymbolMacroName(toml::value const&	generationSettings,
														ILogger*			logger)					noexcept;

		public:
			/**
			*	@brief Setter for _generatedHeaderFileNamePattern.
			*	
			*	@param generatedHeaderFileNamePattern Header file name pattern.
			*/
			void				setGeneratedHeaderFileNamePattern(std::string const& generatedHeaderFileNamePattern)	noexcept;

			/**
			*	@brief Setter for _generatedSourceFileNamePattern.
			*	
			*	@param generatedSourceFileNamePattern Source file name pattern.
			*/
			void				setGeneratedSourceFileNamePattern(std::string const& generatedSourceFileNamePattern)	noexcept;

			/**
			*	@brief Setter for _classFooterMacroPattern.
			*
			*	@param classFooterMacroPattern Class footer macro pattern.
			*/
			void				setClassFooterMacroPattern(std::string const& classFooterMacroPattern)					noexcept;

			/**
			*	@brief Setter for _headerFileFooterMacroPattern.
			*
			*	@param headerFileFooterMacroPattern Header file footer macro pattern.
			*/
			void				setHeaderFileFooterMacroPattern(std::string const& headerFileFooterMacroPattern)		noexcept;

			/**
			*	@brief Setter for the field _exportSymbolMacroName.
			* 
			*	@param exportSymbolMacroName New _exportSymbolMacroName value.
			*/
			void				setExportSymbolMacroName(std::string const& exportSymbolMacroName)						noexcept;

			/**
			*	@brief Setter for the field _internalSymbolMacroName.
			* 
			*	@param exportSymbolMacroName New _internalSymbolMacroName value.
			*/
			void				setInternalSymbolMacroName(std::string const& internalSymbolMacroName)					noexcept;

			/**
			*	@brief Getter for _generatedHeaderFileNamePattern.
			*
			*	@return _generatedHeaderFileNamePattern.
			*/
			std::string const&	getGeneratedHeaderFileNamePattern()												const	noexcept;

			/**
			*	@brief Get the header file name generated for the given file.
			*			This actually returns the header file name pattern by replacing all ##FILENAME## instances by the actual file name.
			* 
			*	@param targetFile Full path to the target file.
			* 
			*	@return The generated header file name (not full path, only file name + extension).
			*/
			fs::path			getGeneratedHeaderFileName(fs::path const& targetFile)							const	noexcept;

			/**
			*	@brief Getter for _generatedSourceFileNamePattern.
			*
			*	@return _generatedSourceFileNamePattern.
			*/
			std::string const&	getGeneratedSourceFileNamePattern()												const	noexcept;

			/**
			*	@brief Get the source file name generated for the given file.
			*			This actually returns the source file name pattern by replacing all ##FILENAME## instances by the actual file name.
			* 
			*	@param targetFile Full path to the target file.
			* 
			*	@return The generated source file name (not full path, only file name + extension).
			*/
			fs::path			getGeneratedSourceFileName(fs::path const& targetFile)							const	noexcept;

			/**
			*	@brief Getter for _classFooterMacroPattern.
			*
			*	@return _classFooterMacroPattern.
			*/
			std::string const&	getClassFooterMacroPattern()													const	noexcept;

			/**
			*	@brief Get the class footer macro corresponding to the provided entity.
			*			This actually returns the class footer macro pattern by replacing all ##CLASSNAME## and ##CLASSFULLNAME## instances.
			* 
			*	@param structClassInfo Struct/class information.
			* 
			*	@return The class footer macro name for the provided struct/class.
			*/
			virtual std::string	getClassFooterMacro(StructClassInfo const& structClassInfo)						const	noexcept;

			/**
			*	@brief Getter for _headerFileFooterMacroPattern.
			*
			*	@return _headerFileFooterMacroPattern.
			*/
			std::string const&	getHeaderFileFooterMacroPattern()												const	noexcept;

			/**
			*	@brief Get the header file footer macro for the given file.
			*			This actually returns the header file footer macro name pattern by replacing all ##FILENAME## instances by the actual file name.
			* 
			*	@param targetFile Full path to the target file.
			* 
			*	@return The generated header file footer macro name.
			*/
			virtual std::string	getHeaderFileFooterMacro(fs::path const& targetFile)							const	noexcept;

			/**
			*	@brief Getter for the field _exportSymbolMacroName.
			* 
			*	@return _exportSymbolMacroName.
			*/
			std::string const&	getExportSymbolMacroName()		const	noexcept;

			/**
			*	@brief Getter for the field _internalSymbolMacroName.
			* 
			*	@return _internalSymbolMacroName.
			*/
			std::string const&	getInternalSymbolMacroName()	const	noexcept;
	};
}