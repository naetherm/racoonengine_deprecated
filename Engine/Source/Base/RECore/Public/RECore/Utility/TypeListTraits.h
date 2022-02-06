
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
#include "RECore/Utility/FunctionTraits.h"
#include "RECore/Utility/TypeTraits.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

class MetaType;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
struct RECORE_API TTNullType {};

template<typename T>
struct RawType {
  typedef T Result;
};
template<typename T>
struct RawType<const T> {
  typedef typename RawType<T>::Result Result;
};
template<typename T>
struct RawType<T*> {
  typedef typename RawType<T>::Result Result;
};
template<typename T>
struct RawType<T* const> {
  typedef typename RawType<T>::Result Result;
};
template<typename T>
struct RawType<volatile T> {
  typedef typename RawType<T>::Result Result;
};
template<typename T>
struct RawType<T* volatile> {
  typedef typename RawType<T>::Result Result;
};
template<typename T>
struct RawType<const volatile T> {
  typedef typename RawType<T>::Result Result;
};
template<typename T>
struct RawType<T* const volatile > {
  typedef typename RawType<T>::Result Result;
};
template<typename T>
struct RawType<T&> {
  typedef typename RawType<T>::Result Result;
};
template<typename T, sizeT N>
struct RawType<T[N]> {
  typedef typename RawType<T>::Result Result;
};
template<typename T, sizeT N>
struct RawType<const T[N]> {
  typedef typename RawType<T[N]>::Result Result;
};
template<typename T>
using RawTypeT = typename RawType<T>::Result;



template<typename T, typename TEnable=void>
struct PointerCountImpl {
  static constexpr sizeT Size = 0;
};
template<typename T>
struct PointerCountImpl<
  T,
  RECore::EnableIfT<
    RECore::IsPointer<T>::Value && !RECore::IsFunctionPtr<T>::Value && !RECore::IsMemberPointer<T>::Value>> {
  static constexpr sizeT Size = PointerCountImpl<RECore::RemovePointerT<T>>::Size + 1;
};
template<typename T>
using PointerCount = RECore::IntegralConstant<sizeT, PointerCountImpl<T>::Size>;



template<typename T, typename Enable = void>
struct RawArrayType {
  using Type = RawTypeT<T>;
};
template<typename T>
struct RawArrayTypeImpl;
template<typename T, sizeT N>
struct RawArrayTypeImpl<T[N]> {
  using Type = typename RawArrayType<T>::Type;
};
template<typename T>
struct RawArrayType<T, typename RECore::EnableIf<RECore::IsArray<T>::Value>::Type> {
  using Type = typename RawArrayTypeImpl< RECore::RemoveCVT<T> >::Type;
};
template<typename T>
using RawArrayTypeT = typename RawArrayType<T>::Type;



template<typename T>
using IsOneDimCharArray = RECore::IntegralConstant<
  bool,
  RECore::IsArray<T>::Value && RECore::IsSame<char, RawArrayTypeT<T>>::Value && (RECore::Rank<T>::Value == 1)>;



template < typename T, typename Enable = void >
struct RemovePointersExceptOne
{
  using Type = T;
};
template < typename T >
struct RemovePointersExceptOne < T, RECore::EnableIfT < RECore::IsPointer < T >::Value>>
{
using Type = RECore::ConditionalT < RECore::IsPointer < RECore::RemovePointerT < T > >::Value, typename RemovePointersExceptOne < RECore::RemovePointerT < T>>::Type, T >;
};

template < typename T > using RemovePointersExceptOneT = typename RemovePointersExceptOne < T >::Type;




template<bool... B>
struct StaticAllOf;
template<bool... TTail>
struct StaticAllOf<true, TTail...> : StaticAllOf<TTail...> {};
template<bool... TTail>
struct StaticAllOf<false, TTail...> : RECore::FalseType {};
template<>
struct StaticAllOf<> : RECore::TrueType {};



template<bool... B>
struct StaticAnyOf;
template<bool... TTail>
struct StaticAnyOf<false, TTail...> : StaticAllOf<TTail...> {};
template<bool... TTail>
struct StaticAnyOf<true, TTail...> : RECore::TrueType {};
template<>
struct StaticAnyOf<> : RECore::FalseType {};



template<typename T>
class HasGetTypeFuncImpl {
  typedef char YesType[1];
  typedef char NoType[2];
  template<typename U, RECore::MetaType (U::*)() const>
  class Check {};
  template<typename C>
  static YesType& F(Check<C, &C::GetType>*);
  template<typename C>
  static NoType& F(...);
public:
  static constexpr bool Value = (sizeof(F<typename RawType<T>::Type>(0)) == sizeof(YesType));
};
template<class T, typename TEnable=void>
struct HasGetTypeFunc : RECore::FalseType {};
template<class T>
struct HasGetTypeFunc<T, RECore::EnableIfT<HasGetTypeFuncImpl<T>::Value>> : TrueType {};



template<typename T>
class HasGetPtrFuncImpl {
  typedef char YesType[1];
  typedef char NoType[2];
  template<typename U, RECore::MetaType (U::*)() const>
  class Check {};
  template<typename C>
  static YesType& F(Check<C, &C::GetPtr>*);
  template<typename C>
  static NoType& F(...);
public:
  static constexpr bool Value = (sizeof(F<typename RawType<T>::Type>(0)) == sizeof(YesType));
};
template<class T, typename TEnable=void>
struct HasGetPtrFunc : RECore::FalseType {};
template<class T>
struct HasGetPtrFunc<T, RECore::EnableIfT<HasGetPtrFuncImpl<T>::Value>> : TrueType {};



template<typename T>
class HasGetDerivedInfoFuncImpl {
  typedef char YesType[1];
  typedef char NoType[2];
  template<typename U, RECore::MetaType (U::*)() const>
  class Check {};
  template<typename C>
  static YesType& F(Check<C, &C::GetPtr>*);
  template<typename C>
  static NoType& F(...);
public:
  static constexpr bool Value = (sizeof(F<typename RawType<T>::Type>(0)) == sizeof(YesType));
};
template<class T, typename TEnable=void>
struct HasGetDerivedInfoFunc : RECore::FalseType {};
template<class T>
struct HasGetDerivedInfoFunc<T, RECore::EnableIfT<HasGetDerivedInfoFuncImpl<T>::Value>> : TrueType {};



template < typename T >
struct GetPtrImpl {
  static inline void * Get (T & data) {
    return const_cast<void *>(reinterpret_cast<const void *>(&data));
  }
};

template < typename T >
struct GetPtrImpl < T * > {
  static inline void * Get (T * data) {
    return GetPtrImpl < T >::Get(*data);
  }
};

template <>
struct GetPtrImpl < void * > {
  static inline void * Get (void * data) {
    return data;
  }
};

template <>
struct GetPtrImpl < const void * > {
  static inline void * Get (const void * data) {
    return const_cast<void *>(data);
  }
};

template < typename T >
static inline void * GetVoidPtr (T * data) {
  return GetPtrImpl < T * >::Get(data);
}

template < typename T >
static inline void * GetVoidPtr (T & data) {
  return GetPtrImpl < T >::Get(data);
}



template < typename... T >
struct TypeList {
};

template < typename...T >
struct AsTypeList {
  using Type = TypeList < T... >;
};

template < template < class... > class RTypeList, typename...Ts >
struct AsTypeList < RTypeList < Ts...>> {
  using Type = TypeList < Ts... >;
};

template < typename...T >
using AsTypeListT = typename AsTypeList < T... >::Type;

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

template < int32 I, typename T >
struct TypeListElement;

template < int32 I, typename Head, typename... Tail >
struct TypeListElement < I, TypeList < Head, Tail...>> : TypeListElement < I - 1, TypeList < Tail...>> {
};

template < typename Head, typename... Tail >
struct TypeListElement < 0, TypeList < Head, Tail...>> {
  using Type = Head;
};

template < int32 I, typename List >
using TypeListElementT = typename TypeListElement < I, List >::Type;

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
template<typename... T>
struct AsStdTuple {
  using Type = std::tuple<T...>;
};
template<template<class...> class TTypeList, typename... Ts>
struct AsStdTuple<TTypeList<Ts...>> {
  using Type = std::tuple<Ts...>;
};
template<typename... T>
using AsStdTupleT = typename AsStdTuple<T...>::Type;

template<typename... T>
struct AsTuple {
  using Type = RECore::Tuple<T...>;
};
template<template<class...> class TTypeList, typename... Ts>
struct AsTuple<TTypeList<Ts...>> {
  using Type = RECore::Tuple<Ts...>;
};
template<typename... T>
using AsTupleT = typename AsTuple<T...>::Type;

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct TypeListSizeImpl;

template<typename...Ts>
struct TypeListSizeImpl<TypeList<Ts...>> : RECore::IntegralConstant<sizeT, sizeof...(Ts)> {};

template<typename...T>
using TypeListSize = typename TypeListSizeImpl<AsTypeListT<T...>>::Type;

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// pushed the given item \p T at the front of the type list

template<typename T, typename List>
struct PushFront;

template<typename T, template<class...> class TTypeList, typename...Ts>
struct PushFront<T, TTypeList<Ts...>> {
  using Type = TTypeList<T, Ts...>;
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// pushed the given item \p T N times at the front of the type list

template<typename T, typename TList, sizeT N>
struct PushFrontN {
  using Type = typename PushFrontN<T, typename PushFront<T, TList>::Type, N - 1>::Type;
};

template<typename T, typename TList>
struct PushFrontN<T, TList, 0> {
  using Type = TList;
};

template<typename T, typename TList, sizeT N>
using PushFrontNT = typename PushFrontN<T, TList, N>::Type;

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename... TTypes>
struct Contains : StaticAnyOf<RECore::IsSame<T, TTypes>::Value...> {};

template<typename T, template<class...> class TContainer, typename... TTypes>
struct Contains<T, TContainer<TTypes...>> : Contains<T, TTypes...> {};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

template < typename T, typename...Ts >
struct MaxSizeofListImpl;

template < typename T >
struct MaxSizeofListImpl < T > {
  static const sizeT Value = sizeof(T);
};

template < typename T1, typename T2, typename... U >
struct MaxSizeofListImpl < T1, T2, U... > {
  static const sizeT Value = MaxSizeofListImpl < RECore::ConditionalT <
                                                 sizeof(T1) >= sizeof(T2), T1, T2 >, U... >::Value;
};

template < template < class... > class List, class... Ts >
struct MaxSizeofListImpl < List < Ts...>> {
  static const sizeT Value = MaxSizeofListImpl < Ts... >::Value;
};


/////////////////////////////////////////////////////////////////////////////////////
// Returns the maximum sizeof value from all given types
// use it like this:
// max_size_of_list<int, bool, double>::value => 8

template < typename... Ts > using MaxSizeofList = RECore::IntegralConstant < std::size_t, MaxSizeofListImpl < Ts... >::Value >;


template < typename T, typename...Ts >
struct MaxAlignofListImpl;

template < typename T >
struct MaxAlignofListImpl < T > {
  static const sizeT Value = RECore::AlignmentOf < T >::Value;
};

template < typename T1, typename T2, typename... U >
struct MaxAlignofListImpl < T1, T2, U... > {
  static const sizeT Value = MaxAlignofListImpl < RECore::ConditionalT <
                                                  RECore::AlignmentOf < T1 >::Value >= RECore::AlignmentOf < T2 >::Value, T1, T2 >, U... >::Value;
};

template < template < class... > class List, typename... Ts >
struct MaxAlignofListImpl < List < Ts...>> {
  static const sizeT Value = MaxAlignofListImpl < Ts... >::Value;
};

/////////////////////////////////////////////////////////////////////////////////////
// Returns the maximum sizeof value from all given types
// use it like this:
// max_alignof_list<int, bool, double>::value => 8

template < typename... Ts >
using MaxAlignofList = RECore::IntegralConstant < std::size_t, MaxAlignofListImpl < Ts... >::Value >;



static inline bool CheckAllParameters () {
  return true;
}

template < typename... TArgs>
static inline bool CheckAllParameters (
  bool arg1, TArgs... args) {
  return arg1 && CheckAllParameters(args...);
}


template < typename T >
struct DecayExceptArray {
  using Tp    = RECore::RemoveReferenceT < T >;
  using Type  = RECore::ConditionalT < RECore::IsFunction < Tp >::Value, RECore::AddPointerT < Tp >, RECore::RemoveCVT < Tp > >;
};

template < typename T > using DecayExceptArrayT = typename DecayExceptArray < T >::Type;



//
// CountTypes
//
template < typename T, typename TYPELIST >
struct CountTypeImpl;

template < typename T >
struct CountTypeImpl < T, TypeList < >> {
  static const int32 Result = 0;
};

template < typename T, typename ... TAIL >
struct CountTypeImpl < T, TypeList < T, TAIL...>> {
  static const int32 Result = 1 + CountTypeImpl < T, TypeList < TAIL...>>::Result;
};

template < typename T, typename U, typename ... TAIL >
struct CountTypeImpl < T, TypeList < U, TAIL...>> {
  static const int32 Result = CountTypeImpl < T, TypeList < TAIL...>>::Result;
};

template < typename T, typename TYPELIST > using CountType = RECore::IntegralConstant < int32, CountTypeImpl < T, AsTypeListT < TYPELIST>>::Result >;




template<typename T>
struct PropertyTraits;

template<typename A, typename C>
struct PropertyTraits<A(C::*)> {
  using ClassType = C;
};


//
// Argument count information
//
template < typename ... ARGS >
struct IsNotOneArgument : RECore::IntegralConstant < bool, (sizeof...(ARGS) != 1) > {

};
template < typename ... ARGS >
struct IsOneArgument : RECore::IntegralConstant < bool, (sizeof...(ARGS) == 1) > {

};
template < typename ... ARGS >
struct IsNoneArgument : RECore::IntegralConstant < bool, (sizeof...(ARGS) == 0) > {

};

template<typename...TArgs>
struct IsEmptyArgs : RECore::IntegralConstant<bool, (sizeof...(TArgs) == 0)> {};

//
//
//
template < template < class > class COND, typename T >
struct FindIfImpl;
template < template < class > class COND, typename T, typename ... ARGS >
struct FindIfImpl < COND, TypeList < T, ARGS...>> {
  using Type = RECore::ConditionalT < COND < T >::Value, T, typename FindIfImpl < COND, TypeList < ARGS...>>::Type >;
};
template < template < class > class COND >
struct FindIfImpl < COND, TypeList < >> {
  using Type = TTNullType;
};

template < template < class > class COND, typename ... ARGS > using FindIfT = typename FindIfImpl < COND, TypeList < ARGS...>>::Type;






template < typename List1, typename List2, typename Enable = void >
struct IsSameNullptrImpl;

template < template < class... > class List1, typename T, typename... T1,
  template < class... > class List2, typename U, typename... U1 >
struct IsSameNullptrImpl < List1 < T, T1... >, List2 < U, U1... >, void > {
  using Type = RECore::ConditionalT < RECore::IsSame < T, U >::Value, typename IsSameNullptrImpl < List1 < T1... >, List2 < U1...>>::Type, RECore::ConditionalT <
    RECore::IsPointer < T >::Value &&
    RECore::IsSame < U, std::nullptr_t >::Value, typename IsSameNullptrImpl < List1 < T1... >, List2 < U1...>>::Type, RECore::ConditionalT <
                                                                                                                         RECore::IsPointer < U >::Value &&
                                                                                                                         RECore::IsSame < T, std::nullptr_t >::Value, typename IsSameNullptrImpl < List1 < T1... >, List2 < U1...>>::Type, RECore::FalseType > > >;
};

template < template < class... > class List1, typename ...T,
  template < class... > class List2, typename ...U >
struct IsSameNullptrImpl < List1 < T... >, List2 < U... >, RECore::EnableIfT <
                                                           IsNoneArgument < T... >::Value && IsNoneArgument < U... >::Value > > {
  using Type = RECore::TrueType;
};

template < template < class... > class List1, typename ...T,
  template < class... > class List2, typename ...U >
struct IsSameNullptrImpl < List1 < T... >, List2 < U... >, RECore::EnableIfT <
                                                           (!IsNoneArgument < T... >::Value && IsNoneArgument < U... >::Value) ||
                                                           (IsNoneArgument < T... >::Value && !IsNoneArgument < U... >::Value) > > {
  using Type = RECore::FalseType;
};


template < typename T1, typename T2 >
using IsSameNullptr = typename IsSameNullptrImpl < T1, T2 >::Type;




template<typename O, typename T>
struct HasDoubleTypesImpl;

template<typename...Orig_Types>
struct HasDoubleTypesImpl<TypeList<Orig_Types...>, TypeList<>> : RECore::FalseType {};

template<typename...Orig_Types, typename T, typename... Tail>
struct HasDoubleTypesImpl<TypeList<Orig_Types...>, TypeList<T, Tail...>> : RECore::ConditionalT< CountType<T, TypeList<Orig_Types...>>::Value >= 2,
RECore::TrueType, typename HasDoubleTypesImpl<TypeList<Orig_Types...>, TypeList<Tail...>>::Type > {};

template <typename... T>
using HasDoubleTypes = typename HasDoubleTypesImpl<RECore::AsTypeListT<T...>, RECore::AsTypeListT<T...>>::Type;




template < template < class > class COND, typename T, typename Enable = void >
struct CountIfImpl;
template < template < class > class COND >
struct CountIfImpl < COND, TypeList <> > {
  static constexpr uint32 Value = 0;
};
template < template < class > class COND, typename T, typename ... ARGS >
struct CountIfImpl < COND, TypeList < T, ARGS... >, RECore::EnableIfT < COND < T >::Value>> {
  static constexpr uint32 Value = CountIfImpl < COND, TypeList < ARGS...>>::Value + 1;
};
template < template < class > class COND, typename T, typename ... ARGS >
struct CountIfImpl < COND, TypeList < T, ARGS... >, RECore::EnableIfT < !COND < T >::Value>> {
  static constexpr uint32 Value = CountIfImpl < COND, TypeList < ARGS...>>::Value;
};
template < template < class > class COND, typename ... ARGS > using CountIf = RECore::IntegralConstant < uint32, CountIfImpl < COND, TypeList < RawTypeT < ARGS >...>>::Value >;



template < class T, T Index, sizeT N >
struct SequenceGenerator
{
  using Type = typename SequenceGenerator < T, Index - 1, N - 1 >::Type::Next;
};
template < class T, T Index >
struct SequenceGenerator < T, Index, 0ul >
{
  using Type = RECore::IntegerSequence < T >;
};

template < sizeT ... I > using IndexSequence = RECore::IntegerSequence < sizeT, I... >;

/*
template < class T, T N >
struct MakeIntegerSequenceImpl
{
  using Type = typename SequenceGenerator < T, N, N >::Type;
};

template < class T, T N >
struct MakeIndexSequenceImpl
{
  using Type = typename MakeIntegerSequenceImpl < T, N >::Type;
};
 */

// integer sequences
//template < class T, T N > using MakeIntegerSequence = typename MakeIntegerSequenceImpl < T, N >::Type;
//template < sizeT N > using MakeIndexSequence = typename MakeIntegerSequenceImpl < sizeT, N >::Type;

template < class... T > using IndexSequenceFor = MakeIndexSequence < sizeof...(T) >;



template<typename TIndexSequence, sizeT TIndex>
struct EraseSequenceTill;
template<sizeT TIndex, sizeT TIdx, sizeT... I>
struct EraseSequenceTill<RECore::IndexSequence<TIdx, I...>, TIndex> {
  using Type = RECore::ConditionalT<
    TIdx == TIndex,
    IndexSequence<TIdx, I...>,
      typename EraseSequenceTill<IndexSequence<I...>, TIndex>::Type>;
};
template<sizeT TIndex>
struct EraseSequenceTill<IndexSequence<>, TIndex> {
  using Type = RECore::IndexSequence<>;
};



template<typename T>
struct RemoveFirstIndexImpl {
  using Type = RECore::IndexSequence<>;
};
template<sizeT TFirst, sizeT... I>
struct RemoveFirstIndexImpl<RECore::IndexSequence<TFirst, I...>> {
  using Type = RECore::IndexSequence<I...>;
};
template<typename T>
using RemoveFirstIndex = typename RemoveFirstIndexImpl<T>::Type;


template<typename, typename>
struct ConcatIndexSequence {};
template<sizeT... Ts, sizeT... Us>
struct ConcatIndexSequence<RECore::IndexSequence<Ts...>, RECore::IndexSequence<Us...>> {
  using Type = RECore::IndexSequence<Ts..., Us...>;
};


template<class T>
struct RemoveLastIndexImpl {};
template<sizeT TLast>
struct RemoveLastIndexImpl<RECore::IndexSequence<TLast>> {
  using Type = RECore::IndexSequence<>;
};
template<sizeT TFirst, sizeT... I>
struct RemoveLastIndexImpl<RECore::IndexSequence<TFirst, I...>> {
  using Type = typename ConcatIndexSequence<
    RECore::IndexSequence<TFirst>,
      typename RemoveLastIndexImpl<RECore::IndexSequence<I...>>::Type>::Type;
};
template<typename T>
using RemoveLastIndex = typename RemoveLastIndexImpl<T>::Type;


template<typename T, sizeT TIndex>
struct MetaTypeIdentity {
  using Type = T;
};


template<typename T, sizeT N, typename TIndices = RECore::MakeIndexSequence<N>>
struct CreateTypeList;
template<typename T, sizeT N, sizeT... TIndices>
struct CreateTypeList<T, N, RECore::IndexSequence<TIndices...>> {
  using Type = RECore::TypeList<typename MetaTypeIdentity<T, TIndices>::Type...>;
};
template<typename T, sizeT N>
using CreateTypeListT = typename CreateTypeList<T, N>::Type;


template<typename TElementType>
struct CopyArrayImpl {
  void operator()(const TElementType &in, TElementType &out) {
    out = in;
  }
};

template<typename TElementType, sizeT TCount>
struct CopyArrayImpl<TElementType[TCount]> {
  void operator()(const TElementType (&in)[TCount], TElementType (&out)[TCount]) {
    for(sizeT i = 0; i < TCount; ++i)
      CopyArrayImpl<TElementType>()(in[i], out[i]);
  }
};

template<typename TElementType, sizeT TCount>
auto CopyArray(const TElementType (&in)[TCount], TElementType (&out)[TCount]) -> TElementType (&)[TCount] {
  CopyArrayImpl<TElementType[TCount]>()(in, out);
  return out;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// checks whether the given type T has a less than operator
template<typename T, typename = decltype(RECore::DeclVal<T>() < RECore::DeclVal<T>() )>
RECore::TrueType  SupportsLessThanTest(const T&);
RECore::FalseType SupportsLessThanTest(...);

template<typename T>
struct HasLessThanOperator : RECore::IntegralConstant<bool, RECore::IsSame<RECore::TrueType, decltype(SupportsLessThanTest(RECore::DeclVal<T>()))>::Value> {};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// checks whether the given type T has the equal operator
template<typename T, typename = decltype(RECore::DeclVal<T>() == RECore::DeclVal<T>() )>
RECore::TrueType  SupportsEqualTest(const T&);
RECore::FalseType SupportsEqualTest(...);


template<typename T>
struct HasEqualOperator : RECore::IntegralConstant<bool, RECore::IsSame<RECore::TrueType, decltype(SupportsEqualTest(RECore::DeclVal<T>()))>::Value> {};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// find from a list of types
// find_types<std::tuple<int, bool>, bool, float>::type => std::tuple<bool>

template<typename Result_List, typename Types_To_Find, typename Types_To_Iterate>
struct FindTypesImpl;

template<typename...Rs, typename...Tf>
struct FindTypesImpl<RECore::TypeList<Rs...>, RECore::TypeList<Tf...>, RECore::TypeList<>> {
  using Type = RECore::TypeList<Rs...>;
};


template<typename...Rs, typename Types_To_Find, typename T, typename...Tail>
struct FindTypesImpl<RECore::TypeList<Rs...>, Types_To_Find, RECore::TypeList<T, Tail...>> {
  using Type = RECore::ConditionalT< Contains<T, Types_To_Find>::Value,
    typename FindTypesImpl<RECore::TypeList<Rs..., T>, Types_To_Find, RECore::TypeList<Tail...>>::Type,
  typename FindTypesImpl<RECore::TypeList<Rs...>, Types_To_Find, RECore::TypeList<Tail...>>::Type
  >;
};

template<typename Types_To_Find, typename Types_To_Iterate>
using FindTypes = FindTypesImpl<RECore::TypeList<>, RECore::AsTypeListT<Types_To_Find>, RECore::AsTypeListT<Types_To_Iterate>>;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore
