/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>

#include "REHeaderTool/CodeGen/CodeGenEnv.h"
#include "REHeaderTool/CodeGen/Macro/ECodeGenLocation.h"

namespace REHeaderTool
{
	//Forward declaration
	class MacroCodeGenUnit;

	class MacroCodeGenEnv : public CodeGenEnv
	{
		//MacroCodeGenUnit is the only class allowed to set the private fields directly.
		//Other classes must access the fields through getters.
		friend MacroCodeGenUnit;

		private:
			/** Location the code should be generated in. */
			ECodeGenLocation	_codeGenLocation		= ECodeGenLocation::Count;

			/** Separator to use to split the generated code. */
			std::string			_separator;

			/** Macro to use to export a symbol when generated code is injected in a dynamic library. */
			std::string			_exportSymbolMacro		= "";

			/** Macro to use to hide a symbol when generated code is injected in a dynamic library. */
			std::string			_internalSymbolMacro	= "";

		public:
			virtual ~MacroCodeGenEnv() = default;

			/**
			*	@brief Getter for field _codeGenLocation.
			* 
			*	@return _codeGenLocation.
			*/
			inline ECodeGenLocation		getCodeGenLocation()		const	noexcept;

			/**
			*	@brief Getter for field _separator.
			* 
			*	@return _separator.
			*/
			inline std::string const&	getSeparator()				const	noexcept;

			/**
			*	@brief Getter for field _exportSymbolMacro.
			* 
			*	@return _exportSymbolMacro.
			*/
			inline std::string const&	getExportSymbolMacro()		const	noexcept;

			/**
			*	@brief Getter for field _internalSymbolMacro.
			* 
			*	@return _internalSymbolMacro.
			*/
			inline std::string const&	getInternalSymbolMacro()	const	noexcept;
	};

	#include "REHeaderTool/CodeGen/Macro/MacroCodeGenEnv.inl"
}