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
	#include <Lua/lauxlib.h>
}
#include <RECore/Reflect/Object.h>
#include "REScriptLua/Script.h"
#include "REScriptLua/LuaContext.h"
#include "REScriptLua/RTTIObjectSlotPointer.h"
#include "REScriptLua/RTTIObjectSignalMethodPointer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace RECore;
namespace REScriptLua {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the build in method defined by a given string
*/
RTTIObjectSignalMethodPointer::EMethod RTTIObjectSignalMethodPointer::StringToMethod(const String &sMethod)
{
	if (sMethod == "Connect")
		return MethodConnect;
	else if (sMethod == "Disconnect")
		return MethodDisconnect;
	else
		return MethodUnknown;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
RTTIObjectSignalMethodPointer::RTTIObjectSignalMethodPointer() : RTTIObjectSignalPointerBase(TypeObjectSignalMethodPointer),
	m_nMethod(MethodUnknown)
{
}

/**
*  @brief
*    Destructor
*/
RTTIObjectSignalMethodPointer::~RTTIObjectSignalMethodPointer()
{
}


//[-------------------------------------------------------]
//[ Protected virtual LuaUserData functions               ]
//[-------------------------------------------------------]
int RTTIObjectSignalMethodPointer::indexMetamethod(lua_State *pLuaState)
{
	// Error! A signal method can't be called like an object...
	// [TODO] Write an error message into the log? (with current script line etc.)

	// Error!
	return 0;
}

int RTTIObjectSignalMethodPointer::newIndexMetamethod(lua_State *pLuaState)
{
	// Error! A signal method can't be called like an object...
	// [TODO] Write an error message into the log? (with current script line etc.)

	// Done
	return 0;
}

void RTTIObjectSignalMethodPointer::cgMetamethod(lua_State *pLuaState)
{
	// De-initializes this instance
	RTTIObjectPointerBase::deInitializeInstance();
	m_pDynEvent = nullptr;
	m_nMethod = MethodUnknown;

	// Release this instance, but do not delete it because we can reuse it later on
	LuaContext::releaseRTTIObjectSignalMethodPointer(*this);
}

void RTTIObjectSignalMethodPointer::callMetamethod(lua_State *pLuaState)
{
	// Is there a RTTI object, a RTTI object signal and a valid build in signal method?
	if (getObject() && m_pDynEvent && m_nMethod != MethodUnknown) {
		// Choose the method
		switch (m_nMethod) {
			// Connect method
			case MethodConnect:
			{
				// Get the valid RTTI slot to connect the RTTI signal with from the Lua stack without removing it
				DynEventHandlerPtr pDynEventHandler = getSlotFromLuaStack(pLuaState);
				if (pDynEventHandler) {
					// Connect the RTTI slot with the RTTI signal
					m_pDynEvent->connect(*pDynEventHandler);
				}
				break;
			}

			// Disconnect method
			case MethodDisconnect:
			{
				// Get the valid RTTI slot to disconnect the RTTI signal with from the Lua stack without removing it
				DynEventHandlerPtr pDynEventHandler = getSlotFromLuaStack(pLuaState);
				if (pDynEventHandler) {
					// Disconnect the RTTI slot with the RTTI signal
					m_pDynEvent->disconnect(*pDynEventHandler);

					// The generic event handler stays alive, this is not really a problem and speeds up a reconnect (the owner script will destroy the instances)
				}
				break;
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private static event callback functions               ]
//[-------------------------------------------------------]
/*
*  @brief
*    Event callback function
*/
void RTTIObjectSignalMethodPointer::eventCallback(DynParams &cDynParams, void *pUserData)
{
	// There must be user data provided
	if (pUserData) {
		Script::EventUserData *pEventUserData = static_cast<Script::EventUserData*>(pUserData);

		// Get the required information out of the provided user data (the provided data must be valid)
		Script    *pScript   = pEventUserData->pScript;
		lua_State *pLuaState = pScript->getLuaState();

		// Push the function to be called onto the Lua state stack
		lua_rawgeti(pLuaState, LUA_REGISTRYINDEX, pEventUserData->nLuaFunctionReference);
		if (lua_isfunction(pLuaState, -1)) {
			// Get the number of parameters
			const uint32 nNumOfParameters = cDynParams.getNumOfParameters();

			// Push all parameters onto the Lua stack
			for (uint32 i=0; i<nNumOfParameters; i++) {
				// Check the type of the parameter
				switch (cDynParams.getParameterTypeID(i)) {
					case TypeVoid:																																break;	// Just do nothing
					case TypeBool:		lua_pushboolean(pLuaState,							*static_cast<bool*>  (cDynParams.getPointerToParameterValue(i)));	break;
					case TypeDouble:	lua_pushnumber (pLuaState,							*static_cast<double*>(cDynParams.getPointerToParameterValue(i)));	break;
					case TypeFloat:		lua_pushnumber (pLuaState,							*static_cast<float*> (cDynParams.getPointerToParameterValue(i)));	break;
					case TypeInt:		lua_pushinteger(pLuaState,							*static_cast<int*>   (cDynParams.getPointerToParameterValue(i)));	break;
					case TypeInt16:		lua_pushinteger(pLuaState,							*static_cast<int16*> (cDynParams.getPointerToParameterValue(i)));	break;
					case TypeInt32:		lua_pushinteger(pLuaState,							*static_cast<int32*> (cDynParams.getPointerToParameterValue(i)));	break;
					case TypeInt64:		lua_pushinteger(pLuaState, static_cast<lua_Integer>(*static_cast<int64*> (cDynParams.getPointerToParameterValue(i))));	break;	// [TODO] TypeInt64 is currently handled just as long
					case TypeInt8:		lua_pushinteger(pLuaState,							*static_cast<int8*>  (cDynParams.getPointerToParameterValue(i)));	break;
					case TypeString:	lua_pushstring (pLuaState,							*static_cast<String*>(cDynParams.getPointerToParameterValue(i)));	break;
					case TypeUInt16:	lua_pushinteger(pLuaState,							*static_cast<uint16*>(cDynParams.getPointerToParameterValue(i)));	break;
					case TypeUInt32:	lua_pushinteger(pLuaState,							*static_cast<uint32*>(cDynParams.getPointerToParameterValue(i)));	break;
					case TypeUInt64:	lua_pushinteger(pLuaState, static_cast<lua_Integer>(*static_cast<uint64*>(cDynParams.getPointerToParameterValue(i))));	break;	// [TODO] TypeUInt64 is currently handled just as long
					case TypeUInt8:		lua_pushinteger(pLuaState,							*static_cast<uint8*> (cDynParams.getPointerToParameterValue(i)));	break;

					// [HACK] Currently, classes derived from "RECore::Object" are just recognized as type "void*"... but "RECore::Object*" type would be perfect
					case TypeRef:
					case TypePtr:
					case TypeObjectPtr:
						RTTIObjectPointer::LuaStackPush(*pScript, *static_cast<Object**>(cDynParams.getPointerToParameterValue(i)));
						break;

					default:			lua_pushnil(pLuaState);																			break;	// Unknown type
				}
			}

			// Do the call ('nNumOfParameters' arguments, 0 results -> By definition, events don't have any return value)
			const int nResult = lua_pcall(pLuaState, nNumOfParameters, 0, 0);
			if (nResult) {
				// Write a log message
				String sErrorDescription;
				switch (nResult) {
					case LUA_ERRRUN:
						sErrorDescription = " (A runtime error)";
						break;

					case LUA_ERRMEM:
						sErrorDescription = " (Memory allocation error)";
						break;

					case LUA_ERRERR:
						sErrorDescription = " (Error while running the error handler function)";
						break;
				}

				// [TODO] Write an error message into the log?
				//logOutput(Log::Error, "Error running function '" + m_sCurrentFunction + '\'' + sErrorDescription);

				// Report Lua errors
				//reportErrors();
			}
		} else {
			// Error!
			// [TODO] Write an error message into the log?
			lua_pop(pLuaState, 1);	// Remove the function reference from the Lua state runtime stack
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes this instance
*/
void RTTIObjectSignalMethodPointer::initializeInstance(Script &cScript, Object *pRTTIObject, DynEventPtr pDynEvent, EMethod nMethod)
{
	// Call base implementation
	RTTIObjectSignalPointerBase::initializeInstance(cScript, pRTTIObject, pDynEvent);

	// Set given data
	m_nMethod = nMethod;
}

/**
*  @brief
*    Returns a RTTI slot from the Lua stack without removing it
*/
DynEventHandlerPtr RTTIObjectSignalMethodPointer::getSlotFromLuaStack(lua_State *pLuaState)
{
	// Is there user data on the Lua stack?
	if (lua_isuserdata(pLuaState, 2)) {
		// Get user data from the Lua stack without removing it
		LuaUserData *pLuaUserData = getUserDataFromLuaStack(pLuaState, 2);

		// Must be a slot
		if (pLuaUserData && pLuaUserData->getType() == TypeObjectSlotPointer) {
			DynEventHandlerPtr pDynEventHandler = reinterpret_cast<RTTIObjectSlotPointer*>(pLuaUserData)->getDynEventHandler();
			if (pDynEventHandler) {
				// The signatures must match
				if (m_pDynEvent->getSignature() == pDynEventHandler->getSignature()) {
					// Finally, return the valid RTTI slot
					return pDynEventHandler;
				} else {
					// Error, RTTI signal/slot signature mismatch!

					// [TODO] Write an error message into the log? (with current script line etc.)
				}
			} else {
				// Error, there's no RTTI valid slot on the Lua stack!

				// [TODO] Write an error message into the log? (with current script line etc.)
			}
		} else {
			// Error, there's no RTTI slot on the Lua stack!

			// [TODO] Write an error message into the log? (with current script line etc.)
		}

	// Is there a function on the Lua stack?
	} else if (lua_isfunction(pLuaState, 2)) {
		// Get a pointer to the Lua function, this pointer is only used to identify the script-slot
		const void *pLuaFunction = lua_topointer(pLuaState, 2);

		// Check whether or not there's already a generic event handler for this registered within the owner script
		Script::EventUserData *pEventUserData = m_pScript->getEventUserData(m_pDynEvent, pLuaFunction);
		if (pEventUserData) {
			// Return the valid RTTI slot
			return pEventUserData->pDynEventHandler;
		} else {
			// Create an instance of the event user data
			pEventUserData = new Script::EventUserData;

			// Create a generic event handler which is compatible with this dynamic event
			pEventUserData->pDynEventHandler = m_pDynEvent->createGenericEventHandler(&eventCallback, pEventUserData);
			if (pEventUserData->pDynEventHandler) {
				// Get a reference to the Lua function
				lua_pushvalue(pLuaState, 2);
				pEventUserData->nLuaFunctionReference = luaL_ref(pLuaState, LUA_REGISTRYINDEX);
				if (pEventUserData->nLuaFunctionReference != LUA_NOREF) {
					// Tell the event user data about the owner script
					pEventUserData->pScript = m_pScript;

					// Register the generic event handler within the owner script
					m_pScript->addEventUserData(m_pDynEvent, pLuaFunction, pEventUserData);

					// Return the valid RTTI slot
					return pEventUserData->pDynEventHandler;
				}

				// Error!
				delete pEventUserData->pDynEventHandler;
			}

			// Error!
			delete pEventUserData;
		}

	// Error, there's no RTTI slot on the Lua stack!
	} else {
		// Error, there's no RTTI slot or Lua function on the Lua stack!

		// [TODO] Write an error message into the log? (with current script line etc.)
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua
