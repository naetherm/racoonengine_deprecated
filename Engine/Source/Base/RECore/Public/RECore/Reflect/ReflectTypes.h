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
#include "RECore/RECore.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type IDs
*/
enum ETypeID {
  TypeInvalid = -1,
  TypeNull = 0,
  TypeVoid,
  TypeBool,
  TypeInt,
  TypeFloat,
  TypeDouble,
  TypeInt8,
  TypeInt16,
  TypeInt32,
  TypeInt64,
  TypeUInt8,
  TypeUInt16,
  TypeUInt32,
  TypeUInt64,
  TypeString,
  TypePtr,
  TypeRef,
  TypeObjectPtr
};

/**
*  @brief
*    Member types
*/
enum EMemberType {
  MemberAttribute = 0,
  MemberMethod,
  MemberConstructor,
  MemberEvent,
  MemberEventHandler
};

/**
*  @brief
*    Default value mode
*/
enum EDefaultValue {
  WithDefault = 0,    /**< Retrieve all variables, including those with a default value */
  NoDefault        /**< Retrieve only variables that are different than their default value */
};

/**
*  @brief
*    Recursive mode
*/
enum ERecursive {
  NonRecursive = 0,    /**< Do (whatever) not recursively */
  Recursive        /**< Do (whatever) recursively */
};

/**
*  @brief
*    Include base class in class searches
*/
enum EIncludeBase {
  IncludeBase = 0,    /**< Include that base class itself in the list of derived classes */
  NoBase          /**< Only include derived classes, not the base class itself */
};

/**
*  @brief
*    Include abstract classes in class searches
*/
enum EIncludeAbstract {
  IncludeAbstract = 0,  /**< Include abstract classes in list of classes */
  NoAbstract        /**< Do not include abstract classes */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
