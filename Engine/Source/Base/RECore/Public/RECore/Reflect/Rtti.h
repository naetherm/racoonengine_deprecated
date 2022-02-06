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
#include "RECore/Reflect/ClassReal.h"
#include "RECore/Reflect/ClassManager.h"
#include "RECore/Reflect/Var/Var.h"
#include "RECore/Reflect/Var/Attribute.h"
#include "RECore/Reflect/Func/Method.h"
#include "RECore/Reflect/Func/DescCreateStaticInstHelper.h"
#include "RECore/Reflect/Func/FuncConstructorDynFunc.h"
#include "RECore/Reflect/Event/Signal.h"
#include "RECore/Reflect/Event/EventDynEvent.h"
#include "RECore/Reflect/Event/Slot.h"
#include "RECore/Reflect/Event/EventHandlerDynEventHandler.h"
#include "RECore/Reflect/Tools/TypeTraits.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Debug mode
*/
#ifdef DEBUG
	#define RECORE_IS_DEBUGMODE true
#else
	#define RECORE_IS_DEBUGMODE false
#endif


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class to give each module a unique ID
*
*  @remarks
*    This is implemented as a template to ensure, that the compiler will create a *unique* class
*    in each module (e.g. library or application) that is using that class. So, e.g. calling
*    ModuleID<int> from project A will result in another class being used than calling it
*    from project B, which allows the class to request a unique identifier in each module.
*    The class is accessed using ModuleID<int>.
*/
template <typename T> class ModuleID
{


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get module ID
		*
		*  @return
		*    Unique module ID
		*/
		static uint32 getModuleID();

		/**
		*  @brief
		*    Register module
		*
		*  @param[in] sName
		*    Module name
		*  @param[in] sVendor
		*    Vendor name
		*  @param[in] sLicense
		*    Module license
		*  @param[in] sDescription
		*    Module description
		*/
		static void registerModule(const String &sName, const String &sVendor, const String &sLicense, const String &sDescription);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		static uint32 m_nModuleID;	/**< Unique module ID */


};

// Get module ID
template <typename T> uint32 ModuleID<T>::getModuleID()
{
	// Request module ID from ClassManager
	if (m_nModuleID == 0)
		m_nModuleID = ClassManager::instance().getUniqueModuleID();

	// Return module ID
	return m_nModuleID;
}

template <typename T> void ModuleID<T>::registerModule(const String &sName, const String &sVendor, const String &sLicense, const String &sDescription)
{
	// Ensure we have a valid module ID
	getModuleID();

	// Register module, and provide a pointer to our static module ID variable ("memory anchor")
	ClassManager::instance().registerModule(&m_nModuleID, sName, sVendor, sLicense, sDescription);
}

// Module ID
template <typename T> uint32	ModuleID<T>::m_nModuleID = 0;


//[-------------------------------------------------------]
//[ Internal macros                                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create class for an enumeration type
*
*  @param[in] ENUM
*    Enumeration name
*/
#define __re_enum(ENUM) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef int  _BaseType; \
			typedef ENUM _Type; \
			\
			static bool getEnumValue(int nIndex, ENUM &nValue, RECore::String &sName, RECore::String &sDescription) { \
				int nCount = 0; \

#define re_class_enum_metadata(CLASS, ENUM) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef int  _BaseType; \
			typedef CLASS::ENUM _Type; \
			\
			static bool getEnumValue(int nIndex, CLASS::ENUM &nValue, RECore::String &sName, RECore::String &sDescription) { \
				int nCount = 0; \
				
#define re_class_enum_metadata2(ENUM) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef int  _BaseType; \
			typedef _Self::ENUM _Type; \
			\
			static bool getEnumValue(int nIndex, _Self::ENUM &nValue, RECore::String &sName, RECore::String &sDescription) { \
				int nCount = 0; \

#define re_class_flag_metadata2(ENUM) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef RECore::uint32  _BaseType; \
			typedef _Self::ENUM _Type; \
			\
			static bool getEnumValue(int nIndex, _Self::ENUM &nValue, RECore::String &sName, RECore::String &sDescription) { \
				int nCount = 0; \

#define __re_flag(ENUM) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef RECore::uint32  _BaseType; \
			typedef ENUM _Type; \
			\
			static bool getEnumValue(int nIndex, ENUM &nValue, RECore::String &sName, RECore::String &sDescription) { \
				int nCount = 0; \

/**
*  @brief
*    Create class for an direct enumeration type (not using enum{}, e.g. for float 'enums')
*
*  @param[in] ENUM
*    Enumeration name
*  @param[in] TYPE
*    Enumeration type
*/
#define __re_enum_direct(ENUM, TYPE) \
	class ENUM##__plcore_enum__ { \
		public: \
			typedef TYPE _BaseType; \
			typedef TYPE _Type; \
			\
			static bool getEnumValue(int nIndex, TYPE &nValue, RECore::String &sName, RECore::String &sDescription) { \
				int nCount = 0; \

/**
*  @brief
*    Declare base enumeration type (add all values of an existing enumeration type)
*
*  @param[in] ENUM
*    Enumeration name of base data type
*/
#define __re_enum_base(ENUM) \
				nCount = RECore::EnumType<ENUM##__plcore_enum__>::getNumOfEnumValues(); \
				if (nIndex >= 0 && nIndex < nCount) { \
					return ENUM##__plcore_enum__::getEnumValue(nIndex, reinterpret_cast<ENUM&>(nValue), sName, sDescription); \
				} else if (nIndex == -1) { \
					if (ENUM##__plcore_enum__::getEnumValue(-1, reinterpret_cast<ENUM&>(nValue), sName, sDescription)) \
						return true; \
				} \

/**
*  @brief
*    Add enumeration value
*
*  @param[in] VALUE
*    Enumeration value
*  @param[in] DESCRIPTION
*    Enumeration description
*/
#define __re_enum_value(VALUE, DESCRIPTION) \
					 if (nIndex == nCount)										{ nValue = VALUE; sName = #VALUE; return true; } \
				else if (nIndex == -1 && !sName.length() && nValue == VALUE)	{ sName = #VALUE; return true; } \
				else if (nIndex == -1 && sName == #VALUE)						{ nValue = VALUE; sDescription = DESCRIPTION; return true; } \
				nCount++; \

#define re_class_enum_value(VALUE, DESCRIPTION) \
					 if (nIndex == nCount)										{ nValue = _Self::VALUE; sName = #VALUE; return true; } \
				else if (nIndex == -1 && !sName.length() && nValue == _Self::VALUE)	{ sName = #VALUE; return true; } \
				else if (nIndex == -1 && sName == #VALUE)						{ nValue = _Self::VALUE; sDescription = DESCRIPTION; return true; } \
				nCount++; \

/**
*  @brief
*    Add enumeration value by directly specifying the value
*
*  @param[in] NAME
*    Enumeration name
*  @param[in] VALUE
*    Enumeration value
*  @param[in] DESCRIPTION
*    Enumeration description
*/
#define __re_enum_value_direct(NAME, VALUE, DESCRIPTION) \
					 if (nIndex == nCount)										{ nValue = VALUE; sName = #NAME; return true; } \
				else if (nIndex == -1 && !sName.length() && nValue == VALUE)	{ sName = #NAME;  return true; } \
				else if (nIndex == -1 && sName == #NAME)						{ nValue = VALUE; sDescription = DESCRIPTION; return true; } \
				nCount++; \

/**
*  @brief
*    End enumeration class
*/
#define __re_enum_end \
				return false; \
			} \
	}; \

/**
*  @brief
*    Create guard for class
*
*  @param[in] CLASS
*    Class name (without namespace)
*/
#define __re_guard(CLASS) \
		/* Creation and destruction guard */ \
		class _Guard { \
			public: \
				/* Constructor */ \
				_Guard() { \
					_Class::GetSingleton(); \
				} \
				\
				/* Destructor */ \
				~_Guard() { \
					/* Make sure that class instance is removed on shutdown */ \
					_Class::GetSingleton(false); \
				} \
		}; \

/**
*  @brief
*    Create get/set storage class for an attribute
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] STORAGE
*    Attribute storage type (DirectValue/GetSet/ModifyAttr)
*/
#define __re_attr_stor(NAME, TYPE, STORAGE) \
		template <typename CLASS> class NAME##_GetSet { \
			public: \
				typedef RECore::StorageGetSet StorageType; \
				typedef RECore::StorageGetSet BaseStorageType; \
				static inline TYPE get(RECore::Object *pObject) { \
					return static_cast<CLASS*>(pObject)->Get##NAME(); \
				} \
				static inline void set(RECore::Object *pObject, const TYPE &Value) { \
					static_cast<CLASS*>(pObject)->Set##NAME(Value); \
				} \
		}; \
		\
		template <typename BASE> class NAME##_ModAttr { \
			public: \
				typedef RECore::StorageModifyAttr						StorageType; \
				typedef typename BASE::NAME##_Storage::BaseStorageType	BaseStorageType; \
				static inline TYPE get(RECore::Object *pObject) { \
					return static_cast<BASE*>(pObject)->NAME.get(); \
				} \
				static inline void set(RECore::Object *pObject, const TYPE &Value) { \
					static_cast<BASE*>(pObject)->NAME.set(Value); \
				} \
		}; \

/**
*  @brief
*    Create descriptor class for an attribute
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*  @param[in] DESCRIPTION
*    Attribute description
*  @param[in] ANNOTATION
*    Attribute annotation
*/
#define __re_attr_desc_metadata(NAME, TYPE, DEFAULT, ACCESS, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public RECore::VarDesc { \
			public: \
				NAME##_Desc() : RECore::VarDesc(RECore::Type<TYPE>::TypeID, RECore::Type<TYPE>::getTypeName(), #NAME, DESCRIPTION, ANNOTATION) { \
					registerThis(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual RECore::String getDefault() const override { \
					return RECore::Type<TYPE>::convertToString(DEFAULT); \
				} \
				virtual RECore::DynVarPtr getAttribute(const RECore::Object &cObject) const override { \
					return new RECore::AttributeWrapper<TYPE, RECore::Access##ACCESS, _Self::NAME##_Attr::StorageType>(&reinterpret_cast<_Self&>(const_cast<RECore::Object&>(cObject)).NAME, this); \
				} \
		}; \

/**
*  @brief
*    Create class for an attribute
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*  @param[in] STORAGE
*    Attribute storage type (DirectValue/GetSet/ModifyAttr)
*/
#define __re_attr_attr_def(NAME, TYPE, DEFAULT, ACCESS, STORAGE) \
		class NAME##_Attr : public RECore::Attribute<TYPE, RECore::Access##ACCESS, STORAGE> { \
			public: \
				typedef STORAGE	StorageType; \
				NAME##_Attr(_Self *pObject) : RECore::Attribute<TYPE, RECore::Access##ACCESS, STORAGE>(DEFAULT, pObject) { \
				} \
				\
				NAME##_Attr &operator =(const TYPE &Value) { \
					return dynamic_cast<NAME##_Attr&>(RECore::Attribute<TYPE, RECore::Access##ACCESS, STORAGE>::operator =(Value)); \
				} \
		}; \

/**
*  @brief
*    Create class for an attribute with DirectValue as Storage type
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define __re_attr_attr_directvalue(NAME, TYPE, DEFAULT, ACCESS) \
		__re_attr_attr_def(NAME, TYPE, DEFAULT, ACCESS, RECore::StorageDirectValue)

/**
*  @brief
*    Create class and helper storage class for an attribute with GetSet as Storage type
*
*  @param[in] CLASSTYPE
*    Name of the class which contains this attribute
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define __re_attr_attr_getset(CLASSTYPE, NAME, TYPE, DEFAULT, ACCESS) \
		template <typename CLASS> class NAME##_GetSet { \
			public: \
				typedef RECore::StorageGetSet StorageType; \
				typedef RECore::StorageGetSet BaseStorageType; \
				static inline TYPE get(RECore::Object *pObject) { \
					return static_cast<CLASS*>(pObject)->Get##NAME(); \
				} \
				static inline void set(RECore::Object *pObject, const TYPE &Value) { \
					static_cast<CLASS*>(pObject)->Set##NAME(Value); \
				} \
		}; \
		\
		__re_attr_attr_def(NAME, TYPE, DEFAULT, ACCESS, NAME##_GetSet<CLASSTYPE>)

/**
*  @brief
*    Create class and helper storage class for an attribute with ModifyAttr as Storage type
*
*  @param[in] BASETYPE
*    Name of the base class which contains the attribute to be modified by this attribute
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define __re_attr_attr_modifyattr(BASETYPE, NAME, TYPE, DEFAULT, ACCESS) \
		template <typename BASE> class NAME##_ModAttr { \
			public: \
				typedef RECore::StorageModifyAttr StorageType; \
				typedef typename BASE::NAME##_Attr::StorageType BaseStorageType; \
				static inline TYPE get(RECore::Object *pObject) { \
					return static_cast<BASE*>(pObject)->NAME.get(); \
				} \
				static inline void set(RECore::Object *pObject, const TYPE &Value) { \
					static_cast<BASE*>(pObject)->NAME.set(Value); \
				} \
		}; \
		\
		__re_attr_attr_def(NAME, TYPE, DEFAULT, ACCESS, NAME##_ModAttr<BASETYPE>)

/**
*  @brief
*    Create attribute variable
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*/
#define __re_attr_decl(NAME, TYPE) \
		NAME##_Attr NAME; \

/**
*  @brief
*    Create descriptor class for a method
*
*  @param[in] NAME
*    Method name
*  @param[in] RET
*    Return type
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Method description
*  @param[in] ANNOTATION
*    Method annotation
*/
#define __re_method_desc_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public RECore::FuncDesc { \
			public: \
				typedef _Self																																ClassType; \
				typedef RECore::FunctorWrapper<RET,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>													FuncWrapperType; \
				typedef RECore::Signature<RET,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>														SigType; \
				typedef RECore::ClassTypelist< _Self, RECore::Typelist<RET,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15> >::TypeMethodSignature	MethType; \
			public: \
				NAME##_Desc() : RECore::FuncDesc(SigType::getSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) { \
					registerThis(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
				virtual RECore::DynFuncPtr getMethod(RECore::Object &cObject) const override { \
					return new NAME##_Method(reinterpret_cast<_Self&>(cObject)); \
				} \
		}; \

/**
*  @brief
*    Create class for a method
*
*  @param[in] NAME
*    Method name
*/
#define __re_method_meth_metadata(NAME) \
		class NAME##_Method : public RECore::Method<NAME##_Desc> { \
			public: \
				/* Cast away the const within the method pointer using a good old C-style cast to be as flexible as possible in here, if this is not done, only non-const methods can be exposed to the RTTI which isn't that comfortable */ \
				NAME##_Method() : RECore::Method<NAME##_Desc>((NAME##_Desc::MethType::MemFuncType)(&_Self::NAME), nullptr) { \
					/* There are no automatic RTTI class method instances per RTTI class instance because there's no need for it and this safes RTTI class instance memory */ \
				} \
				/* Cast away the const within the method pointer using a good old C-style cast to be as flexible as possible in here, if this is not done, only non-const methods can be exposed to the RTTI which isn't that comfortable */ \
				NAME##_Method(_Self &cObject) : RECore::Method<NAME##_Desc>((NAME##_Desc::MethType::MemFuncType)(&_Self::NAME), &cObject) { \
				} \
		}; \

/**
*  @brief
*    Create descriptor class for a signal
*
*  @param[in] NAME
*    Signal name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Signal description
*  @param[in] ANNOTATION
*    Signal annotation
*/
#define __re_signal_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public RECore::EventDesc { \
			public: \
				typedef RECore::Signature<void,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	SigType; \
			public: \
				NAME##_Desc() : RECore::EventDesc(SigType::getSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) { \
					registerThis(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual RECore::DynEventPtr getSignal(const RECore::Object &cObject) const override { \
					return new RECore::EventWrapper<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>(&reinterpret_cast<_Self&>(const_cast<RECore::Object&>(cObject)).NAME, this); \
				} \
		}; \

/**
*  @brief
*    Create class for a signal
*
*  @param[in] NAME
*    Signal name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] API_EXPORT
*    specifier if the getDesc Method should be exported (__declspec(dllexport) under MS VC++ compiler or __attribute__ ((visibility ("default"))) under linux and gcc when the visibility feature is activated)
*/
#define __re_signal_evnt_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
		typedef RECore::Event<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	NAME##_EventType; \
		class NAME##_Signal : public RECore::Signal<NAME##_EventType> { \
			public: \
				NAME##_Signal() : RECore::Signal<NAME##_EventType>() { \
				} \
		}; \

/**
*  @brief
*    Create signal variable
*
*  @param[in] NAME
*    Signal name
*/
#define __re_signal_decl(NAME) \
		NAME##_Signal NAME; \

/**
*  @brief
*    Create descriptor class for a slot
*
*  @param[in] CLASS
*    Name of class which contains the slot which is described by this descriptor
*  @param[in] NAME
*    Event handler name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Event handler description
*  @param[in] ANNOTATION
*    Event handler annotation
*/
#define __re_slot_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public RECore::EventHandlerDesc { \
			public: \
				typedef RECore::Signature<void,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	SigType; \
			public: \
				NAME##_Desc() : RECore::EventHandlerDesc(SigType::getSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) { \
					registerThis(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual RECore::DynEventHandlerPtr getSlot(const RECore::Object &cObject) const override { \
					return new RECore::EventHandlerWrapper<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>(&reinterpret_cast<_Self&>(const_cast<RECore::Object&>(cObject)).Slot##NAME, this); \
				} \
		}; \

/**
*  @brief
*    Create class for a slot
*
*  @param[in] CLASSTYPE
*    Name of the class which contains this slot
*  @param[in] NAME
*    Slot name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] API_EXPORT
*    specifier if the getDesc Method should be exported (__declspec(dllexport) under MS VC++ compiler or __attribute__ ((visibility ("default"))) under linux and gcc when the visibility feature is activated)
*/
#define __re_slot_evth_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
		typedef RECore::EventHandler<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>															NAME##_EventHandlerType; \
		typedef RECore::ClassTypelist< CLASSTYPE, RECore::Typelist<void,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15> >::TypeMethodSignature	NAME##_MethType; \
		\
		class NAME##_Slot : public RECore::Slot<NAME##_EventHandlerType, CLASSTYPE, NAME##_MethType> { \
			public: \
				NAME##_Slot(CLASSTYPE *pObject) : RECore::Slot<NAME##_EventHandlerType,CLASSTYPE, NAME##_MethType>(&CLASSTYPE::NAME, pObject) { \
				} \
		}; \

/**
*  @brief
*    Create slot variable
*
*  @param[in] NAME
*    Event handler name
*/
#define __re_slot_decl(NAME) \
		NAME##_Slot Slot##NAME; \

/**
*  @brief
*    Create descriptor class for a constructor
*
*  @param[in] NAME
*    Constructor name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Constructor description
*  @param[in] ANNOTATION
*    Constructor annotation
*/
#define __re_constructor_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
		class NAME##_Desc : public RECore::ConstructorDesc { \
			public: \
				typedef _Self																						ClassType; \
				typedef RECore::Params<RECore::Object*,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>		ParamType; \
				typedef const RECore::Params<RECore::Object*,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	ConstParamType; \
				typedef RECore::FuncConstructorWrapper<_Self,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	ConstWrapperType; \
				typedef RECore::Signature<RECore::Object*,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>	SigType; \
			public: \
				NAME##_Desc() : RECore::ConstructorDesc(SigType::getSignatureID(), #NAME, #DESCRIPTION, #ANNOTATION) \
					, m_ConstructorWrapper(&m_Constructor) \
				{ \
					registerThis(_Class::GetSingleton()); \
				} \
				~NAME##_Desc() { \
				} \
			private: \
				virtual RECore::DynFunc *getConstructor() const override { \
					return const_cast<ConstWrapperType*>(&m_ConstructorWrapper); \
				} \
				virtual RECore::Object *create(const RECore::DynParams &cConstParams) override { \
					if (cConstParams.getSignature() == m_ConstructorWrapper.getSignature()) { \
						ParamType cParams = static_cast<ConstParamType&>(cConstParams); \
						m_ConstructorWrapper.call(cParams); \
						return static_cast<ParamType&>(cParams).Return; \
					} else return nullptr; \
				} \
				virtual RECore::Object *create(const RECore::String &sConstParams) override { \
					ParamType cParams = ConstParamType::fromString(sConstParams); \
					m_ConstructorWrapper.call(cParams); \
					return cParams.Return; \
				} \
				ConstWrapperType::FuncConstructorType	m_Constructor; \
				ConstWrapperType 						m_ConstructorWrapper; \
		}; \

/**
*  @brief
*    Create class for an event handler
*
*  @param[in] NAME
*    Event handler name
*/
#define __re_constructor_cons_metadata(NAME) \
		class NAME##_DescCreateStaticInstHelper : public RECore::DescCreateStaticInstHelper<NAME##_Desc> { \
		public: \
			NAME##_DescCreateStaticInstHelper() : RECore::DescCreateStaticInstHelper< NAME##_Desc >(){} \
		}; \


//[-------------------------------------------------------]
//[ RTTI macros                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get current module ID
*/
#define re_current_module_id() ( RECore::ModuleID<int>::getModuleID() )

/**
*  @brief
*    Declare enumeration type
*
*  @param[in] ENUM
*    Enumeration name
*/
#define re_enum(ENUM) \
	__re_enum(ENUM) \
	
#define re_flag(ENUM) \
	__re_flag(ENUM) \

/**
*  @brief
*    Declare direct enumeration type (not using enum{}, e.g. for float 'enums')
*
*  @param[in] ENUM
*    Enumeration name
*  @param[in] TYPE
*    Enumeration type
*/
#define re_enum_direct(ENUM, TYPE) \
	__re_enum_direct(ENUM, TYPE) \

/**
*  @brief
*    Declare base enumeration type (add all values of an existing enumeration type)
*
*  @param[in] ENUM
*    Enumeration name of base data type
*/
#define re_enum_base(ENUM) \
	__re_enum_base(ENUM) \

/**
*  @brief
*    Declare enumeration value
*
*  @param[in] VALUE
*    Enumeration value
*  @param[in] DESCRIPTION
*    Enumeration description
*/
#define re_enum_value(VALUE, DESCRIPTION) \
	__re_enum_value(VALUE, DESCRIPTION) \

/**
*  @brief
*    Add enumeration value by directly specifying the value
*
*  @param[in] NAME
*    Enumeration name
*  @param[in] VALUE
*    Enumeration value
*  @param[in] DESCRIPTION
*    Enumeration description
*/
#define re_enum_value_direct(NAME, VALUE, DESCRIPTION) \
	__re_enum_value_direct(NAME, VALUE, DESCRIPTION) \

/**
*  @brief
*    End enumeration
*/
#define re_enum_end \
	__re_enum_end \

/**
*  @brief
*    Declare accessor for the RTTI class for this class
*
*  @param[in] API_EXPORT
*    specifier if the getClass Method should be exported (__declspec(dllexport) under MS VC++ compiler or __attribute__ ((visibility ("default"))) under linux and gcc when the visibility feature is activated)
*/
#define re_class_def(API_EXPORT) \
	public: \
		__re_getclass_def(API_EXPORT) \


/**
*  @brief
*    Begin class properties
*/
#define re_properties \
		/* Class properties */ \
		static inline void _RegisterProperties(RECore::ClassReal *pClass) { \

/**
*  @brief
*    Declare class property
*
*  @param[in] NAME
*    Property name
*  @param[in] VALUE
*    Property value
*/
#define re_property(NAME, VALUE) \
			static_cast<_Class*>(pClass)->addProperty(NAME, VALUE); \

/**
*  @brief
*    End class properties
*/
#define re_properties_end \
		} \

/**
*  @brief
*    Declare attribute with DirectValue as storage type
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define re_attribute_directvalue(NAME, TYPE, DEFAULT, ACCESS) \
	__re_attr_attr_directvalue(NAME, TYPE, DEFAULT, ACCESS) \
	__re_attr_decl(NAME, TYPE) \

/**
*  @brief
*    Declare attribute with GetSet as storage type
*
*  @param[in] CLASSTYPE
*    Name of the class which contains this attribute
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define re_attribute_getset(CLASSTYPE, NAME, TYPE, DEFAULT, ACCESS) \
	__re_attr_attr_getset(CLASSTYPE, NAME, TYPE, DEFAULT, ACCESS) \
	__re_attr_decl(NAME, TYPE) \

/**
*  @brief
*    Declare attribute with ModifyAttr as storage type
*
*  @param[in] BASETYPE
*    Name of the base class which contains the attribute to be modified by this attribute
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] ACCESS
*    Attribute access type (ReadWrite/ReadOnly)
*/
#define re_attribute_modifyattr(BASETYPE, NAME, TYPE, DEFAULT, ACCESS) \
	__re_attr_attr_modifyattr(BASETYPE, NAME, TYPE, DEFAULT, ACCESS) \
	__re_attr_decl(NAME, TYPE) \

/**
*  @brief
*    Declare descriptor for an attribute
*
*  @param[in] NAME
*    Attribute name
*  @param[in] TYPE
*    Attribute type
*  @param[in] DEFAULT
*    Attribute default value
*  @param[in] DESCRIPTION
*    Attribute description
*  @param[in] ANNOTATION
*    Attribute annotation
*/
#define re_attribute_metadata(NAME, TYPE, DEFAULT, ACCESS, DESCRIPTION, ANNOTATION) \
	__re_attr_desc_metadata(NAME, TYPE, DEFAULT, ACCESS, DESCRIPTION, ANNOTATION) \
	class NAME##_DescCreateStaticInstHelper : public RECore::DescCreateStaticInstHelper<NAME##_Desc> { \
	public: \
		NAME##_DescCreateStaticInstHelper() : RECore::DescCreateStaticInstHelper< NAME##_Desc >(){} \
	}; \

/**
*  @brief
*    Declare an enum type for an attribute
*
*  @param[in] ENUM
*    Name of enum type
*/
#define re_enum_type(ENUM) RECore::EnumType< ENUM##__plcore_enum__ >

#define re_enum_type_def(ENUM) RECore::EnumType< _Self::ENUM##__plcore_enum__ >

#define re_enum_type_def2(CLASS, ENUM) RECore::EnumType< CLASS##_ENUM##__plcore_enum__ >

#define re_enum_type_def3(CLASS, ENUM) RECore::EnumType< CLASS::ENUM##__plcore_enum__ >

#define re_enum_type_def4(CLASS, ENUM) RECore::EnumType< RTTI_##CLASS::ENUM##__plcore_enum__ >

/**
*  @brief
*    Declare a flags type for an attribute
*
*  @param[in] ENUM
*    Name of enum type
*/
#define re_flag_type(ENUM) RECore::FlagType< ENUM##__plcore_enum__ >

#define re_flag_type_def3(CLASS, ENUM) RECore::FlagType< CLASS::ENUM##__plcore_enum__ >

#define re_flag_type_def4(CLASS, ENUM) RECore::FlagType< RTTI_##CLASS::ENUM##__plcore_enum__ >


/**
*  @brief
*    Declare method
*
*  @param[in] NAME
*    Method name
*  @param[in] RET
*    Return type
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Method description
*  @param[in] ANNOTATION
*    Method annotation
*/
#define re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	/* RTTI class methods are private to avoid misuse */ \
	__re_method_desc_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__re_method_meth_metadata(NAME) \

#define re_method_15_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_14_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_13_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_12_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_11_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_10_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_9_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, T8, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_8_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, T7, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_7_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, T6, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_6_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, T5, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_5_metadata(NAME, RET, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, T4, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_4_metadata(NAME, RET, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, T3, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_3_metadata(NAME, RET, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, T2, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_2_metadata(NAME, RET, T0, T1, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, T1, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_1_metadata(NAME, RET, T0, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, T0, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_method_0_metadata(NAME, RET, DESCRIPTION, ANNOTATION) \
	re_method_16_metadata(NAME, RET, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

/**
*  @brief
*    Declare signal
*
*  @param[in] NAME
*    Event name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] API_EXPORT
*    specifier if the getDesc Method should be exported (__declspec(dllexport) under MS VC++ compiler or __attribute__ ((visibility ("default"))) under linux and gcc when the visibility feature is activated)
*/
#define re_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
	__re_signal_evnt_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
	__re_signal_decl(NAME) \

#define re_signal_15_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) \
	re_signal_16_def(CLASS, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, RECore::NullType)

#define re_signal_14_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) \
	re_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, RECore::NullType, RECore::NullType)

#define re_signal_13_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
	re_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_12_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
	re_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_11_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
	re_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_10_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) \
	re_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_9_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8) \
	re_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_8_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7) \
	re_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, T7, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_7_def(NAME, T0, T1, T2, T3, T4, T5, T6) \
	re_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, T6, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_6_def(NAME, T0, T1, T2, T3, T4, T5) \
	re_signal_16_def(NAME, T0, T1, T2, T3, T4, T5, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_5_def(NAME, T0, T1, T2, T3, T4) \
	re_signal_16_def(NAME, T0, T1, T2, T3, T4, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_4_def(NAME, T0, T1, T2, T3) \
	re_signal_16_def(NAME, T0, T1, T2, T3, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_3_def(NAME, T0, T1, T2) \
	re_signal_16_def(NAME, T0, T1, T2, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_2_def(NAME, T0, T1) \
	re_signal_16_def(NAME, T0, T1, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_1_def(NAME, T0) \
	re_signal_16_def(NAME, T0, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_signal_0_def(NAME) \
	re_signal_16_def(NAME, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

/**
*  @brief
*    Declare metadata for a signal
*
*  @param[in] NAME
*    Event name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Event description
*  @param[in] ANNOTATION
*    Event annotation
*/
#define re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__re_signal_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	class NAME##_DescCreateStaticInstHelper : public RECore::DescCreateStaticInstHelper<NAME##_Desc> { \
	public: \
		NAME##_DescCreateStaticInstHelper() : RECore::DescCreateStaticInstHelper< NAME##_Desc >(){} \
	}; \

#define re_signal_15_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_14_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_13_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_12_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_11_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_10_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_9_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_8_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_7_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_6_metadata(NAME, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, T5, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_5_metadata(NAME, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, T4, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_4_metadata(NAME, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, T3, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_3_metadata(NAME, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, T2, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_2_metadata(NAME, T0, T1, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, T1, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_1_metadata(NAME, T0, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, T0, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_signal_0_metadata(NAME, DESCRIPTION, ANNOTATION) \
	re_signal_16_metadata(NAME, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

/**
*  @brief
*    Declare slot
*
*  @param[in] CLASSTYPE
*    Name of the class which contains this slot
*  @param[in] NAME
*    Event handler name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] API_EXPORT
*    specifier if the getDesc Method should be exported (__declspec(dllexport) under MS VC++ compiler or __attribute__ ((visibility ("default"))) under linux and gcc when the visibility feature is activated)
*/
#define re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
	__re_slot_evth_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
	__re_slot_decl(NAME) \

#define re_slot_15_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, RECore::NullType)

#define re_slot_14_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, RECore::NullType, RECore::NullType)

#define re_slot_13_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_12_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_11_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_10_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_9_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_8_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, T7, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_7_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, T6, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_6_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, T5, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_5_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, T4, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_4_def(CLASSTYPE, NAME, T0, T1, T2, T3) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, T3, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_3_def(CLASSTYPE, NAME, T0, T1, T2) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, T2, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_2_def(CLASSTYPE, NAME, T0, T1) \
	re_slot_16_def(CLASSTYPE, NAME, T0, T1, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_1_def(CLASSTYPE, NAME, T0) \
	re_slot_16_def(CLASSTYPE, NAME, T0, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

#define re_slot_0_def(CLASSTYPE, NAME) \
	re_slot_16_def(CLASSTYPE, NAME, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType)

/**
*  @brief
*    Declare metadata for a slot
*
*  @param[in] CLASS
*    Name of class which contains the slot which is described by this metadata
*  @param[in] NAME
*    Event handler name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Event handler description
*  @param[in] ANNOTATION
*    Event handler annotation
*/
#define re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__re_slot_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	class NAME##_DescCreateStaticInstHelper : public RECore::DescCreateStaticInstHelper<NAME##_Desc> { \
	public: \
		NAME##_DescCreateStaticInstHelper() : RECore::DescCreateStaticInstHelper< NAME##_Desc >(){} \
	}; \

#define re_slot_15_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_14_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_13_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_12_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_11_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_10_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_9_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_8_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_7_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_6_metadata(NAME, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, T5, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_5_metadata(NAME, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, T4, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_4_metadata(NAME, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, T3, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_3_metadata(NAME, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, T2, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_2_metadata(NAME, T0, T1, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, T1, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_1_metadata(NAME, T0, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_0_metadata(NAME, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_slot_1_metadata(NAME, T0, DESCRIPTION, ANNOTATION) \
	re_slot_16_metadata(NAME, T0, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

/**
*  @brief
*    Declare constructor
*
*  @param[in] NAME
*    Constructor name
*  @param[in] T0 - T15
*    Parameter types
*  @param[in] DESCRIPTION
*    Constructor  description
*  @param[in] ANNOTATION
*    Constructor  annotation
*/
#define re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__re_constructor_desc_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, DESCRIPTION, ANNOTATION) \
	__re_constructor_cons_metadata(NAME) \

#define re_constructor_15_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_14_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_13_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_12_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_11_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_10_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_9_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, T8, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_8_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, T7, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_7_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, T6, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_6_metadata(NAME, T0, T1, T2, T3, T4, T5, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, T5, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_5_metadata(NAME, T0, T1, T2, T3, T4, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, T4, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_4_metadata(NAME, T0, T1, T2, T3, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, T3, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_3_metadata(NAME, T0, T1, T2, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, T2, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_2_metadata(NAME, T0, T1, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, T1, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_1_metadata(NAME, T0, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, T0, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

#define re_constructor_0_metadata(NAME, DESCRIPTION, ANNOTATION) \
	re_constructor_16_metadata(NAME, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, RECore::NullType, DESCRIPTION, ANNOTATION)

/**
*  @brief
*    Declare return type for a method
*
*  @param[in] RET
*    Return type
*
*  @remarks
*    This macro is there to provide a better readability of the RTTI declaration macros by marking the first type
*    explicitly as a return type. Actually you don't need to use it, but it is recommended to do so :-)
*/
#define re_ret_type(RET) RET

// Needed to avoid that every class have to include
// re_properties
// re_properties_end
// even a class which doesn't define properties
class RTTI_MetadataBase
{
	protected:
		static inline void _RegisterProperties(RECore::ClassReal *pClass) { }
};


//[-------------------------------------------------------]
//[ Internal macros                                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create virtual function getClass()
*/
#define __re_getclass_def(API_EXPORT) \
		/* Public virtual RECore::Object function */ \
		virtual API_EXPORT RECore::Class *getClass() const override; \

#define __re_getclass_impl_def(CLASS) \
		RECore::Class * CLASS::getClass() const \
		{ \
			return RTTI_##CLASS::_Class::GetSingleton()->getClass(); \
		} \

/**
*  @brief
*    Create class
*
*  @param[in] CLASS
*    Class name (without namespace)
*  @param[in] NAMESPACE
*    Namespace
*  @param[in] BASECLASS
*    Base class name (with namespace)
*  @param[in] DESCRIPTION
*    Class description
*/
#define __re_class_metadata(CLASS, NAMESPACE, BASECLASS, BASECLASSNAME, DESCRIPTION) \
class RTTI_##CLASS : public RECore::RTTI_MetadataBase { \
	/* All RTTI members are public */ \
	public: \
		/* Class type */ \
		typedef CLASS     _Self; \
		typedef BASECLASS _Base; \
		\
		/* Class description */ \
		class _Class : public RECore::ClassReal { \
			friend class CLASS; \
			friend class RTTI_##CLASS; \
			public: \
				/* Check base class */ \
				static void Error() { \
					RECore::CheckBaseClass<CLASS, BASECLASS>::Type::Error(); \
				} \
				\
				/* Singleton */ \
				static _Class *GetSingleton(bool bGet = true) {\
					static bool    MyShutdown = false; \
					static _Class *MyInstance = nullptr; \
					if (bGet) { \
						/* Get or create instance */ \
						if (!MyInstance && !MyShutdown) { \
							MyInstance = new _Class(); \
							RTTI_##CLASS::_RegisterProperties(MyInstance); \
						} \
					} else { \
						/* Destroy instance and make sure that it won't be recreated */ \
						MyShutdown = true; \
						if (MyInstance) { \
							delete MyInstance; \
							MyInstance = nullptr; \
						} \
					} \
					return MyInstance; \
				} \
				\
				/* Constructor */ \
				_Class() : RECore::ClassReal(RECore::ModuleID<int>::getModuleID(), #CLASS, DESCRIPTION, NAMESPACE, BASECLASSNAME) { \
				}; \
				\
				/* Destructor */ \
				virtual ~_Class() { \
				}; \
		}; \


//[-------------------------------------------------------]
//[ RTTI macros                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Declare metadata class
*
*  @param[in] CLASS
*    Class name (without namespace)
*  @param[in] NAMESPACE
*    Namespace
*  @param[in] BASECLASS
*    Base class name (with namespace)
*  @param[in] DESCRIPTION
*    Class description
*/
#define re_class_metadata(CLASS, NAMESPACE, BASECLASS, DESCRIPTION) \
	__re_class_metadata(CLASS, NAMESPACE, BASECLASS, #BASECLASS, DESCRIPTION) \
	__re_guard(CLASS) \

#define re_class_def_end \
	private:

/**
*  @brief
*    Declare metadata class - internal (only for RECore::Object)
*
*  @param[in] CLASS
*    Class name (without namespace)
*  @param[in] NAMESPACE
*    Namespace
*  @param[in] BASECLASS
*    Base class name (with namespace)
*  @param[in] DESCRIPTION
*    Class description
*
*  @remarks
*    This macro is only used for RECore::Object. As this is the base class for all
*    RTTI classes, the virtual function getClass() can not be overwritten here.
*/
#define re_class_def_internal(CLASS, API_EXPORT) \
	public: \
		typedef CLASS     _Self; \
		typedef RECore::ObjectBase _Base; \
		__re_getclass_def(API_EXPORT) \

#define re_class_metadata_internal(CLASS, NAMESPACE, BASECLASS, DESCRIPTION) \
	__re_class_metadata(CLASS, NAMESPACE, RECore::ObjectBase, #BASECLASS, DESCRIPTION) \
	__re_guard(CLASS) \

/**
*  @brief
*    End metadata class declaration
*/
#define re_class_metadata_end(CLASS) \
	}; \
	\
	__re_getclass_impl_def(CLASS) \
	/* Guard */ \
	RTTI_##CLASS::_Guard cGuard_##CLASS; \


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
