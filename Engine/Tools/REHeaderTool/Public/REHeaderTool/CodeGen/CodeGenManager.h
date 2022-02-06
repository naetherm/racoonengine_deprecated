/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <set>
#include <cassert>
#include <type_traits>	//std::is_base_of
#include <chrono>		//std::chrono::high_resolution_clock

#include "REHeaderTool/Misc/ILogger.h"
#include "REHeaderTool/CodeGen/CodeGenResult.h"
#include "REHeaderTool/CodeGen/CodeGenUnit.h"
#include <REHeaderTool/CodeGen/CodeGenManagerSettings.h>
#include "REHeaderTool/Parsing/FileParser.h"
#include "REHeaderTool/Threading/ThreadPool.h"
#include "REHeaderTool/Threading/TaskHelper.h"

namespace REHeaderTool
{
	class CodeGenManager
	{
		private:
			/** Thread pool used for files processing. */
			ThreadPool	_threadPool;

			/**
			*	@brief Process all provided files on multiple threads.
			*	
			*	@param fileParser		Original file parser to use to parse registered files. A copy of this parser will be used for each generation thread.
			*	@param codeGenUnit		Generation unit used to generate files. It must have a clean state when this method is called.
			*	@param toProcessFiles	Collection of all files to process.
			*	@param out_genResult	Reference to the generation result to fill during file generation.
			*/
			template <typename FileParserType, typename CodeGenUnitType>
			void	processFiles(FileParserType&			fileParser,
								 CodeGenUnitType&			codeGenUnit,
								 std::set<fs::path> const&	toProcessFiles,
								 CodeGenResult&				out_genResult)										noexcept;

			/**
			*	@brief Identify all files which will be parsed & regenerated.
			*	
			*	@param codeGenUnit			Generation unit used to determine whether a file should be reparsed/regenerated or not.
			*	@param out_genResult		Reference to the generation result to fill during file generation.
			*	@param forceRegenerateAll	Should all files be regenerated or not (regardless of CodeGenManager::shouldRegenerateFile() returned value).
			*
			*	@return A collection of all files which will be regenerated.
			*/
			std::set<fs::path>		identifyFilesToProcess(CodeGenUnit const&	codeGenUnit,
														   CodeGenResult&		out_genResult,
														   bool					forceRegenerateAll)				noexcept;

			/**
			*	@brief	Get the number of threads to use based on the provided thread count.
			*			If 0 is provided, std::thread::hardware_concurrency is used, or 8 if std::thread::hardware_concurrency returns 0.
			*			For all other initial thread count values, the function returns immediately this number.
			* 
			*	@param initialThreadCount The number of threads to use.
			* 
			*	@return The number of threads to use.
			*/
			uint32_t					getThreadCount(uint32_t initialThreadCount)							const	noexcept;

			/**
			*	@brief Generate / update the entity macros file.
			*	
			*	@param parsingSettings	Parsing settings.
			*	@param outputDirectory	Directory in which the macro file should be generated.
			*/
			void					generateMacrosFile(ParsingSettings const&	parsingSettings,
													   fs::path const&			outputDirectory)		const	noexcept;

			/**
			*	@brief Check that everything is setup correctly for generation.
			* 
			*	@param fileParser	The file parser to using during the generation process.
			*	@param codeGenUnit	The code generation to use during the generation process.
			* 
			*	@return true if all settings are correct, else false.
			*/
			bool					checkGenerationSetup(FileParser const&	fileParser,
														 CodeGenUnit const& codeGenUnit)						noexcept;

		public:
			/** Logger used to issue logs from the CodeGenManager. */
			ILogger*				logger		= nullptr;

			/** Struct containing all generation settings. */
			CodeGenManagerSettings	settings;

			/**
			*	@brief Construct a CodeGenManager that will work with the specified number of threads.
			* 
			*	@param threadCount	Number of threads to use for file parsing and generation.
			*							If 0 is provided, the number of concurrent threads supported by the implementation will be used (std::thread::hardware_concurrency(), and 8 if std::thread::hardware_concurrency() returns 0).
			*							If 1 is provided, all the process will be handled by the main thread.
			*/
			CodeGenManager(uint32_t threadCount = 0u)	noexcept;

			/**
			*	@brief	Parse registered files if they were modified since last generation (or don't exist)
			*			and forward them to individual file generation unit for code generation.
			*
			*	@param fileParser			Original file parser to use to parse registered files. A copy of this parser will be used for each generation thread.
			*	@param codeGenUnit			Generation unit used to generate code. It must have a clean state when this method is called.
			*	@param forceRegenerateAll	Ignore the last write time check and reparse / regenerate all files.
			*
			*	@return Structure containing file generation report.
			*/
			template <typename FileParserType, typename CodeGenUnitType>
			CodeGenResult run(FileParserType&	fileParser,
							  CodeGenUnitType&	codeGenUnit,
							  bool				forceRegenerateAll	= false)	noexcept;
	};

	#include "REHeaderTool/CodeGen/CodeGenManager.inl"
}