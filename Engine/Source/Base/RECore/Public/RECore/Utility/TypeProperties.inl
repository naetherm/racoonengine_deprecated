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
 * UnderlyingType
 * @tparam T
 */
template<typename T>
struct UnderlyingType {
  typedef __underlying_type(T) Type;
};
template<typename T>
using UnderlyingTypeT = typename UnderlyingType<T>::Type;


/**
 * @struct
 * HasUniqueObjectRepresentations
 *
 * @tparam T
 */
template<typename T>
struct HasUniqueObjectRepresentations : public IntegralConstant<bool, __has_unique_object_representations(RemoveCVT<RemoveAllExtentsT<T>>)> {};
template<typename T>
constexpr auto HasUniqueObjectRepresentationsT = HasUniqueObjectRepresentations<T>::Value;


/**
 * @struct
 * IsSigned
 *
 * @tparam T
 */
template<typename T>
struct IsSignedInternal : public FalseType {};
template<> struct IsSignedInternal<signed char> : public TrueType {};
template<> struct IsSignedInternal<signed short> : public TrueType {};
template<> struct IsSignedInternal<signed int> : public TrueType {};
template<> struct IsSignedInternal<signed long> : public TrueType {};
template<> struct IsSignedInternal<signed long long> : public TrueType {};
template<> struct IsSignedInternal<float> : public TrueType {};
template<> struct IsSignedInternal<double> : public TrueType {};
template<> struct IsSignedInternal<long double> : public TrueType {};

template<typename T>
struct IsSigned : public IsSignedInternal<RemoveCVT<T>> {};
template<typename T>
constexpr bool IsSignedV = IsSigned<T>::Value;

#define RE_IS_SIGNED(T) \
template<> struct IsSigned<T> : public TrueType {}; \
template<> struct IsSigned<const T> : public TrueType {}; \
template<> struct IsSigned<volatile T> : public TrueType {}; \
template<> struct IsSigned<const volatile T> : public TrueType {};


/**
 * @struct
 * IsUnsigned
 *
 * @tparam T
 */
template<typename T>
struct IsUnsignedInternal : public FalseType {};
template<> struct IsUnsignedInternal<unsigned char> : public TrueType {};
template<> struct IsUnsignedInternal<unsigned short> : public TrueType {};
template<> struct IsUnsignedInternal<unsigned int> : public TrueType {};
template<> struct IsUnsignedInternal<unsigned long> : public TrueType {};
template<> struct IsUnsignedInternal<unsigned long long> : public TrueType {};

template<typename T>
struct IsUnsigned : public IsUnsignedInternal<RemoveCVT<T>> {};
template<typename T>
constexpr bool IsUnsignedV = IsUnsigned<T>::Value;

#define RE_IS_UNSIGNED(T) \
template<> struct IsUnsigned<T> : public TrueType {}; \
template<> struct IsUnsigned<const T> : public TrueType {}; \
template<> struct IsUnsigned<volatile T> : public TrueType {}; \
template<> struct IsUnsigned<const volatile T> : public TrueType {};


/**
 * @struct
 * AlignmentOf
 *
 * @tparam T
 */
template<typename T>
struct AlignmentOfValue {
  static const sizeT Value = RE_ALIGN_OF(T);
};
template<typename T>
struct AlignmentOf : public IntegralConstant<sizeT, AlignmentOfValue<T>::Value> {};
template<typename T>
constexpr sizeT AlignmentOfV = AlignmentOf<T>::Value;


/**
 * @struct
 * IsAligned
 *
 * @tparam T
 */
template<typename T>
struct IsAlignedValue {
  static const sizeT Value = (RE_ALIGN_OF(T) > 8);
};
template<typename T>
struct IsAligned : public IntegralConstant<sizeT, IsAlignedValue<T>::Value> {};
template <typename T>
constexpr sizeT IsAlignedV = IsAligned<T>::Value;


/**
 * @struct
 * Rank
 *
 * @tparam T
 */
template<typename T>
struct Rank : public IntegralConstant<sizeT, 0> {};
template<typename T>
struct Rank<T[]> : public IntegralConstant<sizeT, Rank<T>::Value + 1> {};
template<typename T, sizeT N>
struct Rank<T[N]> : public IntegralConstant<sizeT, Rank<T>::Value + 1> {};
template<class T>
constexpr auto RankV = Rank<T>::Value;


/**
 * @struct
 * IsBaseOf
 *
 * @tparam TBase
 * @tparam TDerived
 */
template<typename TBase, typename TDerived>
struct IsBaseOf : public IntegralConstant<bool, IsSameV<TBase, TDerived> || __is_base_of(TBase, TDerived)> {};
template<typename TBase, typename TDerived>
constexpr bool IsBaseOfV = IsBaseOf<TBase, TDerived>::Value;


/**
 * @struct
 * IsLValueReference
 *
 * @tparam T
 */
template<typename T>
struct IsLValueReference : public FalseType {};
template<typename T>
struct IsLValueReference<T&> : public TrueType {};
template<typename T>
constexpr bool IsLValueReferenceV = IsLValueReference<T>::Value;


/**
 * @struct
 * IsRValueReference
 *
 * @tparam T
 */
template<typename T>
struct IsRValueReference : public FalseType {};
template<typename T>
struct IsRValueReference<T&&> : public TrueType {};
template<typename T>
constexpr bool IsRValueReferenceV = IsRValueReference<T>::Value;


template<typename>
struct ResultOf;
template<typename F, typename... TArgs>
struct ResultOf<F(TArgs...)> {
  typedef decltype(DeclVal<F>()(DeclVal<TArgs>()...)) Type;
};
template<typename T>
using ResultOfT = typename ResultOf<T>::Type;


template<typename, typename = VoidT<>>
struct HasEquality : public FalseType {};
template<typename T>
struct HasEquality<T, VoidT<decltype(DeclVal<T>() == DeclVal<T>())>> : TrueType {};
template<typename T>
constexpr auto HasEqualityV = HasEquality<T>::Value;

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore