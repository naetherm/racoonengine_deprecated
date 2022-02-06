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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

/**
 * @struct
 * Extent
 *
 * @tparam T
 * @tparam N
 */
template<typename T, unsigned N>
struct ExtentInternal : public IntegralConstant<sizeT, 0> {};

template<typename T, unsigned I>
struct ExtentInternal<T[I], 0> : public IntegralConstant<sizeT, I> {};

template<typename T, unsigned N, unsigned I>
struct ExtentInternal<T[I], N> : public ExtentInternal<T, N - 1> { };

template<typename T, unsigned N>
struct ExtentInternal<T[], N> : public ExtentInternal<T, N - 1> {};

template<typename T, unsigned N = 0>
struct Extent : public ExtentInternal<T, N> { };

template<typename T, unsigned N = 0> 
constexpr auto ExtentV = Extent<T, N>::Value;


/**
 * @struct
 * IsArray
 *
 * @tparam T
 */
template<typename T>
struct IsArray : public FalseType {};
template<typename T>
struct IsArray<T[]> : public TrueType {};
template<typename T, sizeT N>
struct IsArray<T[N]> : public TrueType {};
template<class T>
constexpr bool IsArrayV = IsArray<T>::Value;


/**
 * @struct
 * IsArrayOfKnownBounds
 *
 * @tparam T
 */
template<typename T>
struct IsArrayOfKnownBounds : public IntegralConstant<bool, Extent<T>::Value != 0> {};


/**
 * @struct
 * IsArrayOfUnknownBounds
 *
 * @tparam T
 */
template<typename T>
struct IsArrayOfUnknownBounds : public IntegralConstant<bool, IsArray<T>::Value && (Extent<T>::Value == 0)> {};


/**
 * @struct
 * IsMemFunPointer
 * @tparam T
 */
template<typename T>
struct IsMemFunPointerValue : public FalseType {};

template<typename R, typename T>
struct IsMemFunPointerValue<R (T::*)()> : public TrueType {};
template<typename R, typename T>
struct IsMemFunPointerValue<R (T::*)() const> : public TrueType {};
template<typename R, typename T>
struct IsMemFunPointerValue<R (T::*)() volatile> : public TrueType {};
template<typename R, typename T>
struct IsMemFunPointerValue<R (T::*)() const volatile> : public TrueType {};

template<typename R, typename T, typename... TArgs>
struct IsMemFunPointerValue<R (T::*)(TArgs...)> : public TrueType {};
template<typename R, typename T, typename... TArgs>
struct IsMemFunPointerValue<R (T::*)(TArgs...) const> : public TrueType {};
template<typename R, typename T, typename... TArgs>
struct IsMemFunPointerValue<R (T::*)(TArgs...) volatile> : public TrueType {};
template<typename R, typename T, typename... TArgs>
struct IsMemFunPointerValue<R (T::*)(TArgs...) const volatile> : public TrueType {};

template<typename T>
struct IsMemberFunctionPointer : public IntegralConstant<bool, IsMemFunPointerValue<T>::Value> {};
template<typename T>
constexpr bool IsMemFunPointerV = IsMemberFunctionPointer<T>::Value;


/**
 * @struct
 * IsMemberPointer
 * @tparam T
 */
template<typename T>
struct IsMemberPointer : public IntegralConstant<bool, IsMemberFunctionPointer<T>::Value> {};
template<typename T, typename U>
struct IsMemberPointer<U T::*> : public TrueType {};
template<typename T>
constexpr bool IsMemberPointerV = IsMemberPointer<T>::Value;


/**
 * @struct
 * IsMemberObjectPointer
 *
 * @tparam T
 */
template<typename T>
struct IsMemberObjectPointer : public IntegralConstant<bool, IsMemberPointerV<T> && !IsMemFunPointerV<T>> {};
template<typename T>
constexpr bool IsMemberObjectPointerV = IsMemberObjectPointer<T>::Value;


template<typename T>
struct IsPointerInternal : public FalseType {};
template<typename T>
struct IsPointerInternal<T*> : public TrueType {};
template<typename T>
struct IsPointerInternal<T* const> : public TrueType {};
template<typename T>
struct IsPointerInternal<T* volatile> : public TrueType {};
template<typename T>
struct IsPointerInternal<T* const volatile> : public TrueType {};
template<typename T>
struct IsPointerValue : public TypeAnd<IsPointerInternal<T>::Value, TypeNot<IsMemberPointer<T>::Value>::Value> {};
template<typename T>
struct IsPointer : public IntegralConstant<bool, IsPointerValue<T>::Value> {};
template<typename T>
constexpr bool IsPointerV = IsPointer<T>::Value;


/**
 * @struct
 * IsConvertible
 */
#if (defined(_MSC_VER) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_union)))
template<typename TFrom, typename TTo>
struct IsConvertible : public IntegralConstant<bool, __is_convertible_to(TFrom, TTo)> {};
#else
template<typename TFrom, typename TTo, bool = IsVoid<TFrom>::Value || IsFunction<TTo>::Value || IsArray<TTo>::Value >
struct IsConvertibleHelper // Anything is convertible to void. Nothing is convertible to a function or an array.
{ static const bool Value = IsVoid<TTo>::Value; };

template<typename TFrom, typename TTo>
class IsConvertibleHelper<TFrom, TTo, false>
{
  template<typename To1>
  static void ToFunction(To1);    // We try to call this function with an instance of From. It is valid if From can be converted to To.

  template<typename /*From1*/, typename /*To1*/>
  static NoType Is(...);

  template<typename From1, typename To1>
  static decltype(ToFunction<To1>(DeclVal<From1>()), YesType()) Is(int);

public:
  static const bool Value = sizeof(Is<TFrom, TTo>(0)) == 1;
};

template<typename TFrom, typename TTo>
struct IsConvertible : public IntegralConstant<bool, IsConvertibleHelper<TFrom, TTo>::Value> {};
#endif
template<typename TFrom, typename TTo>
constexpr bool IsConvertibleT = IsConvertible<TFrom, TTo>::Value;


/**
 * @struct
 * IsUnion
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_union)))
template<typename T>
struct IsUnion : public IntegralConstant<bool, __is_union(T)> {};
#else

#endif
template<typename T>
constexpr bool IsUnionV = IsUnion<T>::ValueType();


/**
 * @struct
 * IsClass
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_class)))
template<typename T>
struct IsClass : public IntegralConstant<bool, __is_class(T)> {};
#else

#endif
template<typename T>
constexpr bool IsClassV = IsClass<T>::Value;


/**
 * @struct
 * IsEnum
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_enum)))
template<typename T>
struct IsEnum : public IntegralConstant<bool, __is_enum(T)> {};
#else

#endif
template<typename T>
constexpr bool IsEnumV = IsEnum<T>::Value;



/**
 * @struct
 * IsPolymorphic
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_polymorphic)))
template<typename T>
struct IsPolymorphic : public IntegralConstant<bool, __is_polymorphic(T)> {};
#else

#endif
template<typename T>
constexpr bool IsPolymorphicV = IsPolymorphic<T>::ValueType();


/**
 * @struct
 * IsObject
 *
 * @tparam T
 */
template<typename T>
struct IsObject : public IntegralConstant<bool, !IsReferenceV<T> && !IsVoidV<T> && !IsFunctionV<T>> {};
template<typename T>
constexpr bool IsObjectV = IsObject<T>::Value;


/**
 * @struct
 * IsScalar
 *
 * @tparam T
 */
template<typename T>
struct IsScalar : public IntegralConstant<
  bool,
  IsArithmeticV<T> || IsEnumV<T> || IsPointerV<T> || IsMemberPointerV<T> || IsNullPointerV<T>
> {};
template<typename T>
struct IsScalar<T*> : public TrueType {};
template<typename T>
struct IsScalar<T* const> : public TrueType {};
template<typename T>
struct IsScalar<T* volatile> : public TrueType {};
template<typename T>
struct IsScalar<T* const volatile> : public TrueType {};
template<typename T>
constexpr bool IsScalarV = IsScalar<T>::Value;



/**
 * @struct
 * IsCompound
 *
 * @tparam T
 */
template<typename T>
struct IsCompound : public IntegralConstant<bool, !IsFundamentalV<T>> {};
template<typename T>
constexpr bool IsCompoundV = IsCompound<T>::Value;


/**
 * @struct
 * Decay
 *
 * @tparam T
 */
template<typename T>
struct Decay {
  typedef typename RemoveReference<T>::Type U;

  typedef typename Conditional<
    IsArray<U>::Value,
    typename RemoveExtent<U>::Type*,
    typename Conditional<
      IsFunction<U>::Value,
      typename AddPointer<U>::Type,
      typename RemoveCV<U>::Type
    >::Type
  >::Type Type;
};

template<typename T>
using DecayT = typename Decay<T>::Type;


/**
 * @struct
 * CommonType
 *
 * @tparam T
 */
template<typename... T>
struct CommonType;
template<typename T>
struct CommonType<T> {
  typedef DecayT<T> Type;
};
template<typename T, typename U>
struct CommonType<T, U> {
  typedef DecayT<decltype(true ? DeclVal<T>() : DeclVal<U>())> Type;
};
template<typename T, typename U, typename... V>
struct CommonType<T, U, V...> {
  typedef typename CommonType<typename CommonType<T, U>::Type, V...>::Type Type;
};
template<typename... T>
using CommonTypeT = typename CommonType<T...>::Type;


/**
 * @struct
 * IsFinal
 */
#if RE_COMPILER_HAS_FEATURE(s_final)
template<typename T>
struct IsFinal : public InIntegralConstant<bool __is_final(T)> {};
#else
template<typename T>
struct IsFinal : public FalseType {};
#endif
template<typename T>
constexpr bool IsFinalV = IsFinal<T>::Value;


/**
 * @struct
 * IsAggregate
 */
#if RE_COMPILER_HAS_FEATURE(is_aggregate) || defined(_MSC_VER) && (_MSC_VER >= 1916)
template<typename T>
struct IsAggregate : public IntegralConstant<bool, __is_aggregate(T)> {};
#else
template<typename T>
struct IsAggregate : public FalseType {};
#endif
template<typename T>
constexpr bool IsAggregateV = IsAggregate<T>::Value;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore