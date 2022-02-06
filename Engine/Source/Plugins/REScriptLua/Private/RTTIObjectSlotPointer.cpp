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
#include "REScriptLua/LuaContext.h"
#include "REScriptLua/RTTIObjectSlotPointer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace RECore;
namespace REScriptLua {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
RTTIObjectSlotPointer::RTTIObjectSlotPointer() : RTTIObjectPointerBase(TypeObjectSlotPointer),
	m_pDynEventHandler(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
RTTIObjectSlotPointer::~RTTIObjectSlotPointer()
{
}

/**
*  @brief
*    Returns the pointer to the RTTI object slot to wrap
*/
DynEventHandlerPtr RTTIObjectSlotPointer::getDynEventHandler() const
{
	return m_pDynEventHandler;
}


//[-------------------------------------------------------]
//[ Protected virtual LuaUserData functions               ]
//[-------------------------------------------------------]
int RTTIObjectSlotPointer::indexMetamethod(lua_State *pLuaState)
{
	// Error! A slot can't be called like an object...
	// [TODO] Write an error message into the log? (with current script line etc.)

	// Error!
	return 0;
}

int RTTIObjectSlotPointer::newIndexMetamethod(lua_State *pLuaState)
{
	// Error! A slot can't be called like an object...
	// [TODO] Write an error message into the log? (with current script line etc.)

	// Done
	return 0;
}

void RTTIObjectSlotPointer::cgMetamethod(lua_State *pLuaState)
{
	// De-initializes this instance
	RTTIObjectPointerBase::deInitializeInstance();
	m_pDynEventHandler = nullptr;

	// Release this instance, but do not delete it because we can reuse it later on
	LuaContext::releaseRTTIObjectSlotPointer(*this);
}

void RTTIObjectSlotPointer::callMetamethod(lua_State *pLuaState)
{
	// Error! A slot can't be called like a function...
	// [TODO] Write an error message into the log? (with current script line etc.)
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes this instance
*/
void RTTIObjectSlotPointer::initializeInstance(Script &cScript, Object *pRTTIObject, RECore::DynEventHandlerPtr pDynEventHandler)
{
	// Call base implementation
	RTTIObjectPointerBase::initializeInstance(cScript, pRTTIObject);

	// Set given data
	m_pDynEventHandler = pDynEventHandler;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua
