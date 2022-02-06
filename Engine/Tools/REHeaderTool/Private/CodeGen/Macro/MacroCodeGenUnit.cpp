#include "REHeaderTool/CodeGen/Macro/MacroCodeGenUnit.h"

//#include "REHeaderTool/Config.h"
#include "REHeaderTool/CodeGen/GeneratedFile.h"
#include "REHeaderTool/CodeGen/CodeGenHelpers.h"
#include "REHeaderTool/CodeGen/Macro/MacroCodeGenUnitSettings.h"
#include "REHeaderTool/CodeGen/Macro/MacroCodeGenModule.h"

using namespace REHeaderTool;

std::array<std::string, static_cast<size_t>(ECodeGenLocation::Count)> const MacroCodeGenUnit::_separators =
{
	"\n",	//HeaderFileHeader is not wrapped inside a macro, so can use \n without breaking the code
	"\\\n",	//ClassFooter is wrapped in a macro so must use \ to keep multiline generated code valid
	"\\\n",	//HeaderFileFooter is wrapped in a macro so must use \ to keep multiline generated code valid
	"\n"	//SourceFileHeader is not wrapped in a macro, so can use \n without breaking the code
};

MacroCodeGenEnv* MacroCodeGenUnit::createCodeGenEnv() const noexcept
{
	return new MacroCodeGenEnv();
}

void MacroCodeGenUnit::initialGenerateCode(CodeGenEnv& env, std::function<void(CodeGenEnv&, std::string&)> generate) noexcept
{
	MacroCodeGenEnv& macroEnv = static_cast<MacroCodeGenEnv&>(env);

	//Generate code for each code location
	for (int i = 0u; i < static_cast<int>(ECodeGenLocation::Count); i++)
	{
		macroEnv._codeGenLocation	= static_cast<ECodeGenLocation>(i);
		macroEnv._separator			= _separators[i];

		/**
		*	No initial call when the CodeGenLocation is ClassFooter
		*/
		if (macroEnv._codeGenLocation == ECodeGenLocation::ClassFooter)
		{
			continue;
		}
		else
		{
			generate(macroEnv, _generatedCodePerLocation[i]);
		}
	}
}

void MacroCodeGenUnit::finalGenerateCode(CodeGenEnv& env, std::function<void(CodeGenEnv&, std::string&)> generate) noexcept
{
	//Exactly same flow as initialGenerateCode
	initialGenerateCode(env, generate);
}

void MacroCodeGenUnit::generateCodeForEntity(EntityInfo const& entity, CodeGenEnv& env, std::function<void(EntityInfo const&, CodeGenEnv&, std::string&)> generate)	noexcept
{
	MacroCodeGenEnv& macroEnv = static_cast<MacroCodeGenEnv&>(env);

	//Generate code for each code location
	for (int i = 0u; i < static_cast<int>(ECodeGenLocation::Count); i++)
	{
		macroEnv._codeGenLocation	= static_cast<ECodeGenLocation>(i);
		macroEnv._separator			= _separators[i];

		/**
		*	Forward ECodeGenLocation::ClassFooter generation only if the entity is a
		*	struct, class, method or field
		*/
		if (macroEnv._codeGenLocation == ECodeGenLocation::ClassFooter)
		{
			if (!(entity.entityType == EEntityType::Struct || entity.entityType == EEntityType::Class ||
				entity.entityType == EEntityType::Method || entity.entityType == EEntityType::Field))
			{
				continue;
			}
			else
			{
				generateEntityClassFooterCode(entity, macroEnv, generate);
			}
		}
		else
		{
			generate(entity, macroEnv, _generatedCodePerLocation[i]);
		}
	}
}

bool MacroCodeGenUnit::preGenerateCode(FileParsingResult const& parsingResult, CodeGenEnv& env) noexcept
{
	if (CodeGenUnit::preGenerateCode(parsingResult, env))
	{
		MacroCodeGenEnv& macroEnv = static_cast<MacroCodeGenEnv&>(env);
		macroEnv._exportSymbolMacro = getSettings()->getExportSymbolMacroName();
		macroEnv._internalSymbolMacro = getSettings()->getInternalSymbolMacroName();

		//Reset variables before the generation step begins
		_classFooterGeneratedCode.clear();

		for (std::string& generatedCode : _generatedCodePerLocation)
		{
			generatedCode.clear();
		}

		return true;
	}

	return false;
}

bool MacroCodeGenUnit::postGenerateCode(CodeGenEnv& env) noexcept
{
	//Create generated header & generated source files
	generateHeaderFile(static_cast<MacroCodeGenEnv&>(env));
	generateSourceFile(static_cast<MacroCodeGenEnv&>(env));

	return true;
}

void MacroCodeGenUnit::generateHeaderFile(MacroCodeGenEnv& env) noexcept
{
	GeneratedFile generatedHeader(getGeneratedHeaderFilePath(env.getFileParsingResult()->parsedFile), env.getFileParsingResult()->parsedFile);

	MacroCodeGenUnitSettings const* castSettings = getSettings();

	generatedHeader.writeLine("#pragma once\n");

	//Include the entity file
	generatedHeader.writeLine("#include \"" + CodeGenUnitSettings::entityMacrosFilename.string() + "\"\n");

	//Write header file header code
	generatedHeader.writeLine(std::move(_generatedCodePerLocation[static_cast<int>(ECodeGenLocation::HeaderFileHeader)]));

	//Write all struct/class footer macros
	//We must iterate over all structs/class from scratch since registered generators are not guaranteed to traverse all struct/class
	env.getFileParsingResult()->foreachEntityOfType(EEntityType::Class | EEntityType::Struct,
													[this, &generatedHeader, castSettings](EntityInfo const& entity)
													{
														//Cast is safe since we only iterate on structs & classes
														StructClassInfo const* struct_ = reinterpret_cast<StructClassInfo const*>(&entity);

														if (!struct_->isForwardDeclaration)
														{
															auto it = _classFooterGeneratedCode.find(struct_);

															generatedHeader.writeMacro(castSettings->getClassFooterMacro(*struct_), (it != _classFooterGeneratedCode.end()) ? std::move(it->second) : std::string());
														}
													});

	//Write header file footer code
	generatedHeader.writeMacro(castSettings->getHeaderFileFooterMacro(env.getFileParsingResult()->parsedFile),
							   std::move(_generatedCodePerLocation[static_cast<int>(ECodeGenLocation::HeaderFileFooter)]));
}

void MacroCodeGenUnit::generateSourceFile(MacroCodeGenEnv& env) noexcept
{
	GeneratedFile generatedFile(getGeneratedSourceFilePath(env.getFileParsingResult()->parsedFile), env.getFileParsingResult()->parsedFile);

	generatedFile.writeLine("#pragma once\n");

	//Include the header file
	generatedFile.writeLine("#include \"" + FilesystemHelpers::normalizeSeparator(generatedFile.getSourceFilePath().lexically_relative(generatedFile.getPath().parent_path())).string() + "\"\n");

	generatedFile.writeLine(std::move(_generatedCodePerLocation[static_cast<int>(ECodeGenLocation::SourceFileHeader)]));
}

bool MacroCodeGenUnit::isUpToDate(fs::path const& sourceFile) const noexcept
{
	fs::path generatedHeaderPath = getGeneratedHeaderFilePath(sourceFile);

	//If the generated header doesn't exist, create it and return false
	if (!fs::exists(generatedHeaderPath))
	{
		GeneratedFile generatedHeader(fs::path(generatedHeaderPath), sourceFile);
	}
	else if (isFileNewerThan(generatedHeaderPath, sourceFile))
	{
		fs::path generatedSource = getGeneratedSourceFilePath(sourceFile);

		return fs::exists(generatedSource) && isFileNewerThan(generatedSource, sourceFile);
	}

	return false;
}

void MacroCodeGenUnit::generateEntityClassFooterCode(EntityInfo const& entity, CodeGenEnv& env, std::function<void(EntityInfo const&, CodeGenEnv&, std::string&)> generate) noexcept
{
	if (entity.entityType == EEntityType::Struct || entity.entityType == EEntityType::Class)
	{
		//If the entity is a struct/class, append to the footer of the struct/class
		generate(entity, env, _classFooterGeneratedCode[&reinterpret_cast<StructClassInfo const&>(entity)]);
	}
	else
	{
		assert(entity.outerEntity != nullptr);
		assert(entity.outerEntity->entityType == EEntityType::Struct || entity.outerEntity->entityType == EEntityType::Class);

		//If the entity is NOT a struct/class, append to the footer of the outer struct/class
		generate(entity, env, _classFooterGeneratedCode[reinterpret_cast<StructClassInfo const*>(entity.outerEntity)]);
	}
}

fs::path MacroCodeGenUnit::getGeneratedHeaderFilePath(fs::path const& sourceFile) const noexcept
{
	return settings->getOutputDirectory() / getSettings()->getGeneratedHeaderFileName(sourceFile);
}

fs::path MacroCodeGenUnit::getGeneratedSourceFilePath(fs::path const& sourceFile) const noexcept
{
	return settings->getOutputDirectory() / getSettings()->getGeneratedSourceFileName(sourceFile);
}

void MacroCodeGenUnit::addModule(MacroCodeGenModule& generationModule) noexcept
{
	CodeGenUnit::addModule(generationModule);
}

MacroCodeGenUnitSettings const* MacroCodeGenUnit::getSettings() const noexcept
{
	return reinterpret_cast<MacroCodeGenUnitSettings const*>(settings);
}

void MacroCodeGenUnit::setSettings(MacroCodeGenUnitSettings const& cguSettings) noexcept
{
	settings = &cguSettings;
}