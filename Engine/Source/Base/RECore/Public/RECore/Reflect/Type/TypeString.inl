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
*    Type wrapper for 'String'
*/
template <>
class Type<String> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef String _Type;			/**< Real type */
		typedef String _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = TypeString;

		// Get type name
		static String getTypeName()
		{
			return "string";
		}

		// Convert var to string
		static String convertFromVar(const DynVar &cValue)
		{
			return cValue.getString();
		}

		// Convert string to bool
		static bool convertToBool(const String &sString)
		{
			return sString.getBool();
		}

		// Convert bool to string
		static String ConvertFromBool(bool bValue)
		{
			return (String() + bValue);
		}

		// Convert string to int
		static int convertToInt(const String &sString)
		{
			return sString.getInt();
		}

		// Convert int to string
		static String convertFromInt(int nValue)
		{
			return (String() + nValue);
		}

		// Convert string to int8
		static int8 convertToInt8(const String &sString)
		{
			return static_cast<int8>(sString.getInt());
		}

		// Convert int8 to string
		static String convertFromInt8(int8 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to int16
		static int16 convertToInt16(const String &sString)
		{
			return static_cast<int16>(sString.getInt());
		}

		// Convert int16 to string
		static String convertFromInt16(int16 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to int32
		static int32 convertToInt32(const String &sString)
		{
			return static_cast<int32>(sString.getInt());
		}

		// Convert int32 to string
		static String convertFromInt32(int32 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to int64
		static int64 convertToInt64(const String &sString)
		{
			return static_cast<int64>(sString.getInt());
		}

		// Convert int64 to string
		static String convertFromInt64(int64 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to uint8
		static uint8 convertToUInt8(const String &sString)
		{
			return static_cast<uint8>(sString.getInt());
		}

		// Convert uint8 to string
		static String convertFromUInt8(uint8 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to uint16
		static uint16 convertToUInt16(const String &sString)
		{
			return static_cast<uint16>(sString.getInt());
		}

		// Convert uint16 to string
		static String convertFromUInt16(uint16 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to uint32
		static uint32 convertToUInt32(const String &sString)
		{
			return static_cast<uint32>(sString.getInt());
		}

		// Convert uint32 to string
		static String convertFromUInt32(uint32 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to uint64
		static uint64 convertToUInt64(const String &sString)
		{
			return static_cast<uint64>(sString.getInt());
		}

		// Convert uint64 to string
		static String convertFromUInt64(uint64 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to uint_ptr
		static uint_ptr convertToUIntPtr(const String &sString)
		{
			return static_cast<uint_ptr>(sString.getInt());
		}

		// Convert uint_ptr to string
		static String convertFromUIntPtr(uint_ptr nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to float
		static float convertToFloat(const String &sString)
		{
			return sString.getFloat();
		}

		// Convert float to string
		static String convertFromFloat(float fValue)
		{
			return (String() + fValue);
		}

		// Convert string to double
		static double convertToDouble(const String &sString)
		{
			return sString.getDouble();
		}

		// Convert double to string
		static String convertFromDouble(double dValue)
		{
			return (String() + dValue);
		}

		// Convert string to string
		static String convertToString(const String &sString)
		{
			return sString;
		}

		// Convert string to string
		static String convertFromString(const String &sString)
		{
			return sString;
		}

		// Convert real to storage type
		static String convertRealToStorage(String sValue)
		{
			return sValue;
		}

		// Convert storage to real type
		static String convertStorageToReal(String sValue)
		{
			return sValue;
		}

};
