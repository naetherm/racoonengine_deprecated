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
 * Empty
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_empty)))
template<typename T>
struct IsEmpty : public IntegralConstant<bool, __is_empty(T)> {};
#else

#endif
template<typename T>
constexpr bool IsEmptyV = IsEmpty<T>::Value;


/**
 * @struct
 * IsPod
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_pod)))
template<typename T>
struct IsPod : public IntegralConstant<bool, __is_pod(T) || IsVoid<T>::Value || IsScalar<T>::Value> {};
template<typename T, sizeT N>
struct IsPod<T[N]> : public IsPod<T> {};
#else

#endif
template<typename T>
constexpr bool IsPodV = IsPod<T>::Value;

#define RE_IS_POD(T, IS_POD) \
template<> struct IsPod<T> : public IntegralConstant<bool, IS_POD> {}; \
template<> struct IsPod<const T> : public IntegralConstant<bool, IS_POD> {}; \
template<> struct IsPod<volatile T> : public IntegralConstant<bool, IS_POD> {}; \
template<> struct IsPod<const volatile T> : public IntegralConstant<bool, IS_POD> {};


/**
 * @struct
 * IsStandardLayout
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(has_trivial_constructor)))
template<typename T>
struct IsStandardLayout : public IntegralConstant<bool, __is_standard_layout(T) || IsVoid<T>::Value || IsScalar<T>::Value> {};
#else

#endif
template<class T>
constexpr bool IsStandardLayoutV = IsStandardLayout<T>::Value;

#define RE_IS_STANDARD_LAYOUT(T, IS_STD_LAYOUT) \
template<> struct IsStandardLayout<T> : public IntegralConstant<bool, IS_STD_LAYOUT> {}; \
template<> struct IsStandardLayout<const T> : public IntegralConstant<bool, IS_STD_LAYOUT> {}; \
template<> struct IsStandardLayout<volatile T> : public IntegralConstant<bool, IS_STD_LAYOUT> {}; \
template<> struct IsStandardLayout<const volatile T> : public IntegralConstant<bool, IS_STD_LAYOUT> {};

/**
 * @struct
 * HasTrivialConstructor
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(has_trivial_constructor)))
template<typename T>
struct HasTrivialConstructor : public IntegralConstant<bool, __has_trivial_constructor(T) || IsPodV<T>> {};
#else

#endif


/**
 * @struct
 * HasTrivialCopy
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(has_trivial_copy)))
template<typename T>
struct HasTrivialCopy : public IntegralConstant<bool, (__has_trivial_copy(T) || IsPodV<T>) && (!IsVolatileV<T> && !IsReferenceV<T>)> {};
#else

#endif


/**
 * @struct
 * HasTrivialAssign
 *
 * @tparam T
 */
template<typename T>
struct HasTrivialAssign : public IntegralConstant<bool, (__has_trivial_assign(T) || IsPodV<T>) && !IsConstV<T> && !IsVolatileV<T>> {};


/**
 * @struct
 * HasTrivialDestructor
 *
 * @tparam T
 */
template<typename T>
struct HasTrivialDestructor : public IntegralConstant<bool, (__has_trivial_destructor(T) || IsPodV<T>)> {};



/**
 * @struct
 * HasTrivialRelocate
 *
 * @tparam T
 */
template<typename T>
struct HasTrivialRelocate : public BoolConstant<IsPodV<T> && !IsVolatileV<T>> {};


/**
 * @struct
 * HasNothrowConstructor
 */
#if (defined(RE_COMPILER_CLANG) || defined(RE_COMPILER_GNUC))
template<typename T>
struct HasNothrowConstructor : public IntegralConstant<bool, __has_nothrow_constructor(T)> {};
#elif defined(_MSC_VER)
template<typename T>
struct HasNothrowConstructor : public IntegralConstant<bool, __HasNothrowConstructor(T) || IsScalar<typename RemoveAllExtents<T>::Type>::Value || IsReference<T>::Value> {};
#else
template<typename T>
struct HasNothrowConstructor : public IntegralConstant<bool, IsScalar<typename RemoveAllExtents<T>::Type>::Value || IsReference<T>::Value> {};
#endif


/**
 * @struct
 * HasNothrowCopy
 */
#if (defined(RE_COMPILER_CLANG) || defined(RE_COMPILER_GNUC))
template<typename T>
struct HasNothrowCopy : public IntegralConstant<bool, __has_nothrow_copy(T)> {};
#elif defined(_MSC_VER)
template<typename T>
struct HasNothrowCopy : public IntegralConstant<bool, __HasNothrowCopy(T) || IsScalar<typename RemoveAllExtents<T>::Type>::Value || IsReference<T>::Value> {};
#else
template<typename T>
struct HasNothrowCopy : public IntegralConstant<bool, IsScalar<typename RemoveAllExtents<T>::Type>::Value || IsReference<T>::Value> {};
#endif


#if (defined(RE_COMPILER_CLANG) || defined(RE_COMPILER_GNUC))
template<typename T>
struct HasNothrowAssign : public IntegralConstant<bool, __has_nothrow_assign(T)> {};
#elif defined(_MSC_VER)
template<typename T>
struct HasNothrowAssign : public IntegralConstant<bool, __has_nothrow_assign(T) || IsScalar<typename RemoveAllExtents<T>::Type>::Value || IsReference<T>::Value> {};
#else
template<typename T>
struct HasNothrowAssign : public IntegralConstant<bool, IsScalar<typename RemoveAllExtents<T>::Type>::Value || IsReference<T>::Value> {};
#endif


#if (defined(_MSC_VER) || defined(RE_COMPILER_CLANG) || defined(RE_COMPILER_GNUC))
template<typename T>
struct HasVirtualDestructor : public IntegralConstant<bool, __has_virtual_destructor(T)> {};
#else
template<typename T>
struct HasVirtualDestructor : public FalseType {};
#endif
template<typename T>
constexpr bool HasVirtualDestructorV = HasVirtualDestructor<T>::Value;


/**
 * @struct
 * IsLiteralType
 */
#if (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_literal))
template<typename T>
struct IsLiteralType : public IntegralConstant<bool, __is_literal(T)> {};
#elif defined(RE_COMPILER_GNUC)
template<typename T>
struct IsLiteralType : public IntegralConstant<bool, __is_literal_type(T)> {};
#else
template<typename T>
struct IsLiteralType : public IntegralConstant<bool, IsScalar<typename RemoveReference<typename RemoveAllExtents<T>::Type>::Type>::Value> {};
#endif
template<typename T>
constexpr bool IsLiteralTypeV = IsLiteralType<T>::Value;


/**
 * @struct
 * IsAbstract
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_abstract)))
template<typename T>
struct IsAbstract : public IntegralConstant<bool, __is_abstract(T)> {};
#else

#endif
template<class T>
constexpr bool IsAbstractV = IsAbstract<T>::Value;



/**
 * @struct
 * IsTriviallyCopyable
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_trivially_copyable)))
template<typename T>
struct IsTriviallyCopyable : public IntegralConstant<bool, __is_trivially_copyable(T)> {};
#else

#endif
template<class T>
constexpr bool IsTriviallyCopyableV = IsTriviallyCopyable<T>::Value;


/**
 * @struct
 * IsConstructible
 */
#if (defined(_MSC_VER) || defined(RE_COMPILER_GNUC) || (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(is_constructible)))
template<typename T, typename... TArgs>
struct IsConstructible : public BoolConstant<__is_constructible(T, TArgs...)> {};
#else

#endif
template<class T, class... TArgs>
constexpr bool IsConstructibleV = IsConstructible<T, TArgs...>::Value;


template <typename T>
struct IsDestructible
  : public RECore::IntegralConstant<bool, !RECore::IsArrayOfUnknownBounds<T>::Value &&
                                          !RECore::IsVoid<T>::Value                    &&
                                          !RECore::IsFunction<T>::Value                &&
                                          !RECore::IsAbstract<T>::Value> {};
template <class T>
constexpr bool IsDestructibleV = IsDestructible<T>::Value;

template<typename T>
struct IsTriviallyDestructible : public RECore::IntegralConstant<bool, RECore::IsDestructible<T>::Value && ((__has_trivial_destructor(T) && !RECore::IsHatType<T>::Value) || RECore::IsScalar<typename RECore::RemoveAllExtents<T>::Type>::Value)> {};

template<class T>
constexpr bool IsTriviallyDestructibleV = IsTriviallyDestructible<T>::Value;


/**
 * @struct
 * IsTriviallyConstructible
 */
#if (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(IsTriviallyConstructible))
template<typename T, typename... TArgs>
struct IsTriviallyConstructible : public IntegralConstant<bool, IsConstructible<T, TArgs...>::Value & __IsTriviallyConstructible(T, TArgs...)> {};
#else
template <typename T, typename... TArgs>
struct IsTriviallyConstructible : public FalseType {};

template <typename T>
struct IsTriviallyConstructible<T> : public IntegralConstant<bool, IsConstructible<T>::Value && HasTrivialConstructor<typename RemoveAllExtents<T>::Type>::Value> {};

// It's questionable whether we can use has_trivial_copy here, as it could theoretically Create a false-positive.
template <typename T>
struct IsTriviallyConstructible<T, T> : public IntegralConstant<bool, IsConstructible<T>::Value && HasTrivialCopy<T>::Value> {};

template <typename T>
struct IsTriviallyConstructible<T, T&&> : public IntegralConstant<bool, IsConstructible<T>::Value && HasTrivialCopy<T>::Value> {};

template <typename T>
struct IsTriviallyConstructible<T, T&> : public IntegralConstant<bool, IsConstructible<T>::Value && HasTrivialCopy<T>::Value> {};

template <typename T>
struct IsTriviallyConstructible<T, const T&> : public IntegralConstant<bool, IsConstructible<T>::Value && HasTrivialCopy<T>::Value> {};

template <typename T>
struct IsTriviallyConstructible<T, volatile T&> : public IntegralConstant<bool, IsConstructible<T>::Value && HasTrivialCopy<T>::Value> {};

template <typename T>
struct IsTriviallyConstructible<T, const volatile T&> : public IntegralConstant<bool, IsConstructible<T>::Value && HasTrivialCopy<T>::Value> {};
#endif
template<class T>
  constexpr bool IsTriviallyConstructibleV = IsTriviallyConstructible<T>::Value;

#define RE_IS_TRIVIALLY_CONSTRUCTIBLE(T, IS_) \
namespace RECore { \
  template<> struct IsTriviallyConstructible<T> : public IntegralConstant<bool, IS_> {}; \
  template<> struct IsTriviallyConstructible<const T> : public IntegralConstant<bool, IS_> {}; \
  template<> struct IsTriviallyConstructible<volatile T> : public IntegralConstant<bool, IS_> {}; \
  template<> struct IsTriviallyConstructible<const volatile T> : public IntegralConstant<bool, IS_> {}; \
}


template<typename T>
struct IsTriviallyDefaultConstructible : public IsTriviallyConstructible<T> {};


template<typename T>
struct IsTrivial : public IntegralConstant<bool, IsTriviallyCopyable<T>::Value && IsTriviallyDefaultConstructible<T>::Value> {};


template <typename T, typename... Args>
struct IsNothrowConstructible
  : public RECore::FalseType {};

template <typename T>
struct IsNothrowConstructible<T>
  : public RECore::IntegralConstant<bool, RECore::HasNothrowConstructor<T>::Value> {};

template <typename T>
struct IsNothrowConstructible<T, T>
  : public RECore::IntegralConstant<bool, RECore::HasNothrowCopy<T>::Value> {};

template <typename T>
struct IsNothrowConstructible<T, const T&>
  : public RECore::IntegralConstant<bool, RECore::HasNothrowCopy<T>::Value> {};

template <typename T>
struct IsNothrowConstructible<T, T&>
  : public RECore::IntegralConstant<bool, RECore::HasNothrowCopy<T>::Value> {};

template <typename T>
struct IsNothrowConstructible<T, T&&>
  : public RECore::IntegralConstant<bool, RECore::HasNothrowCopy<T>::Value> {};


template<typename T>
struct IsDefaultConstructible : public IsConstructible<T> {};
template<typename T>
constexpr bool IsDefaultConstructibleV = IsDefaultConstructible<T>::Value;



template<typename T>
struct IsCopyConstructible : public IsConstructible<T, typename AddLValueReference<typename AddConst<T>::Type>::Type> {};
template<typename T>
constexpr bool IsCopyConstructibleV = IsCopyConstructible<T>::Value;


template <typename T>
struct IsTriviallyCopyConstructible : public RECore::IsTriviallyConstructible<T, typename RECore::AddLValueReference<typename RECore::AddConst<T>::Type>::Type> {};
template<typename T>
constexpr bool IsTriviallyCopyConstructibleV = IsTriviallyCopyConstructible<T>::Value;



template <typename T>
struct IsNothrowCopyConstructible : public IsNothrowConstructible<T, typename RECore::AddLValueReference<typename RECore::AddConst<T>::Type>::Type> {};

template <class T>
constexpr bool IsNothrowCopyConstructible_v = IsNothrowCopyConstructible<T>::Value;


template <typename T>
struct IsTriviallyMoveConstructible : public RECore::IsTriviallyConstructible<T, typename RECore::AddRValueReference<typename RECore::AddConst<T>::Type>::Type> {};
template<typename T>
constexpr bool IsTriviallyMoveConstructibleV = IsTriviallyMoveConstructible<T>::Value;


/**
 * IsAssignable
 * @tparam T
 * @tparam U
 */
template<typename T, typename U>
struct IsAssignableInternal {
  template<typename, typename>
  static RECore::NoType Is(...);

  template<typename T1, typename U1>
  static decltype(RECore::DeclVal<T1>() = DeclVal<U1>(), YesType()) Is(int);

  static const bool Value = (sizeof(Is<T, U>(0)) == sizeof(YesType));
};

template<typename T, typename U>
struct IsAssignable : public RECore::IntegralConstant<bool, IsAssignableInternal<T, U>::Value> {};


/**
 * IsLValueAssignable
 * @tparam T
 * @tparam U
 */
template<typename T, typename U>
struct IsLValueAssignable : public IsAssignable<
  typename AddLValueReference<T>::Type,
  typename AddLValueReference<typename AddConst<U>::Type>::Type> {};


#if (defined(RE_COMPILER_CLANG) && RE_COMPILER_HAS_FEATURE(IsTriviallyAssignable))
template<typename T, typename U>
struct IsTriviallyAssignable : IntegralConstant<bool, __IsTriviallyAssignable(T, U)> {};
#else
template <typename T, typename U>
struct IsTriviallyAssignable : IntegralConstant<bool, IsAssignable<T, U>::Value && (IsPod<typename RemoveReference<T>::Type>::Value || __has_trivial_assign(typename RemoveReference<T>::Type))> {};
#endif


template <bool, typename T, typename U>
struct IsNothrowAssignable_helper;

template <typename T, typename U>
struct IsNothrowAssignable_helper<false, T, U>
  : public FalseType {};

template <typename T, typename U>
struct IsNothrowAssignable_helper<true, T, U> // Set to true if the assignment (same as is_assignable) cannot generate an exception.
  : public RECore::IntegralConstant<bool, noexcept(RECore::DeclVal<T>() = RECore::DeclVal<U>()) >
{
};

template <typename T, typename U>
struct IsNothrowAssignable
  : public RECore::IsNothrowAssignable_helper<RECore::IsAssignable<T, U>::value, T, U>
{
};
template <class T, class U>
constexpr bool IsNothrowAssignable_v = IsNothrowAssignable<T, U>::value;


template <typename T>
struct IsCopyAssignable : public IsAssignable<typename AddLValueReference<T>::Type, typename AddLValueReference<typename AddConst<T>::Type>::Type> {};



template <typename T>
struct IsTriviallyCopyAssignable
  : public RECore::IsTriviallyAssignable<typename RECore::AddLValueReference<T>::Type,
    typename RECore::AddLValueReference<typename RECore::AddConst<T>::Type>::Type> {};
template <class T>
constexpr bool IsTriviallyCopyAssignableV = IsTriviallyCopyAssignable<T>::Value;


template <typename T>
struct IsNothrowCopyAssignable
  : public RECore::IsNothrowAssignable<typename RECore::AddLValueReference<T>::Type,
    typename RECore::AddLValueReference<typename RECore::AddConst<T>::type>::Type> {};
template <class T>
constexpr bool IsNothrowCopyAssignableV = IsNothrowCopyAssignable<T>::Value;


template <typename T>
struct IsMoveAssignable : public IsAssignable<typename AddLValueReference<T>::Type, typename AddRValueReference<T>::Type> {};
template <class T>
constexpr bool IsMoveAssignableV = IsMoveAssignable<T>::value;


template <typename T>
struct IsTriviallyMoveAssignable : public IsTriviallyAssignable<typename AddLValueReference<T>::Type, typename AddRValueReference<T>::Type> {};
template <class T>
constexpr bool IsTriviallyMoveAssignableV = IsTriviallyMoveAssignable<T>::Value;


template <typename T>
struct IsNothrowMoveAssignable
  : public RECore::IsNothrowAssignable<typename RECore::AddLValueReference<T>::Type,
    typename RECore::AddRValueReference<T>::type> {};
template <class T>
constexpr bool IsNothrowMoveAssignableV = IsNothrowMoveAssignable<T>::value;


template <typename T>
struct IsNothrowDestructibleHelper
  : public RECore::IntegralConstant<bool, RECore::IsScalar<T>::Value || RECore::IsReference<T>::Value> {};

template <typename T>
struct IsNothrowDestructible
  : public RECore::IsNothrowDestructibleHelper<typename RECore::RemoveAllExtents<T>::Type> {};


template <typename T>
struct IsNothrowDefaultConstructible
  : public RECore::IsNothrowConstructible<T> {};
template <class T>
constexpr bool IsNothrowDefaultConstructibleV = IsNothrowDefaultConstructible<T>::Value;


template <typename T>
struct IsNothrowMoveConstructible
  : public RECore::IsNothrowConstructible<T, typename RECore::AddRValueReference<T>::Type> {};
template <class T>
constexpr bool IsNothrowMoveConstructibleV = IsNothrowMoveConstructible<T>::Value;

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore