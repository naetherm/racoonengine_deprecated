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
#include "RECore/Reflect/ClassImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get module the class belongs to
*/
inline const Module *Class::getModule() const
{
	// Return module from class manager
	return m_pClassImpl->getModule();
}

/**
*  @brief
*    Get full class name (with namespace)
*/
inline String Class::getClassName() const
{
	// Return name of class and namespace
	return m_pClassImpl->getClassName();
}

/**
*  @brief
*    Get full name of base class (with namespace)
*/
inline String Class::getBaseClassName() const
{
	// Return base class
	return m_pClassImpl->getBaseClassName();
}

/**
*  @brief
*    Get class name
*/
inline String Class::getName() const
{
	// Return name of class
	return m_pClassImpl->getName();
}

/**
*  @brief
*    Get class description
*/
inline String Class::getDescription() const
{
	// Return description
	return m_pClassImpl->getDescription();
}

/**
*  @brief
*    Get namespace
*/
inline String Class::getNamespace() const
{
	// Return namespace
	return m_pClassImpl->getNamespace();
}

/**
*  @brief
*    Get base class
*/
inline const Class *Class::getBaseClass() const
{
	// Return base class
	return m_pClassImpl->getBaseClass();
}

/**
*  @brief
*    Check if class is derived from another class
*/
inline bool Class::isDerivedFrom(const Class &cBaseClass) const
{
	// Call implementation
	return m_pClassImpl->isDerivedFrom(cBaseClass);
}

/**
*  @brief
*    Check if class is derived from another class
*/
inline bool Class::isDerivedFrom(const String &sBaseClass) const
{
	// Call implementation
	return m_pClassImpl->isDerivedFrom(sBaseClass);
}

/**
*  @brief
*    Get properties
*/
inline const std::map<String, String> &Class::getProperties() const
{
	// Return property map
	return m_pClassImpl->getProperties();
}

/**
*  @brief
*    Get attributes
*/
inline const std::list<VarDesc*> &Class::getAttributes() const
{
	// Return attributes
	return m_pClassImpl->getAttributes();
}

/**
*  @brief
*    Get attribute
*/
inline const VarDesc *Class::getAttribute(const String &sName) const
{
	// Get attribute
	return m_pClassImpl->getAttribute(sName);
}

/**
*  @brief
*    Get methods
*/
inline const std::list<FuncDesc*> &Class::getMethods() const
{
	// Return methods
	return m_pClassImpl->getMethods();
}

/**
*  @brief
*    Get method
*/
inline const FuncDesc *Class::getMethod(const String &sName) const
{
	// Get method
	return m_pClassImpl->getMethod(sName);
}

/**
*  @brief
*    Get signals
*/
inline const std::list<EventDesc*> &Class::getSignals() const
{
	// Return signals
	return m_pClassImpl->getSignals();
}

/**
*  @brief
*    Get signal
*/
inline const EventDesc *Class::getSignal(const String &sName) const
{
	// Get signal
	return m_pClassImpl->getSignal(sName);
}

/**
*  @brief
*    Get slots
*/
inline const std::list<EventHandlerDesc*> &Class::getSlots() const
{
	// Return slots
	return m_pClassImpl->getSlots();
}

/**
*  @brief
*    Get slot
*/
inline const EventHandlerDesc *Class::getSlot(const String &sName) const
{
	// Get slot
	return m_pClassImpl->getSlot(sName);
}

/**
*  @brief
*    Check if class has any constructors
*/
inline bool Class::hasConstructor() const
{
	// Check if there is at least one constructor
	return m_pClassImpl->hasConstructor();
}

/**
*  @brief
*    Check if class has a default constructor
*/
inline bool Class::hasDefaultConstructor() const
{
	// Check if class has a default constructor
	return m_pClassImpl->hasDefaultConstructor();
}

/**
*  @brief
*    Get constructors
*/
inline const std::list<ConstructorDesc*> &Class::getConstructors() const
{
	// Return constructors
	return m_pClassImpl->getConstructors();
}

/**
*  @brief
*    Get constructor
*/
inline const ConstructorDesc *Class::getConstructor(const String &sName) const
{
	// Get constructor
	return m_pClassImpl->getConstructor(sName);
}

/**
*  @brief
*    Create object by using the default constructor
*/
inline Object *Class::create() const
{
	// Create object
	return m_pClassImpl->create();
}

/**
*  @brief
*    Create object by using typed constructor parameters in order to identity the constructor automatically
*/
inline Object *Class::create(const DynParams &cParams) const
{
	// Create object
	return m_pClassImpl->create(cParams);
}

/**
*  @brief
*    Create object by using a given constructor name and typed constructor parameters
*/
inline Object *Class::create(const String &sName, const DynParams &cParams) const
{
	// Create object
	return m_pClassImpl->create(sName, cParams);
}

/**
*  @brief
*    Create object by using a given constructor name and typeless constructor parameters
*/
inline Object *Class::create(const String &sName, const String &sParams) const
{
	// Create object
	return m_pClassImpl->create(sName, sParams);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
