
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

// Adapted from: https://github.com/electronicarts/EASTL/

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/Utility/Pair.h"
#include "RECore/Utility/Functional.h"
#include "RECore/Utility/IntegerSequence.h"
#include "RECore/Utility/Move.h"
#include "RECore/Utility/TypeTraits.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename... T>
class Tuple;
template<typename TTuple>
class TupleSize;
template<sizeT I, typename TTuple>
class TupleElement;
template<sizeT I, typename TTuple>
using TupleElementT = typename TupleElement<I, TTuple>::Type;
template<sizeT I, typename TTuple>
using ConstTupleElementT = typename Conditional<
  IsLValueReference<TupleElementT <I, TTuple>>::Value,
  AddLValueReferenceT<const RemoveReferenceT<TupleElementT<I, TTuple>>>,
    const TupleElementT<I, TTuple>>::Type;

template<sizeT I, typename... Ts>
TupleElementT<I, Tuple<Ts...>>& Get(Tuple<Ts...>& t);
template<sizeT I, typename... Ts>
ConstTupleElementT<I, Tuple<Ts...>>& Get(const Tuple<Ts...>& t);
template<sizeT I, typename... Ts>
TupleElementT<I, Tuple<Ts...>>&& Get(Tuple<Ts...>&& t);
template<typename T, typename... Ts>
T& Get(Tuple<Ts...>& t);
template<typename T, typename... Ts>
const T& Get(const Tuple<Ts...>& t);
template<typename T, typename... Ts>
T&& Get(Tuple<Ts...>&& t);


template <typename... Ts> struct TupleTypes {};

template <typename T> class TupleSize {};
template <typename T> class TupleSize<const T>          : public TupleSize<T> {};
template <typename T> class TupleSize<volatile T>       : public TupleSize<T> {};
template <typename T> class TupleSize<const volatile T> : public TupleSize<T> {};

template <typename... Ts> class TupleSize<TupleTypes<Ts...>> : public IntegralConstant<sizeT, sizeof...(Ts)> {};
template <typename... Ts> class TupleSize<Tuple<Ts...>>      : public IntegralConstant<sizeT, sizeof...(Ts)> {};
template <class T>
constexpr sizeT TupleSizeV = TupleSize<T>::Value;

template <typename TTupleIndices, typename... Ts>
struct TupleImpl;

template <typename TIndices, typename... Ts>
class TupleSize<TupleImpl<TIndices, Ts...>> : public IntegralConstant<sizeT, sizeof...(Ts)> {};


template <sizeT I, typename T>
class TupleElement {};

template <sizeT I>
class TupleElement<I, TupleTypes<>> {
public:
  //static_assert(I != I, "TupleElement index out of range");
};

template <typename H, typename... Ts>
class TupleElement<0, TupleTypes<H, Ts...>> {
public:
  typedef H Type;
};

template <sizeT I, typename H, typename... Ts>
class TupleElement<I, TupleTypes<H, Ts...>> {
public:
  typedef TupleElementT<I - 1, TupleTypes<Ts...>> Type;
};

// specialization for tuple
template <sizeT I, typename... Ts>
class TupleElement<I, Tuple<Ts...>> {
public:
  typedef TupleElementT<I, TupleTypes<Ts...>> Type;
};

template <sizeT I, typename... Ts>
class TupleElement<I, const Tuple<Ts...>> {
public:
  typedef typename AddConst<TupleElementT<I, TupleTypes<Ts...>>>::Type Type;
};

template <sizeT I, typename... Ts>
class TupleElement<I, volatile Tuple<Ts...>> {
public:
  typedef typename AddVolatile<TupleElementT<I, TupleTypes<Ts...>>>::Type Type;
};

template <sizeT I, typename... Ts>
class TupleElement<I, const volatile Tuple<Ts...>> {
public:
  typedef typename AddCV<TupleElementT<I, TupleTypes<Ts...>>>::Type Type;
};

// specialization for TupleImpl
template <sizeT I, typename Indices, typename... Ts>
class TupleElement<I, TupleImpl<Indices, Ts...>> : public TupleElement<I, Tuple<Ts...>> {};

template <sizeT I, typename Indices, typename... Ts>
class TupleElement<I, const TupleImpl<Indices, Ts...>> : public TupleElement<I, const Tuple<Ts...>> {};

template <sizeT I, typename Indices, typename... Ts>
class TupleElement<I, volatile TupleImpl<Indices, Ts...>> : public TupleElement<I, volatile Tuple<Ts...>> {};

template <sizeT I, typename Indices, typename... Ts>
class TupleElement<I, const volatile TupleImpl<Indices, Ts...>> : public TupleElement<I, const volatile Tuple<Ts...>> {};


template <typename T, typename Tuple>
struct TupleIndex {};

template <typename T>
struct TupleIndex<T, TupleTypes<>> {
  typedef void DuplicateTypeCheck;
  TupleIndex() = delete; // TupleIndex should only be used for compile-time assistance, and never be instantiated
  static const sizeT Index = 0;
};

template <typename T, typename... TsRest>
struct TupleIndex<T, TupleTypes<T, TsRest...>> {
  typedef int DuplicateTypeCheck;
  // after finding type T in the list of types, try to find type T in TsRest.
  // If we stumble back into this version of TupleIndex, i.e. type T appears twice in the list of types, then DuplicateTypeCheck will be of type int, and the static_assert will fail.
  // If we don't, then we'll go through the version of TupleIndex above, where all of the types have been exhausted, and DuplicateTypeCheck will be void.
  static_assert(IsVoid<typename TupleIndex<T, TupleTypes<TsRest...>>::DuplicateTypeCheck>::Value, "duplicate type T in tuple_vector::get<T>(); unique types must be provided in declaration, or only use get<sizeT>()");

  static const sizeT Index = 0;
};

template <typename T, typename TsHead, typename... TsRest>
struct TupleIndex<T, TupleTypes<TsHead, TsRest...>> {
  typedef typename TupleIndex<T, TupleTypes<TsRest...>>::DuplicateTypeCheck DuplicateTypeCheck;
  static const sizeT Index = TupleIndex<T, TupleTypes<TsRest...>>::Index + 1;
};

template <typename T, typename Indices, typename... Ts>
struct TupleIndex<T, TupleImpl<Indices, Ts...>> : public TupleIndex<T, TupleTypes<Ts...>> {};


template <typename... Ts>
void Swallow(Ts&&...) {}

// TupleLeaf
//
template <size_t I, typename ValueType, bool IsEmpty = IsEmptyV<ValueType>>
class TupleLeaf;
template <size_t I, typename ValueType, bool IsEmpty>
inline void Swap(TupleLeaf<I, ValueType, IsEmpty>& a, TupleLeaf<I, ValueType, IsEmpty>& b) {
  RECore::Swap(a.GetInternal(), b.GetInternal());
}
template <size_t I, typename ValueType, bool IsEmpty>
class TupleLeaf {
public:
  TupleLeaf() : mValue() {}
  TupleLeaf(const TupleLeaf&) = default;
  TupleLeaf& operator=(const TupleLeaf&) = delete;

  // We shouldn't need this explicit constructor as it should be handled by the template below but OSX clang
  // IsConstructible type trait incorrectly gives false for IsConstructible<T&&, T&&>::Value
  explicit TupleLeaf(ValueType&& v) : mValue(RECore::Move(v)) {}

  template <typename T, typename = typename EnableIf<IsConstructible<ValueType, T&&>::Value>::Type>
  explicit TupleLeaf(T&& t)
  : mValue(RECore::Forward<T>(t)) {
  }

  template <typename T>
  explicit TupleLeaf(const TupleLeaf<I, T>& t)
  : mValue(t.GetInternal()) {
  }

  template <typename T>
  TupleLeaf& operator=(T&& t) {
    mValue = RECore::Forward<T>(t);
    return *this;
  }

  int Swap(TupleLeaf& t) {
    Swap(*this, t);
    return 0;
  }

  ValueType& GetInternal() { return mValue; }
  const ValueType& GetInternal() const { return mValue; }

private:
  ValueType mValue;
};
// TupleLeaf: Specialize for when ValueType is a reference
template <size_t I, typename ValueType, bool IsEmpty>
class TupleLeaf<I, ValueType&, IsEmpty>
{
public:
  TupleLeaf(const TupleLeaf&) = default;
  TupleLeaf& operator=(const TupleLeaf&) = delete;

  template <typename T, typename = typename EnableIf<IsConstructible<ValueType, T&&>::Value>::Type>
  explicit TupleLeaf(T&& t)
    : mValue(RECore::Forward<T>(t))
  {
  }

  explicit TupleLeaf(ValueType& t) : mValue(t)
  {
  }

  template <typename T>
  explicit TupleLeaf(const TupleLeaf<I, T>& t)
    : mValue(t.GetInternal())
  {
  }

  template <typename T>
  TupleLeaf& operator=(T&& t)
  {
    mValue = RECore::Forward<T>(t);
    return *this;
  }

  int Swap(TupleLeaf& t)
  {
    RECore::Swap(*this, t);
    return 0;
  }

  ValueType& GetInternal() { return mValue; }
  const ValueType& GetInternal() const { return mValue; }

private:
  ValueType& mValue;
};
// TupleLeaf: partial specialization for when we can use the Empty Base Class Optimization
template <size_t I, typename ValueType>
class TupleLeaf<I, ValueType, true> : private ValueType
{
public:
  // TrueType / FalseType constructors for case where ValueType is default constructible and should be value
  // initialized and case where it is not
  TupleLeaf(const TupleLeaf&) = default;

  template <typename T, typename = typename EnableIf<IsConstructible<ValueType, T&&>::Value>::Type>
  explicit TupleLeaf(T&& t)
    : ValueType(RECore::Forward<T>(t))
  {
  }

  template <typename T>
  explicit TupleLeaf(const TupleLeaf<I, T>& t)
    : ValueType(t.GetInternal())
  {
  }

  template <typename T>
  TupleLeaf& operator=(T&& t)
  {
    ValueType::operator=(RECore::Forward<T>(t));
    return *this;
  }

  int Swap(TupleLeaf& t)
  {
    RECore::Swap(*this, t);
    return 0;
  }

  ValueType& GetInternal() { return static_cast<ValueType&>(*this); }
  const ValueType& GetInternal() const { return static_cast<const ValueType&>(*this); }

private:
  TupleLeaf& operator=(const TupleLeaf&) = delete;
};
// MakeTupleTypes
//
//
template <typename TupleTypes, typename Tuple, size_t Start, size_t End>
struct MakeTupleTypesImpl;

template <typename... Types, typename Tuple, size_t Start, size_t End>
struct MakeTupleTypesImpl<TupleTypes<Types...>, Tuple, Start, End> {
  typedef typename RemoveReference<Tuple>::Type TupleType;
  typedef typename MakeTupleTypesImpl<
    TupleTypes<Types..., typename Conditional<IsLValueReference<Tuple>::Value,
      // append ref if Tuple is ref
      TupleElementT<Start, TupleType>&,
      // append non-ref otherwise
      TupleElementT<Start, TupleType>>::Type>,
  Tuple, Start + 1, End>::Type Type;
};

template <typename... Types, typename Tuple, size_t End>
struct MakeTupleTypesImpl<TupleTypes<Types...>, Tuple, End, End> {
  typedef TupleTypes<Types...> Type;
};

template <typename Tuple>
using MakeTupleTypesT = typename MakeTupleTypesImpl<TupleTypes<>, Tuple, 0,
  TupleSize<typename RemoveReference<Tuple>::Type>::Value>::Type;


// TupleImpl
//
//
template <size_t I, typename Indices, typename... Ts>
TupleElementT<I, TupleImpl<Indices, Ts...>>& Get(TupleImpl<Indices, Ts...>& t);

template <size_t I, typename Indices, typename... Ts>
ConstTupleElementT<I, TupleImpl<Indices, Ts...>>& Get(const TupleImpl<Indices, Ts...>& t);

template <size_t I, typename Indices, typename... Ts>
TupleElementT<I, TupleImpl<Indices, Ts...>>&& Get(TupleImpl<Indices, Ts...>&& t);

template <typename T, typename Indices, typename... Ts>
T& Get(TupleImpl<Indices, Ts...>& t);

template <typename T, typename Indices, typename... Ts>
const T& Get(const TupleImpl<Indices, Ts...>& t);

template <typename T, typename Indices, typename... Ts>
T&& Get(TupleImpl<Indices, Ts...>&& t);

template <size_t... Indices, typename... Ts>
struct TupleImpl<IntegerSequence<size_t, Indices...>, Ts...> : public TupleLeaf<Indices, Ts>...
{
  constexpr TupleImpl() = default;

  // index_sequence changed to IntegerSequence due to issues described below in VS2015 CTP 6.
  // https://connect.microsoft.com/VisualStudio/feedback/details/1126958/error-in-template-parameter-pack-expansion-of-std-index-sequence
  //
  template <typename... Us, typename... ValueTypes>
  explicit TupleImpl(IntegerSequence<size_t, Indices...>, TupleTypes<Us...>, ValueTypes&&... values)
  : TupleLeaf<Indices, Ts>(RECore::Forward<ValueTypes>(values))...
  {
  }

  template <typename OtherTuple>
  TupleImpl(OtherTuple&& t)
  : TupleLeaf<Indices, Ts>(RECore::Forward<TupleElementT<Indices, MakeTupleTypesT<OtherTuple>>>(Get<Indices>(t)))...
  {
  }

  template <typename OtherTuple>
  TupleImpl& operator=(OtherTuple&& t)
  {
    Swallow(TupleLeaf<Indices, Ts>::operator=(
      RECore::Forward<TupleElementT<Indices, MakeTupleTypesT<OtherTuple>>>(Get<Indices>(t)))...);
    return *this;
  }

  TupleImpl& operator=(const TupleImpl& t)
  {
    Swallow(TupleLeaf<Indices, Ts>::operator=(static_cast<const TupleLeaf<Indices, Ts>&>(t).GetInternal())...);
    return *this;
  }

  void swap(TupleImpl& t) { Swallow(TupleLeaf<Indices, Ts>::Swap(static_cast<TupleLeaf<Indices, Ts>&>(t))...); }
};

template <size_t I, typename Indices, typename... Ts>
inline TupleElementT<I, TupleImpl<Indices, Ts...>>& Get(TupleImpl<Indices, Ts...>& t) {
  typedef TupleElementT<I, TupleImpl<Indices, Ts...>> Type;
  return static_cast<TupleLeaf<I, Type>&>(t).GetInternal();
}

template <size_t I, typename Indices, typename... Ts>
inline ConstTupleElementT<I, TupleImpl<Indices, Ts...>>& Get(const TupleImpl<Indices, Ts...>& t) {
  typedef TupleElementT<I, TupleImpl<Indices, Ts...>> Type;
  return static_cast<const TupleLeaf<I, Type>&>(t).GetInternal();
}

template <size_t I, typename Indices, typename... Ts>
inline TupleElementT<I, TupleImpl<Indices, Ts...>>&& Get(TupleImpl<Indices, Ts...>&& t) {
  typedef TupleElementT<I, TupleImpl<Indices, Ts...>> Type;
  return static_cast<Type&&>(static_cast<TupleLeaf<I, Type>&>(t).GetInternal());
}

template <typename T, typename Indices, typename... Ts>
inline T& Get(TupleImpl<Indices, Ts...>& t) {
  typedef TupleIndex<T, TupleImpl<Indices, Ts...>> Index;
  return static_cast<TupleLeaf<Index::Index, T>&>(t).GetInternal();
}

template <typename T, typename Indices, typename... Ts>
inline const T& Get(const TupleImpl<Indices, Ts...>& t) {
  typedef TupleIndex<T, TupleImpl<Indices, Ts...>> Index;
  return static_cast<const TupleLeaf<Index::index, T>&>(t).GetInternal();
}

template <typename T, typename Indices, typename... Ts>
inline T&& Get(TupleImpl<Indices, Ts...>&& t) {
  typedef TupleIndex<T, TupleImpl<Indices, Ts...>> Index;
  return static_cast<T&&>(static_cast<TupleLeaf<Index::Index, T>&>(t).GetInternal());
}


// TupleLike
//
// type-trait that determines if a type is an eastl::tuple or an eastl::pair.
//
template <typename T> struct TupleLike                   : public FalseType {};
template <typename T> struct TupleLike<const T>          : public TupleLike<T> {};
template <typename T> struct TupleLike<volatile T>       : public TupleLike<T> {};
template <typename T> struct TupleLike<const volatile T> : public TupleLike<T> {};

template <typename... Ts>
struct TupleLike<Tuple<Ts...>> : public TrueType {};

template <typename First, typename Second>
struct TupleLike<Pair<First, Second>> : public TrueType {};


// TupleConvertible
//
//
//
template <bool IsSameSize, typename From, typename To>
struct TupleConvertibleImpl : public FalseType {};

template <typename... FromTypes, typename... ToTypes>
struct TupleConvertibleImpl<true, TupleTypes<FromTypes...>,	TupleTypes<ToTypes...>>
  : public IntegralConstant<bool, Conjunction<IsConvertible<FromTypes, ToTypes>...>::Value> {};

template <typename From, typename To,
  bool = TupleLike<typename RemoveReference<From>::Type>::Value,
  bool = TupleLike<typename RemoveReference<To>::Type>::Value>
struct TupleConvertible : public FalseType {};

template <typename From, typename To>
struct TupleConvertible<From, To, true, true>
  : public TupleConvertibleImpl<TupleSize<typename RemoveReference<From>::Type>::Value ==
                                TupleSize<typename RemoveReference<To>::Type>::Value,
    MakeTupleTypesT<From>, MakeTupleTypesT<To>> {};


// TupleAssignable
//
//
//
template <bool IsSameSize, typename Target, typename From>
struct TupleAssignableImpl : public FalseType {};

template <typename... TargetTypes, typename... FromTypes>
struct TupleAssignableImpl<true, TupleTypes<TargetTypes...>, TupleTypes<FromTypes...>>
  : public BoolConstant<Conjunction<IsAssignable<TargetTypes, FromTypes>...>::Value> {};

template <typename Target, typename From,
  bool = TupleLike<typename RemoveReference<Target>::Type>::Value,
  bool = TupleLike<typename RemoveReference<From>::Type>::Value>
struct TupleAssignable : public FalseType {};

template <typename Target, typename From>
struct TupleAssignable<Target, From, true, true>
  : public TupleAssignableImpl<
    TupleSize<typename RemoveReference<Target>::Type>::Value ==
    TupleSize<typename RemoveReference<From>::Type>::Value,
    MakeTupleTypesT<Target>, MakeTupleTypesT<From>> {};


// TupleImplicitlyConvertible and TupleExplicitlyConvertible
//
// helpers for constraining conditionally-explicit ctors
//
template <bool IsSameSize, typename TargetType, typename... FromTypes>
struct TupleImplicitlyConvertibleImpl : public FalseType {};


template <typename... TargetTypes, typename... FromTypes>
struct TupleImplicitlyConvertibleImpl<true, TupleTypes<TargetTypes...>, FromTypes...>
: public Conjunction<
  IsConstructible<TargetTypes, FromTypes>...,
IsConvertible<FromTypes, TargetTypes>...> {};

template <typename TargetTupleType, typename... FromTypes>
struct TupleImplicitlyConvertible
  : public TupleImplicitlyConvertibleImpl<
    TupleSize<TargetTupleType>::Value == sizeof...(FromTypes),
    MakeTupleTypesT<TargetTupleType>, FromTypes...>::Type {};

template<typename TargetTupleType, typename... FromTypes>
using TupleImplicitlyConvertibleT = EnableIfT<TupleImplicitlyConvertible<TargetTupleType, FromTypes...>::Value, bool>;

template <bool IsSameSize, typename TargetType, typename... FromTypes>
struct TupleExplicitlyConvertibleImpl : public FalseType {};

template <typename... TargetTypes, typename... FromTypes>
struct TupleExplicitlyConvertibleImpl<true, TupleTypes<TargetTypes...>, FromTypes...>
: public Conjunction<
  IsConstructible<TargetTypes, FromTypes>...,
Negation<Conjunction<IsConvertible<FromTypes, TargetTypes>...>>> {};

template <typename TargetTupleType, typename... FromTypes>
struct TupleExplicitlyConvertible
  : public TupleExplicitlyConvertibleImpl<
    TupleSize<TargetTupleType>::Value == sizeof...(FromTypes),
    MakeTupleTypesT<TargetTupleType>, FromTypes...>::Type {};

template<typename TargetTupleType, typename... FromTypes>
using TupleExplicitlyConvertibleT = EnableIfT<TupleExplicitlyConvertible<TargetTupleType, FromTypes...>::Value, bool>;


