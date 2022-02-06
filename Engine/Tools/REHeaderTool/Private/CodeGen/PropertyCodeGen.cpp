#include "REHeaderTool/CodeGen/PropertyCodeGen.h"

using namespace REHeaderTool;

PropertyCodeGen::PropertyCodeGen(std::string const&	propertyName, EEntityType eligibleEntityMask) noexcept:
	_propertyName{propertyName},
	_eligibleEntityMask{eligibleEntityMask}
{
}

ETraversalBehaviour PropertyCodeGen::generateCodeForEntity(EntityInfo const& entity, CodeGenEnv& env, std::string& inout_result, void const* data) noexcept
{
	AdditionalData const* additionalData = reinterpret_cast<AdditionalData const*>(data);

	return generateCodeForEntity(entity, *additionalData->property, additionalData->propertyIndex, env, inout_result) ? ETraversalBehaviour::Recurse : ETraversalBehaviour::AbortWithFailure;
}

bool PropertyCodeGen::shouldGenerateCodeForEntity(EntityInfo const& entity, Property const& property, uint8_t /* propertyIndex */) const noexcept
{
	return property.name == _propertyName && (entity.entityType && _eligibleEntityMask);
}

bool PropertyCodeGen::shouldIterateOnNestedEntities(EntityInfo const& entity) const noexcept
{
	switch (entity.entityType)
	{
		case EEntityType::Namespace:
			return NamespaceInfo::nestedEntityTypes && _eligibleEntityMask;

		case EEntityType::Struct:
			[[fallthrough]];
		case EEntityType::Class:
			return StructClassInfo::nestedEntityTypes && _eligibleEntityMask;

		case EEntityType::Enum:
			return EnumInfo::nestedEntityTypes && _eligibleEntityMask;

		default:
			//EnumValue
			//Functions
			//Methods
			//Fields
			//Variables
			return false;
	}
}

ETraversalBehaviour PropertyCodeGen::callVisitorOnEntity(EntityInfo const& entity, CodeGenEnv& env, std::function<ETraversalBehaviour(ICodeGenerator&, EntityInfo const&, CodeGenEnv&, void const*)> visitor) noexcept
{
	assert(visitor != nullptr);

	//Call the visitor if the entity type is contained in the _eligibleEntities mask
	if (_eligibleEntityMask && entity.entityType)
	{
		AdditionalData data;

		//Execute the visitor on each property contained in the entity
		for (uint8_t i = 0; i < entity.properties.size(); i++)
		{
			data.propertyIndex = i;
			data.property = &entity.properties[i];

			if (shouldGenerateCodeForEntity(entity, *data.property, data.propertyIndex))
			{
				if (visitor(*this, entity, env, &data) == ETraversalBehaviour::AbortWithFailure)
				{
					return ETraversalBehaviour::AbortWithFailure;
				}
			}
		}
	}

	return shouldIterateOnNestedEntities(entity) ? ETraversalBehaviour::Recurse : ETraversalBehaviour::Continue;
}