/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include "REHeaderTool/CodeGen/PropertyCodeGen.h"
#include "REHeaderTool/CodeGen/Macro/MacroCodeGenEnv.h"
#include "REHeaderTool/CodeGen/Macro/MacroCodeGenerator.h"

namespace REHeaderTool
{
	class MacroPropertyCodeGen : public PropertyCodeGen, public MacroCodeGenerator
	{
		protected:
			/**
			*	@brief Generate code in the header file header for the given entity.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateHeaderFileHeaderCodeForEntity(EntityInfo const&	entity,
																  Property const&	property,
																  uint8_t				propertyIndex,
																  MacroCodeGenEnv&	env,
																  std::string&		inout_result)	noexcept;

			/**
			*	@brief	Generate code in the class footer for the given entity.
			*			This method can only be called with a struct, class, field or method entity.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateClassFooterCodeForEntity(EntityInfo const&	entity,
															 Property const&	property,
															 uint8_t				propertyIndex,
															 MacroCodeGenEnv&	env,
															 std::string&		inout_result)		noexcept;

			/**
			*	@brief Generate code in the header file footer for the given entity.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateHeaderFileFooterCodeForEntity(EntityInfo const&	entity,
																  Property const&	property,
																  uint8_t				propertyIndex,
																  MacroCodeGenEnv&	env,
																  std::string&		inout_result)	noexcept;

			/**
			*	@brief Generate code in the source file header for the given entity.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateSourceFileHeaderCodeForEntity(EntityInfo const&	entity,
																  Property const&	property,
																  uint8_t				propertyIndex,
																  MacroCodeGenEnv&	env,
																  std::string&		inout_result)	noexcept;

			/**
			*	@brief	Called just before calling generateHeaderFileHeaderCode, generateClassFooterCode, generateHeaderFileFooterCode,
			*			and generateSourceFileHeaderCode on a given entity/property pair.
			*			Can be used to initialize the data structure or perform any pre generation initialization or validity check.
			* 
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			* 
			*	@return true if the pre-generation completed successfully, else false.
			*/
			virtual bool	preGenerateCodeForEntity(EntityInfo const&	entity,
													 Property const&	property,
													 uint8_t				propertyIndex,
													 MacroCodeGenEnv&	env)						noexcept;

			/**
			*	@brief	Called right after generateHeaderFileHeaderCode, generateClassFooterCode, generateHeaderFileFooterCode,
			*			and generateSourceFileHeaderCode have been called on a given entity/property pair.
			*			Can be used to modify the data structure or perform any post generation cleanup.
			* 
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			* 
			*	@return true if the post-generation completed successfully, else false.
			*/
			virtual bool	postGenerateCodeForEntity(EntityInfo const&	entity,
													  Property const&	property,
													  uint8_t				propertyIndex,
													  MacroCodeGenEnv&	env)						noexcept;

		public:
			//Use the same constructor as PropertyCodeGen
			using PropertyCodeGen::PropertyCodeGen;

			/**
			*	@brief	Generate code for a given entity.
			*			This method analyzes the code location retrieved from the MacroCodeGenEnv
			*			and dispatch the call to the relevant method.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	generateCodeForEntity(EntityInfo const&	entity,
												  Property const&	property,
												  uint8_t				propertyIndex,
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
			virtual bool	initialGenerateCode(CodeGenEnv&		env,
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
			virtual bool	finalGenerateCode(CodeGenEnv&	env,
											  std::string&	inout_result)			noexcept override final;
	};
}