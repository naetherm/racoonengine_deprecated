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
*    Type wrapper for 'uint64'
*/
template <>
class Type<uint64> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef uint64 _Type;			/**< Real type */
		typedef uint64 _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const uint32 TypeID = TypeUInt64;

		// Get type name
		static String getTypeName()
		{
			return "uint64";
		}

		// Convert var to uint64
		static uint64 convertFromVar(const DynVar &cValue)
		{
			return cValue.getUInt64();
		}

		// Convert uint64 to bool
		static bool convertToBool(uint64 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint64
		static uint64 ConvertFromBool(bool bValue)
		{
			return static_cast<uint64>(bValue);
		}

		// Convert uint64 to int
		static int convertToInt(uint64 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to uint64
		static uint64 convertFromInt(int nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to int8
		static int8 convertToInt8(uint64 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to uint64
		static uint64 convertFromInt8(int8 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to int16
		static int16 convertToInt16(uint64 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to uint64
		static uint64 convertFromInt16(int16 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to int32
		static int32 convertToInt32(uint64 nValue)
		{
			return static_cast<int32>(nValue);
		}

		// Convert int32 to uint64
		static uint64 convertFromInt32(int32 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to int64
		static int64 convertToInt64(uint64 nValue)
		{
			return static_cast<int64>(nValue);
		}

		// Convert int64 to uint64
		static uint64 convertFromInt64(int64 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to uint8
		static uint8 convertToUInt8(uint64 nValue)
		{
			return static_cast<uint8>(nValue);
		}

		// Convert uint8 to uint64
		static uint64 convertFromUInt8(uint8 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to uint16
		static uint16 convertToUInt16(uint64 nValue)
		{
			return static_cast<uint16>(nValue);
		}

		// Convert uint16 to uint64
		static uint64 convertFromUInt16(uint16 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to uint32
		static uint32 convertToUInt32(uint64 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to uint64
		static uint64 convertFromUInt32(uint32 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to uint64
		static uint64 convertToUInt64(uint64 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to uint64
		static uint64 convertFromUInt64(uint64 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to uint_ptr
		static uint_ptr convertToUIntPtr(uint64 nValue)
		{
			return static_cast<uint_ptr>(nValue);
		}

		// Convert uint_ptr to uint64
		static uint64 convertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to float
		static float convertToFloat(uint64 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to uint64
		static uint64 convertFromFloat(float fValue)
		{
			return static_cast<uint64>(fValue);
		}

		// Convert uint64 to double
		static double convertToDouble(uint64 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to uint64
		static uint64 convertFromDouble(double dValue)
		{
			return static_cast<uint64>(dValue);
		}

		// Convert uint64 to string
		static String convertToString(uint64 nValue)
		{
			return String() + static_cast<int>(nValue);
		}

		// Convert string to uint64
		static uint64 convertFromString(const String &sString)
		{
			return sString.getInt();
		}

		// Convert real to storage type
		static uint64 convertRealToStorage(uint64 nValue)
		{
			return nValue;
		}

		// Convert storage to real type
		static uint64 convertStorageToReal(uint64 nValue)
		{
			return nValue;
		}

};
