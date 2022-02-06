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
#include <RECore/Log/Log.h>
#include "REScriptBindings/ScriptBinding_RE_Log.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace RECore;
namespace REScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(ScriptBinding_RE_Log, "REScriptBindings", RECore::ScriptBinding, "Log script binding class")
	// Properties
	re_properties
		re_property("Namespace",	"PL.Log")
	re_properties_end
	// Constructors
	re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	re_method_1_metadata(outputAlways,		re_ret_type(void),	RECore::String,	"Writes the given string into the log ('always' log level)",	"")
	re_method_1_metadata(outputCritical,	re_ret_type(void),	RECore::String,	"Writes the given string into the log ('critical' log level)",	"")
	re_method_1_metadata(outputError,		re_ret_type(void),	RECore::String,	"Writes the given string into the log ('error' log level)",		"")
	re_method_1_metadata(outputWarning,		re_ret_type(void),	RECore::String,	"Writes the given string into the log ('warning' log level)",	"")
	re_method_1_metadata(outputInfo,		re_ret_type(void),	RECore::String,	"Writes the given string into the log ('info' log level)",		"")
	re_method_1_metadata(outputDebug,		re_ret_type(void),	RECore::String,	"Writes the given string into the log ('debug' log level)",		"")
re_class_metadata_end(ScriptBinding_RE_Log)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
void ScriptBinding_RE_Log::outputAlways(String sText)
{
	m_pLog->output(Log::All, sText.cstr());
}

void ScriptBinding_RE_Log::outputCritical(String sText)
{
	m_pLog->output(Log::Critical, sText.cstr());
}

void ScriptBinding_RE_Log::outputError(String sText)
{
	m_pLog->output(Log::Error, sText.cstr());
}

void ScriptBinding_RE_Log::outputWarning(String sText)
{
	m_pLog->output(Log::Warning, sText.cstr());
}

void ScriptBinding_RE_Log::outputInfo(String sText)
{
	m_pLog->output(Log::Info, sText.cstr());
}

void ScriptBinding_RE_Log::outputDebug(String sText)
{
	m_pLog->output(Log::Debug, sText.cstr());
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBinding_RE_Log::ScriptBinding_RE_Log() :
	m_pLog(&Log::instance())
{
}

/**
*  @brief
*    Destructor
*/
ScriptBinding_RE_Log::~ScriptBinding_RE_Log()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptBindings
