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
*    Type wrapper for 'int'
*/
template <>
class Type<int> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef int _Type;			/**< Real type */
		typedef int _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = TypeInt;

		// Get type name
		static String getTypeName()
		{
			return "int";
		}

		// Convert var to int
		static int convertFromVar(const DynVar &cValue)
		{
			return cValue.getInt();
		}

		// Convert int to bool
		static bool convertToBool(int nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int
		static int ConvertFromBool(bool bValue)
		{
			return static_cast<int>(bValue);
		}

		// Convert int to int
		static int convertToInt(int nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to int
		static int convertFromInt(int nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to int8
		static int8 convertToInt8(int nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to int
		static int convertFromInt8(int8 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to int16
		static int16 convertToInt16(int nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to int
		static int convertFromInt16(int16 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to int32
		static int32 convertToInt32(int nValue)
		{
			return static_cast<int32>(nValue);
		}

		// Convert int32 to int
		static int convertFromInt32(int32 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to int64
		static int64 convertToInt64(int nValue)
		{
			return static_cast<int64>(nValue);
		}

		// Convert int64 to int
		static int convertFromInt64(int64 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to uint8
		static uint8 convertToUInt8(int nValue)
		{
			return static_cast<uint8>(nValue);
		}

		// Convert uint8 to int
		static int convertFromUInt8(uint8 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to uint16
		static uint16 convertToUInt16(int nValue)
		{
			return static_cast<uint16>(nValue);
		}

		// Convert uint16 to int
		static int convertFromUInt16(uint16 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to uint32
		static uint32 convertToUInt32(int nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to int
		static int convertFromUInt32(uint32 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to uint64
		static uint64 convertToUInt64(int nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to int
		static int convertFromUInt64(uint64 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to uint_ptr
		static uint_ptr convertToUIntPtr(int nValue)
		{
			return static_cast<uint_ptr>(nValue);
		}

		// Convert uint_ptr to int
		static int convertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to float
		static float convertToFloat(int nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to int
		static int convertFromFloat(float fValue)
		{
			return static_cast<int>(fValue);
		}

		// Convert int to double
		static double convertToDouble(int nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to int
		static int convertFromDouble(double dValue)
		{
			return static_cast<int>(dValue);
		}

		// Convert int to string
		static String convertToString(int nValue)
		{
			return String() + nValue;
		}

		// Convert string to int
		static int convertFromString(const String &sString)
		{
			return sString.getInt();
		}

		// Convert real to storage type
		static int convertRealToStorage(int nValue)
		{
			return nValue;
		}

		// Convert storage to real type
		static int convertStorageToReal(int nValue)
		{
			return nValue;
		}

};
