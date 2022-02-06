/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <array>
#include <unordered_map>

#include "REHeaderTool/CodeGen/CodeGenUnit.h"
#include "REHeaderTool/CodeGen/Macro/MacroCodeGenEnv.h"

namespace REHeaderTool
{
	//Forward declaration
	class MacroCodeGenUnitSettings;
	class MacroCodeGenModule;

	/**
	*	CodeGenEnv type: MacroCodeGenEnv
	*/
	class MacroCodeGenUnit final : public CodeGenUnit
	{
		private:
			/** Separator used for each code location. */
			static std::array<std::string, static_cast<size_t>(ECodeGenLocation::Count)> const _separators;

			/** Array containing the generated code per location. ClassFooter value is not used since code is generated in _classFooterGeneratedCode. */
			std::array<std::string, static_cast<size_t>(ECodeGenLocation::Count)>	_generatedCodePerLocation;

			/** Map containing the class footer generated code for each struct/class. */
			std::unordered_map<StructClassInfo const*, std::string>					_classFooterGeneratedCode;
			
			//Make the addModule method taking a CodeGenModule private to replace it with a more restrictive method accepting MacroCodeGenModule only.
			using CodeGenUnit::addModule;

			/**
			*	@brief Handle the code generation for class footer code gen location.
			* 
			*	@param entity	Entity we generate the code for. Must be one of Struct/Class/Field/Method.
			*	@param env		Generation environment.
			*	@param generate	Code generation method.
			*/
			void		generateEntityClassFooterCode(EntityInfo const&						entity,
													  CodeGenEnv&							env,
													  std::function<void(EntityInfo const&,
																		 CodeGenEnv&,
																		 std::string&)>		generate)	noexcept;

			/**
			*	@brief	(Re)generate the header file.
			* 
			*	@param env Generation environment.
			*/
			void		generateHeaderFile(MacroCodeGenEnv&	env)										noexcept;

			/**
			*	@brief	(Re)generate the source file.
			* 
			*	@param env Generation environment.
			*/
			void		generateSourceFile(MacroCodeGenEnv&	env)										noexcept;

			/**
			*	@brief Compute the path of the header file generated from the provided source file.
			* 
			*	@param sourceFile Path to the source file.
			* 
			*	@return the path of the header file generated from the provided source file.
			*/
			fs::path	getGeneratedHeaderFilePath(fs::path const& sourceFile)					const	noexcept;

			/**
			*	@brief Compute the path of the source file generated from the provided source file.
			* 
			*	@param sourceFile Path to the source file.
			* 
			*	@return the path of the source file generated from the provided source file.
			*/
			fs::path	getGeneratedSourceFilePath(fs::path const& sourceFile)					const	noexcept;

		protected:
			/**
			*	@brief	Instantiate a MacroCodeGenEnv object (using new).
			* 
			*	@return A dynamically instantiated (new) MacroCodeGenEnv object used during the whole generation process.
			*/
			virtual MacroCodeGenEnv*	createCodeGenEnv()												const	noexcept	override;

			/**
			*	@brief	Call generate 3 times with the given environment, by updating the environment between each call
			*			(MacroCodeGenEnv::codeGenLocation and MacroCodeGenEnv::separator are updated).
			*			ECodeGenLocation::ClassFooter since this location depends on an entity.
			*
			*	@param env		Generation environment structure.
			*	@param generate	Generation function to call to generate code.
			*/
			virtual void				initialGenerateCode(CodeGenEnv&							env,
															std::function<void(CodeGenEnv&,
																			   std::string&)>	generate)		noexcept	override;

			/**
			*	@brief	Call generate 3 times with the given environment, by updating the environment between each call
			*			(MacroCodeGenEnv::codeGenLocation and MacroCodeGenEnv::separator are updated).
			*			ECodeGenLocation::ClassFooter since this location depends on an entity.
			*
			*	@param env		Generation environment structure.
			*	@param generate	Generation function to call to generate code.
			*/
			virtual void				finalGenerateCode(CodeGenEnv&						env,
														  std::function<void(CodeGenEnv&,
																			 std::string&)>	generate)			noexcept	override;	

			/**
			*	@brief	Call generate 4 times with the given entity and environment, by updating the environment between each call
			*			(MacroCodeGenEnv::codeGenLocation and MacroCodeGenEnv::separator are updated).
			*
			*	@param entity	Target entity for code generation.
			*	@param env		Generation environment structure.
			*	@param generate	Generation function to call to generate code.
			*/
			virtual void				generateCodeForEntity(EntityInfo const&						entity,
															  CodeGenEnv&							env,
															  std::function<void(EntityInfo const&,
																				 CodeGenEnv&,
																				 std::string&)>		generate)	noexcept	override;

			/**
			*	@brief Reset internally used variables to prepare the generation step.
			* 
			*	@param parsingResult	Result of a file parsing used to generate code.
			*	@param env				Generation environment structure.
			* 
			*	@return true if the method completed successfully, else false.
			*/
			virtual bool				preGenerateCode(FileParsingResult const&	parsingResult,
														CodeGenEnv&					env)						noexcept	override;

			/**
			*	@brief	Create/update the header and source files and fill them with the generated code.
			* 
			*	@param env				Generation environment structure.
			* 
			*	@return true if the method completed successfully, else false.
			*/
			virtual bool				postGenerateCode(CodeGenEnv& env)										noexcept	override;

		public:
			/**
			*	@brief	Check that both the generated header and source files are newer than the source file.
			*			If the generated header file doesn't exist, create it and leave it empty.
			*			We do that because since the generated header is included in the source code,
			*			it could generate an undefined behaviour if the header doesn't exist.
			* 
			*	@param sourceFile Path to the source file.
			*
			*	@return true if the code generated for sourceFile is up-to-date, else false.
			*/
			virtual bool					isUpToDate(fs::path const& sourceFile)				const	noexcept	override;

			/**
			*	@brief	Add a module to the internal list of generation modules.
			*			This method is a more restrictive replacement for the CodeGenUnit::addModule(CodeGenModule&) method.
			* 
			*	@param generationModule The generation module to add.
			*/
			void							addModule(MacroCodeGenModule& generationModule)				noexcept;

			/**
			*	@brief Getter for settings field, cast to MacroCodeGenUnitSettings.
			* 
			*	@return settings cast to MacroCodeGenUnitSettings.
			*/
			MacroCodeGenUnitSettings const*	getSettings()										const	noexcept;

			/**
			*	@brief Setter for the inherited settings field with suitable derived settings class.
			*/
			void							setSettings(MacroCodeGenUnitSettings const& cguSettings)	noexcept;
	};
}