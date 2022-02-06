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
#include "RECore/Log/Log.h"
#include "RECore/Reflect/Class.h"
#include "RECore/Scripting/ScriptManager.h"
#include "RECore/Scripting/Script.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(Script, "RECore", RECore::Object, "Abstract script base class")
	// Properties
	re_properties
		re_property("Language",	"Unknown")
		re_property("Formats",	"Unknown")
	re_properties_end
	// Attributes
	re_attribute_metadata(Name,	String,	"",	ReadWrite,	"Name of this script, optional but recommended for better debugging",	"")
re_class_metadata_end(Script)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Script::~Script()
{
}

/**
*  @brief
*    Returns the name of the script language the script is using
*/
String Script::getScriptLanguage() const
{
  if (getClass()) {
    auto iter = getClass()->getProperties().find("Language");
    if (iter != getClass()->getProperties().end()) {
      return iter->second;
    }
  }
  return "";
}

/**
*  @brief
*    Returns a list of file formats this script supports
*/
void Script::getFormats(std::vector<String> &lstFormats) const
{
  auto iter = getClass()->getProperties().find("Formats");
  if (iter != getClass()->getProperties().end()) {
    const String sFormats = iter->second;
    if (sFormats.length()) {
      CTokenizer cTokenizer;
      cTokenizer.Start(sFormats);
      cTokenizer.SetDelimiters(" ,\t\r\n");
      cTokenizer.SetSingleChars("");
      String sToken = cTokenizer.GetNextToken();
      while (sToken.length()) {
        lstFormats.push_back(sToken);
        sToken = cTokenizer.GetNextToken();
      }
      cTokenizer.Stop();
    }
  }
}

/**
*  @brief
*    Adds a script binding to connect the given RTTI class instance with this script
*/
void Script::addBinding(Object &cObject, const String &sNamespace)
{
	// Get a list of all callable RTTI methods of the given RTTI class instance
	std::list<DynFuncPtr> lstMethods;
	cObject.getMethods(lstMethods);

	// Add all methods to this script
	std::list<DynFuncPtr>::iterator cIterator = lstMethods.begin();
	while (cIterator != lstMethods.end()) {
		// Get the dynamic function
		DynFunc *pDynFunc = *cIterator;

		// Get the function descriptor
		const FuncDesc *pFuncDesc = pDynFunc->getDesc();
		if (pFuncDesc)
			addGlobalFunction(pFuncDesc->getName(), *pDynFunc, sNamespace);

    // Increment
    ++cIterator;
	}
}

/**
*  @brief
*    Add all script bindings to this script
*/
void Script::addBindings()
{
	// Get a list of all script binding instances
	const std::vector<ScriptBinding*> &lstScriptBindings = ScriptManager::instance().getScriptBindings();
	for (uint32 i=0; i<lstScriptBindings.size(); i++) {
		Object *pScriptBinding = reinterpret_cast<Object*>(lstScriptBindings[i]);
    auto iter = pScriptBinding->getClass()->getProperties().find("Namespace");
    if (iter != pScriptBinding->getClass()->getProperties().end()) {
      addBinding(*pScriptBinding, iter->second);
    } else {
      addBinding(*pScriptBinding, "");
    }
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Script::Script() :
	Name(this)
{
}

/**
*  @brief
*    Write a string into the log
*/
bool Script::logOutput(uint8 nLogLevel, const String &sText)
{
	return Log::instance().output(nLogLevel, ((Name.get().length() ? ("Script '" + Name.get() + "': ") : "Script: ") + sText).cstr());
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Script::Script(const Script &cSource) :
	Name(this)
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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
