/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <vector>

#include "REHeaderTool/Misc/Filesystem.h"

namespace REHeaderTool
{
	class CodeGenResult
	{
		public:
			/**
			*	This boolean is set to true if the whole generation process has been completed successfully,
			*	and false otherwise. Make sure to check the logs to get some hints about the failure cause.
			*/
			bool					completed	= false;

			/** Time elapsed (in seconds) to discover files to parse, parse, generate and collect results of all files. */
			float					duration	= 0.0f;

			/** List of paths to files that have been parsed and got their metadata regenerated. */
			std::vector<fs::path>	parsedFiles;

			/** List of paths to files which metadata are up-to-date. */
			std::vector<fs::path>	upToDateFiles;

			/**
			*	@brief Merge a result to this result.
			*	
			*	@param otherResult	The result to merge with this result.
			*						After the call, otherResult state is UB.
			*/
			void mergeResult(CodeGenResult&& otherResult)	noexcept;
	};
}