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
	#include <Lua/lualib.h>
	#include <Lua/lauxlib.h>
}
#include <RECore/Log/Log.h>
#include "REScriptLua/LuaContext.h"
#include "REScriptLua/RTTIObjectPointer.h"
#include "REScriptLua/RTTIObjectMethodPointer.h"
#include "REScriptLua/Script.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace RECore;
namespace REScriptLua {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(Script, "REScriptLua", RECore::Script, "Lua (http://www.lua.org/) script implementation")
	// Properties
	re_properties
		re_property("Language",	"Lua")
		re_property("Formats",	"lua,LUA")
	re_properties_end
	// Constructors
	re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
re_class_metadata_end(Script)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Script::Script() :
	m_pLuaState(nullptr),
	m_bFunctionResult(false),
	m_nCurrentArgument(0)
{
	// Add a context reference
	LuaContext::addContextReference();
}

/**
*  @brief
*    Destructor
*/
Script::~Script()
{
	// Clear the script
	clear();

	// Remove all global functions
	removeAllGlobalFunctions();

	// Release a context reference
	LuaContext::releaseContextReference();
}

/**
*  @brief
*    Returns the Lua state
*/
lua_State *Script::getLuaState() const
{
	return m_pLuaState;
}

/**
*  @brief
*    Writes the current Lua stack content into the log
*/
void Script::luaStackDump()
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Get the number of elements on the Lua stack
		const int nNumOfStackElements = lua_gettop(m_pLuaState);

		// Write this number into the log
		logOutput(Log::Info, String("Number of elements on the Lua stack: ") + nNumOfStackElements);

		// Iterate through the Lua stack
		for (int i=1; i<=nNumOfStackElements; i++) {
			const int nLuaType = lua_type(m_pLuaState, i);
			String sValue;
			switch (nLuaType) {
				case LUA_TNIL:
					sValue = "nil";
					break;

				case LUA_TNUMBER:
					sValue = lua_tonumber(m_pLuaState, i);
					break;

				case LUA_TBOOLEAN:
					sValue = lua_toboolean(m_pLuaState, i) ? "true" : "false";
					break;

				case LUA_TSTRING:
					sValue = lua_tostring(m_pLuaState, i);
					break;

				case LUA_TTABLE:
					sValue = "Table";
					break;

				case LUA_TFUNCTION:
					sValue = lua_iscfunction(m_pLuaState, i) ? "C-function" : "Lua-function";
					break;

				case LUA_TUSERDATA:
					sValue = "User data";
					break;

				case LUA_TTHREAD:
					sValue = "Thread";
					break;

				case LUA_TLIGHTUSERDATA:
					sValue = "Light user data";
					break;

				default:
					sValue = "?";
					break;
			}
			logOutput(Log::Info, String("Lua stack element ") + (i-1) + ": \"" + sValue + "\" (Lua type name: \"" + lua_typename(m_pLuaState, nLuaType) + "\")");
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual RECore::Script functions               ]
//[-------------------------------------------------------]
bool Script::isGlobalFunction(const String &sName, const String &sNamespace)
{
	bool bIsGlobalFunction = false;	// By default, the global script function does not exist

	// Is there a Lua state? If so, get a nested Lua table
	if (m_pLuaState && getNestedTable(sNamespace)) {
		// Table key
		lua_pushstring(m_pLuaState, sName);	// Push the function name onto the Lua stack

		// This function pops the key from the stack - this gets the function
		lua_gettable(m_pLuaState, -2);

		// Is it a function?
		bIsGlobalFunction = lua_isfunction(m_pLuaState, -1);

		// Pop the function from the Lua state stack
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}

	// Done
	return bIsGlobalFunction;
}

bool Script::addGlobalFunction(const String &sFunction, const DynFunc &cDynFunc, const String &sNamespace)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Error!
		return false;
	} else {
		// Add the global function
		GlobalFunction *psGlobalFunction = new GlobalFunction;
		psGlobalFunction->pScript	 = this;
		psGlobalFunction->sFunction  = sFunction;
		psGlobalFunction->pDynFunc   = cDynFunc.clone();
		psGlobalFunction->sNamespace = sNamespace;
		m_lstGlobalFunctions.push_back(psGlobalFunction);

		// Done
		return true;
	}
}

bool Script::removeAllGlobalFunctions()
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Error!
		return false;
	} else {
		// Destroy the global functions
		for (uint32 i=0; i<m_lstGlobalFunctions.size(); i++) {
			delete m_lstGlobalFunctions[i]->pDynFunc;
			delete m_lstGlobalFunctions[i];
		}
		m_lstGlobalFunctions.clear();

		// Done
		return true;
	}
}

String Script::getSourceCode() const
{
	return m_sSourceCode;
}

bool Script::setSourceCode(const String &sSourceCode)
{
	// Clear the previous script
	clear();

	// Backup the given source code
	m_sSourceCode = sSourceCode;

	// Is there source code?
	if (m_sSourceCode.length()) {
		// Create the Lua state
		m_pLuaState = luaL_newstate();
		if (m_pLuaState) {
			// Open all standard Lua libraries into the given state
			luaL_openlibs(m_pLuaState);

			// Create the metatable for LuaUserData events
			LuaUserData::createMetatable(m_pLuaState);

			// Add the global functions
			for (uint32 i=0; i<m_lstGlobalFunctions.size(); i++) {
				// Get the global function
				GlobalFunction *psGlobalFunction = m_lstGlobalFunctions[i];

				// Create a nested Lua table
				if (createNestedTable(psGlobalFunction->sNamespace)) {
					// Table key
					lua_pushstring(m_pLuaState, psGlobalFunction->sFunction);		// Push the function name onto the Lua stack

					// Table value: Store a pointer to the global function in the c-closure
					lua_pushlightuserdata(m_pLuaState, psGlobalFunction);			// Push a pointer to the global function onto the Lua stack
					lua_pushcclosure(m_pLuaState, &Script::luaFunctionCallback, 1);	// Push the function pointer onto the Lua stack

					// This function pops both the key and the value from the stack
					lua_settable(m_pLuaState, -3);

					// Pop the table from the Lua stack
					lua_pop(m_pLuaState, 1);
				}
			}

			// Load the script
			const int nResult = luaL_loadbuffer(m_pLuaState, sSourceCode, sSourceCode.length(), Name.get());
			if (!nResult && !lua_pcall(m_pLuaState, 0, 0, 0)) {
				// Done
				return true;
			} else {
				// Error!

				// Write a log message
				String sErrorDescription;
				switch (nResult) {
					case LUA_ERRSYNTAX:
						sErrorDescription = " (Syntax error during pre-compilation)";
						break;

					case LUA_ERRMEM:
						sErrorDescription = " (Memory allocation error)";
						break;
				}
				logOutput(Log::Error, "Failed to compile the script" + sErrorDescription);

				// Report Lua errors
				reportErrors();
			}
		}
	} else {
		// No script at all - done
		return true;
	}

	// Error!
	return false;
}

void Script::getAssociatedFilenames(std::vector<String> &lstFilenames)
{
	// We want to have a list of script filenames which were included within this script
	// -> It appears that there's no "easy" way in Lua to get this kind of information :/

	// Contains "Application", "Interaction" and so on (no final filenames)
  std::vector<String> lstRequire;

	// Get a list of loaded "require"-files
	{ // -> The files loaded within a Lua script by using "require" can be accessed by using the
		//    global control table variable "_LOADED". See http://www.lua.org/pil/8.1.html for details.
		lua_getfield(m_pLuaState, LUA_REGISTRYINDEX, "_LOADED");
		if (lua_istable(m_pLuaState, -1)) {
			lua_pushnil(m_pLuaState);
			while (lua_next(m_pLuaState, -2)) {
				if (lua_isstring(m_pLuaState, -2))
					lstRequire.push_back(lua_tostring(m_pLuaState, -2));
				lua_pop(m_pLuaState, 1);
			}
		}

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}

	// Get the content of "package.path" used by "require" to search for a Lua loader
	// -> The content looks like "?.lua;C:\SomePath\?.lua;"
	const String sPackagePath = getGlobalVariable("path", "package");

	// Iterate over the "require"-list
	const String sToReplace = "?.";
	for (uint32 i=0; i<lstRequire.size(); i++) {
		// Get the current "require"
		const String sRequire = lstRequire[i] + '.';

		// Get the index of the first ";" within the package path
		int nPreviousIndex = 0;
		int nIndex = sPackagePath.findFirstOf(';');
		while (nIndex>RECore::String::NPOS) {
			// Get current package search path, we now have e.g. "C:\SomePath\?.lua"
			String sFilename = sPackagePath.substr(nPreviousIndex, nIndex-nPreviousIndex);

			// Replace "?." with the "require"-name
			sFilename.replaceAll(sToReplace, sRequire);

			// Does this file exist?
			if (true) {
				// We found a match!
				lstFilenames.push_back(sFilename);

				// Get us out of the while-loop
				nIndex = -1;
			} else {
				// Get the index of the next ";" within the package path
				nPreviousIndex = nIndex + 1;
				nIndex = sPackagePath.findFirstOf(';', nPreviousIndex);
			}
		}
	}
}

bool Script::execute(const String &sSourceCode)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Execute
		const bool bResult = !luaL_dostring(m_pLuaState, sSourceCode.cstr());

		// -> For now we don't want to bother the script API user with something
		//    like garbage collection, so do this automatically
		// -> If this is not done, reuse of "RTTIObjectPointer"-instances (as well as derived classes of course)
		//    will not work that well because unused stuff will not be removed automatically by Lua when not asking
		//    Lua to do so
		lua_gc(m_pLuaState, LUA_GCSTEP, 1);

		// Done
		return bResult;
	}

	// Error!
	return false;
}

void Script::getGlobalVariables(std::vector<String> &lstGlobalVariables, const String &sNamespace)
{
	// Is there a Lua state? If so, get a nested Lua table
	if (m_pLuaState && getNestedTable(sNamespace)) {
		// Push the first key onto the Lua stack
		lua_pushnil(m_pLuaState);

		// Iterate through the Lua table
		while (lua_next(m_pLuaState, 1) != 0) {
			// Lua stack content: The 'key' is at index -2 and the 'value' at index -1

			// Check the 'key' type (at index -2) - must be a string
			if (lua_isstring(m_pLuaState, -2)) {
				// Check whether or not the 'value' (at index -1) is a global variable
				// (something like "_VERSION" is passing this test as well, but that's probably ok because it's just a Lua build in global variable)
				if (lua_isnumber(m_pLuaState, -1) || lua_isstring(m_pLuaState, -1)) {
					// Add the global variable to our list
					lstGlobalVariables.push_back(lua_tostring(m_pLuaState, -2));
				}
			}

			// Next, please (removes 'value'; keeps 'key' for next iteration)
			lua_pop(m_pLuaState, 1);
		}

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}
}

bool Script::isGlobalVariable(const String &sName, const String &sNamespace)
{
	bool bGlobalVariable = false;

	// Is there a Lua state? If so, get a nested Lua table
	if (m_pLuaState && getNestedTable(sNamespace)) {
		// Table key
		lua_pushstring(m_pLuaState, sName);	// Push the variable name onto the Lua stack

		// This function pops the key from the stack - this gets the variable
		lua_gettable(m_pLuaState, -2);

		// Check the type of the variable
		bGlobalVariable = (lua_isnumber(m_pLuaState, -1) || lua_isstring(m_pLuaState, -1));

		// Pop the variable from the Lua state stack
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}

	// Done
	return bGlobalVariable;
}

ETypeID Script::getGlobalVariableTypeID(const String &sName, const String &sNamespace)
{
	ETypeID nType = TypeInvalid;

	// Is there a Lua state? If so, get a nested Lua table
	if (m_pLuaState && getNestedTable(sNamespace)) {
		// Table key
		lua_pushstring(m_pLuaState, sName);	// Push the variable name onto the Lua stack

		// This function pops the key from the stack - this gets the variable
		lua_gettable(m_pLuaState, -2);

		// Check the type of the variable
		if (lua_isboolean(m_pLuaState, -1))
			nType = TypeBool;
		else if (lua_isnumber(m_pLuaState, -1))
			nType = TypeDouble;
		else if (lua_isstring(m_pLuaState, -1))
			nType = TypeString;
		else if (lua_isuserdata(m_pLuaState, -1))
			nType = TypeObjectPtr;	// [TODO] Do any type tests in here?

		// Pop the variable from the Lua state stack
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}

	// Done
	return nType;
}

String Script::getGlobalVariable(const String &sName, const String &sNamespace)
{
	String sValue;

	// Is there a Lua state? If so, get a nested Lua table
	if (m_pLuaState && getNestedTable(sNamespace)) {
		// Table key
		lua_pushstring(m_pLuaState, sName);	// Push the variable name onto the Lua stack

		// This function pops the key from the stack - this gets the variable
		lua_gettable(m_pLuaState, -2);

		// Get the value of the variable as string
		sValue = lua_tostring(m_pLuaState, -1);

		// Pop the variable from the Lua state stack
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}

	// Done
	return sValue;
}

void Script::setGlobalVariable(const String &sName, const DynVar &cValue, const String &sNamespace)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Get the type of the variable because we don't want to change it's type
		int nTypeID = getGlobalVariableTypeID(sName);
		if (nTypeID == TypeInvalid) {
			// Ok, this must be a new variable
			nTypeID = cValue.getTypeID();
		}

		// Create a nested Lua table
		if (createNestedTable(sNamespace)) {
			// Table key
			lua_pushstring(m_pLuaState, sName);	// Push the variable name onto the Lua stack

			// Table value: Push the value of the variable onto the Lua stack
			switch (nTypeID) {
				case TypeVoid:																								return;	// ? Yeah, that's really funny!
				case TypeBool:		lua_pushboolean(m_pLuaState, cValue.getBool());											break;
				case TypeDouble:	lua_pushnumber (m_pLuaState, cValue.getDouble());										break;
				case TypeFloat:		lua_pushnumber (m_pLuaState, cValue.getFloat());										break;
				case TypeInt:		lua_pushinteger(m_pLuaState, cValue.getInt());											break;
				case TypeInt16:		lua_pushinteger(m_pLuaState, cValue.getInt());											break;
				case TypeInt32:		lua_pushinteger(m_pLuaState, cValue.getInt());											break;
				case TypeInt64:		lua_pushinteger(m_pLuaState, cValue.getInt());											break;	// [TODO] TypeInt64 is currently handled just as long
				case TypeInt8:		lua_pushinteger(m_pLuaState, cValue.getInt());											break;
				case TypeString:	lua_pushstring (m_pLuaState, cValue.getString());										break;
				case TypeUInt16:	lua_pushinteger(m_pLuaState, cValue.getUInt16());										break;
				case TypeUInt32:	lua_pushinteger(m_pLuaState, cValue.getUInt32());										break;
				case TypeUInt64:	lua_pushinteger(m_pLuaState, static_cast<lua_Integer>(cValue.getUInt64()));				break;	// [TODO] TypeUInt64 is currently handled just as long
				case TypeUInt8:		lua_pushinteger(m_pLuaState, cValue.getUInt8());										break;

				// [HACK] Currently, classes derived from "RECore::Object" are just recognized as type "void*"... but "RECore::Object*" type would be perfect
				case TypePtr:
				case TypeObjectPtr:	RTTIObjectPointer::LuaStackPush(*this, reinterpret_cast<Object*>(cValue.getUIntPtr()));	break;

				default:			lua_pushstring (m_pLuaState, cValue.getString());										break;	// Unknown type
			}

			// This function pops both the key and the value from the stack - this sets the variable
			lua_settable(m_pLuaState, -3);

			// Pop the table from the Lua stack
			lua_pop(m_pLuaState, 1);
		}
	}
}

bool Script::beginCall(const String &sFunctionName, const String &sFunctionSignature, const String &sNamespace)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Get a nested Lua table
		if (getNestedTable(sNamespace)) {
			// Table key
			lua_pushstring(m_pLuaState, sFunctionName);	// Push the function name onto the Lua stack

			// Push the function to be called onto the Lua state stack
			lua_gettable(m_pLuaState, -2);
			if (lua_isfunction(m_pLuaState, -1)) {
				// Backup the name of the current function (we may need it for error log output)
				m_sCurrentFunction = sFunctionName;

				// Has the current function a result?
				m_bFunctionResult = (sFunctionSignature.compare(0, 5, "void(") != 0);

				// Current argument is 0
				m_nCurrentArgument = 0;

				// Done
				return true;
			} else {
				// Error!
				if (sNamespace.length())
					logOutput(Log::Error, "The function '" + sFunctionName + "' was not found within the namespace '" + sNamespace + '\'');
				else
					logOutput(Log::Error, "The function '" + sFunctionName + "' was not found");
			}
		} else {
			// Error!
			logOutput(Log::Error, "The namespace '" + sNamespace + "' of the function '" + sFunctionName + "' was not found");
		}
	}

	// Error!
	return false;
}

void Script::pushArgument(bool bValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushboolean(m_pLuaState, bValue);
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(float fValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushnumber(m_pLuaState, fValue);
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(double fValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushnumber(m_pLuaState, fValue);
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(int8 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(int16 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(int32 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(int64 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// [TODO] There's no int64 support in Lua (?)
		lua_pushinteger(m_pLuaState, static_cast<lua_Integer>(nValue));
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(uint8 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(uint16 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(uint32 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(uint64 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// [TODO] There's no uint64 support in Lua (?)
		lua_pushinteger(m_pLuaState, static_cast<lua_Integer>(nValue));
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(const String &sString)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushstring(m_pLuaState, sString);
		m_nCurrentArgument++;
	}
}

void Script::pushArgument(Object *pObject)
{
	RTTIObjectPointer::LuaStackPush(*this, pObject);
	m_nCurrentArgument++;
}

void Script::pushArgument(Object &cObject)
{
	RTTIObjectPointer::LuaStackPush(*this, &cObject);
	m_nCurrentArgument++;
}

bool Script::endCall()
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Do the call ('m_nCurrentArgument' arguments, 1 result)
		const int nResult = lua_pcall(m_pLuaState, m_nCurrentArgument, m_bFunctionResult, 0);

		// Perform an incremental step of garbage collection
		// -> For now we don't want to bother the script API user with something
		//    like garbage collection, so do this automatically
		// -> If this is not done, reuse of "RTTIObjectPointer"-instances (as well as derived classes of course)
		//    will not work that well because unused stuff will not be removed automatically by Lua when not asking
		//    Lua to do so
		lua_gc(m_pLuaState, LUA_GCSTEP, 1);

		// Evaluate the result
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
			logOutput(Log::Error, "Error running function '" + m_sCurrentFunction + '\'' + sErrorDescription);

			// Report Lua errors
			reportErrors();
		} else {
			// If there's no function result we need to pop the table from the Lua stack right now, else this is done within the "getReturn()"-method
			if (!m_bFunctionResult)
				lua_pop(m_pLuaState, -1);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool Script::getReturn(bool nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a boolean
		if (lua_isboolean(m_pLuaState, -1)) {
			// Get the result
			nValue = (lua_toboolean(m_pLuaState, -1) != 0);
		} else {
			// Error!
			nValue = false;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a boolean");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = false;
	}

	// Done
	return nValue;
}

float Script::getReturn(float nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<float>(lua_tonumber(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0.0f;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0.0f;
	}

	// Done
	return nValue;
}

double Script::getReturn(double nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = lua_tonumber(m_pLuaState, -1);
		} else {
			// Error!
			nValue = 0.0;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0.0;
	}

	// Done
	return nValue;
}

int8 Script::getReturn(int8 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<uint8>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

int16 Script::getReturn(int16 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<uint16>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

int32 Script::getReturn(int32 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the value
			nValue = static_cast<int32>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

int64 Script::getReturn(int64 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// [TODO] There's no int64 support in Lua (?)
			// Get the result
			nValue = lua_tointeger(m_pLuaState, -1);
		} else {
			// Error!
			nValue = 0;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

uint8 Script::getReturn(uint8 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<uint8>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

uint16 Script::getReturn(uint16 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<uint16>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

uint32 Script::getReturn(uint32 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<uint32>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

uint64 Script::getReturn(uint64 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// [TODO] There's no uint64 support in Lua (?)
			// Get the result
			nValue = lua_tointeger(m_pLuaState, -1);
		} else {
			// Error!
			nValue = 0;
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

String Script::getReturn(String nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a string
		if (lua_isstring(m_pLuaState, -1)) {
			// Get the result
			nValue = lua_tostring(m_pLuaState, -1);
		} else {
			// Error!
			nValue = "";
			logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a string");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = "";
	}

	// Done
	return nValue;
}

Object *Script::getReturn(Object *nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be user data or nil
		if (lua_isuserdata(m_pLuaState, -1)) {
			// Get user data from the top of the Lua stack and removes it from the Lua stack
			// [TODO] Do any type tests in here?
			nValue = reinterpret_cast<RTTIObjectPointer*>(LuaUserData::popUserDataFromLuaStack(m_pLuaState))->getObject();
		} else {
			// Nil is fine, too
			nValue = nullptr;
			if (!lua_isnil(m_pLuaState, -1)) {
				// Error!
				logOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return user data");
			}
			lua_pop(m_pLuaState, 1);
		}

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = nullptr;
	}

	// Done
	return nValue;
}

Object &Script::getReturn(Object &nValue)
{
	// ... please note that in here, we can't return a null pointer...
	Object *pObject = getReturn(&nValue);
	return pObject ? *pObject : nValue;
}


//[-------------------------------------------------------]
//[ Private static Lua callback functions                 ]
//[-------------------------------------------------------]
/*
*  @brief
*    Lua function callback
*/
int Script::luaFunctionCallback(lua_State *pLuaState)
{
	// Get the global function
	GlobalFunction *psGlobalFunction = reinterpret_cast<GlobalFunction*>(lua_touserdata(pLuaState, lua_upvalueindex(1)));

	// Call the dynamic function
	return psGlobalFunction ? RTTIObjectMethodPointer::callDynFunc(*psGlobalFunction->pScript, *psGlobalFunction->pDynFunc, false) : 0;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Script::Script(const Script &cSource) :
	m_pLuaState(nullptr),
	m_bFunctionResult(false),
	m_nCurrentArgument(0)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Script &Script::operator =(const Script &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Reports Lua errors
*/
void Script::reportErrors()
{
	// Output the Lua errors
	logOutput(Log::Error, lua_tostring(m_pLuaState, -1));

	// Pop the error value from the stack
	lua_pop(m_pLuaState, 1);
}

/**
*  @brief
*    Clears the script
*/
void Script::clear()
{
	// Is there a Lua state?
	if (m_pLuaState) {
		//  Destroy all registered event user data (a kind of "disconnect all slots at once")
		destroyEventUserData();

		// Reset the source code
		m_sSourceCode = "";

		// Verify the stack and write a warning into the log if the script stack is not empty
		if (lua_gettop(m_pLuaState)) {
			logOutput(Log::Warning, "Script termination, but the stack is not empty");
			luaStackDump();
		}

		// Clear all global variables
		while (true) {
			lua_pushnil(m_pLuaState);
			if (!lua_rawgeti(m_pLuaState, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS)) // lua_next(m_pLuaState, LUA_GLOBALSINDEX))
				break;
			lua_pop(m_pLuaState, 1);
			lua_pushnil(m_pLuaState);
      lua_rawseti(m_pLuaState, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
		}

		// Force the garbage collector to cleanup right now... because required stuff like the LuaUserData metatable are still there at the moment...
		lua_gc(m_pLuaState, LUA_GCCOLLECT, 0);

		// Destroy the metatable for LuaUserData events
		LuaUserData::destroyMetatable(m_pLuaState);

		// Close the Lua state
		lua_close(m_pLuaState);
		m_pLuaState = nullptr;

		// Reset states
		m_sCurrentFunction = "";
		m_bFunctionResult  = false;
		m_nCurrentArgument = 0;
	}
}

/**
*  @brief
*    Creates a nested Lua table
*/
bool Script::createNestedTable(const String &sTableName)
{
	// Is there a table name?
	if (sTableName.length()) {
		// Loop through all components within the given nested Lua table name
		uint32 nPartBegin = 0;
		while (nPartBegin<=sTableName.length()) {
			// Find the next "." within the given nested Lua table name
			int nPartEnd = sTableName.findFirstOf(".", nPartBegin);
			if (nPartEnd == RECore::String::NPOS)
				nPartEnd = sTableName.length();

			// Get the current Lua table name
			const String sSubTableName = sTableName.substr(nPartBegin, nPartEnd - nPartBegin);

			// Each table must have a name!
			if (!sSubTableName.length())
				return false;	// Error!

			// Does the Lua table already exist within the current Lua table?
			if (nPartBegin) {
				// We're already within a Lua table
				lua_pushstring(m_pLuaState, sSubTableName);
				lua_gettable(m_pLuaState, -2);
				if (!lua_isnil(m_pLuaState, -1))
					lua_remove(m_pLuaState, -2);
			} else {
				// Currently we're in global space
				lua_pushstring(m_pLuaState, sSubTableName);
        lua_rawgeti(m_pLuaState, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
			}

			// Lua table found?
			if (lua_isnil(m_pLuaState, -1)) {
				// Nope, create a new one

				// Pop nil from the Lua stack
				lua_pop(m_pLuaState, 1);

				// Create new Lua table on the Lua stack
				lua_newtable(m_pLuaState);
				lua_pushstring(m_pLuaState, sSubTableName);
				lua_pushvalue(m_pLuaState, -2);	// Pushes a copy of the element at the given valid index onto the stack
				if (nPartBegin) {
					// We're already within a Lua table
					lua_settable(m_pLuaState, -4);
					lua_remove(m_pLuaState, -2);
				} else {
					// Currently we're in global space
          lua_rawseti(m_pLuaState, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
				}
			}

			// Skip "."
			nPartBegin = nPartEnd + 1;
		}
	} else {
		// No table name given, so just push the global Lua table onto the stack
		lua_getglobal(m_pLuaState, "_G");
		if (!lua_istable(m_pLuaState, -1))
			return false;	// Error!
	}

	// Done
	return true;
}

/**
*  @brief
*    Gets a nested Lua table
*/
bool Script::getNestedTable(const String &sTableName)
{
	// Is there a table name?
	if (sTableName.length()) {
		// Loop through all components within the given nested Lua table name
		uint32 nPartBegin = 0;
		while (nPartBegin<=sTableName.length()) {
			// Find the next "." within the given nested Lua table name
			int nPartEnd = sTableName.findFirstOf(".", nPartBegin);
			if (nPartEnd == RECore::String::NPOS)
				nPartEnd = sTableName.length();

			// Get the current Lua table name
			const String sSubTableName = sTableName.substr(nPartBegin, nPartEnd - nPartBegin);

			// Each table must have a name!
			if (!sSubTableName.length())
				return false;	// Error!

			// Does the Lua table already exist within the current Lua table?
			if (nPartBegin) {
				// We're already within a Lua table
				lua_pushstring(m_pLuaState, sSubTableName);
				lua_gettable(m_pLuaState, -2);
				if (!lua_isnil(m_pLuaState, -1))
					lua_remove(m_pLuaState, -2);
			} else {
				// Currently we're in global space
				lua_pushstring(m_pLuaState, sSubTableName);
        lua_rawgeti(m_pLuaState, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
			}

			// Lua table found?
			if (lua_isnil(m_pLuaState, -1)) {
				// Nope, don't create a new one in here

				// Pop nil from the Lua stack
				lua_pop(m_pLuaState, 1);

				// Cleanup the Lua stack
				if (nPartBegin) {
					// We're already within a Lua table

					// Pop the table from the Lua stack
					lua_pop(m_pLuaState, 1);
				} else {
					// Currently we're in global space
				}

				// Error!
				return false;
			}

			// Skip "."
			nPartBegin = nPartEnd + 1;
		}
	} else {
		// No table name given, so just push the global Lua table onto the stack
		lua_getglobal(m_pLuaState, "_G");
		if (!lua_istable(m_pLuaState, -1))
			return false;	// Error!
	}

	// Done
	return true;
}

/**
*  @brief
*    Returns event user data key
*/
String Script::getEventUserDataKey(DynEventPtr pDynEvent, const void *pLuaPointer) const
{
	return String().appendFormat("%p", pDynEvent.GetPointer()) + String().appendFormat("%p", const_cast<void*>(pLuaPointer));
}

/**
*  @brief
*    Returns event user data
*/
Script::EventUserData *Script::getEventUserData(DynEventPtr pDynEvent, const void *pLuaPointer) const
{
	return m_mapEventUserData.find(getEventUserDataKey(pDynEvent, pLuaPointer))->second;
}

/**
*  @brief
*    Adds event user data
*/
void Script::addEventUserData(DynEventPtr pDynEvent, const void *pLuaPointer, EventUserData *pEventUserData)
{
	m_mapEventUserData.emplace(getEventUserDataKey(pDynEvent, pLuaPointer), pEventUserData);
}

/**
*  @brief
*    Destroys all registered event user data (a kind of "disconnect all slots at once")
*/
void Script::destroyEventUserData()
{
	// Iterate through the map content
	auto cKeyIterator = m_mapEventUserData.begin();

	for (;cKeyIterator != m_mapEventUserData.end(); ++cKeyIterator) {
		// Get the event user data instance
		EventUserData *pEventUserData = m_mapEventUserData.find(cKeyIterator->first)->second;
		
		// Destroy the event user data
		luaL_unref(m_pLuaState, LUA_REGISTRYINDEX, pEventUserData->nLuaFunctionReference);
		delete pEventUserData;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptLua
