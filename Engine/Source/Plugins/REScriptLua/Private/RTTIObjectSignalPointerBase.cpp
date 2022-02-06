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
#include <RECore/Reflect/Object.h>
#include "REScriptLua/Script.h"
#include "REScriptLua/LuaContext.h"
#include "REScriptLua/RTTIObjectMethodPointer.h"
#include "REScriptLua/RTTIObjectSignalMethodPointer.h"
#include "REScriptLua/RTTIObjectSignalPointerBase.h"


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
*    Destructor
*/
RTTIObjectSignalPointerBase::~RTTIObjectSignalPointerBase()
{
}

/**
*  @brief
*    Returns the pointer to the RTTI object signal to wrap
*/
DynEventPtr RTTIObjectSignalPointerBase::getDynEvent() const
{
	return m_pDynEvent;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
RTTIObjectSignalPointerBase::RTTIObjectSignalPointerBase(EType nType) : RTTIObjectPointerBase(nType),
	m_pDynEvent(nullptr)
{
}

/**
*  @brief
*    Initializes this instance
*/
void RTTIObjectSignalPointerBase::initializeInstance(Script &cScript, Object *pRTTIObject, DynEventPtr pDynEvent)
{
	// Call base implementation
	RTTIObjectPointerBase::initializeInstance(cScript, pRTTIObject);

	// Set given data
	m_pDynEvent = pDynEvent;
}


//[-------------------------------------------------------]
//[ Protected virtual LuaUserData functions               ]
//[-------------------------------------------------------]
int RTTIObjectSignalPointerBase::indexMetamethod(lua_State *pLuaState)
{
	// This method deals with build in signal methods

	// There must be a string Lua stack (first argument is a user data)
	if (lua_isstring(pLuaState, 2)) {
		// Figure out the build in signal method to be called
		const RTTIObjectSignalMethodPointer::EMethod nMethod = RTTIObjectSignalMethodPointer::StringToMethod(lua_tolstring(pLuaState, 2, nullptr));
		if (nMethod != RTTIObjectSignalMethodPointer::MethodUnknown) {
			// It's a build in signal method... just put another user data instance on the Lua stack...
			// The destruction of the new RTTIObjectSignalMethodPointer instance is done by the Lua garbage collector
			LuaContext::getRTTIObjectSignalMethodPointer(*m_pScript, getObject(), m_pDynEvent, nMethod);

			// Done
			return 1;
		} else {
			// Error, this is no valid build in signal method!

			// [TODO] Write an error message into the log? (with current script line etc.)
		}
	}

	// [TODO] Write an error message into the log? (with current script line etc.)

	// Error!
	return 0;
}

int RTTIObjectSignalPointerBase::newIndexMetamethod(lua_State *pLuaState)
{
	// Error! A signal can't be called like an object...
	// [TODO] Write an error message into the log? (with current script line etc.)

	// Done
	return 0;
}

void RTTIObjectSignalPointerBase::callMetamethod(lua_State *pLuaState)
{
	// Is there a RTTI object and a RTTI object signal?
	if (getObject() && m_pDynEvent) {
		// Get the current Lua function parameters on the Lua stack as string
    std::vector<String> lstTempStrings;
		const String sParams = RTTIObjectMethodPointer::getLuaFunctionParametersAsString(*m_pScript, *m_pDynEvent, true, lstTempStrings);

		// Emit the RTTI object signal
		m_pDynEvent->emit(sParams);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua
