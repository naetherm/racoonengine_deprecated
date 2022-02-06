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
 * IsVoid
 *
 * @tparam T
 */
template<typename T>
struct IsVoid : public FalseType {};
template<>
struct IsVoid<void> : public TrueType {};
template<>
struct IsVoid<void const> : public TrueType {};
template<>
struct IsVoid<void volatile> : public TrueType {};
template<>
struct IsVoid<void const volatile> : public TrueType {};
template<class T>
constexpr bool IsVoidV = IsVoid<T>::Value;

/**
 *
 * @tparam TArgs
 */
template<typename... TArgs>
struct HasVoidArg;
template<>
struct HasVoidArg<> : FalseType {};
template <typename A0, typename...TArgs>
struct HasVoidArg<A0, TArgs...> {
  static const bool Value = (IsVoid<A0>::Value || HasVoidArg<TArgs...>::Value);
};


/**
 * @struct
 * IsNullPointer
 */
#if !defined(_MSC_VER)
template<typename T>
struct IsNullPointer : public IsSame<typename RemoveCV<T>::Type, decltype(nullptr)> {};
#else
template<typename T>
struct IsNullPointer : public IsSame<typename RemoveCV<T>::Type, nullptr_t> {};
#endif
template<class T>
constexpr bool IsNullPointerV = IsNullPointer<T>::Value;


template<typename T>
struct IsNullPtrT : RECore::FalseType {};
template<>
struct IsNullPtrT<std::nullptr_t> : RECore::TrueType {};


/**
 * @struct
 * IsIntegral
 *
 * @tparam T
 */
template<typename T>
struct IsIntegralInternal : public FalseType {};
template<> struct IsIntegralInternal<unsigned char> : public TrueType {};
template<> struct IsIntegralInternal<unsigned short> : public TrueType {};
template<> struct IsIntegralInternal<unsigned int> : public TrueType {};
template<> struct IsIntegralInternal<unsigned long> : public TrueType {};
template<> struct IsIntegralInternal<unsigned long long> : public TrueType {};
template<> struct IsIntegralInternal<signed char> : public TrueType {};
template<> struct IsIntegralInternal<signed short> : public TrueType {};
template<> struct IsIntegralInternal<signed int> : public TrueType {};
template<> struct IsIntegralInternal<signed long> : public TrueType {};
template<> struct IsIntegralInternal<signed long long> : public TrueType {};
template<> struct IsIntegralInternal<bool> : public TrueType {};
template<> struct IsIntegralInternal<char> : public TrueType {};

template<typename T>
struct IsIntegral : public IsIntegralInternal<typename RemoveCV<T>::Type> {};

template<typename T>
constexpr bool IsIntegralV = IsIntegral<T>::Value;

#define RE_IS_INTEGRAL(T) \
template<> struct IsIntegral<T> : public TrueType {}; \
template<> struct IsIntegral<const T> : public TrueType {}; \
template<> struct IsIntegral<volatile T> : public TrueType {}; \
template<> struct IsIntegral<const volatile T> : public TrueType {};



template<typename TFrom, typename TTo>
using IsInteger = RECore::IntegralConstant<
  bool,
  !RECore::IsSame<TFrom, TTo>::Value &&
  RECore::IsIntegral<TFrom>::Value && RECore::IsIntegral<TTo>::Value>;


/**
 * @struct
 * IsFloatingPoint
 *
 * @tparam T
 */
template<typename T>
struct IsFloatingPointInternal : public FalseType {};
template<> struct IsFloatingPointInternal<float> : public TrueType {};
template<> struct IsFloatingPointInternal<double> : public TrueType {};
template<> struct IsFloatingPointInternal<long double> : public TrueType {};

template<typename T>
struct IsFloatingPoint : public IsFloatingPointInternal<typename RemoveCV<T>::Type> {};

template<typename T>
constexpr bool IsFloatingPointV = IsFloatingPoint<T>::Value;

#define RE_IS_FLOATING_POINT(T) \
template<> struct IsFloatingPoint<T> : public TrueType {}; \
template<> struct IsFloatingPoint<const T> : public TrueType {}; \
template<> struct IsFloatingPoint<volatile T> : public TrueType {}; \
template<> struct IsFloatingPoint<const volatile T> : public TrueType {};


/**
 * @struct
 * IsArithmetic
 *
 * @tparam T
 */
template<typename T>
struct IsArithmetic : public IntegralConstant<bool, IsIntegralV<T> || IsFloatingPointV<T>> {};

template<typename T>
constexpr bool IsArithmeticV = IsArithmetic<T>::Value;


// SignedIntType
template<int TNumBytes>
struct SignedIntType {};
template<> struct SignedIntType<1> { using Type = int8; };
template<> struct SignedIntType<2> { using Type = int16; };
template<> struct SignedIntType<4> { using Type = int32; };
template<> struct SignedIntType<8> { using Type = int64; };
template<int TNumBytes>
using SignedIntTypeT = typename SignedIntType<TNumBytes>::Type;

// UnsignedIntType
template<int TNumBytes>
struct UnsignedIntType {};
template<> struct UnsignedIntType<1> { using Type = uint8; };
template<> struct UnsignedIntType<2> { using Type = uint16; };
template<> struct UnsignedIntType<4> { using Type = uint32; };
template<> struct UnsignedIntType<8> { using Type = uint64; };
template<int TNumBytes>
using UnsignedIntTypeT = typename UnsignedIntType<TNumBytes>::Type;


/**
 * @struct
 * IsFundamental
 *
 * @tparam T
 */
template<typename T>
struct IsFundamental : public BoolConstant<IsVoidV<T> || IsIntegralV<T> || IsFloatingPointV<T> || IsNullPointerV<T>> {};

template<typename T>
constexpr bool IsFundamentalV = IsFundamental<T>::Value;


/**
 * @struct
 * IsHatType
 * @tparam T
 */
template<typename T>
struct IsHatTypeInternal : public FalseType {};

template<typename T>
struct IsHatType : public IsHatTypeInternal<T> {};

template<typename T>
constexpr bool IsHatTypeV = IsHatType<T>::Value;

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore