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
#include "RECore/Reflect/Module.h"
#include "RECore/Reflect/ClassManager.h"
#include "RECore/Reflect/ClassDummy.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
std::map<String, MemberDesc*> ClassDummy::m_mapMembers;
std::list<VarDesc*>				 ClassDummy::m_lstAttributes;
std::list<FuncDesc*>				 ClassDummy::m_lstMethods;
std::list<EventDesc*>			 ClassDummy::m_lstSignals;
std::list<EventHandlerDesc*>		 ClassDummy::m_lstSlots;
std::list<ConstructorDesc*>		 ClassDummy::m_lstConstructors;


//[-------------------------------------------------------]
//[ Protected virtual ClassImpl functions                 ]
//[-------------------------------------------------------]
bool ClassDummy::isDummy() const
{
	// Bah! It's just a dummy!
	return true;
}

/**
*  @brief
*    Initialize class and class members
*/
void ClassDummy::initClass() const
{
	// Check if class has already been initialized
	if (!m_bInitialized) {
		// Get base class
		if (m_sBaseClass.length())
			m_pBaseClass = ClassManager::instance().getClass(m_sBaseClass);

		// Check if a valid base class has been found
		if (m_pBaseClass || !m_sBaseClass.length()) {
			// Do we have a base class? (only Object doesn't, but that must count, too)
			if (m_pBaseClass) {
				// Initialize base class
				static_cast<ClassImpl*>(m_pBaseClass->m_pClassImpl)->initClass();

				// Add properties from base class
				ClassImpl *pBaseClassReal = static_cast<ClassImpl*>(m_pBaseClass->m_pClassImpl);
				for (auto iter: pBaseClassReal->m_mapProperties) {
          const String sName = iter.first;
					const String sValue = pBaseClassReal->m_mapProperties.at(sName);
					m_mapProperties[sName] = sValue;
				}

				// The dummy doesn't support attributes
				// The dummy doesn't support methods
				// The dummy doesn't support signals
				// The dummy doesn't support slots

				// Constructors are not copied from base classes, only the own constructors can be used!
			}

			// Add own properties
			for (auto iter: m_mapOwnProperties) {
				const String sName  = iter.first;
				const String sValue = m_mapOwnProperties.at(sName);
				m_mapProperties[sName] = sValue;
			}

			// The dummy doesn't support attributes
			// The dummy doesn't support methods
			// The dummy doesn't support signals
			// The dummy doesn't support slots

			// Done
			m_bInitialized = true;
		} else {
			// Error! Could not find base class
		}
	}
}

/**
*  @brief
*    De-Initialize class and class members
*/
void ClassDummy::deInitClass() const
{
	// Clear lists
	m_mapProperties.clear();
	// The dummy doesn't support members
	// The dummy doesn't support attributes
	// The dummy doesn't support methods
	// The dummy doesn't support signals
	// The dummy doesn't support slots
	// The dummy doesn't support constructors

	// Remove base class
	m_pBaseClass = nullptr;

	// Class de-initialized
	m_bInitialized = false;

	// De-initialize derived classes
	std::list<const Class*> lstClasses;
	ClassManager::instance().getClasses(lstClasses, m_sClassName, NonRecursive, NoBase, IncludeAbstract);
	for (const Class* cls: lstClasses)
		cls->m_pClassImpl->deInitClass();
}

const std::list<VarDesc*> &ClassDummy::getAttributes() const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->getAttributes() : m_lstAttributes;
}

const VarDesc *ClassDummy::getAttribute(const String &sName) const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->getAttribute(sName) : nullptr;
}

const std::list<FuncDesc*> &ClassDummy::getMethods() const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->getMethods() : m_lstMethods;
}

const FuncDesc *ClassDummy::getMethod(const String &sName) const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->getMethod(sName) : nullptr;
}

const std::list<EventDesc*> &ClassDummy::getSignals() const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->getSignals() : m_lstSignals;
}

const EventDesc *ClassDummy::getSignal(const String &sName) const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->getSignal(sName) : nullptr;
}

const std::list<EventHandlerDesc*> &ClassDummy::getSlots() const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->getSlots() : m_lstSlots;
}

const EventHandlerDesc *ClassDummy::getSlot(const String &sName) const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->getSlot(sName) : nullptr;
}

bool ClassDummy::hasConstructor() const
{
	return m_bHasConstructor;
}

bool ClassDummy::hasDefaultConstructor() const
{
	return m_bHasDefaultConstructor;
}

const std::list<ConstructorDesc*> &ClassDummy::getConstructors() const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->getConstructors() : m_lstConstructors;
}

const ConstructorDesc *ClassDummy::getConstructor(const String &sName) const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->getConstructor(sName) : nullptr;
}

Object *ClassDummy::create() const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->create() : nullptr;
}

Object *ClassDummy::create(const DynParams &cParams) const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->create(cParams) : nullptr;
}

Object *ClassDummy::create(const String &sName, const DynParams &cParams) const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->create(sName, cParams) : nullptr;
}

Object *ClassDummy::create(const String &sName, const String &sParams) const
{
	// For this action, we really need the real class: Requests the real class implementation
	ClassImpl *pRealClassImpl = GetRealClassImpl();
	return pRealClassImpl ? pRealClassImpl->create(sName, sParams) : nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ClassDummy::ClassDummy(uint32 nModuleID, const String &sName, const String &sDescription, const String &sNamespace, const String &sBaseClass, bool bHasConstructor, bool bHasDefaultConstructor) : ClassImpl(nModuleID, sName, sDescription, sNamespace, sBaseClass),
	m_bHasConstructor(bHasConstructor),
	m_bHasDefaultConstructor(bHasDefaultConstructor)
{
}

/**
*  @brief
*    Destructor
*/
ClassDummy::~ClassDummy()
{
	// De-initialize class
	if (m_bInitialized)
		deInitClass();
}

/*
*  @brief
*    Requests the real class implementation
*/
ClassImpl *ClassDummy::GetRealClassImpl() const
{
	// Get the module instance this dummy implementation is in
	const Module *pModule = getModule();
	if (pModule) {
		// Backup some pointers on the runtime stack - this class implementation dummy will probably die in the next step...
		const ClassImpl *pThisClassImpl = this;
		Class *pClass = m_pClass;

		// Get the class manager instance
		ClassManager &pClassManager = ClassManager::instance();

		// Load the real module, this also creates the real class implementations
		pClassManager.loadModule(pModule->getFilename());

		// Class implementation changed?
		if (pClass->m_pClassImpl != pThisClassImpl) {
			// The dummy class implementation is no more, this means the dummy module has now to go as well

			// Module has been unloaded (emit event)
			pClassManager.EventModuleUnloaded(pModule);

			// Remove module
			pClassManager.m_lstModules.remove(pModule);
			pClassManager.m_mapModules.erase(pModule->getModuleID());
			delete pModule;

			// Return the real class implementation
			return pClass->m_pClassImpl;
		}
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
