/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

inline ECodeGenLocation MacroCodeGenEnv::getCodeGenLocation() const noexcept
{
	return _codeGenLocation;
}

inline std::string const& MacroCodeGenEnv::getSeparator() const noexcept
{
	return _separator;
}

inline std::string const& MacroCodeGenEnv::getExportSymbolMacro() const noexcept
{
	return _exportSymbolMacro;
}

inline std::string const& MacroCodeGenEnv::getInternalSymbolMacro() const noexcept
{
	return _internalSymbolMacro;
}