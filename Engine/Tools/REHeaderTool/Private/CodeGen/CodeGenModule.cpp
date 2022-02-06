#include "REHeaderTool/CodeGen/CodeGenModule.h"

#include <algorithm>

#include "REHeaderTool/CodeGen/PropertyCodeGen.h"
#include "REHeaderTool/CodeGen/CodeGenEnv.h"

using namespace REHeaderTool;

int32_t CodeGenModule::getGenerationOrder() const noexcept
{
	auto it = std::max_element(_propertyCodeGenerators.cbegin(), _propertyCodeGenerators.cend(),
							   [](PropertyCodeGen* const& lhs, PropertyCodeGen* const& rhs)
							   {
								   return lhs->getIterationCount() < rhs->getIterationCount();
							   });

	return (*it)->getIterationCount();
}

ETraversalBehaviour CodeGenModule::generateCodeForEntity(EntityInfo const& entity, CodeGenEnv& env, std::string& inout_result, void const* /* data */) noexcept
{
	return generateCodeForEntity(entity, env, inout_result);
}

void CodeGenModule::addPropertyCodeGen(PropertyCodeGen& propertyCodeGen) noexcept
{
	_propertyCodeGenerators.push_back(&propertyCodeGen);
}

bool CodeGenModule::removePropertyCodeGen(PropertyCodeGen const& propertyCodeGen) noexcept
{
	auto it = std::find(_propertyCodeGenerators.cbegin(), _propertyCodeGenerators.cend(), &propertyCodeGen);

	if (it != _propertyCodeGenerators.cend())
	{
		_propertyCodeGenerators.erase(it);

		return true;
	}

	return false;
}

ETraversalBehaviour CodeGenModule::callVisitorOnEntity(EntityInfo const& entity, CodeGenEnv& env, std::function<ETraversalBehaviour(ICodeGenerator&, EntityInfo const&, CodeGenEnv&, void const*)> visitor) noexcept
{
	assert(visitor != nullptr);

	return visitor(*this, entity, env, nullptr);
}

std::vector<PropertyCodeGen*> const& CodeGenModule::getPropertyCodeGenerators() const noexcept
{
	return _propertyCodeGenerators;
}