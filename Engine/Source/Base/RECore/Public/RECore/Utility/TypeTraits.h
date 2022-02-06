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

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

template<typename T, T V>
struct IntegralConstant {
  static constexpr T Value = V;
  typedef T ValueType;
  typedef IntegralConstant<T, V> Type;

  constexpr operator ValueType () const noexcept {
    return Value;
  }

  constexpr ValueType operator()() const noexcept {
    return Value;
  }
};

typedef IntegralConstant<bool, true> TrueType;
typedef IntegralConstant<bool, false> FalseType;

template<bool B>
using BoolConstant = IntegralConstant<bool, B>;


typedef char YesType;
struct NoType { char padding[8]; };

struct Unused {};



struct ArgumentSink {
  template<typename... TArgs>
  ArgumentSink(TArgs&&...) {

  }
};


template<bool TCONDITION, class TTRUE, class TFALSE>
struct TypeSelect {
  typedef TTRUE Type;
};
template<typename TTRUE, class TFALSE>
struct TypeSelect<false, TTRUE, TFALSE> {
  typedef TFALSE Type;
};

template<bool TCONDITION, class TTRUE, class TFALSE>
using TypeSelectT = typename TypeSelect<TCONDITION, TTRUE, TFALSE>::Type;



template<typename T, typename = Unused, typename = Unused>
struct FirstTypeSelect {
  typedef T Type;
};



template<bool B1, bool B2, bool B3 = false, bool B4 = false, bool B5 = false>
struct TypeOr;

template<bool B1, bool B2, bool B3, bool B4, bool B5>
struct TypeOr {
  static const bool Value = true;
};

template<>
struct TypeOr<false, false, false, false, false> {
  static const bool Value = false;
};



template<bool B1, bool B2, bool B3 = true, bool B4 = true, bool B5 = true>
struct TypeAnd;

template<bool B1, bool B2, bool B3, bool B4, bool B5>
struct TypeAnd {
  static const bool Value = false;
};

template<>
struct TypeAnd<true, true, true, true, true> {
  static const bool Value = true;
};


template<int B1, int B2>
struct TypeEqual {
  static const bool Value = (B1 == B2);
};


template<int B1, int B2>
struct TypeNotEqual {
  static const bool Value = (B1 != B2);
};


template<bool B>
struct TypeNot {
  static const bool Value = true;
};
template<>
struct TypeNot<true> {
  static const bool Value = false;
};



template<bool B, typename T = void>
struct EnableIf {

};
template<typename T>
struct EnableIf<true, T> {
  typedef T Type;
};
template<bool B, class T = void>
using EnableIfT = typename EnableIf<B, T>::Type;



template<bool B, typename TTRUE, typename TFALSE>
struct Conditional {
  typedef TTRUE Type;
};

template<typename TTRUE, typename TFALSE>
struct Conditional<false, TTRUE, TFALSE> {
  typedef TFALSE Type;
};

template<bool B, class TTRUE, class TFALSE>
using ConditionalT = typename Conditional<B, TTRUE, TFALSE>::Type;



template<class...>
struct Conjunction : TrueType {};

template<class B>
struct Conjunction<B> : public B {};

template<class B, class... Bs>
struct Conjunction<B, Bs...> : Conditional<bool(B::Value), Conjunction<Bs...>, B>::Type {};

template<class... Bs>
inline constexpr bool ConjunctionV = Conjunction<Bs...>::Value;


template<class...>
struct Disjunction : FalseType {};

template<class B>
struct Disjunction<B> : B {};

template<class B, class... Bs>
struct Disjunction<B, Bs...> : Conditional<bool(B::Value), B, Disjunction<Bs...>>::Type {};

template<class... Bs>
inline constexpr bool DisjunctionV = Disjunction<Bs...>::Value;



template<class B>
struct Negation : BoolConstant <!bool(B::Value)> {};
template<class B>
inline constexpr bool NegationV = Negation<B>::Value;



template<typename T>
struct Identity {
  using Type = T;
};
template<typename T>
using IdentityT = typename Identity<T>::Type;


template<typename T>
struct TypeIdentity {
  using Type = T;
};
template<typename T>
using TypeIdentityT = typename TypeIdentity<T>::Type;


template<typename T1, typename T2>
struct IsSame : public FalseType {};
template<typename T>
struct IsSame<T, T> : public TrueType {};
template<class T1, class T2>
constexpr bool IsSameV = IsSame<T1, T2>::Value;



template<typename T>
struct IsConstValue : public FalseType {};
template<typename T>
struct IsConstValue<const T*> : public TrueType {};
template<typename T>
struct IsConstValue<const volatile T*> : public TrueType {};

template<typename T>
struct IsConst : public IsConstValue<T*> {};
template<typename T>
struct IsConst<T&> : public FalseType {};
template<class T>
constexpr bool IsConstV = IsConst<T>::Value;



template<typename T>
struct IsVolatileValue : public FalseType {};
template<typename T>
struct IsVolatileValue<volatile T*> : public TrueType {};
template<typename T>
struct IsVolatileValue<const volatile T*> : public TrueType {};

template<typename T>
struct IsVolatile : IsVolatileValue<T*> {};
template<typename T>
struct IsVolatile<T&> : public FalseType {};
template<class T>
constexpr bool IsVolatileV = IsVolatile<T>::Value;



template<typename T>
struct IsReference : public FalseType {};
template<typename T>
struct IsReference<T&> : public TrueType {};
template<typename T>
constexpr bool IsReferenceV = IsReference<T>::Value;



template<typename>
struct IsFunction : public FalseType {};
template <typename TReturnValue, typename... TArgPack>
struct IsFunction<TReturnValue (TArgPack...)> : public TrueType {};

template <typename TReturnValue, typename... TArgPack>
struct IsFunction<TReturnValue (TArgPack..., ...)> : public TrueType {};

template<typename T>
constexpr bool IsFunctionV = IsFunction<T>::Value;


template<typename T>
struct RemoveConst { typedef T Type; };
template<typename T>
struct RemoveConst<const T> { typedef T Type; };
template<typename T>
struct RemoveConst<const T[]> { typedef T Type[]; };
template<typename T, sizeT N>
struct RemoveConst<const T[N]> { typedef T Type[N]; };
template<typename T>
using RemoveConstT = typename RemoveConst<T>::Type;



template<typename T>
struct RemoveVolatile { typedef T Type; };
template<typename T>
struct RemoveVolatile<volatile T> { typedef T Type; };
template<typename T>
struct RemoveVolatile<volatile T[]> { typedef T Type[]; };
template<typename T, sizeT N>
struct RemoveVolatile<volatile T[N]> { typedef T Type[N]; };
template<typename T>
using RemoveVolatileT = typename RemoveVolatile<T>::Type;



template<typename T>
struct RemoveCV {
  typedef typename RemoveVolatile<typename RemoveConst<T>::Type>::Type Type;
};
template<typename T>
using RemoveCVT = typename RemoveCV<T>::Type;



template<typename T>
struct AddReferenceImpl { typedef T&    Type; };
template<typename T>
struct AddReferenceImpl<T&> { typedef T&    Type; };
template<>
struct AddReferenceImpl<void> { typedef void Type; };
#if defined(_MSC_VER) && (_MSC_VER <= 1600)
template<typename T>
struct AddReferenceImpl<T[0]> { typedef T    Type; };
#endif
template<typename T>
struct AddReference {
  typedef typename AddReferenceImpl<T>::Type Type;
};



template<typename T>
struct RemoveReference { typedef T Type; };
template<typename T>
struct RemoveReference<T&> { typedef T Type; };
template<typename T>
struct RemoveReference<T&&> { typedef T Type; };
template<typename T>
using RemoveReferenceT = typename RemoveReference<T>::Type;



template<typename T>
struct RemoveCVRef {
  typedef typename RemoveVolatile<typename RemoveConst<typename RemoveReference<T>::Type>::Type>::Type Type;
};
template<typename T>
using RemoveCVRefT = typename RemoveCVRef<T>::Type;


template<typename T>
struct AddLValueReference           { typedef T& Type; };
template<typename T>
struct AddLValueReference<T&>       { typedef T& Type; };
template<>
struct AddLValueReference<void>     { typedef void Type; };
template<>
struct AddLValueReference<const void>     { typedef const void Type; };
template<>
struct AddLValueReference<volatile void>     { typedef volatile void Type; };
template<>
struct AddLValueReference<const volatile void>     { typedef const volatile void Type; };
template<typename T>
using AddLValueReferenceT = typename AddLValueReference<T>::Type;


template<typename T>
struct AddRValueReference           { typedef T&& Type; };
template<typename T>
struct AddRValueReference<T&>       { typedef T& Type; };
template<>
struct AddRValueReference<void>     { typedef void Type; };
template<>
struct AddRValueReference<const void>     { typedef const void Type; };
template<>
struct AddRValueReference<volatile void>     { typedef volatile void Type; };
template<>
struct AddRValueReference<const volatile void>     { typedef const volatile void Type; };
template<typename T>
using AddRValueReferenceT = typename AddRValueReference<T>::Type;



template<typename T>
typename AddRValueReference<T>::Type DeclVal() noexcept;




template <sizeT I0, sizeT ...in>
struct StaticMin;

template <sizeT I0>
struct StaticMin<I0> {
  static const sizeT Value = I0;
};

template <sizeT I0, sizeT I1, sizeT ...in>
struct StaticMin<I0, I1, in...> {
  static const sizeT Value = ((I0 <= I1) ? StaticMin<I0, in...>::Value : StaticMin<I1, in...>::Value);
};



template <sizeT I0, sizeT ...in>
struct StaticMax;

template <sizeT I0>
struct StaticMax<I0> {
  static const sizeT Value = I0;
};

template <sizeT I0, sizeT I1, sizeT ...in>
struct StaticMax<I0, I1, in...> {
  static const sizeT Value = ((I0 >= I1) ? StaticMax<I0, in...>::Value : StaticMax<I1, in...>::Value);
};


/**
 * @brief
 * IsNull
 *
 * @tparam T
 * @return
 */
template<typename T>
bool IsNull(const T&) {
  return false;
}
template<typename TResult, typename... TArgs>
bool IsNull(TResult (*const& cFunctionPointer)(TArgs...)) {
  return cFunctionPointer == nullptr;
}
template<typename TResult, typename TClass, typename... TArgs>
bool IsNull(TResult (TClass::*const& cFunctionPointer)(TArgs...)) {
  return cFunctionPointer == nullptr;
}
template<typename TResult, typename TClass, typename... TArgs>
bool IsNull(TResult (TClass::*const& cFunctionPointer)(TArgs...) const) {
  return cFunctionPointer == nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore

#include "RECore/Utility/TypeFundamental.inl"
#include "RECore/Utility/TypeTransformations.inl"
#include "RECore/Utility/TypeProperties.inl"
#include "RECore/Utility/TypeCompound.inl"
#include "RECore/Utility/TypePod.inl"