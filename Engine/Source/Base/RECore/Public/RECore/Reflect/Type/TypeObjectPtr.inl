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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Object;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for 'Object*'
*/
template <>
class Type<Object*> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef Object *_Type;			/**< Real type */
		typedef Object *_StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = TypeObjectPtr;

		// Get type name
		static String getTypeName()
		{
			return "Object*";
		}

		// Convert var to pointer
		static Object *convertFromVar(const DynVar &cValue)
		{
			return reinterpret_cast<Object*>(cValue.getUIntPtr());
		}

		// Convert pointer to bool
		static bool convertToBool(Object *pValue)
		{
			return static_cast<bool>(pValue != nullptr);
		}

		// Convert bool to pointer
		static Object *ConvertFromBool(bool bValue)
		{
			return nullptr;
		}

		// Convert pointer to int
		static int convertToInt(Object *pValue)
		{
			return static_cast<int>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert int to pointer
		static Object *convertFromInt(int nValue)
		{
			return reinterpret_cast<Object*>(nValue);
		}

		// Convert pointer to int8
		static int8 convertToInt8(Object *pValue)
		{
			return static_cast<int8>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert int8 to pointer
		static Object *convertFromInt8(int8 nValue)
		{
			return reinterpret_cast<Object*>(nValue);
		}

		// Convert pointer to int16
		static int16 convertToInt16(Object *pValue)
		{
			return static_cast<int16>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert int16 to pointer
		static Object *convertFromInt16(int16 nValue)
		{
			return reinterpret_cast<Object*>(nValue);
		}

		// Convert pointer to int32
		static int32 convertToInt32(Object *pValue)
		{
			return static_cast<int32>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert int32 to pointer
		static Object *convertFromInt32(int32 nValue)
		{
			return reinterpret_cast<Object*>(nValue);
		}

		// Convert pointer to int64
		static int64 convertToInt64(Object *pValue)
		{
			return static_cast<int64>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert int64 to pointer
		static Object *convertFromInt64(int64 nValue)
		{
			return reinterpret_cast<Object*>(nValue);
		}

		// Convert pointer to uint8
		static uint8 convertToUInt8(Object *pValue)
		{
			return static_cast<uint8>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert uint8 to pointer
		static Object *convertFromUInt8(uint8 nValue)
		{
			return reinterpret_cast<Object*>(nValue);
		}

		// Convert pointer to uint16
		static uint16 convertToUInt16(Object *pValue)
		{
			return static_cast<uint16>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert uint16 to pointer
		static Object *convertFromUInt16(uint16 nValue)
		{
			return reinterpret_cast<Object*>(nValue);
		}

		// Convert pointer to uint32
		static uint32 convertToUInt32(Object *pValue)
		{
			return static_cast<uint32>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert uint32 to pointer
		static Object *convertFromUInt32(uint32 nValue)
		{
			return reinterpret_cast<Object*>(nValue);
		}

		// Convert pointer to uint64
		static uint64 convertToUInt64(Object *pValue)
		{
			return static_cast<uint64>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert uint64 to pointer
		static Object *convertFromUInt64(uint64 nValue)
		{

			return reinterpret_cast<Object*>(nValue);
		}

		// Convert pointer to uint_ptr
		static uint_ptr convertToUIntPtr(Object *pValue)
		{
			return reinterpret_cast<uint_ptr>(pValue);
		}

		// Convert uint_ptr to pointer
		static Object *convertFromUIntPtr(uint_ptr nValue)
		{
			return reinterpret_cast<Object*>(nValue);
		}

		// Convert pointer to float
		static float convertToFloat(Object *pValue)
		{
			// No conversion from pointer types in non-integral types!
			return 0.0f;
		}

		// Convert float to pointer
		static Object *convertFromFloat(float fValue)
		{
			// No conversion from pointer types in non-integral types!
			return nullptr;
		}

		// Convert pointer to double
		static double convertToDouble(Object *pValue)
		{
			// No conversion from pointer types in non-integral types!
			return 0.0;
		}

		// Convert double to pointer
		static Object *convertFromDouble(double dValue)
		{
			// No conversion from pointer types in non-integral types!
			return nullptr;
		}

		// Convert pointer to string
		static String convertToString(Object *pValue)
		{
			return String() + reinterpret_cast<uint_ptr>(pValue);
		}

		// Convert string to pointer
		static Object *convertFromString(const String &sString)
		{
			return reinterpret_cast<Object*>(sString.getUIntPtr());
		}

		// Convert real to storage type
		static Object *convertRealToStorage(Object *pValue)
		{
			return pValue;
		}

		// Convert storage to real type
		static Object *convertStorageToReal(Object *pValue)
		{
			return pValue;
		}

};
