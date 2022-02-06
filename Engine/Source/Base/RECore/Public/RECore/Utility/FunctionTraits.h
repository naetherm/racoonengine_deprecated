
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

#pragma once

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/Utility/Tuple.h"
#include "RECore/Utility/TypeTraits.h"
//#include "RECore/Reflect/Tools/Accessor.h"
#include <functional>

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename T>
struct IsStdFunction : public RECore::FalseType {
  typedef T Signature;
};
template<typename T>
struct IsStdFunction<std::function<T>> : public RECore::TrueType {
  typedef T Signature;
};
template<typename T>
struct GetStdFunctionSignature {
  typedef T Type;
};
template<typename T>
struct GetStdFunctionSignature<std::function<T>> {
  typedef T Type;
};



template<typename T>
struct IsFunctionPtr : public RECore::IntegralConstant<
  bool,
  RECore::IsPointer<T>::Value && RECore::IsFunction<typename RECore::RemovePointer<T>::Type>::Value> {};

template<typename... TArgs>
struct FunctionArgs {
  typedef RECore::Tuple<TArgs...> ArgTypes;
};
template<typename T>
struct FunctionTraits : public FunctionTraits<decltype(&T::operator())> {};
template < typename TRes, typename... TArgs >
struct FunctionTraits < TRes (TArgs...) > {
  static const int32 ArgCount = sizeof...(TArgs);
  using ReturnType = TRes;
  using ArgTypes = RECore::Tuple < TArgs... >;
};
template < typename TRes, typename... TArgs >
struct FunctionTraits < TRes(*) (TArgs...) > : FunctionTraits < TRes (TArgs...) > {

};
template < typename TRes, typename... TArgs >
struct FunctionTraits < TRes(&) (TArgs...) > : FunctionTraits < TRes (TArgs...) > {

};
template < typename TRes, typename TClass, typename... TArgs >
struct FunctionTraits < TRes(TClass::*) (TArgs...) > : FunctionTraits < TRes (TArgs...) > {
  using ClassType = TClass;
};
template < typename TRes, typename TClass, typename... TArgs >
struct FunctionTraits < TRes(TClass::*) (TArgs...) const > : FunctionTraits < TRes (TArgs...) > {
  using ClassType = TClass;
};
template < typename TRes, typename TClass, typename... TArgs >
struct FunctionTraits < TRes(TClass::*) (TArgs...) volatile > : FunctionTraits < TRes (TArgs...) > {
  using ClassType = TClass;
};
template < typename TRes, typename TClass, typename... TArgs >
struct FunctionTraits < TRes(TClass::*) (TArgs...) const volatile > : FunctionTraits < TRes (TArgs...) > {
  using ClassType = TClass;
};
template < typename T >
struct FunctionTraits < std::function < T>> : FunctionTraits < T > {

};




template < typename T, int32 Index >
struct ParamTypes {
  using Type = typename RECore::TupleElement < Index, typename FunctionTraits < T >::ArgTypes >::Type;
};

template < typename T, int32 Index > using ParamTypesT = typename ParamTypes < T, Index >::Type;


template < typename T >
struct IsVoidFunc : RECore::Conditional < RECore::IsSame < typename FunctionTraits < T >::ReturnType, void >::Value, RECore::TrueType, RECore::FalseType >::Type {};


struct MemberFuncPtr {
  typedef MemberFuncPtr Type;
};

/*
		struct FunctionPtr
		{
				typedef FunctionPtr type;
		};

		struct MemberObjectPtr
		{
				typedef MemberObjectPtr type;
		};

		struct ObjectPtr
		{
				typedef ObjectPtr type;
		};
*/
/*
template < typename T >
struct PropertyType : RECore::Conditional < RECore::IsMemberFunctionPointer < T >::Value, MemberFuncPtr, typename RECore::Conditional < RECore::IsMemberObjectPointer < T >::Value, MemberObjectPtr, typename RECore::Conditional <
  IsFunctionPtr < T >::Value ||
  IsStdFunction < T >::Value, FunctionPtr, typename RECore::Conditional < RECore::IsPointer < T >::Value, ObjectPtr, void >::Type >::Type >::Type >::Type {};
*/

struct VoidMemberFunc {
  typedef VoidMemberFunc Type;
};

struct ReturnMemberFunc {
  typedef ReturnMemberFunc Type;
};

struct VoidFunc {
  typedef VoidFunc Type;
};

struct ReturnFunc {
  typedef ReturnFunc Type;
};

template < typename T >
struct MethodType : RECore::Conditional < RECore::IsMemberFunctionPointer < T >::Value, typename RECore::Conditional < IsVoidFunc < T >::Value, VoidMemberFunc, ReturnMemberFunc >::Type, typename RECore::Conditional <
  IsFunctionPtr < T >::Value ||
  IsStdFunction < T >::Value, typename RECore::Conditional < IsVoidFunc < T >::Value, VoidFunc, ReturnFunc >::Type, void >::Type >::Type {};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore
