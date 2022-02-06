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
*    Type wrapper for 'double'
*/
template <>
class Type<double> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef double _Type;			/**< Real type */
		typedef double _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = TypeDouble;

		// Get type name
		static String getTypeName()
		{
			return "double";
		}

		// Convert var to double
		static double convertFromVar(const DynVar &cValue)
		{
			return cValue.getDouble();
		}

		// Convert double to bool
		static bool convertToBool(double dValue)
		{
			return static_cast<bool>(dValue != 0.0);
		}

		// Convert bool to double
		static double ConvertFromBool(bool bValue)
		{
			return static_cast<double>(bValue);
		}

		// Convert double to int
		static int convertToInt(double dValue)
		{
			return static_cast<int>(dValue);
		}

		// Convert int to double
		static double convertFromInt(int nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to int8
		static int8 convertToInt8(double dValue)
		{
			return static_cast<int8>(dValue);
		}

		// Convert int8 to double
		static double convertFromInt8(int8 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to int16
		static int16 convertToInt16(double dValue)
		{
			return static_cast<int16>(dValue);
		}

		// Convert int16 to double
		static double convertFromInt16(int16 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to int32
		static int32 convertToInt32(double dValue)
		{
			return static_cast<int32>(dValue);
		}

		// Convert int32 to double
		static double convertFromInt32(int32 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to int64
		static int64 convertToInt64(double dValue)
		{
			return static_cast<int64>(dValue);
		}

		// Convert int64 to double
		static double convertFromInt64(int64 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to uint8
		static uint8 convertToUInt8(double dValue)
		{
			return static_cast<uint8>(dValue);
		}

		// Convert uint8 to double
		static double convertFromUInt8(uint8 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to uint16
		static uint16 convertToUInt16(double dValue)
		{
			return static_cast<uint16>(dValue);
		}

		// Convert uint16 to double
		static double convertFromUInt16(uint16 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to uint32
		static uint32 convertToUInt32(double dValue)
		{
			return static_cast<uint32>(dValue);
		}

		// Convert uint32 to double
		static double convertFromUInt32(uint32 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to uint64
		static uint64 convertToUInt64(double dValue)
		{
			return static_cast<uint64>(dValue);
		}

		// Convert uint64 to double
		static double convertFromUInt64(uint64 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to uint_ptr
		static uint_ptr convertToUIntPtr(double dValue)
		{
			return static_cast<uint_ptr>(dValue);
		}

		// Convert uint_ptr to double
		static double convertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to float
		static float convertToFloat(double dValue)
		{
			return static_cast<float>(dValue);
		}

		// Convert float to double
		static double convertFromFloat(float fValue)
		{
			return static_cast<double>(fValue);
		}

		// Convert double to double
		static double convertToDouble(double dValue)
		{
			return static_cast<double>(dValue);
		}

		// Convert double to double
		static double convertFromDouble(double dValue)
		{
			return static_cast<double>(dValue);
		}

		// Convert double to string
		static String convertToString(double dValue)
		{
			return String() + dValue;
		}

		// Convert string to double
		static double convertFromString(const String &sString)
		{
			return sString.getDouble();
		}

		// Convert real to storage type
		static double convertRealToStorage(double dValue)
		{
			return dValue;
		}

		// Convert storage to real type
		static double convertStorageToReal(double dValue)
		{
			return dValue;
		}

};
