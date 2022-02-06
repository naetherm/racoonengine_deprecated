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
#include <RECore/Tools/Timing.h>
#include "REScriptBindings/ScriptBinding_RE_Timing.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace RECore;
namespace REScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(ScriptBinding_RE_Timing, "REScriptBindings", RECore::ScriptBinding, "Timing script binding class")
	// Properties
	re_properties
		re_property("Namespace",	"PL.Timing")
	re_properties_end
	// Constructors
	re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	re_method_0_metadata(getTimeDifference,		re_ret_type(float),			"Returns the past time since last frame (seconds)",																																																																						"")
	re_method_0_metadata(getFramesPerSecond,	re_ret_type(float),			"Returns the current frames per second (FPS)",																																																																							"")
	re_method_0_metadata(isPaused,				re_ret_type(bool),			"Returns 'true' when the timing is paused, else 'false'. If the timing is paused scene nodes, particles etc. are not updated. The timing will still be updated.",																																										"")
	re_method_1_metadata(pause,					re_ret_type(void),	bool,	"Set pause mode. 'true' as first parameter when timing should be pause, else 'false'.",																																																													"")
	re_method_0_metadata(getTimeScaleFactor,	re_ret_type(float),			"Returns the time scale factor. The global time scale factor should only be manipulated for debugging. A factor of <= 0 is NOT allowed because this may cause problems in certain situations, pause the timer instead by hand! Do NOT make the factor 'too' (for example > 4) extreme, this may cause problems in certain situations!",	"")
	re_method_1_metadata(setTimeScaleFactor,	re_ret_type(bool),	float,	"Sets the time scale factor. Time scale as first parameter (a factor of <= 0 is NOT allowed!). Returns 'true' if all went fine, else 'false' (maybe the given factor is <= 0?).",																																						"")
re_class_metadata_end(ScriptBinding_RE_Timing)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
float ScriptBinding_RE_Timing::getTimeDifference()
{
	return m_pTiming.GetTimeDifference();
}

float ScriptBinding_RE_Timing::getFramesPerSecond()
{
	return m_pTiming.GetFramesPerSecond();
}

bool ScriptBinding_RE_Timing::isPaused()
{
	return m_pTiming.IsPaused();
}

void ScriptBinding_RE_Timing::pause(bool bPause)
{
	m_pTiming.Pause(bPause);
}

float ScriptBinding_RE_Timing::getTimeScaleFactor()
{
	return m_pTiming.GetTimeScaleFactor();
}

bool ScriptBinding_RE_Timing::setTimeScaleFactor(float fFactor)
{
	return m_pTiming.SetTimeScaleFactor(fFactor);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBinding_RE_Timing::ScriptBinding_RE_Timing() :
	m_pTiming(Timing::instance())
{
}

/**
*  @brief
*    Destructor
*/
ScriptBinding_RE_Timing::~ScriptBinding_RE_Timing()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptBindings
