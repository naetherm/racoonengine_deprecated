/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

inline FileParsingResult const* CodeGenEnv::getFileParsingResult() const noexcept
{
	return _fileParsingResult;
}

inline ILogger* CodeGenEnv::getLogger() const noexcept
{
	return _logger;
}