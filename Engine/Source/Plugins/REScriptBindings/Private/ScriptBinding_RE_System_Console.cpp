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
#include <RECore/Platform/Platform.h>
#include <RECore/Platform/Console.h>
#include "REScriptBindings/ScriptBinding_RE_System_Console.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace RECore;
namespace REScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(ScriptBinding_RE_System_Console, "REScriptBindings", RECore::ScriptBinding, "System console script binding class")
	// Properties
	re_properties
		re_property("Namespace",	"PL.System.Console")
	re_properties_end
	// Constructors
	re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	re_method_1_metadata(print,	re_ret_type(void),	RECore::String,	"Prints the given string into the system console",	"")
re_class_metadata_end(ScriptBinding_RE_System_Console)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
void ScriptBinding_RE_System_Console::print(String sText)
{
	m_pConsole->print(sText);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBinding_RE_System_Console::ScriptBinding_RE_System_Console() :
	m_pConsole(&Platform::instance().getConsole())
{
}

/**
*  @brief
*    Destructor
*/
ScriptBinding_RE_System_Console::~ScriptBinding_RE_System_Console()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptBindings
