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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <RECore/Reflect/Event/DynEventHandler.h>
#include "REScriptLua/RTTIObjectSignalPointerBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RECore {
	class DynParams;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REScriptLua {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    RTTI object build in signal method pointer
*/
class RTTIObjectSignalMethodPointer : public RTTIObjectSignalPointerBase {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LuaContext;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Build in method
		*/
		enum EMethod {
			MethodUnknown    = -1,	/**< Unknown method */
			MethodConnect    = 0,	/**< Connect method */
			MethodDisconnect = 1	/**< Disconnect method */
		};


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the build in method defined by a given string
		*
		*  @param[in] sMethod
		*    String to return the build in method from
		*
		*  @return
		*    The build in method defined by the given string
		*/
		static EMethod StringToMethod(const RECore::String &sMethod);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RTTIObjectSignalMethodPointer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RTTIObjectSignalMethodPointer();


	//[-------------------------------------------------------]
	//[ Protected virtual LuaUserData functions               ]
	//[-------------------------------------------------------]
	protected:
		virtual int indexMetamethod(lua_State *pLuaState) override;
		virtual int newIndexMetamethod(lua_State *pLuaState) override;
		virtual void cgMetamethod(lua_State *pLuaState) override;
		virtual void callMetamethod(lua_State *pLuaState) override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		EMethod m_nMethod;	/**< Build in method */


	//[-------------------------------------------------------]
	//[ Private static event callback functions               ]
	//[-------------------------------------------------------]
	private:
		/*
		*  @brief
		*    Event callback function
		*
		*  @param[in] cDynParams
		*    Dynamic parameters
		*  @param[in] pUserData
		*    Pointer to optional user data, can be a null pointer
		*/
		static void eventCallback(RECore::DynParams &cDynParams, void *pUserData);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Initializes this instance
		*
		*  @param[in] cScript
		*    The owner script instance
		*  @param[in] pRTTIObject
		*    Pointer to the RTTI object to wrap, can be a null pointer
		*  @param[in] pDynEvent
		*    Pointer to the RTTI object signal to wrap, can be a null pointer
		*  @param[in] nMethod
		*    Build in method
		*/
		void initializeInstance(Script &cScript, RECore::Object *pRTTIObject, RECore::DynEventPtr pDynEvent, EMethod nMethod);

		/**
		*  @brief
		*    Returns a RTTI slot from the Lua stack without removing it
		*
		*  @param[in] pLuaState
		*    Lua state
		*
		*  @return
		*    The valid RTTI slot, null pointer on error
		*
		*  @note
		*    - Performs also a signal/slot signature match
		*    - m_pDynEvent must be valid
		*/
		RECore::DynEventHandlerPtr getSlotFromLuaStack(lua_State *pLuaState);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua
