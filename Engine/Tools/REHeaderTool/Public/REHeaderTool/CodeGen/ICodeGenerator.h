/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <functional>	//std::function

#include "REHeaderTool/CodeGen/ETraversalBehaviour.h"

namespace REHeaderTool
{
	//Forward declaration
	class EntityInfo;
	class CodeGenEnv;

	class ICodeGenerator
	{
		public:
			ICodeGenerator()						= default;
			ICodeGenerator(ICodeGenerator const&)	= default;
			ICodeGenerator(ICodeGenerator&&)		= default;
			virtual ~ICodeGenerator()				= default;

			/**
			*	@brief	Call the visitor function one or multiple times on this code generator with the provided entity and environment.
			*			The visitor later calls ICodeGenerator::generateCode, and it is possible to forward custom data to it.
			* 
			*	@param entity	The entity provided to the visitor.
			*	@param env		The environment provided to the visitor.
			*	@param visitor	The visitor to run.
			* 
			*	@return A ETraversalBehaviour defining how the CodeGenUnit should pick the next entity.
			*/
			virtual ETraversalBehaviour	callVisitorOnEntity(EntityInfo const&									entity,
															CodeGenEnv&											env,
															std::function<ETraversalBehaviour(ICodeGenerator&,
																							  EntityInfo const&,
																							  CodeGenEnv&,
																							  void const*)>		visitor)	noexcept = 0;

			/**
			*	@brief	Generate code for the provided entity/environment pair.
			* 
			*	@param entity		The entity this generator should generate code for.
			*	@param env			The generation environment structure.
			*	@param inout_result	String the generated code should be appended to.
			*	@param data			Additional data forwarded from ICodeGenerator::callVisitorOnEntity.
			* 
			*	@return A ETraversalBehaviour defining how the CodeGenUnit should pick the next entity.
			*/
			virtual ETraversalBehaviour generateCodeForEntity(EntityInfo const&	entity,
															  CodeGenEnv&		env,
															  std::string&		inout_result,
															  void const*		data)										noexcept = 0;

			/**
			*	@brief	Initial code generation method called before the first generateCodePerEntity call.
			*			It is called as many times as the generate method is called in the CodeGenUnit::generateCodeForEntity method.
			*			This method can also be used to initialize some pre-generation data in the ICodeGenerator.
			* 
			*	@param env			The generation environment structure.
			*	@param inout_result	String the generated code should be appended to.
			* 
			*	@return true if the initialization and initial code generation was successful, else false.
			*			If the method returns false, the code generation is aborted for the running CodeGenUnit.
			*/
			virtual bool				initialGenerateCode(CodeGenEnv&		env,
															std::string&	inout_result)									noexcept = 0;

			/**
			*	@brief	Final code generation method called after the last generateCodePerEntity call.
			*			It is called as many times as the generate method is called in the CodeGenUnit::generateCodeForEntity method.
			*			This method can also be used to deinitialize some post-generation data in the ICodeGenerator.
			* 
			*	@param env			The generation environment structure.
			*	@param inout_result	String the generated code should be appended to.
			* 
			*	@return true if the initialization and initial code generation was successful, else false.
			*			If the method returns false, the code generation is aborted for the running CodeGenUnit.
			*/
			virtual bool				finalGenerateCode(CodeGenEnv&	env,
														  std::string&	inout_result)										noexcept = 0;

			/**
			*	@brief	The generation order is a number defining in which order a code generator will generate code compared to other code generators.
			*			Code generators with a low generation order will execute first, and code generators with a high generation order will execute last.
			*			Code generators having the same generation order will execute in an undefined order.
			*			Default generation order is 0.
			* 
			*	@return The generation order.
			*/
			virtual int32_t				getGenerationOrder()														const	noexcept;

			/**
			*	@brief	The iteration count is the number of consecutive generations required for this generator to work properly.
			*			In most cases the default value of 1 is perfectly fine, but sometimes generated code can be parsed again
			*			to generate new code.
			* 
			*	@return The iteration count.
			*/
			virtual uint8_t				getIterationCount()															const	noexcept;

			ICodeGenerator& operator=(ICodeGenerator const&)	= default;
			ICodeGenerator& operator=(ICodeGenerator&&)			= default;
	};
}