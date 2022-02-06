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
#include "RECore/Reflect/Var/Var.h"
#include "RECore/Reflect/Var/VarDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Attribute of a class
*
*  @remarks
*    This class template represents attributes (variables that belong to objects).
*/
template <typename T, typename ACCESS, typename STORAGE>
class Attribute {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage type
		typedef typename Type<T>::_Type _Type;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] DefaultValue
		*    Default value for the attribute
		*  @param[in] pObject
		*    Pointer to object to which the attribute belongs
		*/
		Attribute(_Type DefaultValue, Object *pObject) : m_Value(DefaultValue, pObject)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Attribute()
		{
		}

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value
		*/
		operator _Type() const
		{
			// Get value
			return m_Value.get();
		}

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] Value
		*    New value
		*
		*  @return
		*    Reference to this instance
		*/
		Attribute &operator =(const _Type &Value)
		{
			// Set value
			m_Value.set(Value);
			return *this;
		}

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value
		*/
		inline _Type get() const
		{
			// Return value
			return m_Value.get();
		}

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] Value
		*    New value
		*/
		inline void set(const _Type &Value)
		{
			m_Value.set(Value);
		}

		/**
		*  @brief
		*    Get default value
		*
		*  @return
		*    Default value
		*/
		inline _Type defaultValue() const
		{
			// Return default value
			return m_Value.getDefault();
		}

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Variable value
		VarAccess<_Type, ACCESS, STORAGE> m_Value;	/**< Value storage and access */


};


/**
*  @brief
*    RTTI wrapper class for an Attribute
*
*  @remarks
*    This class template represents a RTTI wrapper for attributes (variables that belong to objects).
*/
template <typename T, typename ACCESS = RECore::AccessReadWrite, typename STORAGE = RECore::StorageDirectValue >
class AttributeWrapper : public DynVar {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage type
		typedef typename Type<T>::_Type _Type;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		AttributeWrapper(Attribute<_Type, ACCESS, STORAGE> *pItem, const VarDesc *pDesc) :
			m_pValue(pItem), m_pDesc(pDesc)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AttributeWrapper()
		{
		}

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value
		*/
		operator _Type() const
		{
			// Get value
			return m_pValue->get();
		}

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] Value
		*    New value
		*
		*  @return
		*    Reference to this instance
		*/
		AttributeWrapper &operator =(const _Type &Value)
		{
			// Set value
			m_pValue->set(Value);
			return *this;
		}

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value
		*/
		inline _Type get() const
		{
			// Return value
			return m_pValue->get();
		}

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] Value
		*    New value
		*/
		inline void set(const _Type &Value)
		{
			m_pValue->set(Value);
		}

		/**
		*  @brief
		*    Get default value
		*
		*  @return
		*    Default value
		*/
		inline _Type defaultValue() const
		{
			// Return default value
			return m_pValue->getDefault();
		}


	//[-------------------------------------------------------]
	//[ Public virtual DynVar functions                       ]
	//[-------------------------------------------------------]
	public:
		virtual DynTypeInfo &getType() const override
		{
			return TypeInfo< T >::Instance;
		}

		virtual String getTypeName() const override
		{
			return Type<T>::getTypeName();
		}

		virtual int getTypeID() const override
		{
			return Type<T>::TypeID;
		}

		virtual bool isDefault() const override
		{
			return (m_pValue->get() == m_pValue->defaultValue());
		}

		virtual void setDefault() override
		{
			m_pValue->set(m_pValue->defaultValue());
		}

		virtual String getDefault() const override
		{
			return Type<T>::convertToString(m_pValue->defaultValue());
		}

		virtual void setVar(const DynVar &cValue) override
		{
			m_pValue->set(Type<T>::convertFromVar(cValue));
		}

		virtual bool getBool() const override
		{
			return Type<T>::convertToBool(m_pValue->get());
		}

		virtual void setBool(bool bValue) override
		{
			m_pValue->set(Type<T>::ConvertFromBool(bValue));
		}

		virtual int getInt() const override
		{
			return Type<T>::convertToInt(m_pValue->get());
		}

		virtual void setInt(int nValue) override
		{
			m_pValue->set(Type<T>::convertFromInt(nValue));
		}

		virtual int8 getInt8() const override
		{
			return Type<T>::convertToInt8(m_pValue->get());
		}

		virtual void setInt8(int8 nValue) override
		{
			m_pValue->set(Type<T>::convertFromInt8(nValue));
		}

		virtual int16 getInt16() const override
		{
			return Type<T>::convertToInt16(m_pValue->get());
		}

		virtual void setInt16(int16 nValue) override
		{
			m_pValue->set(Type<T>::convertFromInt16(nValue));
		}

		virtual int32 getInt32() const override
		{
			return Type<T>::convertToInt32(m_pValue->get());
		}

		virtual void setInt32(int32 nValue) override
		{
			m_pValue->set(Type<T>::convertFromInt32(nValue));
		}

		virtual int64 getInt64() const override
		{
			return Type<T>::convertToInt64(m_pValue->get());
		}

		virtual void setInt64(int64 nValue) override
		{
			// Disable an annoying warning, we know that this conversion will introduce a potential loss of data
			RE_WARNING_PUSH
				RE_WARNING_DISABLE(4244)	// warning C4244: 'argument' : conversion from 'RECore::int64' to 'RECore::int32', possible loss of data"
				m_pValue->set(Type<T>::convertFromInt64(nValue));
			RE_WARNING_POP
		}

		virtual uint8 getUInt8() const override
		{
			return Type<T>::convertToUInt8(m_pValue->get());
		}

		virtual void setUInt8(uint8 nValue) override
		{
			m_pValue->set(Type<T>::convertFromUInt8(nValue));
		}

		virtual uint16 getUInt16() const override
		{
			return Type<T>::convertToUInt16(m_pValue->get());
		}

		virtual void setUInt16(uint16 nValue) override
		{
			m_pValue->set(Type<T>::convertFromUInt16(nValue));
		}

		virtual uint32 getUInt32() const override
		{
			return Type<T>::convertToUInt32(m_pValue->get());
		}

		virtual void setUInt32(uint32 nValue) override
		{
			m_pValue->set(Type<T>::convertFromUInt32(nValue));
		}

		virtual uint64 getUInt64() const override
		{
			return Type<T>::convertToUInt64(m_pValue->get());
		}

		virtual void setUInt64(uint64 nValue) override
		{
			m_pValue->set(Type<T>::convertFromUInt64(nValue));
		}

		virtual uint_ptr getUIntPtr() const override
		{
			return Type<T>::convertToUIntPtr(m_pValue->get());
		}

		virtual void setUIntPtr(uint_ptr nValue) override
		{
			m_pValue->set(Type<T>::convertFromUIntPtr(nValue));
		}

		virtual float getFloat() const override
		{
			return Type<T>::convertToFloat(m_pValue->get());
		}

		virtual void setFloat(float fValue) override
		{
			m_pValue->set(Type<T>::convertFromFloat(fValue));
		}

		virtual double getDouble() const override
		{
			return Type<T>::convertToDouble(m_pValue->get());
		}

		virtual void setDouble(double dValue) override
		{
			m_pValue->set(Type<T>::convertFromDouble(dValue));
		}

		virtual String getString() const override
		{
			return Type<T>::convertToString(m_pValue->get());
		}

		virtual void setString(const String &sValue) override
		{
			m_pValue->set(Type<T>::convertFromString(sValue));
		}

		virtual const VarDesc *getDesc() const override
		{
			return m_pDesc;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Variable value
		Attribute<_Type, ACCESS, STORAGE>	* m_pValue;	/**< Attribute instance which is wrapped by this instance */
		const VarDesc						* m_pDesc;	/** Descriptor instance for the attribute */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
