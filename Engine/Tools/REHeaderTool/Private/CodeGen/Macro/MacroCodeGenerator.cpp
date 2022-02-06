#include "REHeaderTool/CodeGen/Macro/MacroCodeGenerator.h"

using namespace REHeaderTool;

bool MacroCodeGenerator::initialGenerateCodeImplementation(CodeGenEnv& env, std::string& inout_result) noexcept
{
	//Check that the provided environment is castable to MacroCodeGenEnv
#ifdef RTTI_ENABLED
	if (dynamic_cast<MacroCodeGenEnv*>(&env) == nullptr)
	{
		env.getLogger()->log("MacroCodeGenModule can't be used with non MacroCodeGenEnv environments.", ILogger::ELogSeverity::Error);
		return false;
	}
#else
	//Can't perform the cast check
#endif

	MacroCodeGenEnv& macroEnv = static_cast<MacroCodeGenEnv&>(env);

	//Dispatch code generation call to the right sub-method
	switch (macroEnv.getCodeGenLocation())
	{
		case ECodeGenLocation::HeaderFileHeader:
			return initialGenerateHeaderFileHeaderCode(macroEnv, inout_result);

		case ECodeGenLocation::HeaderFileFooter:
			return initialGenerateHeaderFileFooterCode(macroEnv, inout_result);

		case ECodeGenLocation::SourceFileHeader:
			return initialGenerateSourceFileHeaderCode(macroEnv, inout_result);

		case ECodeGenLocation::ClassFooter:
			[[fallthrough]];
		case ECodeGenLocation::Count:
			[[fallthrough]];
		default:
			//Should never get here
			if (env.getLogger() != nullptr)
			{
				env.getLogger()->log("MacroPropertyCodeGen::generateCode called with ECodeGenLocation::Count location. Abort generation.", ILogger::ELogSeverity::Error);
			}
			return false;
	}
}

bool MacroCodeGenerator::finalGenerateCodeImplementation(CodeGenEnv& env, std::string& inout_result) noexcept
{
	MacroCodeGenEnv& macroEnv = static_cast<MacroCodeGenEnv&>(env);

	//Dispatch code generation call to the right sub-method
	switch (macroEnv.getCodeGenLocation())
	{
		case ECodeGenLocation::HeaderFileHeader:
			return finalGenerateHeaderFileHeaderCode(macroEnv, inout_result);

		case ECodeGenLocation::HeaderFileFooter:
			return finalGenerateHeaderFileFooterCode(macroEnv, inout_result);

		case ECodeGenLocation::SourceFileHeader:
			return finalGenerateSourceFileHeaderCode(macroEnv, inout_result);

		case ECodeGenLocation::ClassFooter:
			[[fallthrough]];
		case ECodeGenLocation::Count:
			[[fallthrough]];
		default:
			//Should never get here
			if (env.getLogger() != nullptr)
			{
				env.getLogger()->log("MacroPropertyCodeGen::generateCode called with ECodeGenLocation::Count location. Abort generation.", ILogger::ELogSeverity::Error);
			}
			return false;
	}
}

bool MacroCodeGenerator::initialGenerateHeaderFileHeaderCode(MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroCodeGenerator::initialGenerateHeaderFileFooterCode(MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroCodeGenerator::initialGenerateSourceFileHeaderCode(MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroCodeGenerator::finalGenerateHeaderFileHeaderCode(MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroCodeGenerator::finalGenerateHeaderFileFooterCode(MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return true;
}

bool MacroCodeGenerator::finalGenerateSourceFileHeaderCode(MacroCodeGenEnv& /* env */, std::string& /* inout_result */) noexcept
{
	//Default implementation generates no code
	return true;
}