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
class Type< EnumTypePlain<ENUM> > {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef ENUM _Type;			/**< Real type */
		typedef ENUM _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = Type<int>::TypeID;

		// Get type name
		static String getTypeName()
		{
			return Type<int>::getTypeName();
		}

		// Convert var to type
		static ENUM convertFromVar(const DynVar &cValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromVar(cValue));
		}

		// Convert type to bool
		static bool convertToBool(ENUM Value)
		{
			return Type<int>::convertToBool(static_cast<int>(Value));
		}

		// Convert bool to type
		static ENUM ConvertFromBool(bool bValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromBool(bValue));
		}

		// Convert type to int
		static int convertToInt(ENUM Value)
		{
			return Type<int>::convertToInt(static_cast<int>(Value));
		}

		// Convert int to type
		static ENUM convertFromInt(int nValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromInt(nValue));
		}

		// Convert type to int8
		static int8 convertToInt8(ENUM Value)
		{
			return Type<int>::convertToInt8(static_cast<int>(Value));
		}

		// Convert int8 to type
		static ENUM convertFromInt8(int8 nValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromInt8(nValue));
		}

		// Convert type to int16
		static int16 convertToInt16(ENUM Value)
		{
			return Type<int>::convertToInt16(static_cast<int>(Value));
		}

		// Convert int16 to type
		static ENUM convertFromInt16(int16 nValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromInt16(nValue));
		}

		// Convert type to int32
		static int32 convertToInt32(ENUM Value)
		{
			return Type<int>::convertToInt32(static_cast<int>(Value));
		}

		// Convert int32 to type
		static ENUM convertFromInt32(int32 nValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromInt32(nValue));
		}

		// Convert type to int64
		static int64 convertToInt64(ENUM Value)
		{
			return Type<int>::convertToInt64(static_cast<int>(Value));
		}

		// Convert int64 to type
		static ENUM convertFromInt64(int64 nValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromInt64(nValue));
		}

		// Convert type to uint8
		static uint8 convertToUInt8(ENUM Value)
		{
			return Type<int>::convertToUInt8(static_cast<int>(Value));
		}

		// Convert uint8 to type
		static ENUM convertFromUInt8(uint8 nValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromUInt8(nValue));
		}

		// Convert type to uint16
		static uint16 convertToUInt16(ENUM Value)
		{
			return Type<int>::convertToUInt16(static_cast<int>(Value));
		}

		// Convert uint16 to type
		static ENUM convertFromUInt16(uint16 nValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromUInt16(nValue));
		}

		// Convert type to uint32
		static uint32 convertToUInt32(ENUM Value)
		{
			return Type<int>::convertToUInt32(static_cast<int>(Value));
		}

		// Convert uint32 to type
		static ENUM convertFromUInt32(uint32 nValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromUInt32(nValue));
		}

		// Convert type to uint64
		static uint64 convertToUInt64(ENUM Value)
		{
			return Type<int>::convertToUInt64(static_cast<int>(Value));
		}

		// Convert uint64 to type
		static ENUM convertFromUInt64(uint64 nValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromUInt64(nValue));
		}

		// Convert type to uint_ptr
		static uint_ptr convertToUIntPtr(ENUM Value)
		{
			return Type<int>::convertToUIntPtr(static_cast<int>(Value));
		}

		// Convert uint_ptr to type
		static ENUM convertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromUIntPtr(nValue));
		}

		// Convert type to float
		static float convertToFloat(ENUM Value)
		{
			return Type<int>::convertToFloat(static_cast<int>(Value));
		}

		// Convert float to type
		static ENUM convertFromFloat(float fValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromFloat(fValue));
		}

		// Convert type to double
		static double convertToDouble(ENUM Value)
		{
			return Type<int>::convertToDouble(static_cast<int>(Value));
		}

		// Convert double to type
		static ENUM convertFromDouble(double dValue)
		{
			return static_cast<ENUM>(Type<int>::convertFromDouble(dValue));
		}

		// Convert type to string
		static String convertToString(ENUM Value)
		{
			return Type<int>::convertToString(static_cast<int>(Value));
		}

		// Convert string to type
		static ENUM convertFromString(const String &sString)
		{
			return static_cast<ENUM>(Type<int>::convertFromString(sString));
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
