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
*    Type wrapper for enum types
*/
template <typename ENUM>
class TypeInfo< EnumType<ENUM> > : public DynTypeInfo {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef typename ENUM::_Type _Type;


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static TypeInfo Instance;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		TypeInfo()
		{
			// Fill enum values
			for (int i=0; i<EnumType<ENUM>::getNumOfEnumValues(); i++)
				m_lstEnumValues.push_back(EnumType<ENUM>::getEnumName(i));
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TypeInfo()
		{
		}


	//[-------------------------------------------------------]
	//[ Public virtual DynTypeInfo functions                  ]
	//[-------------------------------------------------------]
	public:
		virtual int getTypeID() const override
		{
			return Type<_Type>::TypeID;
		}

		virtual String getTypeName() const override
		{
			return Type<_Type>::getTypeName();
		}

		virtual bool isEnumType() const override
		{
			return true;
		}

		virtual bool isFlagType() const override
		{
			return false;
		}

		virtual String getEnumValue(const String &sEnum) const override
		{
			String sName = sEnum;
			String sDesc;
			_Type nValue = DefaultValue<_Type>::defaultValue();
			ENUM::getEnumValue(-1, nValue, sName, sDesc);
			return Type<_Type>::convertToString(nValue);
		}

		virtual String getEnumDescription(const String &sEnum) const override
		{
			String sName = sEnum;
			String sDesc;
			_Type nValue = DefaultValue<_Type>::defaultValue();
			ENUM::getEnumValue(-1, nValue, sName, sDesc);
			return sDesc;
		}


};


//[-------------------------------------------------------]
//[ Static variables                                      ]
//[-------------------------------------------------------]
template <typename ENUM> TypeInfo< EnumType<ENUM> > TypeInfo< EnumType<ENUM> >::Instance;
