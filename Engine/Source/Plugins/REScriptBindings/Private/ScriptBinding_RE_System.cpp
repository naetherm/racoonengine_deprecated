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
#include "REScriptBindings/ScriptBinding_RE_System.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace RECore;
namespace REScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(ScriptBinding_RE_System, "REScriptBindings", RECore::ScriptBinding, "System script binding class")
	// Properties
	re_properties
		re_property("Namespace",	"PL.System")
	re_properties_end
	// Constructors
	re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	re_method_0_metadata(isWindows,					re_ret_type(bool),				"Returns 'true' if we're currently running on a Windows platform, else 'false'",						"")
	re_method_0_metadata(isLinux,					re_ret_type(bool),				"Returns 'true' if we're currently running on a Linux platform, else 'false'",							"")
	re_method_0_metadata(getPlatformArchitecture,	re_ret_type(RECore::String),	"Returns the platform architecture (for instance 'x86', 'x64', 'armeabi', 'armeabi-v7a' and so on)",	"")
re_class_metadata_end(ScriptBinding_RE_System)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ScriptBinding_RE_System::isWindows()
{
	#ifdef WIN32
		return true;
	#else
		return false;
	#endif
}

bool ScriptBinding_RE_System::isLinux()
{
	#ifdef LINUX
		return true;
	#else
		return false;
	#endif
}

String ScriptBinding_RE_System::getPlatformArchitecture()
{
	return ""; // m_pSystem->getPlatformArchitecture();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBinding_RE_System::ScriptBinding_RE_System() :
	m_pSystem(&Platform::instance())
{
}

/**
*  @brief
*    Destructor
*/
ScriptBinding_RE_System::~ScriptBinding_RE_System()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptBindings
