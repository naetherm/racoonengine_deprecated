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
*    Type wrapper for 'float'
*/
template <>
class Type<float> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef float _Type;		/**< Real type */
		typedef float _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = TypeFloat;

		// Get type name
		static String getTypeName()
		{
			return "float";
		}

		// Convert var to float
		static float convertFromVar(const DynVar &cValue)
		{
			return cValue.getFloat();
		}

		// Convert float to bool
		static bool convertToBool(float fValue)
		{
			return static_cast<bool>(fValue != 0.0f);
		}

		// Convert bool to float
		static float ConvertFromBool(bool bValue)
		{
			return static_cast<float>(bValue);
		}

		// Convert float to int
		static int convertToInt(float fValue)
		{
			return static_cast<int>(fValue);
		}

		// Convert int to float
		static float convertFromInt(int nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to int8
		static int8 convertToInt8(float fValue)
		{
			return static_cast<int8>(fValue);
		}

		// Convert int8 to float
		static float convertFromInt8(int8 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to int16
		static int16 convertToInt16(float fValue)
		{
			return static_cast<int16>(fValue);
		}

		// Convert int16 to float
		static float convertFromInt16(int16 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to int32
		static int32 convertToInt32(float fValue)
		{
			return static_cast<int32>(fValue);
		}

		// Convert int32 to float
		static float convertFromInt32(int32 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to int64
		static int64 convertToInt64(float fValue)
		{
			return static_cast<int64>(fValue);
		}

		// Convert int64 to float
		static float convertFromInt64(int64 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to uint8
		static uint8 convertToUInt8(float fValue)
		{
			return static_cast<uint8>(fValue);
		}

		// Convert uint8 to float
		static float convertFromUInt8(uint8 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to uint16
		static uint16 convertToUInt16(float fValue)
		{
			return static_cast<uint16>(fValue);
		}

		// Convert uint16 to float
		static float convertFromUInt16(uint16 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to uint32
		static uint32 convertToUInt32(float fValue)
		{
			return static_cast<uint32>(fValue);
		}

		// Convert uint32 to float
		static float convertFromUInt32(uint32 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to uint64
		static uint64 convertToUInt64(float fValue)
		{
			return static_cast<uint64>(fValue);
		}

		// Convert uint64 to float
		static float convertFromUInt64(uint64 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to uint_ptr
		static uint_ptr convertToUIntPtr(float fValue)
		{
			return static_cast<uint_ptr>(fValue);
		}

		// Convert uint_ptr to float
		static float convertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to float
		static float convertToFloat(float fValue)
		{
			return static_cast<float>(fValue);
		}

		// Convert float to float
		static float convertFromFloat(float fValue)
		{
			return static_cast<float>(fValue);
		}

		// Convert float to double
		static double convertToDouble(float fValue)
		{
			return static_cast<double>(fValue);
		}

		// Convert double to float
		static float convertFromDouble(double dValue)
		{
			return static_cast<float>(dValue);
		}

		// Convert float to string
		static String convertToString(float fValue)
		{
			return String() + fValue;
		}

		// Convert string to float
		static float convertFromString(const String &sString)
		{
			return sString.getFloat();
		}

		// Convert real to storage type
		static float convertRealToStorage(float fValue)
		{
			return fValue;
		}

		// Convert storage to real type
		static float convertStorageToReal(float fValue)
		{
			return fValue;
		}

};
