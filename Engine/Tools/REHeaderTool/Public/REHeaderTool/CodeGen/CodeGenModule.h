/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <vector>

#include "REHeaderTool/Misc/ICloneable.h"
#include "REHeaderTool/CodeGen/ICodeGenerator.h"
#include "REHeaderTool/CodeGen/ETraversalBehaviour.h"

namespace REHeaderTool
{
	//Forward declaration
	class	PropertyCodeGen;
	class	CodeGenEnv;
	class	EntityInfo;

	class CodeGenModule : public ICloneable, public ICodeGenerator
	{
		private:
			/** Collection of all property code generators attached to this module. */
			std::vector<PropertyCodeGen*> _propertyCodeGenerators;

			/**
			*	@brief	Call the visitor method with the provided entity/env pair.
			*			The forwarded data is always nullptr.
			* 
			*	@param entity	The entity provided to the visitor.
			*	@param env		The environment provided to the visitor.
			*	@param visitor	The visitor to run.
			* 
			*	@return	The value returned from the visitor call.
			*/
			virtual ETraversalBehaviour	callVisitorOnEntity(EntityInfo const&	entity,
															CodeGenEnv&			env,
															std::function<ETraversalBehaviour(ICodeGenerator&,
																							  EntityInfo const&,
																							  CodeGenEnv&,
																							  void const*)>		visitor)	noexcept final override;

			/**
			*	@brief	Generate code for the provided entity/environment pair.
			*			Internally call the PropertyCodeGen::generateCode public method.
			* 
			*	@param entity		The entity this generator should generate code for.
			*	@param env			The generation environment structure.
			*	@param inout_result	String the generated code should be appended to.
			*	@param data			nullptr.
			* 
			*	@return A ETraversalBehaviour defining how the CodeGenUnit should pick the next entity.
			*/
			virtual ETraversalBehaviour	generateCodeForEntity(EntityInfo const&	entity, 
															  CodeGenEnv&		env,
															  std::string&		inout_result,
															  void const*		data)										noexcept final override;

		protected:
			/**
			*	@brief Add a property code generator to this generation module.
			* 
			*	@param propertyCodeGen PropertyCodeGen to register.
			*/
			void	addPropertyCodeGen(PropertyCodeGen& propertyCodeGen)			noexcept;

			/**
			*	@brief Remove a property code generator from this generation module.
			* 
			*	@param propertyCodeGen PropertyCodeGen to unregister.
			*/
			bool	removePropertyCodeGen(PropertyCodeGen const& propertyCodeGen)	noexcept;

		public:
			/**
			*	@brief	Generate code using the provided environment as input.
			* 
			*	@param entity			Entity the module is generating code for.
			*	@param env				Data provided by the FileGenerationUnit. You can cast env to a more concrete type if you know the type provided by the FileGenerationUnit.
			*	@param inout_result		String the method should append the generated code to.
			* 
			*	@return true if the code generation completed successfully, else false. If false is returned
			*/
			virtual ETraversalBehaviour				generateCodeForEntity(EntityInfo const&	entity,
																		  CodeGenEnv&		env,
																		  std::string&		inout_result)	noexcept = 0;

			/**
			*	@return The highest iteration count in all registered property code generators.
			*/
			virtual int32_t							getGenerationOrder()							const	noexcept override;

			/**
			*	@brief Getter for _propertyCodeGenerators field.
			*
			*	@return _propertyCodeGenerators.
			*/
			std::vector<PropertyCodeGen*> const&	getPropertyCodeGenerators()						const	noexcept;
	};
}