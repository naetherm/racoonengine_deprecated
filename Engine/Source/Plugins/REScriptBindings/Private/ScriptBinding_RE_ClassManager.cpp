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
#include <RECore/Reflect/Class.h>
#include <RECore/Reflect/ClassManager.h>
#include "REScriptBindings/ScriptBinding_RE_ClassManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace RECore;
namespace REScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(ScriptBinding_RE_ClassManager, "REScriptBindings", RECore::ScriptBinding, "Class manager script binding class")
	// Properties
	re_properties
		re_property("Namespace",	"PL.ClassManager")
	re_properties_end
	// Constructors
	re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	re_method_3_metadata(scanPlugins,			re_ret_type(bool),				RECore::String,	bool,			bool,							"Scan a directory for compatible plugins and load them in. Directory to search in as first parameter, boolean value deciding whether or not to take sub-directories into account as second parameter, boolean value deciding whether or not its allowed to perform delayed shared library loading to speed up the program start as third parameter. Returns 'true' if all went fine, else 'false'.",																																							"")
	re_method_2_metadata(create,				re_ret_type(RECore::Object*),	RECore::String,	RECore::String,									"creates a new RTTI class instance by using the default constructor. Name of the RTTI class to create an instance from as first parameter and optional parameter string for the created instance as second parameter. Returns a pointer to the new RTTI class instance or a null pointer if something went wrong (maybe unknown class). created instance has initially no references, meaning that a script usually automatically destroys the instance when no longer used.",																					"")
	re_method_4_metadata(createByConstructor,	re_ret_type(RECore::Object*),	RECore::String,	RECore::String,	RECore::String,	RECore::String,	"creates a new RTTI class instance by using a specified constructor. Name of the RTTI class to create an instance from as first parameter, constructor name as second parameter, constructor parameters as third parameter and optional parameter string for the created instance as fourth parameter. Returns a pointer to the new RTTI class instance or a null pointer if something went wrong (maybe unknown class). created instance has initially no references, meaning that a script usually automatically destroys the instance when no longer used.",	"")
re_class_metadata_end(ScriptBinding_RE_ClassManager)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ScriptBinding_RE_ClassManager::scanPlugins(String sPath, bool bRecursive, bool bDelayedPluginLoading)
{
	return m_pClassManager->scanPlugins(sPath, bRecursive ? Recursive : NonRecursive, bDelayedPluginLoading);
}

Object *ScriptBinding_RE_ClassManager::create(String sClass, String sParameters)
{
	// Get the requested RTTI class
	const Class *pClass =  m_pClassManager->getClass(sClass);
	if (pClass) {
		// create an instance of the requested RTTI class
		Object *pObject = pClass->create();
		if (pObject) {
			// By default, a created RTTI class instance has an initial reference count of 1
			// -> This RTTI class instance was possibly created by a script, so, set an initial reference
			//    count of 0 so that the instance is destroyed automatically as soon as it's no longer referenced
			pObject->softRelease();

			// Set optional parameters
			if (pObject && sParameters.length())
				pObject->setValues(sParameters);

			// Return the created RTTI class instance
			return pObject;
		}
	}

	// Error!
	return nullptr;
}

Object *ScriptBinding_RE_ClassManager::createByConstructor(String sClass, String sConstructor, String sConstructorParameters, String sParameters)
{
	// Get the requested RTTI class
	const Class *pClass =  m_pClassManager->getClass(sClass);
	if (pClass) {
		// create an instance of the requested RTTI class
		Object *pObject = pClass->create(sConstructor, sConstructorParameters);
		if (pObject) {
			// By default, a created RTTI class instance has an initial reference count of 1
			// -> This RTTI class instance was possibly created by a script, so, set an initial reference
			//    count of 0 so that the instance is destroyed automatically as soon as it's no longer referenced
			pObject->softRelease();

			// Set optional parameters
			if (pObject && sParameters.length())
				pObject->setValues(sParameters);

			// Return the created RTTI class instance
			return pObject;
		}
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBinding_RE_ClassManager::ScriptBinding_RE_ClassManager() :
	m_pClassManager(&ClassManager::instance())
{
}

/**
*  @brief
*    Destructor
*/
ScriptBinding_RE_ClassManager::~ScriptBinding_RE_ClassManager()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REScriptBindings
