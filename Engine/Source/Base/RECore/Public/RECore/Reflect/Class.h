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
#include "RECore/String/String.h"
#include <list>
#include <map>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Module;
class Object;
class VarDesc;
class FuncDesc;
class EventDesc;
class DynParams;
class ClassImpl;
class ConstructorDesc;
class EventHandlerDesc;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Description and interface for classes
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class Class {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ClassImpl;
	friend class ClassReal;
	friend class ClassDummy;
	friend class ClassManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get module the class belongs to
		*
		*  @return
		*    Module (always valid, do not destroy the returned instance!)
		*/
		inline const Module *getModule() const;

		/**
		*  @brief
		*    Get full class name (with namespace)
		*
		*  @return
		*    Name of class and namespace
		*/
		inline String getClassName() const;

		/**
		*  @brief
		*    Get full name of base class (with namespace)
		*
		*  @return
		*    Name of base class and namespace
		*/
		inline String getBaseClassName() const;

		/**
		*  @brief
		*    Get class name (without namespace)
		*
		*  @return
		*    Name of class
		*/
		inline String getName() const;

		/**
		*  @brief
		*    Get class description
		*
		*  @return
		*    Description
		*/
		inline String getDescription() const;

		/**
		*  @brief
		*    Get namespace
		*
		*  @return
		*    Namespace
		*/
		inline String getNamespace() const;

		/**
		*  @brief
		*    Get base class
		*
		*  @return
		*    Pointer to base class (can be a null pointer, do not destroy the returned instance!)
		*/
		inline const Class *getBaseClass() const;

		/**
		*  @brief
		*    Check if class is derived from another class
		*
		*  @param[in] cBaseClass
		*    Base class
		*
		*  @return
		*    'true' if class is derived from given base class, else 'false'
		*/
		inline bool isDerivedFrom(const Class &cBaseClass) const;

		/**
		*  @brief
		*    Check if class is derived from another class
		*
		*  @param[in] sBaseClass
		*    Base class name (with namespace)
		*
		*  @return
		*    'true' if class is derived from given base class, else 'false'
		*/
		inline bool isDerivedFrom(const String &sBaseClass) const;

		/**
		*  @brief
		*    Get derived classes
		*
		*  @param[out] lstClasses
		*    Receives the list of derived classes, given list is not cleared before adding new elements, do not destroy the returned instances!
		*
		*  @remarks
		*    This method always returns all sub-classes of a class.
		*    If you want to search for classes with more specific search criteria,
		*    have a look at ClassManager::getClasses().
		*/
		RECORE_API void getDerivedClasses(std::list<const Class*> &lstClasses) const;

		/**
		*  @brief
		*    Get properties
		*
		*  @return
		*    Hash map of properties (name -> value)
		*
		*  @remarks
		*    A property is a name/value pair of strings, that can be assigned to a class. Use this to
		*    transport additional information for your class, e.g.
		*      "PluginType"  -> "Widget"
		*      "FileFormats" -> "avi mpg mp4"
		*/
		inline const std::map<String, String> &getProperties() const;

		/**
		*  @brief
		*    Get attributes
		*
		*  @return
		*    std::list of attribute descriptors, do not destroy the returned instances!
		*/
		inline const std::list<VarDesc*> &getAttributes() const;

		/**
		*  @brief
		*    Get attribute
		*
		*  @param[in] sName
		*    Attribute name
		*
		*  @return
		*    Attribute descriptor (can be a null pointer, if no member with that name could be found, do not destroy the returned instance!)
		*/
		inline const VarDesc *getAttribute(const String &sName) const;

		/**
		*  @brief
		*    Get methods
		*
		*  @return
		*    std::list of method descriptors, do not destroy the returned instances!
		*/
		inline const std::list<FuncDesc*> &getMethods() const;

		/**
		*  @brief
		*    Get method
		*
		*  @param[in] sName
		*    Method name
		*
		*  @return
		*    Method descriptor (can be a null pointer, if no member with that name could be found, do not destroy the returned instance!)
		*/
		inline const FuncDesc *getMethod(const String &sName) const;

		/**
		*  @brief
		*    Get signals
		*
		*  @return
		*    std::list of signal descriptors, do not destroy the returned instances!
		*/
		inline const std::list<EventDesc*> &getSignals() const;

		/**
		*  @brief
		*    Get signal
		*
		*  @param[in] sName
		*    Signal name
		*
		*  @return
		*    Signal descriptor (can be a null pointer, if no member with that name could be found, do not destroy the returned instance!)
		*/
		inline const EventDesc *getSignal(const String &sName) const;

		/**
		*  @brief
		*    Get slot
		*
		*  @return
		*    std::list of slot descriptors, do not destroy the returned instances!
		*/
		inline const std::list<EventHandlerDesc*> &getSlots() const;

		/**
		*  @brief
		*    Get slot
		*
		*  @param[in] sName
		*    Slot name
		*
		*  @return
		*    Slot descriptor (can be a null pointer, if no member with that name could be found, do not destroy the returned instance!)
		*/
		inline const EventHandlerDesc *getSlot(const String &sName) const;

		/**
		*  @brief
		*    Check if class has any constructors
		*
		*  @return
		*    'true' if class has at least one constructor, else 'false'
		*/
		inline bool hasConstructor() const;

		/**
		*  @brief
		*    Check if class has a default constructor
		*
		*  @return
		*    'true' if class has a default constructor, else 'false'
		*/
		inline bool hasDefaultConstructor() const;

		/**
		*  @brief
		*    Get constructors
		*
		*  @return
		*    std::list of constructor descriptors, do not destroy the returned instances!
		*/
		inline const std::list<ConstructorDesc*> &getConstructors() const;

		/**
		*  @brief
		*    Get constructor
		*
		*  @param[in] sName
		*    Constructor name
		*
		*  @return
		*    Constructor descriptor (can be a null pointer, if no member with that name could be found, do not destroy the returned instance!)
		*/
		inline const ConstructorDesc *getConstructor(const String &sName) const;

		/**
		*  @brief
		*    Create object by using the default constructor
		*
		*  @return
		*    Pointer to created object (can be a null pointer, destroy the returned instance when you no longer need it)
		*
		*  @remarks
		*    This function will call the default constructor of the class.
		*    If the class has no default constructor, the function will fail and return a null pointer.
		*/
		inline Object *create() const;

		/**
		*  @brief
		*    Create object by using typed constructor parameters in order to identity the constructor automatically
		*
		*  @param[in] cParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to created object (can be a null pointer, destroy the returned instance when you no longer need it)
		*
		*  @remarks
		*    This function will search for a constructor that matches the signature of the given parameters.
		*    If no such constructor can be found, the function will fail and return a null pointer.
		*/
		inline Object *create(const DynParams &cParams) const;

		/**
		*  @brief
		*    Create object by using a given constructor name and typed constructor parameters
		*
		*  @param[in] sName
		*    Constructor name
		*  @param[in] cParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to created object (can be a null pointer, destroy the returned instance when you no longer need it)
		*
		*  @remarks
		*    This function will search for a constructor with the specified name. If no such constructor can be found, or
		*    the given parameters do not match the signature of the constructor, the function will fail and return a null pointer.
		*/
		inline Object *create(const String &sName, const DynParams &cParams) const;

		/**
		*  @brief
		*    Create object by using a given constructor name and typeless constructor parameters
		*
		*  @param[in] sName
		*    Constructor name
		*  @param[in] sParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to created object (can be a null pointer, destroy the returned instance when you no longer need it)
		*
		*  @remarks
		*    This function will search for a constructor with the specified name. If no such constructor can be found,
		*    the function will fail and return a null pointer.
		*/
		inline Object *create(const String &sName, const String &sParams) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cClassImpl
		*    Reference to the class specific implementation (this class just shares the given data and doesn't destroy it)
		*/
		Class(ClassImpl &cClassImpl);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Class(const Class &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		~Class();

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
		Class &operator =(const Class &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		mutable ClassImpl *m_pClassImpl;	/**< Pointer to the class specific implementation, just shared pointer (assumed to be never a null pointer!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Reflect/Class.inl"
