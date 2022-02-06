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
#include "RECore/Core/SmartPtr.h"
#include "RECore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class DynTypeInfo;
class VarDesc;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual base class for variables
*
*  @remarks
*    This is the virtual base class to access variables and attributes dynamically.
*    It is a virtual interface that allows you to access a variable regardless
*    of it's actual type and storage.
*/
class DynVar {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		RECORE_API DynVar();

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~DynVar();


	//[-------------------------------------------------------]
	//[ Public virtual DynVar functions                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get variable descriptor
		*
		*  @return
		*    Descriptor (can be a null pointer)
		*/
		RECORE_API virtual const VarDesc *getDesc() const;

		/**
		*  @brief
		*    Get type
		*
		*  @return
		*    Type info
		*/
		RECORE_API virtual DynTypeInfo &getType() const;

		/**
		*  @brief
		*    Get type ID
		*
		*  @return
		*    Type ID
		*/
		RECORE_API virtual int getTypeID() const;

		/**
		*  @brief
		*    Get type name
		*
		*  @return
		*    Type name (e.g. "int")
		*/
		RECORE_API virtual String getTypeName() const;

		/**
		*  @brief
		*    Check if variable is set to default value
		*
		*  @return
		*    'true' if default value is set, else 'false'
		*/
		RECORE_API virtual bool isDefault() const;

		/**
		*  @brief
		*    Set variable to default value
		*/
		RECORE_API virtual void setDefault();

		/**
		*  @brief
		*    Get default value as string
		*
		*  @return
		*    Default value as string
		*/
		RECORE_API virtual String getDefault() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] cValue
		*    Value as dynamic var
		*/
		RECORE_API virtual void setVar(const DynVar &cValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as bool
		*/
		RECORE_API virtual bool getBool() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] bValue
		*    Value as bool
		*/
		RECORE_API virtual void setBool(bool bValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as int
		*/
		RECORE_API virtual int getInt() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as int
		*/
		RECORE_API virtual void setInt(int nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as int8
		*/
		RECORE_API virtual int8 getInt8() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as int8
		*/
		RECORE_API virtual void setInt8(int8 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as int16
		*/
		RECORE_API virtual int16 getInt16() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as int16
		*/
		RECORE_API virtual void setInt16(int16 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as int32
		*/
		RECORE_API virtual int32 getInt32() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as int32
		*/
		RECORE_API virtual void setInt32(int32 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as int64
		*/
		RECORE_API virtual int64 getInt64() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as int64
		*/
		RECORE_API virtual void setInt64(int64 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as uint8
		*/
		RECORE_API virtual uint8 getUInt8() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as uint8
		*/
		RECORE_API virtual void setUInt8(uint8 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as uint16
		*/
		RECORE_API virtual uint16 getUInt16() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as uint16
		*/
		RECORE_API virtual void setUInt16(uint16 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as uint32
		*/
		RECORE_API virtual uint32 getUInt32() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as uint32
		*/
		RECORE_API virtual void setUInt32(uint32 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as uint64
		*/
		RECORE_API virtual uint64 getUInt64() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as uint64
		*/
		RECORE_API virtual void setUInt64(uint64 nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as uint_ptr
		*/
		RECORE_API virtual uint_ptr getUIntPtr() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] nValue
		*    Value as uint_ptr
		*/
		RECORE_API virtual void setUIntPtr(uint_ptr nValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as float
		*/
		RECORE_API virtual float getFloat() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] fValue
		*    Value as float
		*/
		RECORE_API virtual void setFloat(float fValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as double
		*/
		RECORE_API virtual double getDouble() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] dValue
		*    Value as double
		*/
		RECORE_API virtual void setDouble(double dValue);

		/**
		*  @brief
		*    Get value
		*
		*  @return
		*    Value as string
		*/
		RECORE_API virtual String getString() const;

		/**
		*  @brief
		*    Set value
		*
		*  @param[in] sValue
		*    Value as string
		*/
		RECORE_API virtual void setString(const String &sValue);


};


//[-------------------------------------------------------]
//[ Type definitions                                      ]
//[-------------------------------------------------------]
typedef SmartPtr<DynVar> DynVarPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
