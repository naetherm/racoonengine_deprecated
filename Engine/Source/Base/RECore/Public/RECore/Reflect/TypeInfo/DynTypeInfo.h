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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/String/String.h"
#include <list>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual base class for types
*
*  @remarks
*    This is the virtual base class to access dynamic type information.
*    Usually it is better to use the static type template classes
*    such as Type<int> etc. However, sometimes it is necessary go get the
*    type e.g. of a variable in a dynamic (polymorphic) way.
*/
class DynTypeInfo {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		RECORE_API DynTypeInfo();

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~DynTypeInfo();


	//[-------------------------------------------------------]
	//[ Public virtual DynTypeInfo functions                  ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get type ID
		*
		*  @return
		*    Type ID
		*/
		RECORE_API virtual int getTypeID() const;

		/**
		*  @brief
		*    Get type name
		*
		*  @return
		*    Type name
		*/
		RECORE_API virtual String getTypeName() const;

		/**
		*  @brief
		*    Check if this is an enum type
		*
		*  @return
		*    'true' if type is enum type, else 'false'
		*/
		RECORE_API virtual bool isEnumType() const;

		/**
		*  @brief
		*    Check if this is an flag type
		*
		*  @return
		*    'true' if type is flag type, else 'false'
		*/
		RECORE_API virtual bool isFlagType() const;

		/**
		*  @brief
		*    Get enum value names
		*
		*  @return
		*    List of enum value names
		*/
		RECORE_API virtual const std::list<String> &getEnumValues() const;

		/**
		*  @brief
		*    Get enum value
		*
		*  @param[in] sEnum
		*    Enum name
		*
		*  @return
		*    Enum value as string
		*/
		RECORE_API virtual String getEnumValue(const String &sEnum) const;

		/**
		*  @brief
		*    Get enum description
		*
		*  @param[in] sEnum
		*    Enum name
		*
		*  @return
		*    Description for that enum value
		*/
		RECORE_API virtual String getEnumDescription(const String &sEnum) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
  std::list<String> m_lstEnumValues;	/**< List of enum values (empty but for enum-types) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
