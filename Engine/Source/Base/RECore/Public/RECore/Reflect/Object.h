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
#include "RECore/Reflect/ReflectTypes.h"
#include "RECore/Reflect/Rtti.h"
#include <list>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Class;
class DynVar;
class DynConstructor;
class DynParams;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Internal Object base class
*
*  @note
*    - Initially the reference counter is 1 (Lookout! Within the "RefCount"-template it's 0)
*/
class ObjectBase {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	public:
		// Properties
		re_properties
		re_properties_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		RECORE_API ObjectBase();

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~ObjectBase();

		/**
		*  @brief
		*    Get class
		*
		*  @return
		*    Class of the object (do not destroy the returned instance, should never be a null pointer, unless something is *terribly* wrong ;-) )
		*/
		RECORE_API virtual Class *getClass() const;

		//[-------------------------------------------------------]
		//[ Reference counting ("RefCount"-template interface)    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get a pointer to the object
		*
		*  @return
		*    Pointer to the reference counter's object, NEVER a null pointer!
		*/
		RECORE_API const ObjectBase *getPointer() const;
		RECORE_API ObjectBase *getPointer();

		/**
		*  @brief
		*    Increases the reference count
		*
		*  @return
		*    Current reference count
		*/
		RECORE_API uint32 addReference();

		/**
		*  @brief
		*    Decreases the reference count
		*
		*  @return
		*    Current reference count
		*
		*  @note
		*    - When the last reference was released, the instance is destroyed automatically
		*/
		RECORE_API uint32 release();

		/**
		*  @brief
		*    Gets the current reference count
		*
		*  @return
		*    Current reference count
		*/
		RECORE_API uint32 getRefCount() const;

		//[-------------------------------------------------------]
		//[ Reference counting                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Decreases the reference count without destroying this instance automatically
		*
		*  @return
		*    Current reference count
		*
		*  @note
		*    - Whenever possible, do not use this method, use "release()" instead
		*    - Unlike "release()", when the last reference was released the instance is not destroyed automatically
		*    - Use this method e.g. to release the initial set reference so e.g. a script can have the total control over an instance
		*/
		RECORE_API uint32 softRelease();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32 m_nRefCount; /**< Reference count */


};


/**
*  @brief
*    Object base class
*/
class Object : public ObjectBase {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	re_class_def_internal(Object, RECORE_API)
		// Signals
		re_signal_0_def(SignalDestroyed)
	re_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		RECORE_API Object();

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~Object();

		//[-------------------------------------------------------]
		//[ Class and members                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if object is instance of a given class by using a given class reference
		*
		*  @param[in] cClass
		*    Class
		*
		*  @return
		*    'true' if the object is an instance of the class or one of it's derived classes, else 'false'
		*/
		RECORE_API bool isInstanceOfByReference(const Class &cClass) const;

		/**
		*  @brief
		*    Check if object is instance of a given class
		*
		*  @param[in] sClass
		*    Class name (with namespace)
		*
		*  @return
		*    'true' if the object is an instance of the class or one of it's derived classes, else 'false'
		*/
		RECORE_API bool isInstanceOf(const String &sClass) const;

		/**
		*  @brief
		*    Get attributes
		*
		*  @return
		*    List of attributes (do not destroy the returned attribute instances)
		*
		*  @remarks
		*    Use this function with caution, as the list is assembled and copied each time the function is called!
		*    In general it is recommended to use getClass()->getAttributes() to obtain a list of attribute descriptors
		*    and then call getAttribute() from the descriptor to get access to the actual attribute
		*/
		RECORE_API const std::list<DynVarPtr> getAttributes() const;

		/**
		*  @brief
		*    Get attribute
		*
		*  @param[in] sName
		*    Attribute name
		*
		*  @return
		*    Attribute (do not destroy the returned instance, can be a null pointer, if no attribute with that name could be found)
		*/
		RECORE_API DynVarPtr getAttribute(const String &sName) const;

		/**
		*  @brief
		*    Get all callable methods
		*
		*  @param[out] lstMethods
		*    Receives the list of callable methods, the given list is not cleared before new entries are added (do not destroy the returned instances)
		*
		*  @remarks
		*    Performance warning: Use this function with caution, as the list is assembled and callable method
		*    instances are created each time the function is called!
		*    If you only need the method descriptors, use getClass()->getMethods() instead.
		*/
		RECORE_API void getMethods(std::list<DynFuncPtr> &lstMethods);

		/**
		*  @brief
		*    Get callable method by using a given method name
		*
		*  @param[in] sName
		*    Method name
		*
		*  @return
		*    Callable method (do not destroy the returned instance, can be a null pointer, if no method with that name could be found)
		*
		*  @remarks
		*    Performance warning: Use this function with caution, as the callable method instance is
		*    created each time the function is called!
		*    If you only need the method descriptor, use getClass()->getMethod() instead.
		*/
		RECORE_API DynFuncPtr getMethod(const String &sName);

		/**
		*  @brief
		*    Get a list of all signals
		*
		*  @return
		*    List of signals (do not destroy the returned instances)
		*
		*  @remarks
		*    Use this function with caution, as the list is assembled and copied each time the function is called!
		*    In general it is recommended to use getClass()->getSignals() to obtain a list of signal descriptors
		*    and then call getSignal() from the descriptor to get access to the actual signal
		*/
		RECORE_API const std::list<DynEventPtr> getSignals() const;

		/**
		*  @brief
		*    Get signal by using a given signal name
		*
		*  @param[in] sName
		*    Signal name
		*
		*  @return
		*    Signal (do not destroy the returned instance, can be a null pointer, if no signal with that name could be found)
		*/
		RECORE_API DynEventPtr getSignal(const String &sName) const;

		/**
		*  @brief
		*    Get a list of all slots
		*
		*  @return
		*    List of slots (do not destroy the returned instances)
		*
		*  @remarks
		*    Use this function with caution, as the list is assembled and copied each time the function is called!
		*    In general it is recommended to use getClass()->getSlots() to obtain a list of slot descriptors
		*    and then call getSlot() from the descriptor to get access to the actual slot
		*/
		RECORE_API const std::list<DynEventHandlerPtr> getSlots() const;

		/**
		*  @brief
		*    Get slot by using a given slot name
		*
		*  @param[in] sName
		*    Slot name
		*
		*  @return
		*    Slot (do not destroy the returned instance, can be a null pointer, if no slot with that name could be found)
		*/
		RECORE_API DynEventHandlerPtr getSlot(const String &sName) const;

		//[-------------------------------------------------------]
		//[ Direct access functions                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set attribute value by using a given string value
		*
		*  @param[in] sName
		*    Attribute name
		*  @param[in] sValue
		*    Attribute value as string
		*/
		RECORE_API void setAttribute(const String &sName, const String &sValue);

		/**
		*  @brief
		*    Set attribute value by using a given dynamic variable reference
		*
		*  @param[in] sName
		*    Attribute name
		*  @param[in] cVar
		*    Attribute value as dynamic variable reference
		*/
		RECORE_API void setAttribute(const String &sName, const DynVar &cVar);

		/**
		*  @brief
		*    Set attribute value by using a given dynamic variable pointer
		*
		*  @param[in] sName
		*    Attribute name
		*  @param[in] pVar
		*    Attribute value as dynamic variable pointer, in case of a null pointer, nothing happens at all
		*
		*  @remarks
		*    This is a comfort method allowing to write e.g.
		*      pFirstObject->setAttribute("MyAttribute", pSecondObject->getAttribute("MyAttribute"));
		*    instead of
		*      DynVarPtr pDynVar = pSecondObject->getAttribute();
		*      if (pDynVar)
		*        pFirstObject->setAttribute("MyAttribute", *pDynVar);
		*    In case there's no such attribute in "pSecondObject", nothing happens at all.
		*/
		RECORE_API void setAttribute(const String &sName, const DynVar *pVar);

		/**
		*  @brief
		*    Set attribute to it's default value
		*
		*  @param[in] sName
		*    Attribute name
		*/
		RECORE_API void setAttributeDefault(const String &sName);

		/**
		*  @brief
		*    Call method with given dynamic parameters
		*
		*  @param[in] sName
		*    Method name
		*  @param[in] cParams
		*    Dynamic parameters
		*/
		RECORE_API void callMethod(const String &sName, DynParams &cParams);

		/**
		*  @brief
		*    Call method with given constant dynamic parameters
		*
		*  @param[in] sName
		*    Method name
		*  @param[in] cParams
		*    Constant dynamic parameters
		*/
		RECORE_API void callMethod(const String &sName, const DynParams &cParams);

		/**
		*  @brief
		*    Call method with parameters given as string
		*
		*  @param[in] sName
		*    Method name
		*  @param[in] sParams
		*    Parameters as string
		*/
		RECORE_API void callMethod(const String &sName, const String &sParams);

		//[-------------------------------------------------------]
		//[ Object state functions                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get attribute values as a string
		*
		*  @param[in] nDefaultValue
		*    'WithDefault' to retrieve all attributes, 'NoDefault' to only retrieve attributes that are not set to default
		*
		*  @return
		*    String containing attributes and values
		*
		*  @remarks
		*    The returned string contains the attributes and their values.
		*    Example: "Name='Test' IntValue='10'"
		*/
		RECORE_API String getValues(EDefaultValue nDefaultValue = NoDefault) const;

		/**
		*  @brief
		*    Set multiple attribute values as a string at once
		*
		*  @param[in] sVars
		*    String containing attributes and values (e.g. \"Name='Bob' Position='1 2 3'\")
		*/
		RECORE_API void setValues(const String &sVars);

		/**
		*  @brief
		*    Set all attributes to default
		*/
		RECORE_API void setDefaultValues();


	//[-------------------------------------------------------]
	//[ Public virtual Object functions                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get object as string
		*
		*  @return
		*    String representation of object
		*/
		RECORE_API virtual String toString() const;

		/**
		*  @brief
		*    Set object from string
		*
		*  @param[in] sString
		*    String representation of object
		*/
		RECORE_API virtual void fromString(const String &sString);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
