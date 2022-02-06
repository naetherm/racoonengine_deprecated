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
#include "RECore/Reflect/Func/Params.h"
#include "RECore/Reflect/Func/FuncDesc.h"
#include "RECore/Reflect/Func/ConstructorDesc.h"
#include "RECore/Reflect/Event/EventDesc.h"
#include "RECore/Reflect/Event/EventHandlerDesc.h"
#include "RECore/Reflect/ClassReal.h"


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
ClassReal::ClassReal(uint32 nModuleID, const String &sName, const String &sDescription, const String &sNamespace, const String &sBaseClass) : ClassImpl(nModuleID, sName, sDescription, sNamespace, sBaseClass)
{
	// Register at class manager
	ClassManager::instance().registerClass(nModuleID, this);
}

/**
*  @brief
*    Destructor
*/
ClassReal::~ClassReal()
{
	// Unregister at class manager, please note that the class manager may have already been destroyed (random static de-initialization order)
	ClassManager::instance().unregisterClass(m_nModuleID, this);

	// De-initialize class
	if (m_bInitialized)
		deInitClass();
}


//[-------------------------------------------------------]
//[ Protected virtual ClassImpl functions                 ]
//[-------------------------------------------------------]
bool ClassReal::isDummy() const
{
	// This is the real thing!
	return false;
}

/**
*  @brief
*    Initialize class and class members
*/
void ClassReal::initClass() const
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

				{ // Add properties from base class
					ClassImpl *pBaseClassReal = static_cast<ClassImpl*>(m_pBaseClass->m_pClassImpl);
					for (auto iter: pBaseClassReal->m_mapProperties) {
						const String sName  = iter.second;
						const String sValue = pBaseClassReal->m_mapProperties.at(sName);
						m_mapProperties[sName] = sValue;
					}
				}

				// Add attributes from base class
				m_lstAttributes = m_pBaseClass->getAttributes();
				for (VarDesc* pAttr: m_lstAttributes) {
					m_mapMembers.emplace(pAttr->getName(), pAttr);
				}

				// Add methods from base class
				m_lstMethods = m_pBaseClass->getMethods();
				for (FuncDesc* pMethod: m_lstMethods) {
					m_mapMembers.emplace(pMethod->getName(), pMethod);
				}

				// Add signals from base class
				m_lstSignals = m_pBaseClass->getSignals();
				for (EventDesc* pSignal: m_lstSignals) {
					m_mapMembers.emplace(pSignal->getName(), pSignal);
				}

				// Add slots from base class
				m_lstSlots = m_pBaseClass->getSlots();
				for (EventHandlerDesc* pSlot: m_lstSlots) {
					m_mapMembers.emplace(pSlot->getName(), pSlot);
				}

				// Constructors are not copied from base classes, only the own constructors can be used!
			}

			// Add own properties
			for (auto iter: m_mapOwnProperties) {
				const String sName  = iter.second;
				const String sValue = m_mapOwnProperties.at(sName);
				m_mapProperties[sName] = sValue;
			}

			// Add own members
			for (MemberDesc* pMember: m_lstOwnMembers) {

				// Add to hash map and overwrite variables from base classes that are already there (having the same name!)
				auto iter = m_mapMembers.find(pMember->getName());
        MemberDesc *pOverwriteMember = nullptr;
				if (iter != m_mapMembers.end()) {
          pOverwriteMember = iter->second;
					// Overwrite base class member
					m_mapMembers[pMember->getName()] = pMember;
				} else {
					// Add new member
					m_mapMembers.emplace(pMember->getName(), pMember);
				}

				// Check type and add to respective list
				switch (pMember->getMemberType()) {
					// Attribute
					case MemberAttribute:
						if (pOverwriteMember)
              std::replace(this->m_lstAttributes.begin(), this->m_lstAttributes.end(), static_cast<VarDesc*>(pOverwriteMember), static_cast<VarDesc*>(pMember));
						else
							m_lstAttributes.push_back(static_cast<VarDesc*>(pMember));
						break;

					// Method
					case MemberMethod:
						if (pOverwriteMember)
              std::replace(this->m_lstMethods.begin(), this->m_lstMethods.end(), static_cast<FuncDesc*>(pOverwriteMember), static_cast<FuncDesc*>(pMember));
						else
							m_lstMethods.push_back(static_cast<FuncDesc*>(pMember));
						break;

					// Event
					case MemberEvent:
						if (pOverwriteMember)
              std::replace(this->m_lstSignals.begin(), this->m_lstSignals.end(), static_cast<EventDesc*>(pOverwriteMember), static_cast<EventDesc*>(pMember));
						else
							m_lstSignals.push_back(static_cast<EventDesc*>(pMember));
						break;

					// Event handler
					case MemberEventHandler:
						if (pOverwriteMember)
							std::replace(this->m_lstSlots.begin(), this->m_lstSlots.end(), static_cast<EventHandlerDesc*>(pOverwriteMember), static_cast<EventHandlerDesc*>(pMember));
						else
							m_lstSlots.push_back(static_cast<EventHandlerDesc*>(pMember));
						break;

					// Constructor
					case MemberConstructor:
						m_lstConstructors.push_back(static_cast<ConstructorDesc*>(pMember));
						break;
				}
			}

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
void ClassReal::deInitClass() const
{
	// Clear lists
	m_mapProperties.clear();
	m_mapMembers.clear();
	m_lstAttributes.clear();
	m_lstMethods.clear();
	m_lstSignals.clear();
	m_lstSlots.clear();
	m_lstConstructors.clear();

	// Remove base class
	m_pBaseClass = nullptr;

	// Class de-initialized
	m_bInitialized = false;

	// De-initialize derived classes, please note that the class manager may have already been destroyed (random static de-initialization order)
  std::list<const Class*> lstClasses;
  ClassManager::instance().getClasses(lstClasses, m_sClassName, NonRecursive, NoBase, IncludeAbstract);
  for (const Class* cls: lstClasses)
    cls->m_pClassImpl->deInitClass();
}

const std::list<VarDesc*> &ClassReal::getAttributes() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Return attributes
	return m_lstAttributes;
}

const VarDesc *ClassReal::getAttribute(const String &sName) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Get attribute
	auto iter = m_mapMembers.find(sName);
	if (iter != this->m_mapMembers.end() && iter->second->getMemberType() == MemberAttribute)
		return static_cast<VarDesc*>(iter->second);

	// Attribute could not be found
	return nullptr;
}

const std::list<FuncDesc*> &ClassReal::getMethods() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Return methods
	return m_lstMethods;
}

const FuncDesc *ClassReal::getMethod(const String &sName) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Get method
	auto iter = m_mapMembers.find(sName);
	if (iter != m_mapMembers.end() && iter->second->getMemberType() == MemberMethod)
		return static_cast<FuncDesc*>(iter->second);

	// Method could not be found
	return nullptr;
}

const std::list<EventDesc*> &ClassReal::getSignals() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Return signals
	return m_lstSignals;
}

const EventDesc *ClassReal::getSignal(const String &sName) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Get signal
	auto iter = m_mapMembers.find(sName);
	if (iter != this->m_mapMembers.end() && iter->second->getMemberType() == MemberEvent)
		return static_cast<EventDesc*>(iter->second);

	// Signal could not be found
	return nullptr;
}

const std::list<EventHandlerDesc*> &ClassReal::getSlots() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Return slots
	return m_lstSlots;
}

const EventHandlerDesc *ClassReal::getSlot(const String &sName) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Get slot
	auto iter = m_mapMembers.find(sName);
	if (iter != this->m_mapMembers.end() && iter->second->getMemberType() == MemberEventHandler)
		return static_cast<EventHandlerDesc*>(iter->second);

	// Slot could not be found
	return nullptr;
}

bool ClassReal::hasConstructor() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Check if there is at least one constructor
	return (m_lstConstructors.size() > 0);
}

bool ClassReal::hasDefaultConstructor() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Loop through list
	for (ConstructorDesc* pConstructor: m_lstConstructors) {

		// Default constructor?
		if (pConstructor->isDefaultConstructor()) {
			// We found a default constructor
			return true;
		}
	}

	// No default constructor found
	return false;
}

const std::list<ConstructorDesc*> &ClassReal::getConstructors() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Return constructors
	return m_lstConstructors;
}

const ConstructorDesc *ClassReal::getConstructor(const String &sName) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Get constructor
	auto iter = m_mapMembers.find(sName);
	if (iter != this->m_mapMembers.end() && iter->second->getMemberType() == MemberConstructor)
		return static_cast<ConstructorDesc*>(iter->second);

	// Constructor could not be found
	return nullptr;
}

Object *ClassReal::create() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Loop through constructors
  for (ConstructorDesc* pConstructor: m_lstConstructors) {

		// Check if this constructor is a default constructor
		if (pConstructor->getSignature() == "Object*()") {
			// Call constructor
			return pConstructor->create(Params<Object*>());
		}
	}

	// Error, no value constructor found
	return nullptr;
}

Object *ClassReal::create(const DynParams &cParams) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Loop through constructors
  for (ConstructorDesc* pConstructor: m_lstConstructors) {

		// Check if this constructor has a matching signature
		if (pConstructor->getSignature() == cParams.getSignature()) {
			// Call constructor
			return pConstructor->create(cParams);
		}
	}

	// Error, no value constructor found
	return nullptr;
}

Object *ClassReal::create(const String &sName, const DynParams &cParams) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Loop through constructors
  for (ConstructorDesc* pConstructor: m_lstConstructors) {

		// Check if this constructor has a matching name and signature
		if (pConstructor->getName() == sName && pConstructor->getSignature() == cParams.getSignature()) {
			// Call constructor
			return pConstructor->create(cParams);
		}
	}

	// Error, no value constructor found
	return nullptr;
}

Object *ClassReal::create(const String &sName, const String &sParams) const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		initClass();

	// Loop through constructors
  for (ConstructorDesc* pConstructor: m_lstConstructors) {

		// Check if this constructor has a matching name (good luck with the signature)
		if (pConstructor->getName() == sName) {
			// Call constructor
			return pConstructor->create(sParams);
		}
	}

	// Error, no value constructor found
	return nullptr;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Add member
*/
void ClassReal::addMember(MemberDesc *pMemberDesc)
{
	// Check if pointer is valid
	if (pMemberDesc) {
		// De-initialize class
		if (m_bInitialized)
			deInitClass();

		// Add member to list
		m_lstOwnMembers.push_back(pMemberDesc);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
