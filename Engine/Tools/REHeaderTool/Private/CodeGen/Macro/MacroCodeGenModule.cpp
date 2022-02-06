#include "REHeaderTool/CodeGen/Macro/MacroCodeGenModule.h"

#include "REHeaderTool/InfoStructures/EntityInfo.h"
#include "REHeaderTool/CodeGen/CodeGenHelpers.h"
#include "REHeaderTool/CodeGen/Macro/MacroCodeGenEnv.h"
#include "REHeaderTool/CodeGen/Macro/MacroPropertyCodeGen.h"

using namespace REHeaderTool;

uint8_t MacroCodeGenModule::getIterationCount() const noexcept
{
	uint8_t highestPropertyCodeGenItCount = CodeGenModule::getIterationCount();

	//Max between highestPropertyCodeGenItCount and 2
	return (highestPropertyCodeGenItCount > 2u) ? highestPropertyCodeGenItCount : 2u;
}

ETraversalBehaviour MacroCodeGenModule::generateCodeForEntity(EntityInfo const& entity, CodeGenEnv& env, std::string& inout_result) noexcept
{
	MacroCodeGenEnv& macroEnv = static_cast<MacroCodeGenEnv&>(env);

	ETraversalBehaviour result;

	//Dispatch code generation call to the right sub-method
	switch (macroEnv.getCodeGenLocation())
	{
		case ECodeGenLocation::HeaderFileHeader:
			if (preGenerateCodeForEntity(entity, macroEnv))
			{
				return generateHeaderFileHeaderCodeForEntity(entity, macroEnv, inout_result);
			}
			else
			{
				return ETraversalBehaviour::AbortWithFailure;
			}

		case ECodeGenLocation::ClassFooter:
			return generateClassFooterCodeForEntity(entity, macroEnv, inout_result);

		case ECodeGenLocation::HeaderFileFooter:
			return generateHeaderFileFooterCodeForEntity(entity, macroEnv, inout_result);

		case ECodeGenLocation::SourceFileHeader:
			result = generateSourceFileHeaderCodeForEntity(entity, macroEnv, inout_result);

			return (postGenerateCodeForEntity(entity, macroEnv)) ? result : ETraversalBehaviour::AbortWithFailure;

		case ECodeGenLocation::Count:
			[[fallthrough]];
		default:
			//Should never get here
			if (env.getLogger() != nullptr)
			{
				env.getLogger()->log("MacroPropertyCodeGen::generateCode called with unhandled ECodeGenLocation. Abort generation.", ILogger::ELogSeverity::Error);
			}
			return ETraversalBehaviour::AbortWithFailure;
	}
}

bool MacroCodeGenModule::initialGenerateCode(CodeGenEnv& env, std::string& inout_result) noexcept
{
	return initialGenerateCodeImplementation(env, inout_result);
}

bool MacroCodeGenModule::finalGenerateCode(CodeGenEnv& env, std::string& inout_result) noexcept
{
	return finalGenerateCodeImplementation(env, inout_result);
}

ETraversalBehaviour MacroCodeGenModule::generateHeaderFileHeaderCodeForEntity(EntityInfo const& /* entity */, MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return CodeGenHelpers::leastPrioritizedTraversalBehaviour;
}

ETraversalBehaviour MacroCodeGenModule::generateClassFooterCodeForEntity(EntityInfo const& /* entity */, MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return CodeGenHelpers::leastPrioritizedTraversalBehaviour;
}

ETraversalBehaviour MacroCodeGenModule::generateHeaderFileFooterCodeForEntity(EntityInfo const& /* entity */, MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return CodeGenHelpers::leastPrioritizedTraversalBehaviour;
}

ETraversalBehaviour MacroCodeGenModule::generateSourceFileHeaderCodeForEntity(EntityInfo const& /* entity */, MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return CodeGenHelpers::leastPrioritizedTraversalBehaviour;
}

bool MacroCodeGenModule::preGenerateCodeForEntity(EntityInfo const& /* entity */, MacroCodeGenEnv& /* env */) noexcept
{
	//Default implementation does nothing
	return true;
}

bool MacroCodeGenModule::postGenerateCodeForEntity(EntityInfo const& /* entity */, MacroCodeGenEnv& /* env */) noexcept
{
	//Default implementation does nothing
	return true;
}

void MacroCodeGenModule::addPropertyCodeGen(MacroPropertyCodeGen& propertyCodeGen) noexcept
{
	CodeGenModule::addPropertyCodeGen(propertyCodeGen);
}