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
#include "RECore/Reflect/Var/VarDesc.h"
#include "RECore/Reflect/Var/DynVar.h"
#include "RECore/Reflect/Func/FuncDesc.h"
#include "RECore/Reflect/Event/EventDesc.h"
#include "RECore/Reflect/Event/EventHandlerDesc.h"
#include "RECore/Reflect/Event/DynEvent.h"
#include "RECore/Reflect/Event/DynEventHandler.h"
#include "RECore/Reflect/Tools/ParamsParser.h"
#include "RECore/Reflect/Tools/ParamsParserXml.h"
#include "RECore/Reflect/ClassManager.h"
#include "RECore/Reflect/Class.h"
#include "RECore/Reflect/Object.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata_internal(Object, "RECore", /* No base class */, "Object base class")
	// Methods
	re_method_1_metadata(isInstanceOf,			re_ret_type(bool),		const String&,					"Check if object is instance of a given class. Class name (with namespace) as first parameter. Returns 'true' if the object is an instance of the class or one of it's derived classes, else 'false'.",	"")
	re_method_2_metadata(setAttribute,			re_ret_type(void),		const String&,	const String&,	"Set attribute value. Attribute name as first parameter, attribute value as second parameter.",																											"")
	re_method_1_metadata(setAttributeDefault,	re_ret_type(void),		const String&,					"Set attribute to it's default value. Attribute name as first parameter.",																																"")
	re_method_2_metadata(callMethod,			re_ret_type(void),		const String&,	const String&,	"Call method. Method name as first parameter, parameters as string (e.g. \"Param0='x' Param1='y'\") as second parameter.",																				"")
	re_method_1_metadata(setValues,				re_ret_type(void),		const String&,					"Set multiple attribute values as a string at once. String containing attributes and values as first parameter (e.g. \"Name='Bob' Position='1 2 3'\").",												"")
	re_method_0_metadata(setDefaultValues,		re_ret_type(void),										"Set all attributes to default.",																																										"")
	re_method_0_metadata(toString,				re_ret_type(String),									"Get the object as string. Returns string representation of object.",																																	"")
	re_method_1_metadata(fromString,			re_ret_type(void),		const String&,					"Set the object from string. String representation of object as first parameter.",																														"")
	// Signals
	re_signal_0_metadata(SignalDestroyed,	"Object destroyed signal. When this signal is emitted the object is already in the destruction phase and parts may already be invalid. Best to e.g. only update our object pointer.",	"")
re_class_metadata_end(Object)


//[-------------------------------------------------------]
//[ Public ObjectBase functions                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ObjectBase::ObjectBase() :
	m_nRefCount(1)	// Not 0, see class comment
{
}

/**
*  @brief
*    Destructor
*/
ObjectBase::~ObjectBase()
{
}

/**
*  @brief
*    Get class
*/
Class *ObjectBase::getClass() const
{
	return nullptr;
}

/**
*  @brief
*    Get a pointer to the object
*/
const ObjectBase *ObjectBase::getPointer() const
{
	return this;
}

ObjectBase *ObjectBase::getPointer()
{
	return this;
}

/**
*  @brief
*    Increases the reference count
*/
uint32 ObjectBase::addReference()
{
	// Increment reference count
	m_nRefCount++;

	// Return current reference count
	return m_nRefCount;
}

/**
*  @brief
*    Decreases the reference count
*/
uint32 ObjectBase::release()
{
	// Decrement reference count
	if (m_nRefCount > 1) {
		m_nRefCount--;

		// Return current reference count
		return m_nRefCount;

	// Destroy object when no references are left
	} else {
		delete this;

		// This object is no longer
		return 0;
	}
}

/**
*  @brief
*    Gets the current reference count
*/
uint32 ObjectBase::getRefCount() const
{
	// Return current reference count
	return m_nRefCount;
}

/**
*  @brief
*    Decreases the reference count without destroying this instance automatically
*/
uint32 ObjectBase::softRelease()
{
	// Decrement reference count
	if (m_nRefCount > 1) {
		m_nRefCount--;

		// Return current reference count
		return m_nRefCount;

	// Destroy object when no references are left
	} else {
		// Unlike "Release()", do not destroy this instance
		// delete this;
		m_nRefCount = 0;

		// This object is no longer referenced
		return 0;
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Object::Object()
{
}

/**
*  @brief
*    Destructor
*/
Object::~Object()
{
	// emit object destroyed signal
	SignalDestroyed();
}

/**
*  @brief
*    Check if object is instance of a given class by using a given class reference
*/
bool Object::isInstanceOfByReference(const Class &cClass) const
{
	// Get class
	const Class *pClass = getClass();
	if (pClass) {
		// Same class?
		if (pClass == &cClass) {
			return true;
		} else {
			// Check if class is derived from given class
			return pClass->isDerivedFrom(cClass);
		}
	}

	// Class could not be found
	return false;
}

/**
*  @brief
*    Check if object is instance of a given class by using a class name
*/
bool Object::isInstanceOf(const String &sClass) const
{
	// Get class
	const Class *pClass = ClassManager::instance().getClass(sClass);
	if (pClass) {
		// Check class
		return isInstanceOfByReference(*pClass);
	}

	// Class could not be found
	return false;
}

/**
*  @brief
*    Get attributes
*/
const std::list<DynVarPtr> Object::getAttributes() const
{
	// Create attribute list
  std::list<DynVarPtr> lstAttributes;

	// Get class
	Class *pClass = getClass();
	if (pClass) {
		// Get all attributes for this instance and add them to the list
		for (auto iter: pClass->getAttributes())
			lstAttributes.push_back(iter->getAttribute(*this));
	}

	// Return attribute list
	return lstAttributes;
}

/**
*  @brief
*    Get attribute
*/
DynVarPtr Object::getAttribute(const String &sName) const
{
	// Get class
	Class *pClass = getClass();
	if (pClass) {
		// Get descriptor
		const VarDesc *pDescriptor = pClass->getAttribute(sName);
		if (pDescriptor) {
			// Return attribute
			return pDescriptor->getAttribute(*this);
		}
	}

	// Could not find attribute
	return nullptr;
}

/**
*  @brief
*    Get all callable methods
*/
void Object::getMethods(std::list<DynFuncPtr> &lstMethods)
{
	// Get class
	Class *pClass = getClass();
	if (pClass) {
		// Get all callable methods for this instance and add them to the list
		for (auto iter: pClass->getMethods())
			lstMethods.push_back(iter->getMethod(*this));
	}
}

/**
*  @brief
*    Get callable method by using a given method name
*/
DynFuncPtr Object::getMethod(const String &sName)
{
	// Get class
	Class *pClass = getClass();
	if (pClass) {
		// Get descriptor
		const FuncDesc *pDescriptor = pClass->getMethod(sName);
		if (pDescriptor) {
			// Return callable method
			return pDescriptor->getMethod(*this);
		}
	}

	// Could not find method
	return DynFuncPtr(nullptr);
}

/**
*  @brief
*    Get a list of all signals
*/
const std::list<DynEventPtr> Object::getSignals() const
{
	// Create signal list
  std::list<DynEventPtr> lstSignals;

	// Get class
	Class *pClass = getClass();
	if (pClass) {
		// Get all signals for this instance and add them to the list
		for (auto iter: pClass->getSignals())
			lstSignals.push_back(iter->getSignal(*this));
	}

	// Return signal list
	return lstSignals;
}

/**
*  @brief
*    Get signal by using a given signal name
*/
DynEventPtr Object::getSignal(const String &sName) const
{
	// Get class
	Class *pClass = getClass();
	if (pClass) {
		// Get descriptor
		const EventDesc *pDescriptor = pClass->getSignal(sName);
		if (pDescriptor) {
			// Return signal
			return pDescriptor->getSignal(*this);
		}
	}

	// Could not find signal
	return nullptr;
}

/**
*  @brief
*    Get a list of all slots
*/
const std::list<DynEventHandlerPtr> Object::getSlots() const
{
	// Create slot list
  std::list<DynEventHandlerPtr> lstSlots;

	// Get class
	Class *pClass = getClass();
	if (pClass) {
		// Get all slots for this instance and add them to the list
		for (auto iter: pClass->getSlots())
			lstSlots.push_back(iter->getSlot(*this));
	}

	// Return slot list
	return lstSlots;
}

/**
*  @brief
*    Get slot by using a given slot name
*/
DynEventHandlerPtr Object::getSlot(const String &sName) const
{
	// Get class
	Class *pClass = getClass();
	if (pClass) {
		// Get descriptor
		const EventHandlerDesc *pDescriptor = pClass->getSlot(sName);
		if (pDescriptor) {
			// Return slot
			return pDescriptor->getSlot(*this);
		}
	}

	// Could not find slot
	return nullptr;
}

/**
*  @brief
*    Set attribute value by using a given string value
*/
void Object::setAttribute(const String &sName, const String &sValue)
{
	// Get attribute
	DynVarPtr pAttribute = getAttribute(sName);
	if (pAttribute) {
		// Set value
		pAttribute->setString(sValue);
	}
}

/**
*  @brief
*    Set attribute value by using a given dynamic variable reference
*/
void Object::setAttribute(const String &sName, const DynVar &cVar)
{
	// Get attribute
	DynVarPtr pAttribute = getAttribute(sName);
	if (pAttribute) {
		// Set value
		pAttribute->setVar(cVar);
	}
}

/**
*  @brief
*    Set attribute value by using a given dynamic variable pointer
*/
void Object::setAttribute(const String &sName, const DynVar *pVar)
{
	// Is the given dynamic variable pointer valid?
	if (pVar) {
		// Get attribute
		DynVarPtr pAttribute = getAttribute(sName);
		if (pAttribute) {
			// Set value
			pAttribute->setVar(*pVar);
		}
	}
}

/**
*  @brief
*    Set attribute to it's default value
*/
void Object::setAttributeDefault(const String &sName)
{
	// Get attribute
	DynVarPtr pAttribute = getAttribute(sName);
	if (pAttribute) {
		// Set default value
		pAttribute->setDefault();
	}
}

/**
*  @brief
*    Call method with given dynamic parameters
*/
void Object::callMethod(const String &sName, DynParams &cParams)
{
	// Get callable method
	DynFuncPtr pMethod = getMethod(sName);
	if (pMethod) {
		// Call method
		pMethod->call(cParams);
	}
}

/**
*  @brief
*    Call method with given constant dynamic parameters
*/
void Object::callMethod(const String &sName, const DynParams &cParams)
{
	// Get callable method
	DynFuncPtr pMethod = getMethod(sName);
	if (pMethod) {
		// Call method
		pMethod->call(cParams);
	}
}

/**
*  @brief
*    Call method with parameters given as string
*/
void Object::callMethod(const String &sName, const String &sParams)
{
	// Get callable method
	DynFuncPtr pMethod = getMethod(sName);
	if (pMethod) {
		// Call method
		pMethod->call(sParams);
	}
}

/**
*  @brief
*    Get attribute values as a string
*/
String Object::getValues(EDefaultValue nDefaultValue) const
{
	// Init string
	String sVars;

	// Get class
	Class *pClass = getClass();
	if (pClass) {
		// Loop through attributes
		for (VarDesc* pVarDesc: pClass->getAttributes()) {
			if (pVarDesc) {
				// Get attribute
				DynVarPtr pVar = pVarDesc->getAttribute(*this);

				// Ignore variables with default values?
				if (nDefaultValue == WithDefault || !pVar->isDefault()) {
					// Add attribute to string
					if (sVars.length() > 0)
						sVars += ' ';
					sVars += pVarDesc->getName();
					sVars += "=\"";
					sVars += pVar->getString();
					sVars += '\"';
				}
			}
		}
	}

	// Return string
	return sVars;
}

/**
*  @brief
*    Set multiple attribute values as a string at once
*/
void Object::setValues(const String &sVars)
{
	// Parse parameters
	ParamsParser cParams;
	cParams.parseString(sVars);
	while (cParams.hasParam()) {
		// Set attribute
		setAttribute(cParams.getName(), cParams.getValue());

		// Next parameter
		cParams.next();
	}
}

/**
*  @brief
*    Set all attributes to default
*/
void Object::setDefaultValues()
{
	// Get class
	Class *pClass = getClass();
	if (pClass) {
		// Loop through attributes
		for (VarDesc* pVarDesc: pClass->getAttributes()) {
			if (pVarDesc) {
				// Get attribute
				DynVarPtr pVar = pVarDesc->getAttribute(*this);

				// Set default value
				pVar->setDefault();
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual Object functions                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get object as string
*/
String Object::toString() const
{
	// Get attributes
	return getValues(WithDefault);
}

/**
*  @brief
*    Set object from string
*/
void Object::fromString(const String &sString)
{
	// Set attributes
	setValues(sString);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
