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
#include <RECore/Reflect/Func/DynFunc.h>
#include "REScriptLua/RTTIObjectPointerBase.h"
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REScriptLua {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    RTTI object method pointer
*/
class RTTIObjectMethodPointer : public RTTIObjectPointerBase {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LuaContext;


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the current Lua function parameters on the Lua stack as string
		*
		*  @param[in]  cScript
		*    The owner script instance
		*  @param[in]  cDynSignature
		*    Dynamic signature of the function to be called
		*  @param[in]  bIsMethod
		*    'true' if the dynamic function is a method, 'false' if it's a global function
		*  @param[out] lstTempStrings
		*    List were temporary strings can be put onto, see remarks below for details (the given list is not cleared, new entries are just added)
		*
		*  @return
		*    The current Lua function parameters on the Lua stack as string
		*
		*  @remarks
		*    Strings are somewhat of a special case... It's possible that there's a RTTI method with
		*    a "Object*(const RECore::String&)"-signature meaning that the parameter is a reference.
		*    Within scripts, strings are fundamental and therefore it should be possible to use such a
		*    RTTI method by writing for instance
		*      this:GetSceneNode():GetByName("Soldier")
		*    so that the script programmer doesn't need to care whether or not the required RTTI method
		*    parameter is in fact a reference instead of a string. Of course, a real reference a parameter
		*    should still also be possible as well.
		*    To solve this issue, whenever the script programmer provides a string, but a RTTI method is
		*    expecting a reference or a pointer, this given string is stored within a string-list on the
		*    heap during the function call to that the reference/pointer has a valid address. That's what
		*    the "lstTempStrings"-parameter of this method is for.
		*/
		static RECore::String getLuaFunctionParametersAsString(Script &cScript, RECore::DynSignature &cDynSignature, bool bIsMethod, std::vector<RECore::String> &lstTempStrings);

		/**
		*  @brief
		*    Calls the current Lua stack dynamic function
		*
		*  @param[in] cScript
		*    The owner script instance
		*  @param[in] cDynFunc
		*    Dynamic function to be called
		*  @param[in] bIsMethod
		*    'true' if the dynamic function is a method, 'false' if it's a global function
		*
		*  @return
		*    Number of results on the Lua stack
		*/
		static int callDynFunc(Script &cScript, RECore::DynFunc &cDynFunc, bool bIsMethod);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RTTIObjectMethodPointer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RTTIObjectMethodPointer();

		/**
		*  @brief
		*    Returns the pointer to the RTTI object method to wrap
		*
		*  @return
		*    Pointer to the RTTI object method to wrap, can be a null pointer
		*/
		RECore::DynFuncPtr getDynFuncPtr() const;


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
		RECore::DynFuncPtr m_pDynFunc;	/**< Smart pointer to the RTTI object method to wrap, can be a null pointer */


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
		*  @param[in] pDynFunc
		*    Smart pointer to the RTTI object method to wrap, can be a null pointer
		*/
		void initializeInstance(Script &cScript, RECore::Object *pRTTIObject, RECore::DynFuncPtr pDynFunc);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua
