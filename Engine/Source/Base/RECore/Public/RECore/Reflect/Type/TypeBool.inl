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
*    Type wrapper for 'bool'
*/
template <>
class Type<bool> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef bool _Type;			/**< Real type */
		typedef bool _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = TypeBool;

		// Get type name
		static String getTypeName()
		{
			return "bool";
		}

		// Convert var to bool
		static bool convertFromVar(const DynVar &cValue)
		{
			return cValue.getBool();
		}

		// Convert bool to bool
		static bool convertToBool(bool bValue)
		{
			return static_cast<bool>(bValue);
		}

		// Convert bool to bool
		static bool ConvertFromBool(bool bValue)
		{
			return static_cast<bool>(bValue);
		}

		// Convert bool to int
		static int convertToInt(bool bValue)
		{
			return static_cast<int>(bValue);
		}

		// Convert int to bool
		static bool convertFromInt(int nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int8
		static int8 convertToInt8(bool bValue)
		{
			return static_cast<int8>(bValue);
		}

		// Convert int8 to bool
		static bool convertFromInt8(int8 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int16
		static int16 convertToInt16(bool bValue)
		{
			return static_cast<int16>(bValue);
		}

		// Convert int16 to bool
		static bool convertFromInt16(int16 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int32
		static int32 convertToInt32(bool bValue)
		{
			return static_cast<int32>(bValue);
		}

		// Convert int32 to bool
		static bool convertFromInt32(int32 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int64
		static int64 convertToInt64(bool bValue)
		{
			return static_cast<int64>(bValue);
		}

		// Convert int64 to bool
		static bool convertFromInt64(int64 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint8
		static uint8 convertToUInt8(bool bValue)
		{
			return static_cast<uint8>(bValue);
		}

		// Convert uint8 to bool
		static bool convertFromUInt8(uint8 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint16
		static uint16 convertToUInt16(bool bValue)
		{
			return static_cast<uint16>(bValue);
		}

		// Convert uint16 to bool
		static bool convertFromUInt16(uint16 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint32
		static uint32 convertToUInt32(bool bValue)
		{
			return static_cast<uint32>(bValue);
		}

		// Convert uint32 to bool
		static bool convertFromUInt32(uint32 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint64
		static uint64 convertToUInt64(bool bValue)
		{
			return static_cast<uint64>(bValue);
		}

		// Convert uint64 to bool
		static bool convertFromUInt64(uint64 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint_ptr
		static uint_ptr convertToUIntPtr(bool bValue)
		{
			return static_cast<uint_ptr>(bValue);
		}

		// Convert uint_ptr to bool
		static bool convertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to float
		static float convertToFloat(bool bValue)
		{
			return static_cast<float>(bValue);
		}

		// Convert float to bool
		static bool convertFromFloat(float fValue)
		{
			return static_cast<bool>(fValue != 0.0f);
		}

		// Convert bool to double
		static double convertToDouble(bool bValue)
		{
			return static_cast<double>(bValue);
		}

		// Convert double to bool
		static bool convertFromDouble(double dValue)
		{
			return static_cast<bool>(dValue != 0.0);
		}

		// Convert bool to string
		static String convertToString(bool bValue)
		{
			return (bValue ? "true" : "false");
		}

		// Convert string to bool
		static bool convertFromString(const String &sString)
		{
			return sString.getBool();
		}

		// Convert real to storage type
		static bool convertRealToStorage(bool bValue)
		{
			return bValue;
		}

		// Convert storage to real type
		static bool convertStorageToReal(bool bValue)
		{
			return bValue;
		}

};