// TupleEqual
//
//
//
template <size_t I>
struct TupleEqual
{
  template <typename Tuple1, typename Tuple2>
  bool operator()(const Tuple1& t1, const Tuple2& t2)
  {
    static_assert(TupleSize<Tuple1>::Value == TupleSize<Tuple2>::Value, "comparing tuples of different sizes.");
    return TupleEqual<I - 1>()(t1, t2) && Get<I - 1>(t1) == Get<I - 1>(t2);
  }
};

// TupleTypeEqual
//
//
template<sizeT I>
struct TupleTypeEqual {

};
template<>
struct TupleTypeEqual<0> {

};

template <>
struct TupleEqual<0>
{
  template <typename Tuple1, typename Tuple2>
  bool operator()(const Tuple1&, const Tuple2&)
  {
    return true;
  }
};


// TupleLess
//
//
//
template <size_t I>
struct TupleLess
{
  template <typename Tuple1, typename Tuple2>
  bool operator()(const Tuple1& t1, const Tuple2& t2)
  {
    static_assert(TupleSize<Tuple1>::Value == TupleSize<Tuple2>::Value, "comparing tuples of different sizes.");
    return TupleLess<I - 1>()(t1, t2) || (!TupleLess<I - 1>()(t2, t1) && Get<I - 1>(t1) < Get<I - 1>(t2));
  }
};

template <>
struct TupleLess<0>
{
  template <typename Tuple1, typename Tuple2>
  bool operator()(const Tuple1&, const Tuple2&) {
    return false;
  }
};


// MakeTupleReturnImpl
//
//
//
template <typename T> struct MakeTupleReturnImpl                       { typedef T Type; };
template <typename T> struct MakeTupleReturnImpl<ReferenceWrapper<T>> { typedef T& Type; };

template <typename T>
using MakeTupleReturnT = typename MakeTupleReturnImpl<DecayT<T>>::Type;


// tuple_cat helpers
//
//
//

// TupleCat2Impl
template <typename Tuple1, typename Is1, typename Tuple2, typename Is2>
struct TupleCat2Impl;

template <typename... T1s, size_t... I1s, typename... T2s, size_t... I2s>
struct TupleCat2Impl<Tuple<T1s...>, IndexSequence<I1s...>, Tuple<T2s...>, IndexSequence<I2s...>> {
  using ResultType = Tuple<T1s..., T2s...>;

  template <typename Tuple1, typename Tuple2>
  static inline ResultType DoCat2(Tuple1&& t1, Tuple2&& t2) {
    return ResultType(Get<I1s>(RECore::Forward<Tuple1>(t1))..., Get<I2s>(RECore::Forward<Tuple2>(t2))...);
  }
};

// TupleCat2
template <typename Tuple1, typename Tuple2>
struct TupleCat2;

template <typename... T1s, typename... T2s>
struct TupleCat2<Tuple<T1s...>, Tuple<T2s...>> {
  using Is1        = MakeIndexSequence<sizeof...(T1s)>;
  using Is2        = MakeIndexSequence<sizeof...(T2s)>;
  using TCI        = TupleCat2Impl<Tuple<T1s...>, Is1, Tuple<T2s...>, Is2>;
  using ResultType = typename TCI::ResultType;
  
  template <typename Tuple1, typename Tuple2>
  static inline ResultType DoCat2(Tuple1&& t1, Tuple2&& t2) {
    return TCI::DoCat2(RECore::Forward<Tuple1>(t1), RECore::Forward<Tuple2>(t2));
  }
};

// TupleCat
template <typename... TTuples>
struct _TupleCat;

template <typename TTuple1, typename TTuple2, typename... TTuplesRest>
struct _TupleCat<TTuple1, TTuple2, TTuplesRest...> {
  using FirstResultType = typename TupleCat2<TTuple1, TTuple2>::ResultType;
  using ResultType      = typename _TupleCat<FirstResultType, TTuplesRest...>::ResultType;

  template <typename TupleArg1, typename TupleArg2, typename... TupleArgsRest>
  static inline ResultType DoCat(TupleArg1&& t1, TupleArg2&& t2, TupleArgsRest&&... ts) {
    return _TupleCat<FirstResultType, TTuplesRest...>::DoCat(
      TupleCat2<TupleArg1, TupleArg2>::DoCat2(RECore::Forward<TupleArg1>(t1), RECore::Forward<TupleArg2>(t2)),
      RECore::Forward<TupleArgsRest>(ts)...);
  }
};

template <typename Tuple1, typename Tuple2>
struct _TupleCat<Tuple1, Tuple2> {
  using TC2 = TupleCat2<Tuple1, RemoveReferenceT<Tuple2>>;
  using ResultType = typename TC2::ResultType;

  template <typename TupleArg1, typename TupleArg2>
  static inline ResultType DoCat(TupleArg1&& t1, TupleArg2&& t2) {
    return TC2::DoCat2(RECore::Forward<TupleArg1>(t1), RECore::Forward<TupleArg2>(t2));
  }
};




template<typename... Ts>
class Tuple;
template<typename T, typename... Ts>
class Tuple<T, Ts...> {
public:
  constexpr Tuple() = default;

  template <typename T2 = T,
    TupleImplicitlyConvertibleT<Tuple, const T2&, const Ts&...> = 0>
  constexpr Tuple(const T& t, const Ts&... ts)
  : mImpl(MakeIndexSequence<sizeof...(Ts) + 1>{}, MakeTupleTypesT<Tuple>{}, t, ts...)
  {
  }

  template <typename T2 = T,
    TupleExplicitlyConvertibleT<Tuple, const T2&, const Ts&...> = 0>
  explicit constexpr Tuple(const T& t, const Ts&... ts)
  : mImpl(MakeIndexSequence<sizeof...(Ts) + 1>{}, MakeTupleTypesT<Tuple>{}, t, ts...)
  {
  }

  template <typename U, typename... Us,
    TupleImplicitlyConvertibleT<Tuple, U, Us...> = 0>
  constexpr Tuple(U&& u, Us&&... us)
  : mImpl(MakeIndexSequence<sizeof...(Us) + 1>{}, MakeTupleTypesT<Tuple>{}, RECore::Forward<U>(u),
    RECore::Forward<Us>(us)...)
  {
  }

  template <typename U, typename... Us,
    TupleExplicitlyConvertibleT<Tuple, U, Us...> = 0>
  explicit constexpr Tuple(U&& u, Us&&... us)
  : mImpl(MakeIndexSequence<sizeof...(Us) + 1>{}, MakeTupleTypesT<Tuple>{}, RECore::Forward<U>(u),
    RECore::Forward<Us>(us)...)
  {
  }

  template <typename OtherTuple,
    typename EnableIf<TupleConvertible<OtherTuple, Tuple>::Value, bool>::Type = false>
  Tuple(OtherTuple&& t)
  : mImpl(RECore::Forward<OtherTuple>(t))
  {
  }

  template <typename OtherTuple,
    typename EnableIf<TupleAssignable<Tuple, OtherTuple>::Value, bool>::Type = false>
  Tuple& operator=(OtherTuple&& t)
  {
    mImpl.operator=(RECore::Forward<OtherTuple>(t));
    return *this;
  }

  void Swap(Tuple& t) { mImpl.Swap(t.mImpl); }

protected:
private:
  typedef TupleImpl<MakeIndexSequence<sizeof...(Ts) + 1>, T, Ts...> Impl;
  Impl mImpl;

  template <size_t I, typename... Ts_>
  friend TupleElementT<I, Tuple<Ts_...>>& Get(Tuple<Ts_...>& t);

  template <size_t I, typename... Ts_>
  friend ConstTupleElementT<I, Tuple<Ts_...>>& Get(const Tuple<Ts_...>& t);

  template <size_t I, typename... Ts_>
  friend TupleElementT<I, Tuple<Ts_...>>&& Get(Tuple<Ts_...>&& t);

  template <typename T_, typename... ts_>
  friend T_& Get(Tuple<ts_...>& t);

  template <typename T_, typename... ts_>
  friend const T_& Get(const Tuple<ts_...>& t);

  template <typename T_, typename... ts_>
  friend T_&& Get(Tuple<ts_...>&& t);
};

template<>
class Tuple<> {
public:
  void Swap(Tuple&) {}

protected:
private:
};


template <size_t I, typename... Ts>
inline TupleElementT<I, Tuple<Ts...>>& Get(Tuple<Ts...>& t)
{
  return Get<I>(t.mImpl);
}

template <size_t I, typename... Ts>
inline ConstTupleElementT<I, Tuple<Ts...>>& Get(const Tuple<Ts...>& t)
{
  return Get<I>(t.mImpl);
}

template <size_t I, typename... Ts>
inline TupleElementT<I, Tuple<Ts...>>&& Get(Tuple<Ts...>&& t)
{
  return Get<I>(move(t.mImpl));
}

template <typename T, typename... Ts>
inline T& Get(Tuple<Ts...>& t)
{
  return Get<T>(t.mImpl);
}

template <typename T, typename... Ts>
inline const T& Get(const Tuple<Ts...>& t)
{
  return Get<T>(t.mImpl);
}

template <typename T, typename... Ts>
inline T&& Get(Tuple<Ts...>&& t) {
  return Get<T>(RECore::Move(t.mImpl));
}

template <typename... Ts>
inline void Swap(Tuple<Ts...>& a, Tuple<Ts...>& b) {
  a.Swap(b);
}


// tuple operators
//
//
template <typename... T1s, typename... T2s>
inline bool operator==(const Tuple<T1s...>& t1, const Tuple<T2s...>& t2) {
  return TupleEqual<sizeof...(T1s)>()(t1, t2);
}

template <typename... T1s, typename... T2s>
inline bool operator<(const Tuple<T1s...>& t1, const Tuple<T2s...>& t2) {
  return TupleLess<sizeof...(T1s)>()(t1, t2);
}

template <typename... T1s, typename... T2s> inline bool operator!=(const Tuple<T1s...>& t1, const Tuple<T2s...>& t2) { return !(t1 == t2); }
template <typename... T1s, typename... T2s> inline bool operator> (const Tuple<T1s...>& t1, const Tuple<T2s...>& t2) { return t2 < t1; }
template <typename... T1s, typename... T2s> inline bool operator<=(const Tuple<T1s...>& t1, const Tuple<T2s...>& t2) { return !(t2 < t1); }
template <typename... T1s, typename... T2s> inline bool operator>=(const Tuple<T1s...>& t1, const Tuple<T2s...>& t2) { return !(t1 < t2); }


// tuple_cat
//
//
template <typename... TTuples>
inline typename _TupleCat<TTuples...>::ResultType TupleCat(TTuples&&... ts) {
  return _TupleCat<TTuples...>::DoCat(RECore::Forward<TTuples>(ts)...);
}


// make_tuple
//
//
template <typename... Ts>
inline constexpr Tuple<MakeTupleReturnT<Ts>...> MakeTuple(Ts&&... values) {
  return Tuple<MakeTupleReturnT<Ts>...>(RECore::Forward<Ts>(values)...);
}


// forward_as_tuple
//
//
template <typename... Ts>
inline constexpr Tuple<Ts&&...> ForwardAsTuple(Ts&&... ts) noexcept {
  return Tuple<Ts&&...>(RECore::Forward<Ts&&>(ts)...);
}



struct IgnoreT {
  IgnoreT() = default;

  template <typename T>
  const IgnoreT& operator=(const T&) const {
    return *this;
  }
};
static const IgnoreT Ignore;



template <typename... Ts>
inline constexpr Tuple<Ts&...> Tie(Ts&... ts) noexcept {
  return Tuple<Ts&...>(ts...);
}


template <class F, class Tuple, size_t... I>
constexpr decltype(auto) ApplyImpl(F&& f, Tuple&& t, IndexSequence<I...>) {
  return Invoke(RECore::Forward<F>(f), Get<I>(RECore::Forward<Tuple>(t))...);
}
template <class F, class TTuple>
constexpr decltype(auto) Apply(F&& f, TTuple&& t) {
  return ApplyImpl(RECore::Forward<F>(f), RECore::Forward<TTuple>(t), MakeIndexSequence<TupleSizeV<RemoveReferenceT<TTuple>>>{});
}

template <class F, class Tuple, size_t... I>
constexpr decltype(auto) WrappedApplyImpl(F&& f, Tuple&& t, IndexSequence<I...>) {
  return Invoke(RECore::Forward<F>(f), Get<I>(RECore::Forward<Tuple>(t))...);
}
template <class F, class TTuple>
constexpr decltype(auto) WrappedApply(F&& f, TTuple&& t) {
  return ApplyImpl(RECore::Forward<F>(f), RECore::Forward<TTuple>(t), MakeIndexSequence<TupleSizeV<RemoveReferenceT<TTuple>>>{});
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore
