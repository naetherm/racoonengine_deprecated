#include "REHeaderTool/CodeGen/Macro/MacroPropertyCodeGen.h"

#include <cassert>

using namespace REHeaderTool;

bool MacroPropertyCodeGen::generateCodeForEntity(EntityInfo const& entity, Property const& property, uint8_t propertyIndex, CodeGenEnv& env, std::string& inout_result) noexcept
{
	MacroCodeGenEnv& macroData = static_cast<MacroCodeGenEnv&>(env);

	//Dispatch code generation call to the right sub-method
	switch (macroData.getCodeGenLocation())
	{
		case ECodeGenLocation::HeaderFileHeader:
			return	preGenerateCodeForEntity(entity, property, propertyIndex, macroData) &&
					generateHeaderFileHeaderCodeForEntity(entity, property, propertyIndex, macroData, inout_result);

		case ECodeGenLocation::ClassFooter:
			return generateClassFooterCodeForEntity(entity, property, propertyIndex, macroData, inout_result);

		case ECodeGenLocation::HeaderFileFooter:
			return generateHeaderFileFooterCodeForEntity(entity, property, propertyIndex, macroData, inout_result);

		case ECodeGenLocation::SourceFileHeader:
			return	generateSourceFileHeaderCodeForEntity(entity, property, propertyIndex, macroData, inout_result) &&
					postGenerateCodeForEntity(entity, property, propertyIndex, macroData);

		case ECodeGenLocation::Count:
			[[fallthrough]];
		default:
			//Should never get here
			if (env.getLogger() != nullptr)
			{
				env.getLogger()->log("MacroPropertyCodeGen::generateCode called with unhandled ECodeGenLocation. Abort generation.", ILogger::ELogSeverity::Error);
			}
			return false;
	}
}

bool MacroPropertyCodeGen::initialGenerateCode(CodeGenEnv& env, std::string& inout_result) noexcept
{
	return initialGenerateCodeImplementation(env, inout_result);
}

bool MacroPropertyCodeGen::finalGenerateCode(CodeGenEnv& env, std::string& inout_result) noexcept
{
	return finalGenerateCodeImplementation(env, inout_result);
}

bool MacroPropertyCodeGen::generateHeaderFileHeaderCodeForEntity(EntityInfo const& /* entity */, Property const& /* property */, uint8_t /* propertyIndex */,
																 MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroPropertyCodeGen::generateClassFooterCodeForEntity(EntityInfo const& /* entity */, Property const& /* property */, uint8_t /* propertyIndex */,
															MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroPropertyCodeGen::generateHeaderFileFooterCodeForEntity(EntityInfo const& /* entity */, Property const& /* property */, uint8_t /* propertyIndex */,
																 MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroPropertyCodeGen::generateSourceFileHeaderCodeForEntity(EntityInfo const& /* entity */, Property const& /* property */, uint8_t /* propertyIndex */,
																 MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroPropertyCodeGen::preGenerateCodeForEntity(EntityInfo const& /* entity */, Property const& /* property */,
													uint8_t /* propertyIndex */, MacroCodeGenEnv& /* env */) noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroPropertyCodeGen::postGenerateCodeForEntity(EntityInfo const& /* entity */, Property const& /* property */,
													 uint8_t /* propertyIndex */, MacroCodeGenEnv& /* env */) noexcept
{
	//Default implementation generates no code
	return true;
}