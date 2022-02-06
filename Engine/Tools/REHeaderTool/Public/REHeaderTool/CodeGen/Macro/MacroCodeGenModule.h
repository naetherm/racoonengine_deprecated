/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include "REHeaderTool/CodeGen/CodeGenModule.h"
#include "REHeaderTool/CodeGen/Macro/MacroCodeGenerator.h"

namespace REHeaderTool
{
	//Forward declaration
	class MacroCodeGenEnv;
	class MacroPropertyCodeGen;

	/**
	*	Code generation module used with MacroCodeGenEnv environments.
	*/
	class MacroCodeGenModule : public CodeGenModule, public MacroCodeGenerator
	{
  protected:
			//Make the addPropertyCodeGen private to replace it with a more restrictive method accepting MacroPropertyCodeGen.
			using CodeGenModule::addPropertyCodeGen;

		protected:
			/**
			*	@brief Generate code in the header file header for the given entity.
			*	
			*	@param entity		Entity to generate code for.
			*	@param env			Generation environment structure.
			*	@param inout_result	String the method should append the generated code to.
			*	
			*	@return The least prioritized ETraversalBehaviour value (ETraversalBehaviour::Break) to give the full control to
			*			any defined override version returning a more prioritized ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	generateHeaderFileHeaderCodeForEntity(EntityInfo const&	entity,
																			  MacroCodeGenEnv&	env,
																			  std::string&		inout_result)	noexcept;

			/**
			*	@brief	Generate code in the class footer for the given entity.
			*			This method can only be called with a struct, class, field or method entity.
			*	
			*	@param entity		Entity to generate code for.
			*	@param env			Generation environment structure.
			*	@param inout_result	String the method should append the generated code to.
			*	
			*	@return The least prioritized ETraversalBehaviour value (ETraversalBehaviour::Break) to give the full control to
			*			any defined override version returning a more prioritized ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	generateClassFooterCodeForEntity(EntityInfo const&	entity,
																		 MacroCodeGenEnv&	env,
																		 std::string&		inout_result)		noexcept;

			/**
			*	@brief Generate code in the header file footer for the given entity.
			*	
			*	@param entity		Entity to generate code for.
			*	@param env			Generation environment structure.
			*	@param inout_result	String the method should append the generated code to.
			*	
			*	@return The least prioritized ETraversalBehaviour value (ETraversalBehaviour::Break) to give the full control to
			*			any defined override version returning a more prioritized ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	generateHeaderFileFooterCodeForEntity(EntityInfo const&	entity,
																			  MacroCodeGenEnv&	env,
																			  std::string&		inout_result)	noexcept;

			/**
			*	@brief Generate code in the source file header for the given entity.
			*	
			*	@param entity		Entity to generate code for.
			*	@param env			Generation environment structure.
			*	@param inout_result	String the method should append the generated code to.
			*	
			*	@return The least prioritized ETraversalBehaviour value (ETraversalBehaviour::Break) to give the full control to
			*			any defined override version returning a more prioritized ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	generateSourceFileHeaderCodeForEntity(EntityInfo const&	entity,
																			  MacroCodeGenEnv&	env,
																			  std::string&		inout_result)	noexcept;

			/**
			*	@brief	Called just before calling generateHeaderFileHeaderCode, generateClassFooterCode, generateHeaderFileFooterCode,
			*			and generateSourceFileHeaderCode on a given entity.
			*			Can be used to initialize the environment or perform any pre-generation initialization.
			* 
			*	@param entity	Entity to generate code for.
			*	@param env		Generation environment structure.
			* 
			*	@return true. Returning false in an override implementation will abort the code generation process for the unit.
			*/
			virtual bool				preGenerateCodeForEntity(EntityInfo const&	entity,
																 MacroCodeGenEnv&	env)						noexcept;

			/**
			*	@brief	Called right after generateHeaderFileHeaderCode, generateClassFooterCode, generateHeaderFileFooterCode,
			*			and generateSourceFileHeaderCode have been called on a given entity.
			*			Can be used to modify the environment or perform any post-generation cleanup.
			* 
			*	@param entity	Entity to generate code for.
			*	@param env		Generation environment structure.
			* 
			*	@return true. Returning false in an override implementation will abort the code generation process for the unit.
			*/
			virtual bool				postGenerateCodeForEntity(EntityInfo const&	entity,
																  MacroCodeGenEnv&	env)						noexcept;

			/**
			*	@brief Add a property code generator to this generation module.
			* 
			*	@param propertyCodeGen PropertyRule to register.
			*/
			void						addPropertyCodeGen(MacroPropertyCodeGen& propertyCodeGen)				noexcept;

		public:
			/**
			*	@brief	Macro module needs to run at least 2 times to work properly since generated files / macros
			*			might not exist during the first parsing pass.
			* 
			*	@return 2.
			*/
			virtual uint8_t		getIterationCount()								const	noexcept override;

			/**
			*	@brief Generate code using the provided environment as input.
			* 
			*	@param entity			Entity the module is generating code for. Might be nullptr, in which case the code is not generated for a specific entity.
			*	@param env				Environment provided by the FileGenerationUnit. You can cast environment to a more concrete type if you know the type provided by the FileGenerationUnit.
			*	@param inout_result		String the method should append the generated code to.
			* 
			*	@return A combination of all the underlying calls returning a ETraversalBehaviour.
			*/
			virtual ETraversalBehaviour	generateCodeForEntity(EntityInfo const&	entity,
															  CodeGenEnv&		env,
															  std::string&		inout_result)	noexcept override final;

			/**
			*	@brief	Generate initial code for this code generator.
			*			This method analyzes the code location retrieved from the MacroCodeGenEnv
			*			and dispatch the call to the relevant method.
			*	
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool				initialGenerateCode(CodeGenEnv&		env,
															std::string&	inout_result)		noexcept override final;
			
			/**
			*	@brief	Generate final code for this code generator.
			*			This method analyzes the code location retrieved from the MacroCodeGenEnv
			*			and dispatch the call to the relevant method.
			*	
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool				finalGenerateCode(CodeGenEnv&	env,
														  std::string&	inout_result)			noexcept override final;
	};
}