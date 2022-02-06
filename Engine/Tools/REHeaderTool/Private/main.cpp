////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2022 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <filesystem>
#include <utility>

#include <REHeaderTool//Misc/DefaultLogger.h>
#include <REHeaderTool/CodeGen/Macro/MacroCodeGenUnit.h>
#include <REHeaderTool/CodeGen/CodeGenManager.h>
#include <REHeaderTool/CodeGen/Macro/MacroCodeGenUnitSettings.h>

#include "REHeaderTool/Parsing/FileParser.h"
#include "REHeaderTool/CodeGen/ReflectionCodeGenModule.h"

//[-------------------------------------------------------]
//[ Global helper methods                                 ]
//[-------------------------------------------------------]


void printGenerationSetup(REHeaderTool::ILogger& logger, REHeaderTool::CodeGenManagerSettings const& codeGenMgrSettings, REHeaderTool::ParsingSettings const& parsingSettings,
                          REHeaderTool::MacroCodeGenUnitSettings const& codeGenUnitSettings)
{
  //Output dir
  logger.log("Output directory: " + REHeaderTool::FilesystemHelpers::sanitizePath(codeGenUnitSettings.getOutputDirectory()).string());

  //ToParseDirs
  logger.log("Parsed directories:");
  for (fs::path const& path : codeGenMgrSettings.getToProcessDirectories())
  {
    logger.log("\t" + path.string());
  }

  //IgnoredDirs
  logger.log("Ignored directories:");
  for (fs::path const& path : codeGenMgrSettings.getIgnoredDirectories())
  {
    logger.log("\t" + path.string());
  }

  //Project include dirs
  logger.log("Project include directories:");
  for (fs::path const& path : parsingSettings.getProjectIncludeDirectories())
  {
    logger.log("\t" + path.string());
  }
}

bool loadSettings(REHeaderTool::ILogger& logger, REHeaderTool::CodeGenManagerSettings& codeGenMgrSettings, REHeaderTool::ParsingSettings& parsingSettings,
                  REHeaderTool::MacroCodeGenUnitSettings& codeGenUnitSettings, fs::path&& settingsFilePath)
{
  if (!settingsFilePath.empty())
  {
    //Load settings from settings file
    //All settings are localized in a single file for ease of use
    if (!parsingSettings.loadFromFile(settingsFilePath, &logger))
    {
      return false;
    }
    else if (!codeGenMgrSettings.loadFromFile(settingsFilePath, &logger))
    {
      return false;
    }
    else if (!codeGenUnitSettings.loadFromFile(settingsFilePath, &logger))
    {
      return false;
    }
  }

#if RFK_DEV

  //Specify used compiler
#if defined(__GNUC__)
	if (parsingSettings.setCompilerExeName("g++"))
	{
		logger.log("Set compiler: g++");
	}
	else
	{
		logger.log("Failed to set compiler: g++");
	}
#elif defined(__clang__)
	if (parsingSettings.setCompilerExeName("clang++"))
	{
		logger.log("Set compiler: clang++");
	}
	else
	{
		logger.log("Failed to set compiler: clang++");
	}
#elif defined(_MSC_VER)
	if (parsingSettings.setCompilerExeName("msvc"))
	{
		logger.log("Set compiler: msvc");
	}
	else
	{
		logger.log("Failed to set compiler: msvc");
	}
#endif

#endif

  printGenerationSetup(logger, codeGenMgrSettings, parsingSettings, codeGenUnitSettings);

  return true;
}

void printGenerationResult(REHeaderTool::ILogger& logger, REHeaderTool::CodeGenResult const& genResult)
{
  if (genResult.completed)
  {
    logger.log("(Re)generated metadata for " + std::to_string(genResult.parsedFiles.size()) + " file(s) in " + std::to_string(genResult.duration) + " seconds.", REHeaderTool::ILogger::ELogSeverity::Info);
    logger.log("Metadata of " + std::to_string(genResult.upToDateFiles.size()) + " file(s) up-to-date.", REHeaderTool::ILogger::ELogSeverity::Info);
  }
  else
  {
    logger.log("Generation failed to complete successfully.", REHeaderTool::ILogger::ELogSeverity::Error);
  }
}

void parseAndGenerate(fs::path&& settingsFilePath)
{
  REHeaderTool::DefaultLogger logger;

  REHeaderTool::FileParser fileParser;
  fileParser.logger = &logger;

  REHeaderTool::CodeGenManager codeGenMgr;
  codeGenMgr.logger = &logger;

  REHeaderTool::MacroCodeGenUnitSettings codeGenUnitSettings;
  REHeaderTool::MacroCodeGenUnit codeGenUnit;
  codeGenUnit.logger = &logger;
  codeGenUnit.setSettings(codeGenUnitSettings);

  REHeaderTool::ReflectionCodeGenModule reflectionCodeGenModule;
  codeGenUnit.addModule(reflectionCodeGenModule);

  //Load settings
  logger.log("Working Directory: " + fs::current_path().string(), REHeaderTool::ILogger::ELogSeverity::Info);

  //loadSettings(logger, codeGenMgr.settings, fileParser.getSettings(), codeGenUnitSettings, "RefurekuTestsSettings.toml"); //For tests
  loadSettings(logger, codeGenMgr.settings, fileParser.getSettings(), codeGenUnitSettings, std::forward<fs::path>(settingsFilePath));

  //Parse
  REHeaderTool::CodeGenResult genResult = codeGenMgr.run(fileParser, codeGenUnit, false);

  //Result
  printGenerationResult(logger, genResult);
}

/** Can provide the path to the settings file as 1st parameter */
int main(int argc, char** argv)
{
  parseAndGenerate((argc > 1) ? fs::path(argv[1]) : fs::path());

  return EXIT_SUCCESS;
}