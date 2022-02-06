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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for enum types
*/
template <typename ENUM>
class Type< EnumType<ENUM> > {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef typename ENUM::_BaseType _BaseType;
		typedef typename ENUM::_Type	 _Type;			/**< Real type */
		typedef typename ENUM::_Type	 _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = Type<_BaseType>::TypeID;

		// Get type name
		static String getTypeName()
		{
			return Type<_BaseType>::getTypeName();
		}

		// Convert var to type
		static _Type convertFromVar(const DynVar &cValue)
		{
			if (cValue.getTypeID() == TypeString)
				return convertFromString(cValue.getString());
			else
				return static_cast<_Type>(Type<_BaseType>::convertFromVar(cValue));
		}

		// Convert type to bool
		static bool convertToBool(_Type Value)
		{
			return Type<_BaseType>::convertToBool(static_cast<_BaseType>(Value));
		}

		// Convert bool to type
		static _Type ConvertFromBool(bool bValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromBool(bValue));
		}

		// Convert type to int
		static int convertToInt(_Type Value)
		{
			return Type<_BaseType>::convertToInt(static_cast<_BaseType>(Value));
		}

		// Convert int to type
		static _Type convertFromInt(int nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromInt(nValue));
		}

		// Convert type to int8
		static int8 convertToInt8(_Type Value)
		{
			return Type<_BaseType>::convertToInt8(static_cast<_BaseType>(Value));
		}

		// Convert int8 to type
		static _Type convertFromInt8(int8 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromInt8(nValue));
		}

		// Convert type to int16
		static int16 convertToInt16(_Type Value)
		{
			return Type<_BaseType>::convertToInt16(static_cast<_BaseType>(Value));
		}

		// Convert int16 to type
		static _Type convertFromInt16(int16 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromInt16(nValue));
		}

		// Convert type to int32
		static int32 convertToInt32(_Type Value)
		{
			return Type<_BaseType>::convertToInt32(static_cast<_BaseType>(Value));
		}

		// Convert int32 to type
		static _Type convertFromInt32(int32 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromInt32(nValue));
		}

		// Convert type to int64
		static int64 convertToInt64(_Type Value)
		{
			return Type<_BaseType>::convertToInt64(static_cast<_BaseType>(Value));
		}

		// Convert int64 to type
		static _Type convertFromInt64(int64 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromInt64(nValue));
		}

		// Convert type to uint8
		static uint8 convertToUInt8(_Type Value)
		{
			return Type<_BaseType>::convertToUInt8(static_cast<_BaseType>(Value));
		}

		// Convert uint8 to type
		static _Type convertFromUInt8(uint8 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromUInt8(nValue));
		}

		// Convert type to uint16
		static uint16 convertToUInt16(_Type Value)
		{
			return Type<_BaseType>::convertToUInt16(static_cast<_BaseType>(Value));
		}

		// Convert uint16 to type
		static _Type convertFromUInt16(uint16 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromUInt16(nValue));
		}

		// Convert type to uint32
		static uint32 convertToUInt32(_Type Value)
		{
			return Type<_BaseType>::convertToUInt32(static_cast<_BaseType>(Value));
		}

		// Convert uint32 to type
		static _Type convertFromUInt32(uint32 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromUInt32(nValue));
		}

		// Convert type to uint64
		static uint64 convertToUInt64(_Type Value)
		{
			return Type<_BaseType>::convertToUInt64(static_cast<_BaseType>(Value));
		}

		// Convert uint64 to type
		static _Type convertFromUInt64(uint64 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromUInt64(nValue));
		}

		// Convert type to uint_ptr
		static uint_ptr convertToUIntPtr(_Type Value)
		{
			return Type<_BaseType>::convertToUIntPtr(static_cast<_BaseType>(Value));
		}

		// Convert uint_ptr to type
		static _Type convertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromUIntPtr(nValue));
		}

		// Convert type to float
		static float convertToFloat(_Type Value)
		{
			return Type<_BaseType>::convertToFloat(static_cast<_BaseType>(Value));
		}

		// Convert float to type
		static _Type convertFromFloat(float fValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromFloat(fValue));
		}

		// Convert type to double
		static double convertToDouble(_Type Value)
		{
			return Type<_BaseType>::convertToDouble(static_cast<_BaseType>(Value));
		}

		// Convert double to type
		static _Type convertFromDouble(double dValue)
		{
			return static_cast<_Type>(Type<_BaseType>::convertFromDouble(dValue));
		}

		// Convert type to string
		static String convertToString(_Type Value)
		{
			String sValue, sDesc;
			if (ENUM::getEnumValue(-1, Value, sValue, sDesc))
				return sValue;
			else
				return Type<_BaseType>::convertToString(static_cast<_BaseType>(Value));
		}

		// Convert string to type
		static _Type convertFromString(const String &sString)
		{
			String sValue = sString;
			String sDesc;
			_Type Value;
			if (ENUM::getEnumValue(-1, Value, sValue, sDesc))
				return Value;
			else
				return static_cast<_Type>(Type<_BaseType>::convertFromString(sString));
		}

		// Convert real to storage type
		static _StorageType convertRealToStorage(_Type nValue)
		{
			return nValue;
		}

		// Convert storage to real type
		static _Type convertStorageToReal(_StorageType nValue)
		{
			return nValue;
		}

};
