/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>

#include "REHeaderTool/CodeGen/CodeGenEnv.h"
#include "REHeaderTool/CodeGen/ICodeGenerator.h"
#include "REHeaderTool/InfoStructures/EntityInfo.h"

namespace REHeaderTool
{
	class PropertyCodeGen : public ICodeGenerator
	{
		private:
			struct AdditionalData
			{
				uint8_t			propertyIndex;
				Property const* property;
			};

			/** Name of the property this property generator should generator code for. */
			std::string	_propertyName;

			/** Mask defining the type of entities this generator can run on. */
			EEntityType	_eligibleEntityMask = EEntityType::Undefined;

			/**
			*	@brief	Call the visitor method once for each entity/property pair.
			*			The forwarded data is a PropertyCodeGen::AdditionalData const*.
			* 
			*	@param entity	The entity provided to the visitor.
			*	@param env		The environment provided to the visitor.
			*	@param visitor	The visitor to run.
			* 
			*	@return	AbortWithFailure if any of the visitor calls returned AbortWithFailure;
			*			Recurse if the entity can contain entities overlapping with the _eligibleEntityMask;
			*			Continue if the entity doesn't contain any entities overlapping with the _eligibleEntityMask;
			*/
			virtual ETraversalBehaviour	callVisitorOnEntity(EntityInfo const&									entity,
															CodeGenEnv&											env,
															std::function<ETraversalBehaviour(ICodeGenerator&,
																							  EntityInfo const&,
																							  CodeGenEnv&,
																							  void const*)>		visitor)	noexcept final override;

			/**
			*	@brief	Generate code for the provided entity/environment pair.
			*			Internally call the PropertyCodeGen::generateCode public method by unwrapping the data content.
			* 
			*	@param entity		The entity this generator should generate code for.
			*	@param env			The generation environment structure.
			*	@param inout_result	String the generated code should be appended to.
			*	@param data			PropertyCodeGen::AdditionalData const* forwarded from PropertyCodeGen::callVisitorOnEntity.
			* 
			*	@return A ETraversalBehaviour defining how the CodeGenUnit should pick the next entity.
			*/
			virtual ETraversalBehaviour	generateCodeForEntity(EntityInfo const&	entity, 
															  CodeGenEnv&		env,
															  std::string&		inout_result,
															  void const*		data)										noexcept final override;

			/**
			*	@brief	Determine whether this PropertyCodeGen should recurse on the provided entity children or not.
			* 
			*	@param entity The entity to check.
			* 
			*	@return true if the generator should run on the entity's children, else false.
			*/
			bool						shouldIterateOnNestedEntities(EntityInfo const& entity)						const	noexcept;

		public:
			/**
			*	@param propertyName			Name of the property this property generator should generate code for.
			*	@param eligibleEntityMask	A mask defining all the types of entity this PropertyCodeGen instance should run on.
			*/
			PropertyCodeGen(std::string const&	propertyName,
							EEntityType			eligibleEntityMask)	noexcept;

			/**
			*	@brief Generate code for a given entity.
			*	
			*	@param entity			Entity to generate code for.
			*	@param property			Property that triggered the property generation.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool				generateCodeForEntity(EntityInfo const&	entity,
															  Property const&	property,
															  uint8_t				propertyIndex,
															  CodeGenEnv&		env,
															  std::string&		inout_result)					noexcept = 0;

			/**
			*	@brief Check if this property should generate code for the provided entity/property pair.
			*
			*	@param entity			Checked entity.
			*	@param property			Checked property.
			*	@param propertyIndex	Index of the property in the entity's propertyGroup.
			*
			*	@return true if this property should generate code for the provided entity, else false.
			*/
			virtual bool				shouldGenerateCodeForEntity(EntityInfo const&	entity,
																	Property const&		property,
																	uint8_t				propertyIndex)	const	noexcept;

			/**
			*	@brief Getter for _eligibleEntityMask field.
			* 
			*	@return _eligibleEntityMask.
			*/
			inline EEntityType			getEligibleEntityMask()											const	noexcept;

			/**
			*	@brief Getter for _propertyName field.
			* 
			*	@return _propertyName.
			*/
			inline std::string const&	getPropertyName()												const	noexcept;
	};

	#include "REHeaderTool/CodeGen/PropertyCodeGen.inl"
}