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
*    Type wrapper for 'int16'
*/
template <>
class Type<int16> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef int16 _Type;		/**< Real type */
		typedef int16 _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const uint32 TypeID = TypeInt16;

		// Get type name
		static String getTypeName()
		{
			return "int16";
		}

		// Convert var to int16
		static int16 convertFromVar(const DynVar &cValue)
		{
			return cValue.getInt16();
		}

		// Convert int16 to bool
		static bool convertToBool(int16 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int16
		static int16 ConvertFromBool(bool bValue)
		{
			return static_cast<int16>(bValue);
		}

		// Convert int16 to int
		static int convertToInt(int16 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to int16
		static int16 convertFromInt(int nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to int8
		static int8 convertToInt8(int16 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to int16
		static int16 convertFromInt8(int8 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to int16
		static int16 convertToInt16(int16 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to int16
		static int16 convertFromInt16(int16 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to int32
		static int32 convertToInt32(int16 nValue)
		{
			return static_cast<int32>(nValue);
		}

		// Convert int32 to int16
		static int16 convertFromInt32(int32 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to int64
		static int64 convertToInt64(int16 nValue)
		{
			return static_cast<int64>(nValue);
		}

		// Convert int64 to int16
		static int16 convertFromInt64(int64 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to uint8
		static uint8 convertToUInt8(int16 nValue)
		{
			return static_cast<uint8>(nValue);
		}

		// Convert uint8 to int16
		static int16 convertFromUInt8(uint8 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to uint16
		static uint16 convertToUInt16(int16 nValue)
		{
			return static_cast<uint16>(nValue);
		}

		// Convert uint16 to int16
		static int16 convertFromUInt16(uint16 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to uint32
		static uint32 convertToUInt32(int16 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to int16
		static int16 convertFromUInt32(uint32 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to uint64
		static uint64 convertToUInt64(int16 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to int16
		static int16 convertFromUInt64(uint64 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to uint_ptr
		static uint_ptr convertToUIntPtr(int16 nValue)
		{
			return static_cast<uint_ptr>(nValue);
		}

		// Convert uint_ptr to int16
		static int16 convertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to float
		static float convertToFloat(int16 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to int16
		static int16 convertFromFloat(float fValue)
		{
			return static_cast<int16>(fValue);
		}

		// Convert int16 to double
		static double convertToDouble(int16 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to int16
		static int16 convertFromDouble(double dValue)
		{
			return static_cast<int16>(dValue);
		}

		// Convert int16 to string
		static String convertToString(int16 nValue)
		{
			return String() + static_cast<int>(nValue);
		}

		// Convert string to int16
		static int16 convertFromString(const String &sString)
		{
			return static_cast<int16>(sString.getInt());
		}

		// Convert real to storage type
		static int16 convertRealToStorage(int16 nValue)
		{
			return nValue;
		}

		// Convert storage to real type
		static int16 convertStorageToReal(int16 nValue)
		{
			return nValue;
		}

};
