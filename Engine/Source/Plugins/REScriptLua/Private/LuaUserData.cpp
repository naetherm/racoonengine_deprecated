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
#include "REScriptLua/Script.h"
#include "REScriptLua/LuaUserData.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REScriptLua {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
char LuaUserData::LuaMetatable = 42;


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates the metatable for LuaUserData events
*/
void LuaUserData::createMetatable(lua_State *pLuaState)
{
	lua_pushlightuserdata(pLuaState, &LuaMetatable);
	lua_newtable(pLuaState);

	// Lua __index metamethod callback (for table accesses)
	lua_pushstring(pLuaState, "__index");
	lua_pushcfunction(pLuaState, luaIndexMetamethodCallback);
	lua_rawset(pLuaState, -3);

	// Lua __newindex metamethod callback (for table updates)
	lua_pushstring(pLuaState, "__newindex");
	lua_pushcfunction(pLuaState, luaNewIndexMetamethodCallback);
	lua_rawset(pLuaState, -3);

	// Lua __gc metamethod callback (finalizer/destructor called by garbage collector)
	lua_pushstring(pLuaState, "__gc");
	lua_pushcfunction(pLuaState, luaCGMetamethodCallback);
	lua_rawset(pLuaState, -3);

	// Lua __call metamethod callback (called when Lua calls a value)
	lua_pushstring(pLuaState, "__call");
	lua_pushcfunction(pLuaState, luaCallMetamethodCallback);
	lua_rawset(pLuaState, -3);

	// Lua __tostring metamethod callback (called when Lua tries to convert the user data into a string)
	lua_pushstring(pLuaState, "__tostring");
	lua_pushcfunction(pLuaState, luaToStringMetamethodCallback);
	lua_rawset(pLuaState, -3);

	// Lua __eq metamethod callback (called when Lua tries to check for equality)
	lua_pushstring(pLuaState, "__eq");
	lua_pushcfunction(pLuaState, luaEqualityMetamethodCallback);
	lua_rawset(pLuaState, -3);

	lua_rawset(pLuaState, LUA_REGISTRYINDEX);
}

/**
*  @brief
*    Destroys the metatable for LuaUserData events
*/
void LuaUserData::destroyMetatable(lua_State *pLuaState)
{
	lua_pushlightuserdata(pLuaState, &LuaMetatable);
	lua_pushnil(pLuaState);
	lua_rawset(pLuaState, LUA_REGISTRYINDEX);
}

/**
*  @brief
*    Returns user data from the Lua stack without removing it
*/
LuaUserData *LuaUserData::getUserDataFromLuaStack(lua_State *pLuaState, int nIndex)
{
	// Must be a metatable
	if (lua_getmetatable(pLuaState, nIndex)) {
		lua_pushlightuserdata(pLuaState, &LuaMetatable);
		lua_rawget(pLuaState, LUA_REGISTRYINDEX);
		if (lua_rawequal(pLuaState, -2, -1)) {
			lua_pop(pLuaState, 2);
			LuaUserData **ppLuaUserData = static_cast<LuaUserData**>(lua_touserdata(pLuaState, nIndex));
			return *ppLuaUserData;
		}
		lua_pop(pLuaState, 1);
	}
	lua_pop(pLuaState, 1);

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns user data from the top of the Lua stack and removes it from the Lua stack
*/
LuaUserData *LuaUserData::popUserDataFromLuaStack(lua_State *pLuaState)
{
	// Must be a metatable
	if (lua_getmetatable(pLuaState, -1)) {
		lua_pushlightuserdata(pLuaState, &LuaMetatable);
		lua_rawget(pLuaState, LUA_REGISTRYINDEX);
		if (lua_rawequal(pLuaState, -2, -1)) {
			lua_pop(pLuaState, 2);
			LuaUserData **ppLuaUserData = static_cast<LuaUserData**>(lua_touserdata(pLuaState, -1));
			lua_pop(pLuaState, 1);
			return *ppLuaUserData;
		}
		lua_pop(pLuaState, 1);
	}
	lua_pop(pLuaState, 1);
	lua_pop(pLuaState, 1);

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Return the Lua user data type
*/
LuaUserData::EType LuaUserData::getType() const
{
	return m_nType;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
LuaUserData::LuaUserData(EType nType) :
	m_pScript(nullptr),	// Set known value, "initializeInstance()" setting valid data will be called shortly
	m_nType(nType)
{
}

/**
*  @brief
*    Destructor
*/
LuaUserData::~LuaUserData()
{
}

/**
*  @brief
*    Initializes this instance
*/
void LuaUserData::initializeInstance(Script &cScript)
{
	// Set given data
	m_pScript = &cScript;

	// Get the Lua state
	lua_State *pLuaState = m_pScript->getLuaState();

	// Allocate memory for a pointer to to object
	LuaUserData **ppLuaUserData = reinterpret_cast<LuaUserData**>(lua_newuserdata(pLuaState, sizeof(LuaUserData*)));

	// Set as data a pointer to this instance
	*ppLuaUserData = this;

	// Attach metatable
	lua_pushlightuserdata(pLuaState, &LuaMetatable);
	lua_rawget(pLuaState, LUA_REGISTRYINDEX);
	lua_setmetatable(pLuaState, -2);
}

/**
*  @brief
*    Copy operator
*/
LuaUserData &LuaUserData::operator =(const LuaUserData &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Private static Lua callback functions                 ]
//[-------------------------------------------------------]
/*
*  @brief
*    Lua __index metamethod callback (for table accesses)
*/
int LuaUserData::luaIndexMetamethodCallback(lua_State *pLuaState)
{
	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = getUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		return pLuaUserData->indexMetamethod(pLuaState);
	}

	// Error!
	return 0;
}

/*
*  @brief
*    Lua __newindex metamethod callback (for table updates)
*/
int LuaUserData::luaNewIndexMetamethodCallback(lua_State *pLuaState)
{
	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = getUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		return pLuaUserData->newIndexMetamethod(pLuaState);
	}

	// Error!
	return 0;
}

/*
*  @brief
*    Lua __gc metamethod callback (finalizer/destructor called by garbage collector)
*/
int LuaUserData::luaCGMetamethodCallback(lua_State *pLuaState)
{
	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = getUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		pLuaUserData->cgMetamethod(pLuaState);
	}

	// Done
	return 0;
}

/*
*  @brief
*    Lua __call metamethod callback (called when Lua calls a value)
*/
int LuaUserData::luaCallMetamethodCallback(lua_State *pLuaState)
{
	// Get the current top of the Lua stack
	const int nLuaStackTop = lua_gettop(pLuaState);

	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = getUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		pLuaUserData->callMetamethod(pLuaState);
	}

	// Done, inform Lua what's on the stack
	return lua_gettop(pLuaState) - nLuaStackTop;
}

/*
*  @brief
*    Lua __tostring metamethod callback (called when Lua tries to convert the user data into a string)
*/
int LuaUserData::luaToStringMetamethodCallback(lua_State *pLuaState)
{
	// Get the current top of the Lua stack
	const int nLuaStackTop = lua_gettop(pLuaState);

	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = getUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		pLuaUserData->toStringMetamethod(pLuaState);
	}

	// Done, inform Lua what's on the stack
	return lua_gettop(pLuaState) - nLuaStackTop;
}

/*
*  @brief
*    Lua __eq metamethod callback (called when Lua tries to check for equality)
*/
int LuaUserData::luaEqualityMetamethodCallback(lua_State *pLuaState)
{
	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = getUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		return pLuaUserData->equalityMetamethod(pLuaState);
	}

	// Error!
	return 0;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
LuaUserData::LuaUserData(const LuaUserData &cSource)
{
	// No implementation because the copy constructor is never used
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua
