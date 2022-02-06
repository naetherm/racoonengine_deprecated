#include "REHeaderTool/CodeGen/CodeGenManager.h"

#include "REHeaderTool/CodeGen/GeneratedFile.h"
#include "REHeaderTool/Parsing/ParsingSettings.h"	//ParsingSettings::parsingMacro

using namespace REHeaderTool;

CodeGenManager::CodeGenManager(uint32_t threadCount) noexcept:
	_threadPool(getThreadCount(threadCount), ETerminationMode::FinishAll)
{
}

std::set<fs::path> CodeGenManager::identifyFilesToProcess(CodeGenUnit const& codeGenUnit, CodeGenResult& out_genResult, bool forceRegenerateAll) noexcept
{
	std::set<fs::path> result;

	//Iterate over all "toParseFiles"
	for (fs::path path : settings.getToProcessFiles())
	{
		if (fs::exists(path) && !fs::is_directory(path))
		{
			if (!codeGenUnit.isUpToDate(path) || forceRegenerateAll)
			{
				result.emplace(path);
			}
			else
			{
				out_genResult.upToDateFiles.push_back(path);
			}
		}
		else if (logger != nullptr)
		{
			//Add FileGenerationFile invalid path
			logger->log("File " + path.string() + " doesn't exist or is not a file. Skip.", ILogger::ELogSeverity::Warning);
		}
	}

	//Iterate over all "toParseDirectories"
	for (fs::path pathToIncludedDir : settings.getToProcessDirectories())
	{
		if (fs::exists(pathToIncludedDir) && fs::is_directory(pathToIncludedDir))
		{
			for (fs::recursive_directory_iterator directoryIt = fs::recursive_directory_iterator(pathToIncludedDir, fs::directory_options::follow_directory_symlink); directoryIt != fs::recursive_directory_iterator(); directoryIt++)
			{
				fs::directory_entry entry = *directoryIt;

				//Just to make sure the entry hasn't been deleted since beginning of directory iteration
				if (entry.exists())
				{
					if (entry.is_regular_file())
					{
						if (settings.isSupportedFileExtension(entry.path().extension()) && !settings.isIgnoredFile(entry.path()))
						{
							if (!codeGenUnit.isUpToDate(entry.path()) || forceRegenerateAll)
							{
								result.emplace(entry.path());
							}
							else
							{
								out_genResult.upToDateFiles.push_back(entry.path());
							}
						}
					}
					else if (entry.is_directory() && settings.isIgnoredDirectory(entry.path()))
					{
						//Don't iterate on ignored directory content
						directoryIt.disable_recursion_pending();
					}
				}
			}
		}
		else if (logger != nullptr)
		{
			//Add FileGenerationFile invalid path
			logger->log("Directory " + pathToIncludedDir.string() + " is not a directory or doesn't exist. Skip.", ILogger::ELogSeverity::Warning);
		}
	}

	return result;
}

uint32_t CodeGenManager::getThreadCount(uint32_t initialThreadCount) const noexcept
{
	if (initialThreadCount == 0)
	{
		//Use hardware_concurrency if possible
		initialThreadCount = std::thread::hardware_concurrency();

		//If hardware_concurrency hints to 0, use 8 threads
		if (initialThreadCount == 0)
		{
			initialThreadCount = 8u;
		}
	}

	return initialThreadCount;
}

void CodeGenManager::generateMacrosFile(ParsingSettings const& parsingSettings, fs::path const& outputDirectory) const noexcept
{
	GeneratedFile macrosDefinitionFile(outputDirectory / CodeGenUnitSettings::entityMacrosFilename);

	//Define empty entity macros to allow compilation outside of the REHeaderTool parsing
	macrosDefinitionFile.writeLines("#pragma once",
									"",
									"#ifndef " + ParsingSettings::parsingMacro,
									"",
									"#define " + parsingSettings.propertyParsingSettings.namespaceMacroName	+ "(...)",
									"#define " + parsingSettings.propertyParsingSettings.classMacroName		+ "(...)",
									"#define " + parsingSettings.propertyParsingSettings.structMacroName	+ "(...)",
									"#define " + parsingSettings.propertyParsingSettings.variableMacroName	+ "(...)",
									"#define " + parsingSettings.propertyParsingSettings.fieldMacroName		+ "(...)",
									"#define " + parsingSettings.propertyParsingSettings.methodMacroName	+ "(...)",
									"#define " + parsingSettings.propertyParsingSettings.enumMacroName		+ "(...)",
									"#define " + parsingSettings.propertyParsingSettings.enumValueMacroName	+ "(...)",
									"#define " + parsingSettings.propertyParsingSettings.functionMacroName	+ "(...)");

	macrosDefinitionFile.writeLine("\n#endif");
}

bool CodeGenManager::checkGenerationSetup(FileParser const& /* fileParser */, CodeGenUnit const& codeGenUnit) noexcept
{
	bool canLog	= logger != nullptr;
	
	if (CodeGenUnitSettings const* codeGenUnitSettings = codeGenUnit.getSettings())
	{
		//Emit a warning if the output directory content is going to be parsed
		if (fs::exists(codeGenUnitSettings->getOutputDirectory()) &&					//abort check if the output directory doesn't exist
			!fs::is_empty(codeGenUnitSettings->getOutputDirectory()) &&					//abort check if the output directory contains no file
			!settings.isIgnoredDirectory(codeGenUnitSettings->getOutputDirectory()))	//abort check if the output directory is already ignored
		{
			for (fs::path const& parsedDirectory : settings.getToProcessDirectories())
			{
				if (FilesystemHelpers::isChildPath(codeGenUnitSettings->getOutputDirectory(), parsedDirectory))
				{
					if (canLog)
					{
						logger->log("Output directory is contained in a parsed directory, hence generated files will be parsed. If this is not intended, add the output directory to the list of ignored directories.", ILogger::ELogSeverity::Warning);
					}

					break;
				}
			}
		}
	}
	
	return codeGenUnit.checkSettings();
}