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
*    Type wrapper for pointers 'T*'
*/
template <typename T>
class Type<T*> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef T* _Type;			/**< Real type */
		typedef T* _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = TypePtr;

		// Get type name
		static String getTypeName()
		{
			return "void*";
		}

		// Convert var to pointer
		static T *convertFromVar(const DynVar &cValue)
		{
			return static_cast<T*>(cValue.getUIntPtr());
		}

		// Convert pointer to bool
		static bool convertToBool(T *pValue)
		{
			return static_cast<bool>(pValue != nullptr);
		}

		// Convert bool to pointer
		static T *ConvertFromBool(bool bValue)
		{
			return nullptr;
		}

		// Convert pointer to int
		static int convertToInt(T *pValue)
		{
			return static_cast<int>(static_cast<uint_ptr>(pValue));
		}

		// Convert int to pointer
		static T *convertFromInt(int nValue)
		{
			return static_cast<T*>(nValue);
		}

		// Convert pointer to int8
		static int8 convertToInt8(T *pValue)
		{
			return static_cast<int8>(static_cast<uint_ptr>(pValue));
		}

		// Convert int8 to pointer
		static T *convertFromInt8(int8 nValue)
		{
			return static_cast<T*>(nValue);
		}

		// Convert pointer to int16
		static int16 convertToInt16(T *pValue)
		{
			return static_cast<int16>(static_cast<uint_ptr>(pValue));
		}

		// Convert int16 to pointer
		static T *convertFromInt16(int16 nValue)
		{
			return static_cast<T*>(nValue);
		}

		// Convert pointer to int32
		static int32 convertToInt32(T *pValue)
		{
			return static_cast<int32>(static_cast<uint_ptr>(pValue));
		}

		// Convert int32 to pointer
		static T *convertFromInt32(int32 nValue)
		{
			return static_cast<T*>(nValue);
		}

		// Convert pointer to int64
		static int64 convertToInt64(T *pValue)
		{
			return static_cast<int64>(static_cast<uint_ptr>(pValue));
		}

		// Convert int64 to pointer
		static T *convertFromInt64(int64 nValue)
		{
			return static_cast<T*>(nValue);
		}

		// Convert pointer to uint8
		static uint8 convertToUInt8(T *pValue)
		{
			return static_cast<uint8>(static_cast<uint_ptr>(pValue));
		}

		// Convert uint8 to pointer
		static T *convertFromUInt8(uint8 nValue)
		{
			return static_cast<T*>(nValue);
		}

		// Convert pointer to uint16
		static uint16 convertToUInt16(T *pValue)
		{
			return static_cast<uint16>(static_cast<uint_ptr>(pValue));
		}

		// Convert uint16 to pointer
		static T *convertFromUInt16(uint16 nValue)
		{
			return static_cast<T*>(nValue);
		}

		// Convert pointer to uint32
		static uint32 convertToUInt32(T *pValue)
		{
			return static_cast<uint32>(static_cast<uint_ptr>(pValue));
		}

		// Convert uint32 to pointer
		static T *convertFromUInt32(uint32 nValue)
		{
			return static_cast<T*>(nValue);
		}

		// Convert pointer to uint64
		static uint64 convertToUInt64(T *pValue)
		{
			return static_cast<uint64>(static_cast<uint_ptr>(pValue));
		}

		// Convert uint64 to pointer
		static T *convertFromUInt64(uint64 nValue)
		{
			return static_cast<T*>(nValue);
		}

		// Convert pointer to uint_ptr
		static uint_ptr convertToUIntPtr(T *pValue)
		{
			return static_cast<uint_ptr>(pValue);
		}

		// Convert uint_ptr to pointer
		static T *convertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<T*>(nValue);
		}

		// Convert pointer to float
		static float convertToFloat(T *pValue)
		{
			// No conversion from pointer types in non-integral types!
			return 0.0f;
		}

		// Convert float to pointer
		static T *convertFromFloat(float fValue)
		{
			// No conversion from pointer types in non-integral types!
			return nullptr;
		}

		// Convert pointer to double
		static double convertToDouble(T *pValue)
		{
			// No conversion from pointer types in non-integral types!
			return 0.0;
		}

		// Convert double to pointer
		static T *convertFromDouble(double dValue)
		{
			// No conversion from pointer types in non-integral types!
			return nullptr;
		}

		// Convert pointer to string
		static String convertToString(T *pValue)
		{
			return String() + reinterpret_cast<uint_ptr>(pValue);
		}

		// Convert string to pointer
		static T *convertFromString(const String &sString)
		{
			return reinterpret_cast<T*>(sString.getUIntPtr());
		}

		// Convert real to storage type
		static T *convertRealToStorage(T *pValue)
		{
			return pValue;
		}

		// Convert storage to real type
		static T *convertStorageToReal(T *pValue)
		{
			return pValue;
		}

};
