
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
#include "RECore/Utility/Functional.h"
#include "RECore/Utility/TypeTraits.h"
#include "RECore/Utility/Move.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename T1, typename T2>
class Pair {
public:

  typedef T1 FirstType;
  typedef T2 SecondType;
  typedef Pair<T1, T2> ThisType;

public:

  T1 First;
  T2 Second;

public:

  template<
    typename TT1 = T1,
    typename TT2 = T2,
    class = EnableIfT < IsDefaultConstructibleV<TT1> && IsDefaultConstructibleV<TT2>>>

  constexpr Pair()
    : First(), Second() {

  }

  template<
    typename TT1 = T1,
    typename TT2 = T2,
    class = EnableIfT <IsDefaultConstructibleV<TT2>>>
  constexpr Pair(const TT1 &a)
    : First(a), Second() {

  }

  template<
    typename TT2 = T2,
    class = EnableIfT <IsDefaultConstructibleV<TT2>>>
  constexpr Pair(T1 &a)
    : First(RECore::Move(a)), Second() {

  }

  template<
    typename U,
    typename V,
    typename = EnableIfT<IsConvertibleT<const U &, T1> && IsConvertibleT<const V &, T2>>>

  constexpr Pair(const Pair<U, V> &cSource)
    : First(cSource.First), Second(cSource.Second) {

  }

  template<
    typename U,
    typename V,
    typename =
  EnableIfT<IsConvertibleT < U, T1> &&IsConvertibleT<V, T2>>
  >

  constexpr Pair(Pair<U, V> &&cSource)
    : First(RECore::Forward<U>(cSource.First)), Second(RECore::Forward<V>(cSource.Second)) {

  }

  template<
    typename U,
    typename V,
    typename =
  EnableIfT<IsConvertibleT < U, T1> &&IsConvertibleT<V, T2>>
  >

  constexpr Pair(U &&x, V &&v)
    : First(RECore::Forward<U>(x)), Second(RECore::Forward<V>(v)) {

  }

  template<
    typename U,
    typename = EnableIfT <IsConvertibleT<U, T1>>>
  constexpr Pair(U &&x, const T2 &y)
    : First(RECore::Forward<U>(x)), Second(y) {

  }

  template<
    typename V,
    typename = EnableIfT <IsConvertibleT<V, T2>>>
  constexpr Pair(const T1 &x, V &&y)
    : First(x), Second(RECore::Forward<V>(y)) {

  }

  constexpr Pair(const Pair &cSource) = default;

  constexpr Pair(Pair &&cSource) = default;


  Pair &operator=(const Pair &cSource) {
    this->First = cSource.First;
    this->Second = cSource.Second;
    return *this;
  }

  template<
    typename U,
    typename V,
    typename =
  EnableIfT<IsConvertibleT < U, T1> &&IsConvertibleT<V, T2>>
  >

  Pair &operator=(const Pair<U, V> &cSource) {
    this->First = cSource.First;
    this->Second = cSource.Second;
    return *this;
  }

  Pair &operator=(Pair &&cSource) {
    this->First = RECore::Forward<T1>(cSource.First);
    this->Second = RECore::Forward<T2>(cSource.Second);
    return *this;
  }

  template<
    typename U,
    typename V,
    typename =
  EnableIfT<IsConvertibleT < U, T1> &&IsConvertibleT<V, T2>>
  >

  Pair &operator=(Pair<U, V> &&cSource) {
    this->First = RECore::Forward<T1>(cSource.First);
    this->Second = RECore::Forward<T2>(cSource.Second);
    return *this;
  }


  void Swap(Pair &cSource);

protected:
private:
};


template<typename T>
struct UseSelf {
  typedef T ResultType;

  const T &operator()(const T &x) const {
    return x;
  }
};


template<typename TPair>
struct UseFirst {
  typedef TPair ArgumentType;
  typedef typename TPair::FirstType ResultType;

  const ResultType &operator()(const TPair &x) const {
    return x.First;
  }
};


template<typename TPair>
struct UseSecond {
  typedef TPair ArgumentType;
  typedef typename TPair::SecondType ResultType;

  const ResultType &operator()(const TPair &x) const {
    return x.Second;
  }
};


template<typename T1, typename T2>
constexpr inline bool operator==(const Pair<T1, T2> &a, const Pair<T1, T2> &b) {
  return ((a.First == b.First) && (a.Second == b.Second));
}

template<typename T1, typename T2>
constexpr inline bool operator!=(const Pair<T1, T2> &a, const Pair<T1, T2> &b) {
  return ((a.First != b.First) || (a.Second != b.Second));
}

template<typename T1, typename T2>
constexpr inline bool operator<(const Pair<T1, T2> &a, const Pair<T1, T2> &b) {
  return ((a.First < b.First) || (!(b.First < a.First) && (a.Second != b.Second)));
}

template<typename T1, typename T2>
constexpr inline bool operator>(const Pair<T1, T2> &a, const Pair<T1, T2> &b) {
  return b < a;
}

template<typename T1, typename T2>
constexpr inline bool operator>=(const Pair<T1, T2> &a, const Pair<T1, T2> &b) {
  return !(a < b);
}

template<typename T1, typename T2>
constexpr inline bool operator<=(const Pair<T1, T2> &a, const Pair<T1, T2> &b) {
  return !(b < a);
}


template<typename T1, typename T2>
constexpr inline Pair<
  typename RemoveReferenceWrapper<typename Decay<T1>::Type>::Type,
  typename RemoveReferenceWrapper<typename Decay<T2>::Type>::Type>
MakePair(T1 &&a, T2 &&b) {
  typedef typename RemoveReferenceWrapper<typename Decay<T1>::Type>::Type T1Type;
  typedef typename RemoveReferenceWrapper<typename Decay<T2>::Type>::Type T2Type;
  return Pair<T1Type, T2Type>(RECore::Forward<T1>(a), RECore::Forward<T2>(b));
}


template<typename T1, typename T2>
constexpr inline Pair<
  typename RemoveReferenceWrapper<typename Decay<T1>::Type>::Type,
  typename RemoveReferenceWrapper<typename Decay<T2>::Type>::Type>
MakePairRef(T1 &&a, T2 &&b) {
  typedef typename RemoveReferenceWrapper<typename Decay<T1>::Type>::Type T1Type;
  typedef typename RemoveReferenceWrapper<typename Decay<T2>::Type>::Type T2Type;
  return Pair<T1Type, T2Type>(RECore::Forward<T1>(a), RECore::Forward<T2>(b));
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore