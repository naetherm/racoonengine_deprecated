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
*    Type wrapper for flag types
*/
template <typename ENUM>
class Type< FlagType<ENUM> > {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef typename ENUM::_BaseType _BaseType;
		typedef typename ENUM::_BaseType _Type;			/**< Real type */
		typedef typename ENUM::_BaseType _StorageType;	/**< Storage type, for this type identical to the real type */
		typedef typename ENUM::_Type	 _EnumType;

		// Type ID
		static const int TypeID = Type<_BaseType>::TypeID;

		// Get type name
		static String getTypeName()
		{
			return Type<_BaseType>::getTypeName();
		}

		// Convert var to type
		static _BaseType convertFromVar(const DynVar &cValue)
		{
			if (cValue.getTypeID() == TypeString)
				return convertFromString(cValue.getString());
			else
				return Type<_BaseType>::convertFromVar(cValue);
		}

		// Convert type to bool
		static bool convertToBool(_BaseType Value)
		{
			return Type<_BaseType>::convertToBool(Value);
		}

		// Convert bool to type
		static _BaseType ConvertFromBool(bool bValue)
		{
			return Type<_BaseType>::ConvertFromBool(bValue);
		}

		// Convert type to int
		static int convertToInt(_BaseType Value)
		{
			return Type<_BaseType>::convertToInt(Value);
		}

		// Convert int to type
		static _BaseType convertFromInt(int nValue)
		{
			return Type<_BaseType>::convertFromInt(nValue);
		}

		// Convert type to int8
		static int8 convertToInt8(_BaseType Value)
		{
			return Type<_BaseType>::convertToInt8(Value);
		}

		// Convert int8 to type
		static _BaseType convertFromInt8(int8 nValue)
		{
			return Type<_BaseType>::convertFromInt8(nValue);
		}

		// Convert type to int16
		static int16 convertToInt16(_BaseType Value)
		{
			return Type<_BaseType>::convertToInt16(Value);
		}

		// Convert int16 to type
		static _BaseType convertFromInt16(int16 nValue)
		{
			return Type<_BaseType>::convertFromInt16(nValue);
		}

		// Convert type to int32
		static int32 convertToInt32(_BaseType Value)
		{
			return Type<_BaseType>::convertToInt32(Value);
		}

		// Convert int32 to type
		static _BaseType convertFromInt32(int32 nValue)
		{
			return Type<_BaseType>::convertFromInt32(nValue);
		}

		// Convert type to int64
		static int64 convertToInt64(_BaseType Value)
		{
			return Type<_BaseType>::convertToInt64(Value);
		}

		// Convert int64 to type
		static _BaseType convertFromInt64(int64 nValue)
		{
			return Type<_BaseType>::convertFromInt64(nValue);
		}

		// Convert type to uint8
		static uint8 convertToUInt8(_BaseType Value)
		{
			return Type<_BaseType>::convertToUInt8(Value);
		}

		// Convert uint8 to type
		static _BaseType convertFromUInt8(uint8 nValue)
		{
			return Type<_BaseType>::convertFromUInt8(nValue);
		}

		// Convert type to uint16
		static uint16 convertToUInt16(_BaseType Value)
		{
			return Type<_BaseType>::convertToUInt16(Value);
		}

		// Convert uint16 to type
		static _BaseType convertFromUInt16(uint16 nValue)
		{
			return Type<_BaseType>::convertFromUInt16(nValue);
		}

		// Convert type to uint32
		static uint32 convertToUInt32(_BaseType Value)
		{
			return Type<_BaseType>::convertToUInt32(Value);
		}

		// Convert uint32 to type
		static _BaseType convertFromUInt32(uint32 nValue)
		{
			return Type<_BaseType>::convertFromUInt32(nValue);
		}

		// Convert type to uint64
		static uint64 convertToUInt64(_BaseType Value)
		{
			return Type<_BaseType>::convertToUInt64(Value);
		}

		// Convert uint64 to type
		static _BaseType convertFromUInt64(int64 nValue)
		{
			return Type<_BaseType>::convertFromUInt64(nValue);
		}

		// Convert type to uint_ptr
		static uint_ptr convertToUIntPtr(_BaseType Value)
		{
			return Type<_BaseType>::convertToUIntPtr(Value);
		}

		// Convert uint_ptr to type
		static _BaseType convertFromUIntPtr(uint_ptr nValue)
		{
			return Type<_BaseType>::convertFromUIntPtr(nValue);
		}

		// Convert type to float
		static float convertToFloat(_BaseType Value)
		{
			return Type<_BaseType>::convertToFloat(Value);
		}

		// Convert float to type
		static _BaseType convertFromFloat(float fValue)
		{
			return Type<_BaseType>::convertFromFloat(fValue);
		}

		// Convert type to double
		static double convertToDouble(_BaseType Value)
		{
			return Type<_BaseType>::convertToDouble(Value);
		}

		// Convert double to type
		static _BaseType convertFromDouble(double dValue)
		{
			return Type<_BaseType>::convertFromDouble(dValue);
		}

		// Convert type to string
		static String convertToString(_BaseType Value)
		{
			// Init return value
			String sFlags;
			_BaseType nValue = Value;

			// Loop through enum values
			String sName, sDesc;
			_EnumType nFlag;
			for (int i=0; ENUM::getEnumValue(i, nFlag, sName, sDesc); i++) {
				// Check if flag is set
				if (nValue & nFlag) {
					if (sFlags.length() > 0)
						sFlags += "|";
					sFlags += sName;
					nValue = (nValue & ~nFlag);
				}
			}

			// Add rest
			if (nValue != 0) {
				if (sFlags.length() > 0)
					sFlags += "|";
				sFlags += nValue;
			}

			// Return flags
			if (sFlags.length() == 0)
				sFlags = "0";
			return sFlags;
		}

		// Convert string to type
		static _BaseType convertFromString(const String &sString)
		{
			// Init return value
			_BaseType nFlags = 0;

			// Tokenizer flags string
			if (sString.length()) {
				CTokenizer cTokenizer;
				cTokenizer.Start(sString);
				cTokenizer.SetDelimiters(" \t\r\n&,|");
				while (cTokenizer.GetNextToken().length()) {
					// Get next token
					String sToken = cTokenizer.GetToken();
					String sDesc;

					// Try to convert token to number, else look up name in enum
					_EnumType nValue = static_cast<_EnumType>(sToken.getInt());
					if (static_cast<_BaseType>(nValue) == 0)
						ENUM::getEnumValue(-1, nValue, sToken, sDesc);
					nFlags |= nValue;
				}
				cTokenizer.Stop();
			}

			// Return flags value
			return nFlags;
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
