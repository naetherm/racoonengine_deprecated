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
#include <RECore/Scripting/Script.h>
#include "REScriptLua/REScriptLua.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
typedef struct lua_State lua_State;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REScriptLua {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Lua (http://www.lua.org/) script implementation
*/
class Script : public RECore::Script {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class RTTIObjectSignalMethodPointer;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	re_class_def(RESCRIPTLUA_API)
	re_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RESCRIPTLUA_API Script();

		/**
		*  @brief
		*    Destructor
		*/
		RESCRIPTLUA_API virtual ~Script();

		/**
		*  @brief
		*    Returns the Lua state
		*
		*  @return
		*    Lua state, can be a null pointer
		*/
		RESCRIPTLUA_API lua_State *getLuaState() const;

		/**
		*  @brief
		*    Writes the current Lua stack content into the log
		*
		*  @note
		*    - For debugging
		*/
		RESCRIPTLUA_API void luaStackDump();


	//[-------------------------------------------------------]
	//[ Public virtual RECore::Script functions               ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		RESCRIPTLUA_API virtual bool isGlobalFunction(const RECore::String &sName, const RECore::String &sNamespace = "") override;
		RESCRIPTLUA_API virtual bool addGlobalFunction(const RECore::String &sFunction, const RECore::DynFunc &cDynFunc, const RECore::String &sNamespace = "") override;
		RESCRIPTLUA_API virtual bool removeAllGlobalFunctions() override;

		//[-------------------------------------------------------]
		//[ Script source code                                    ]
		//[-------------------------------------------------------]
		RESCRIPTLUA_API virtual RECore::String getSourceCode() const override;
		RESCRIPTLUA_API virtual bool setSourceCode(const RECore::String &sSourceCode) override;
		RESCRIPTLUA_API virtual void getAssociatedFilenames(std::vector<RECore::String> &lstFilenames) override;
		RESCRIPTLUA_API virtual bool execute(const RECore::String &sSourceCode) override;

		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		RESCRIPTLUA_API virtual void getGlobalVariables(std::vector<RECore::String> &lstGlobalVariables, const RECore::String &sNamespace = "") override;
		RESCRIPTLUA_API virtual bool isGlobalVariable(const RECore::String &sName, const RECore::String &sNamespace = "") override;
		RESCRIPTLUA_API virtual RECore::ETypeID getGlobalVariableTypeID(const RECore::String &sName, const RECore::String &sNamespace = "") override;
		RESCRIPTLUA_API virtual RECore::String getGlobalVariable(const RECore::String &sName, const RECore::String &sNamespace = "") override;
		RESCRIPTLUA_API virtual void setGlobalVariable(const RECore::String &sName, const RECore::DynVar &cValue, const RECore::String &sNamespace = "") override;

		//[-------------------------------------------------------]
		//[ Global function call, used by "FuncScriptPtr"         ]
		//[-------------------------------------------------------]
		RESCRIPTLUA_API virtual bool beginCall(const RECore::String &sFunctionName, const RECore::String &sFunctionSignature, const RECore::String &sNamespace = "") override;
		RESCRIPTLUA_API virtual void pushArgument(bool bValue) override;
		RESCRIPTLUA_API virtual void pushArgument(float fValue) override;
		RESCRIPTLUA_API virtual void pushArgument(double fValue) override;
		RESCRIPTLUA_API virtual void pushArgument(RECore::int8 nValue) override;
		RESCRIPTLUA_API virtual void pushArgument(RECore::int16 nValue) override;
		RESCRIPTLUA_API virtual void pushArgument(RECore::int32 nValue) override;
		RESCRIPTLUA_API virtual void pushArgument(RECore::int64 nValue) override;
		RESCRIPTLUA_API virtual void pushArgument(RECore::uint8 nValue) override;
		RESCRIPTLUA_API virtual void pushArgument(RECore::uint16 nValue) override;
		RESCRIPTLUA_API virtual void pushArgument(RECore::uint32 nValue) override;
		RESCRIPTLUA_API virtual void pushArgument(RECore::uint64 nValue) override;
		RESCRIPTLUA_API virtual void pushArgument(const RECore::String &sString) override;
		RESCRIPTLUA_API virtual void pushArgument(RECore::Object *pObject) override;
		RESCRIPTLUA_API virtual void pushArgument(RECore::Object &cObject) override;
		RESCRIPTLUA_API virtual bool endCall() override;
		RESCRIPTLUA_API virtual bool getReturn(bool nValue) override;
		RESCRIPTLUA_API virtual float getReturn(float nValue) override;
		RESCRIPTLUA_API virtual double getReturn(double nValue) override;
		RESCRIPTLUA_API virtual RECore::int8 getReturn(RECore::int8 nValue) override;
		RESCRIPTLUA_API virtual RECore::int16 getReturn(RECore::int16 nValue) override;
		RESCRIPTLUA_API virtual RECore::int32 getReturn(RECore::int32 nValue) override;
		RESCRIPTLUA_API virtual RECore::int64 getReturn(RECore::int64 nValue) override;
		RESCRIPTLUA_API virtual RECore::uint8 getReturn(RECore::uint8 nValue) override;
		RESCRIPTLUA_API virtual RECore::uint16 getReturn(RECore::uint16 nValue) override;
		RESCRIPTLUA_API virtual RECore::uint32 getReturn(RECore::uint32 nValue) override;
		RESCRIPTLUA_API virtual RECore::uint64 getReturn(RECore::uint64 nValue) override;
		RESCRIPTLUA_API virtual RECore::String getReturn(RECore::String nValue) override;
		RESCRIPTLUA_API virtual RECore::Object *getReturn(RECore::Object *nValue) override;
		RESCRIPTLUA_API virtual RECore::Object &getReturn(RECore::Object &nValue) override;


	//[-------------------------------------------------------]
	//[ Private static Lua callback functions                 ]
	//[-------------------------------------------------------]
	private:
		/*
		*  @brief
		*    Lua function callback
		*
		*  @param[in] pLuaState
		*    Lua state
		*
		*  @return
		*    Number of parameters to return to Lua
		*/
		static int luaFunctionCallback(lua_State *pLuaState);


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    A global function
		*/
		struct GlobalFunction {
			Script			*pScript;		/**< Pointer to the owner script instance, always valid! */
			RECore::String   sFunction;		/**< Function name used inside the script to call the global function */
			RECore::DynFunc *pDynFunc;		/**< Dynamic function to be called, always valid, destroy when done */
			RECore::String   sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
		};

		/**
		*  @brief
		*    The structure is used to connect script functions with RTTI signals
		*/
		struct EventUserData {
			RECore::DynEventHandlerPtr 	pDynEventHandler;		/**< The generic event handler, always valid! (delete the instance when no longer required) */
			Script						*pScript;				/**< The owner script instance, always valid! */
			int						 	nLuaFunctionReference;	/**< The Lua-function or C-function to be called, never LUA_NOREF (use luaL_unref(<LuaState>, LUA_REGISTRYINDEX, <Reference>) when no longer required) */
		};


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Script(const Script &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Script &operator =(const Script &cSource);

		/**
		*  @brief
		*    Reports Lua errors
		*
		*  @note
		*    - Do only call this method if m_pLuaState is valid and there was in fact an error
		*/
		void reportErrors();

		/**
		*  @brief
		*    Clears the script
		*/
		void clear();

		/**
		*  @brief
		*    Creates a nested Lua table
		*
		*  @param[in] sTableName
		*    Lua table name (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on), if empty string, the global Lua table is pushed onto the Lua stack
		*
		*  @note
		*    - Creates all required subtables
		*    - Leaves the deepest table on the Lua stack
		*    - Already existing Lua tables are not overwritten
		*    - m_pLuaState must be valid
		*
		*  @return
		*    'true' if all went fine, else 'false' (error within the given Lua table name?)
		*/
		bool createNestedTable(const RECore::String &sTableName);

		/**
		*  @brief
		*    Gets a nested Lua table
		*
		*  @param[in] sTableName
		*    Lua table name (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on), if empty string, the global Lua table is pushed onto the Lua stack
		*
		*  @note
		*    - Leaves the deepest table on the Lua stack
		*    - m_pLuaState must be valid
		*
		*  @return
		*    'true' if all went fine, else 'false' (error within the given Lua table name?)
		*/
		bool getNestedTable(const RECore::String &sTableName);

		//[-------------------------------------------------------]
		//[ Event and event handler stuff                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns event user data key
		*
		*  @param[in] pDynEvent
		*    Dynamic event, must be valid
		*  @param[in] pLuaPointer
		*    Lua pointer to the function, must be valid
		*
		*  @return
		*    Event user data key
		*/
		RECore::String getEventUserDataKey(RECore::DynEventPtr pDynEvent, const void *pLuaPointer) const;

		/**
		*  @brief
		*    Returns event user data
		*
		*  @param[in] pDynEvent
		*    Dynamic event, must be valid
		*  @param[in] pLuaPointer
		*    Lua pointer to the function, must be valid
		*
		*  @return
		*    Event user data, can be a null pointer
		*/
		EventUserData *getEventUserData(RECore::DynEventPtr pDynEvent, const void *pLuaPointer) const;

		/**
		*  @brief
		*    Adds event user data
		*
		*  @param[in] pDynEvent
		*    Dynamic event, must be valid
		*  @param[in] pLuaPointer
		*    Lua pointer to the function, must be valid
		*  @param[in] pEventUserData
		*   Event user data to add, must be valid
		*
		*  @note
		*    - Do only call this method if the event user data is not yet added
		*/
		void addEventUserData(RECore::DynEventPtr pDynEvent, const void *pLuaPointer, EventUserData *pEventUserData);

		/**
		*  @brief
		*    Destroys all registered event user data (a kind of "disconnect all slots at once")
		*/
		void destroyEventUserData();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RECore::String									 m_sSourceCode;			/**< Script source code */
		lua_State										*m_pLuaState;			/**< Lua state, can be a null pointer */
		RECore::String									 m_sCurrentFunction;	/**< Name of the current function */
		bool											 m_bFunctionResult;		/**< Has the current function a result? */
		RECore::uint32									 m_nCurrentArgument;	/**< Current argument, used during function call */
  std::vector<GlobalFunction*>				     m_lstGlobalFunctions;	/**< List of global functions */
		std::map<RECore::String, EventUserData*>  m_mapEventUserData;	/**< Map holding all event user data instances */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua
