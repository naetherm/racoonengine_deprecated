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
#include "RECore/Reflect/Class.h"
#include "RECore/Reflect/ClassManager.h"
#include "RECore/Reflect/Var/VarDesc.h"
#include "RECore/Reflect/Func/FuncDesc.h"
#include "RECore/Reflect/Func/ConstructorDesc.h"
#include "RECore/Reflect/Event/EventDesc.h"
#include "RECore/Reflect/Event/EventHandlerDesc.h"
#include "RECore/Reflect/ClassImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ClassImpl::ClassImpl(uint32 nModuleID, const String &sName, const String &sDescription, const String &sNamespace, const String &sBaseClass) :
	m_pClass(nullptr),
	m_sName(sName),
	m_sNamespace(sNamespace),
	m_sClassName(m_sNamespace.length() ? m_sNamespace + "::" + m_sName : m_sName),
	m_sDescription(sDescription),
	m_sBaseClass(sBaseClass),
	m_nModuleID(nModuleID),
	m_bInitialized(false),
	m_pBaseClass(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
ClassImpl::~ClassImpl()
{
}

/**
*  @brief
*    Get module the class belongs to
*/
const Module *ClassImpl::getModule() const
{
	// Return module from class manager
	return ClassManager::instance().getModuleByID(m_nModuleID);
}

/**
*  @brief
*    Get base class
*/
const Class *ClassImpl::getBaseClass() const
{
	// Get base class?
	if (!m_pBaseClass && m_sBaseClass.length())
		m_pBaseClass = ClassManager::instance().getClass(m_sBaseClass);

	// Return base class
	return m_pBaseClass;
}

/**
*  @brief
*    Check if class is derived from another class
*/
bool ClassImpl::isDerivedFrom(const Class &cBaseClass) const
{
	// Get base class by name
	return isDerivedFrom(cBaseClass.getClassName());
}

/**
*  @brief
*    Check if class is derived from another class
*/
bool ClassImpl::isDerivedFrom(const String &sBaseClass) const
{
	// Start with own class name
	String sName = getBaseClassName();
	while (sName.length()) {
		// Base class found?
		if (sName == sBaseClass)
			return true;

		// Next base class
		const Class *pClass = ClassManager::instance().getClass(sName);
		sName = pClass ? pClass->getBaseClassName() : "";
	}

	// Not a base class
	return false;
}

/**
*  @brief
*    Add property
*/
void ClassImpl::addProperty(const String &sName, const String &sValue)
{
	// De-initialize class
	if (m_bInitialized)
		deInitClass();

	// Check if name is valid
	if (sName.length()) {
		// Add property
		m_mapOwnProperties.emplace(sName, sValue);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
ClassImpl::ClassImpl(const ClassImpl &cSource) :
	m_pClass(nullptr),
	m_nModuleID(0),
	m_bInitialized(false),
	m_pBaseClass(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
ClassImpl &ClassImpl::operator =(const ClassImpl &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
