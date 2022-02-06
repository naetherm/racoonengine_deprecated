/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>

#include "REHeaderTool/CodeGen/Macro/MacroCodeGenEnv.h"

namespace REHeaderTool
{
	class MacroCodeGenerator
	{
		protected:
			/**
			*	@brief Generate initial code in the header file header.
			*	
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	initialGenerateHeaderFileHeaderCode(MacroCodeGenEnv&	env,
																std::string&		inout_result)	noexcept;

			/**
			*	@brief Generate initial code in the header file footer.
			*	
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	initialGenerateHeaderFileFooterCode(MacroCodeGenEnv&	env,
																std::string&		inout_result)	noexcept;

			/**
			*	@brief Generate initial code in the source file header.
			*	
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	initialGenerateSourceFileHeaderCode(MacroCodeGenEnv&	env,
																std::string&		inout_result)	noexcept;

			/**
			*	@brief Generate final code in the header file header.
			*	
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	finalGenerateHeaderFileHeaderCode(MacroCodeGenEnv&	env,
															  std::string&		inout_result)		noexcept;

			/**
			*	@brief Generate final code in the header file footer.
			*	
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	finalGenerateHeaderFileFooterCode(MacroCodeGenEnv&	env,
															  std::string&		inout_result)		noexcept;

			/**
			*	@brief Generate final code in the source file header.
			*	
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			virtual bool	finalGenerateSourceFileHeaderCode(MacroCodeGenEnv&	env,
															  std::string&		inout_result)		noexcept;

		public:
			MacroCodeGenerator()							= default;
			MacroCodeGenerator(MacroCodeGenerator const&)	= default;
			MacroCodeGenerator(MacroCodeGenerator&&)		= default;
			virtual ~MacroCodeGenerator()					= default;

			/**
			*	@brief	Generate initial code for this code generator.
			*			This method analyzes the code location retrieved from the MacroCodeGenEnv
			*			and dispatch the call to the relevant method.
			*	
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			bool	initialGenerateCodeImplementation(CodeGenEnv&	env,
													  std::string&	inout_result)	noexcept;
			
			/**
			*	@brief	Generate final code for this code generator.
			*			This method analyzes the code location retrieved from the MacroCodeGenEnv
			*			and dispatch the call to the relevant method.
			*	
			*	@param env				Generation environment structure.
			*	@param inout_result		String the method should append the generated code to.
			*	
			*	@return true if the generation completed successfully, else false.
			*/
			bool	finalGenerateCodeImplementation(CodeGenEnv&		env,
													std::string&	inout_result)	noexcept;

			MacroCodeGenerator& operator=(MacroCodeGenerator const&)	= default;
			MacroCodeGenerator& operator=(MacroCodeGenerator&&)			= default;
	};
}