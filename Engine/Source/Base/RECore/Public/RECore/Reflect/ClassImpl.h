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
class Class;
class Object;
class Module;
class VarDesc;
class FuncDesc;
class DynParams;
class EventDesc;
class MemberDesc;
class ConstructorDesc;
class EventHandlerDesc;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract class implementation base class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'Class' abstraction
*/
class ClassImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Class;
	friend class ClassReal;
	friend class ClassDummy;
	friend class ClassManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the pointer to the owner class instance wrapping this class implementation
		*
		*  @return
		*    Pointer to the owner class instance (should never be a null pointer, unless something is *terribly* wrong ;-) )
		*/
		inline Class *getClass() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nModuleID
		*    ID of owner module
		*  @param[in] sName
		*    Name
		*  @param[in] sDescription
		*    Description
		*  @param[in] sNamespace
		*    Namespace
		*  @param[in] sBaseClass
		*    Base class
		*/
		RECORE_API ClassImpl(uint32 nModuleID, const String &sName, const String &sDescription, const String &sNamespace, const String &sBaseClass);

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~ClassImpl();

		/**
		*  @brief
		*    Get module the class belongs to
		*
		*  @return
		*    Module (always valid)
		*/
		RECORE_API const Module *getModule() const;

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
		*    Pointer to base class (can be a null pointer)
		*/
		RECORE_API const Class *getBaseClass() const;

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
		RECORE_API bool isDerivedFrom(const Class &cBaseClass) const;

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
		RECORE_API bool isDerivedFrom(const String &sBaseClass) const;

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

		//[-------------------------------------------------------]
		//[ Class management                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Add property
		*
		*  @param[in] sName
		*    Property name
		*  @param[in] sValue
		*    Property value
		*/
		RECORE_API void addProperty(const String &sName, const String &sValue);


	//[-------------------------------------------------------]
	//[ Protected virtual ClassImpl functions                 ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Class management                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not the class implementation is a dummy used for delayed shared library loading
		*
		*  @return
		*    'true' if the class implementation is a dummy used for delayed shared library loading, else 'false'
		*/
		virtual bool isDummy() const = 0;

		/**
		*  @brief
		*    Initialize class and class members
		*
		*  @remarks
		*    This function is called automatically when it is necessary, e.g. the first time
		*    any members are being accessed. It will search for the base class of the class
		*    and initialize all members. If later a class is changed (e.g. a new member is
		*    registered at one of the base classes), that class and all derived classes will
		*    destroy their information and must be initialized again.
		*/
		virtual void initClass() const = 0;

		/**
		*  @brief
		*    De-Initialize class and class members
		*
		*  @remarks
		*    This function destroys all data about the class and it's members. See
		*    initClass() for more information about why this is necessary and when.
		*/
		virtual void deInitClass() const = 0;

		//[-------------------------------------------------------]
		//[ Class interface                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get attributes
		*
		*  @return
		*    List of attribute descriptors
		*/
		virtual const std::list<VarDesc*> &getAttributes() const = 0;

		/**
		*  @brief
		*    Get attribute
		*
		*  @param[in] sName
		*    Attribute name
		*
		*  @return
		*    Attribute descriptor (can be a null pointer, if no member with that name could be found)
		*/
		virtual const VarDesc *getAttribute(const String &sName) const = 0;

		/**
		*  @brief
		*    Get methods
		*
		*  @return
		*    List of method descriptors
		*/
		virtual const std::list<FuncDesc*> &getMethods() const = 0;

		/**
		*  @brief
		*    Get method
		*
		*  @param[in] sName
		*    Method name
		*
		*  @return
		*    Method descriptor (can be a null pointer, if no member with that name could be found)
		*/
		virtual const FuncDesc *getMethod(const String &sName) const = 0;

		/**
		*  @brief
		*    Get signals
		*
		*  @return
		*    List of signal descriptors
		*/
		virtual const std::list<EventDesc*> &getSignals() const = 0;

		/**
		*  @brief
		*    Get signal
		*
		*  @param[in] sName
		*    Signal name
		*
		*  @return
		*    Signal descriptor (can be a null pointer, if no member with that name could be found)
		*/
		virtual const EventDesc *getSignal(const String &sName) const = 0;

		/**
		*  @brief
		*    Get slot
		*
		*  @return
		*    List of slot descriptors
		*/
		virtual const std::list<EventHandlerDesc*> &getSlots() const = 0;

		/**
		*  @brief
		*    Get slot
		*
		*  @param[in] sName
		*    Slot name
		*
		*  @return
		*    Slot descriptor (can be a null pointer, if no member with that name could be found)
		*/
		virtual const EventHandlerDesc *getSlot(const String &sName) const = 0;

		/**
		*  @brief
		*    Check if class has any constructors
		*
		*  @return
		*    'true' if class has at least one constructor, else 'false'
		*/
		virtual bool hasConstructor() const = 0;

		/**
		*  @brief
		*    Check if class has a default constructor
		*
		*  @return
		*    'true' if class has a default constructor, else 'false'
		*/
		virtual bool hasDefaultConstructor() const = 0;

		/**
		*  @brief
		*    Get constructors
		*
		*  @return
		*    List of constructor descriptors
		*/
		virtual const std::list<ConstructorDesc*> &getConstructors() const = 0;

		/**
		*  @brief
		*    Get constructor
		*
		*  @param[in] sName
		*    Constructor name
		*
		*  @return
		*    Constructor descriptor (can be a null pointer, if no member with that name could be found)
		*/
		virtual const ConstructorDesc *getConstructor(const String &sName) const = 0;

		/**
		*  @brief
		*    Create object by using the default constructor
		*
		*  @return
		*    Pointer to created object (can be a null pointer)
		*
		*  @remarks
		*    This function will call the default constructor of the class.
		*    If the class has no default constructor, the function will fail and return a null pointer.
		*/
		virtual Object *create() const = 0;

		/**
		*  @brief
		*    Create object by using typed constructor parameters in order to identity the constructor automatically
		*
		*  @param[in] cParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to created object (can be a null pointer)
		*
		*  @remarks
		*    This function will search for a constructor that matches the signature of the given parameters.
		*    If no such constructor can be found, the function will fail and return a null pointer.
		*/
		virtual Object *create(const DynParams &cParams) const = 0;

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
		*    Pointer to created object (can be a null pointer)
		*
		*  @remarks
		*    This function will search for a constructor with the specified name. If no such constructor can be found, or
		*    the given parameters do not match the signature of the constructor, the function will fail and return a null pointer.
		*/
		virtual Object *create(const String &sName, const DynParams &cParams) const = 0;

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
		virtual Object *create(const String &sName, const String &sParams) const = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Class information
		Class							*m_pClass;				/**< Class instance wrapping this class implementation (can be a null pointer, set and managed by the class manager) */
		String							 m_sName;				/**< Name of class */
		String							 m_sNamespace;			/**< Namespace of class */
		String							 m_sClassName;			/**< Name of class (with namespace) */
		String							 m_sDescription;		/**< Description of class */
		String							 m_sBaseClass;			/**< Name of base class (with namespace) */

		// Own data (does not include data from base classes)
		std::map<String, String>			 m_mapOwnProperties;	/**< Hash map of properties (name -> value) */

		// Runtime data
		mutable uint32					 m_nModuleID;			/**< ID of owner module */
		mutable bool					 m_bInitialized;		/**< Is the class initialized? */
		mutable const Class			    *m_pBaseClass;			/**< Pointer to base class */

		// Member lists (also including the members from base classes)
		mutable std::map<String, String>  m_mapProperties;		/**< Hash map of properties (name -> value) */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		ClassImpl(const ClassImpl &cSource);

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
		ClassImpl &operator =(const ClassImpl &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Reflect/ClassImpl.inl"
