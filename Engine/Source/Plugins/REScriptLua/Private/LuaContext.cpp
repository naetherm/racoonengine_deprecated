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
extern "C" {
	#include <Lua/lua.h>
}
#include <RECore/Log/Log.h>
#include "REScriptLua/LuaContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace RECore;
namespace REScriptLua {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
uint32									LuaContext::m_nContexCounter = 0;
std::list<RTTIObjectPointer>				LuaContext::m_lstRTTIObjectPointer;
std::list<RTTIObjectSlotPointer>			LuaContext::m_lstRTTIObjectSlotPointer;
std::list<RTTIObjectMethodPointer>		LuaContext::m_lstRTTIObjectMethodPointer;
std::list<RTTIObjectSignalPointer>		LuaContext::m_lstRTTIObjectSignalPointer;
std::list<RTTIObjectSignalMethodPointer>	LuaContext::m_lstRTTIObjectSignalMethodPointer;


//[-------------------------------------------------------]
//[ Public static methods                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a context reference
*/
void LuaContext::addContextReference()
{
	// Check context
	if (!m_nContexCounter)
		RE_LOG(Info, std::string("Initialize ") + LUA_RELEASE)
	m_nContexCounter++;
}

/**
*  @brief
*    Releases a context reference
*/
void LuaContext::releaseContextReference()
{
	// Check context
	m_nContexCounter--;
	if (!m_nContexCounter)
  RE_LOG(Info, std::string("De-initialize" ) + LUA_RELEASE)
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua
