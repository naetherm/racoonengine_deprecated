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
#include "RECore/Utility/Internal/FunctionalBase.h"
#include "RECore/Utility/Move.h"
#include "RECore/Utility/TypeTraits.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

template<typename T = void>
struct Plus : public BinaryFunction<T, T, T> {
  constexpr T operator()(const T& a, const T& b) const {
    return a + b;
  }
};
template<>
struct Plus<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) + Forward<B>(b)) {
    return Forward<A>(a) + Forward<B>(b);
  }
};



template<typename T = void>
struct Minus : public BinaryFunction<T, T, T> {
  constexpr T operator()(const T& a, const T& b) const {
    return a - b;
  }
};
template<>
struct Minus<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) - Forward<B>(b)) {
    return Forward<A>(a) - Forward<B>(b);
  }
};



template<typename T = void>
struct Multiplies : public BinaryFunction<T, T, T> {
  constexpr T operator()(const T& a, const T& b) const {
    return a * b;
  }
};
template<>
struct Multiplies<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) * Forward<B>(b)) {
    return Forward<A>(a) * Forward<B>(b);
  }
};



template<typename T = void>
struct Divides : public BinaryFunction<T, T, T> {
  constexpr T operator()(const T& a, const T& b) const {
    return a / b;
  }
};
template<>
struct Divides<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) / Forward<B>(b)) {
    return Forward<A>(a) / Forward<B>(b);
  }
};



template<typename T = void>
struct Modulus : public BinaryFunction<T, T, T> {
  constexpr T operator()(const T& a, const T& b) const {
    return a % b;
  }
};
template<>
struct Modulus<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) % Forward<B>(b)) {
    return Forward<A>(a) % Forward<B>(b);
  }
};



template<typename T = void>
struct Negate : public UnaryFunction<T, T> {
  constexpr T operator()(const T& a) const {
    return -a;
  }
};
template<>
struct Negate<void> {
  typedef int IsTransparent;
  template<typename A>
  constexpr auto operator()(A&& a) const -> decltype(-Forward<A>(a)) {
    return -Forward<A>(a);
  }
};



template<typename T = void>
struct EqualTo : public BinaryFunction<T, T, bool> {
  constexpr bool operator()(const T& a, const T& b) const {
    return a == b;
  }
};
template<>
struct EqualTo<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) == Forward<B>(b)) {
    return Forward<A>(a) == Forward<B>(b);
  }
};



template<typename T = void>
struct NotEqualTo : public BinaryFunction<T, T, bool> {
  constexpr bool operator()(const T& a, const T& b) const {
    return a != b;
  }
};
template<>
struct NotEqualTo<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) != Forward<B>(b)) {
    return Forward<A>(a) != Forward<B>(b);
  }
};



template<typename T = void>
struct Greater : public BinaryFunction<T, T, bool> {
  constexpr bool operator()(const T& a, const T& b) const {
    return a > b;
  }
};
template<>
struct Greater<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) > Forward<B>(b)) {
    return Forward<A>(a) > Forward<B>(b);
  }
};



template<typename T = void>
struct GreaterEqual : public BinaryFunction<T, T, bool> {
  constexpr T operator()(const T& a, const T& b) const {
    return a >= b;
  }
};
template<>
struct GreaterEqual<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) >= Forward<B>(b)) {
    return Forward<A>(a) >= Forward<B>(b);
  }
};



template<typename T = void>
struct Less : public BinaryFunction<T, T, bool> {
  constexpr bool operator()(const T& a, const T& b) const {
    return a < b;
  }
};
template<>
struct Less<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) < Forward<B>(b)) {
    return Forward<A>(a) < Forward<B>(b);
  }
};



template<typename T = void>
struct LessEqual : public BinaryFunction<T, T, bool> {
  constexpr bool operator()(const T& a, const T& b) const {
    return a <= b;
  }
};
template<>
struct LessEqual<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) <= Forward<B>(b)) {
    return Forward<A>(a) <= Forward<B>(b);
  }
};



template<typename T = void>
struct LogicalAnd : public BinaryFunction<T, T, bool> {
  constexpr bool operator()(const T& a, const T& b) const {
    return a && b;
  }
};
template<>
struct LogicalAnd<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) && Forward<B>(b)) {
    return Forward<A>(a) && Forward<B>(b);
  }
};



template<typename T = void>
struct LogicalOr : public BinaryFunction<T, T, bool> {
  constexpr bool operator()(const T& a, const T& b) const {
    return a || b;
  }
};
template<>
struct LogicalOr<void> {
  typedef int IsTransparent;
  template<typename A, typename B>
  constexpr auto operator()(A&& a, B&& b) const -> decltype(Forward<A>(a) || Forward<B>(b)) {
    return Forward<A>(a) || Forward<B>(b);
  }
};



template<typename T = void>
struct LogicalNot : public UnaryFunction<T, bool> {
  constexpr bool operator()(const T& a) const {
    return !a;
  }
};
template<>
struct LogicalNot<void> {
  typedef int IsTransparent;
  template<typename A>
  constexpr auto operator()(A&& a) const -> decltype(!Forward<A>(a)) {
    return !Forward<A>(a);
  }
};




//
// StrEqualTo
//
template<typename T>
struct StrEqualTo : public BinaryFunction<T, T, bool> {
  constexpr bool operator()(T cA, T cB) const {
    while (*cA && (*cA == *cB)) {
      ++cA; ++cB;
    }
    return (*cA == *cB);
  }
};



//
// Hash
//
template<typename T, bool Enabled>
struct EnableHashIf {};
template<typename T>
struct EnableHashIf<T, true> {
  sizeT operator()(const T& p) const {
    return sizeT(p);
  }
};

template<typename T>
struct Hash;

template<typename T>
struct Hash : EnableHashIf<T, IsEnumV<T>> {
  sizeT operator()(T p) const {
    return sizeT(p);
  }
};
template<>
struct Hash<bool> {
  sizeT operator()(bool value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<uint8> {
  sizeT operator()(uint8 value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<uint16> {
  sizeT operator()(uint16 value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<uint32> {
  sizeT operator()(uint32 value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<uint64> {
  sizeT operator()(uint64 value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<int8> {
  sizeT operator()(int8 value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<int16> {
  sizeT operator()(int16 value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<int32> {
  sizeT operator()(int32 value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<int64> {
  sizeT operator()(int64 value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<float> {
  sizeT operator()(float value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<double> {
  sizeT operator()(double value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<char> {
  sizeT operator()(char value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<char16_t> {
  sizeT operator()(char16_t value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<char32_t> {
  sizeT operator()(char32_t value) const { return static_cast<sizeT>(value); }
};
template<>
struct Hash<wchar_t> {
  sizeT operator()(wchar_t value) const { return static_cast<sizeT>(value); }
};

template<>
struct Hash<char *> {
  sizeT operator()(const char *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct Hash<wchar_t *> {
  sizeT operator()(const wchar_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct Hash<char16_t *> {
  sizeT operator()(const char16_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct Hash<char32_t *> {
  sizeT operator()(const char32_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct Hash<const char *> {
  sizeT operator()(const char *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct Hash<const wchar_t *> {
  sizeT operator()(const wchar_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct Hash<const char16_t *> {
  sizeT operator()(const char16_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};
template<>
struct Hash<const char32_t *> {
  sizeT operator()(const char32_t *p) const {
    sizeT c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32 instead of sizeT, so the behavior is the same regardless of size.
    while ((c = (uint8) *p++) != 0)     // Using '!=' disables compiler warnings.
      result = (result * 16777619) ^ c;
    return (sizeT) result;
  }
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore