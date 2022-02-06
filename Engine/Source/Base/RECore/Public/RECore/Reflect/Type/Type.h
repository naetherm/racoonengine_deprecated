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
#include "RECore/Reflect/ReflectTypes.h"
#include "RECore/String/BasicTokenizer.h"
#include "RECore/Reflect/Tools/TypeTraits.h"
#include "RECore/Reflect/Var/DynVar.h"
#include "RECore/Reflect/Type/EnumType.h"
#include "RECore/Reflect/TypeInfo/TypeInfo.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Generic type wrapper
*/
template <typename T>
class Type : public Type< typename CheckType<T>::Type > {
};


//[-------------------------------------------------------]
//[ Include type implementations                          ]
//[-------------------------------------------------------]
#include "TypeInvalid.inl"
#include "TypeNull.inl"
#include "TypeVoid.inl"
#include "TypeBool.inl"
//#include "TypeInt.inl"		// Should be the same as int32
#include "TypeInt8.inl"
#include "TypeInt16.inl"
#include "TypeInt32.inl"
#include "TypeInt64.inl"
#include "TypeUInt8.inl"
#include "TypeUInt16.inl"
#include "TypeUInt32.inl"
#include "TypeUInt64.inl"
#include "TypeFloat.inl"
#include "TypeDouble.inl"
#include "TypeString.inl"
#include "TypeEnumPlain.inl"
#include "TypeEnum.inl"
#include "TypeFlag.inl"
#include "TypePtr.inl"
#include "TypeRef.inl"
#include "TypeObjectPtr.inl"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
