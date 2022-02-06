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
#include "RECore/Utility/CallTraits.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename T1, typename T2>
class CompressedPair;

template<typename T1, typename T2, bool TSame, bool TFirstEmpty, bool TSecondEmpty>
struct CPSwitch;

template<typename T1, typename T2>
struct CPSwitch<T1, T2, false, false, false> {
  static const int32 Value = 0;
};

template<typename T1, typename T2>
struct CPSwitch<T1, T2, false, true, false> {
  static const int32 Value = 1;
};

template<typename T1, typename T2>
struct CPSwitch<T1, T2, false, false, true> {
  static const int32 Value = 2;
};

template<typename T1, typename T2>
struct CPSwitch<T1, T2, false, true, true> {
  static const int32 Value = 3;
};

template<typename T1, typename T2>
struct CPSwitch<T1, T2, true, true, true> {
  static const int32 Value = 4;
};

template<typename T1, typename T2>
struct CPSwitch<T1, T2, true, false, false> {
  static const int32 Value = 5;
};

template<typename T>
inline void CPSwap(T &t1, T &t2) {
  T t = t1;
  t1 = t2;
  t2 = t;
}


template<typename T1, typename T2, int TVersion>
class CompressedPairImpl;

template<typename T1, typename T2>
class CompressedPairImpl<T1, T2, 0> {
public:
  typedef T1 FirstType;
  typedef T2 SecondType;
  typedef typename CallTraits<FirstType>::ParamType FirstParamType;
  typedef typename CallTraits<SecondType>::ParamType SecondParamType;
  typedef typename CallTraits<FirstType>::Reference FirstReference;
  typedef typename CallTraits<SecondType>::Reference SecondReference;
  typedef typename CallTraits<FirstType>::ConstReference FirstConstReference;
  typedef typename CallTraits<SecondType>::ConstReference SecondConstReference;
public:

  CompressedPairImpl();

  CompressedPairImpl(FirstParamType x, SecondParamType y);

  CompressedPairImpl(FirstParamType x);

  CompressedPairImpl(SecondParamType y);


  FirstReference First();

  FirstConstReference First() const;

  SecondReference Second();

  SecondConstReference Second() const;

  void Swap(CompressedPair<T1, T2> &y);

protected:

  FirstType mFirst;
  SecondType mSecond;
};


template<typename T1, typename T2>
class CompressedPairImpl<T1, T2, 1> : private T1 {
public:
  typedef T1 FirstType;
  typedef T2 SecondType;
  typedef typename CallTraits<FirstType>::ParamType FirstParamType;
  typedef typename CallTraits<SecondType>::ParamType SecondParamType;
  typedef typename CallTraits<FirstType>::Reference FirstReference;
  typedef typename CallTraits<SecondType>::Reference SecondReference;
  typedef typename CallTraits<FirstType>::ConstReference FirstConstReference;
  typedef typename CallTraits<SecondType>::ConstReference SecondConstReference;
public:

  CompressedPairImpl();

  CompressedPairImpl(FirstParamType x, SecondParamType y);

  CompressedPairImpl(FirstParamType x);

  CompressedPairImpl(SecondParamType y);


  FirstReference First();

  FirstConstReference First() const;

  SecondReference Second();

  SecondConstReference Second() const;

  void Swap(CompressedPair<T1, T2> &y);

protected:

  SecondType mSecond;
};


template<typename T1, typename T2>
class CompressedPairImpl<T1, T2, 2> : private T2 {
public:
  typedef T1 FirstType;
  typedef T2 SecondType;
  typedef typename CallTraits<FirstType>::ParamType FirstParamType;
  typedef typename CallTraits<SecondType>::ParamType SecondParamType;
  typedef typename CallTraits<FirstType>::Reference FirstReference;
  typedef typename CallTraits<SecondType>::Reference SecondReference;
  typedef typename CallTraits<FirstType>::ConstReference FirstConstReference;
  typedef typename CallTraits<SecondType>::ConstReference SecondConstReference;
public:

  CompressedPairImpl();

  CompressedPairImpl(FirstParamType x, SecondParamType y);

  CompressedPairImpl(FirstParamType x);

  CompressedPairImpl(SecondParamType y);


  FirstReference First();

  FirstConstReference First() const;

  SecondReference Second();

  SecondConstReference Second() const;

  void Swap(CompressedPair<T1, T2> &y);

protected:

  FirstType mFirst;
};


template<typename T1, typename T2>
class CompressedPairImpl<T1, T2, 3> : private T1, private T2 {
public:
  typedef T1 FirstType;
  typedef T2 SecondType;
  typedef typename CallTraits<FirstType>::ParamType FirstParamType;
  typedef typename CallTraits<SecondType>::ParamType SecondParamType;
  typedef typename CallTraits<FirstType>::Reference FirstReference;
  typedef typename CallTraits<SecondType>::Reference SecondReference;
  typedef typename CallTraits<FirstType>::ConstReference FirstConstReference;
  typedef typename CallTraits<SecondType>::ConstReference SecondConstReference;
public:

  CompressedPairImpl();

  CompressedPairImpl(FirstParamType x, SecondParamType y);

  CompressedPairImpl(FirstParamType x);

  CompressedPairImpl(SecondParamType y);


  FirstReference First();

  FirstConstReference First() const;

  SecondReference Second();

  SecondConstReference Second() const;

  void Swap(CompressedPair<T1, T2> &y);
};


template<typename T1, typename T2>
class CompressedPairImpl<T1, T2, 4> : private T1 {
public:
  typedef T1 FirstType;
  typedef T2 SecondType;
  typedef typename CallTraits<FirstType>::ParamType FirstParamType;
  typedef typename CallTraits<SecondType>::ParamType SecondParamType;
  typedef typename CallTraits<FirstType>::Reference FirstReference;
  typedef typename CallTraits<SecondType>::Reference SecondReference;
  typedef typename CallTraits<FirstType>::ConstReference FirstConstReference;
  typedef typename CallTraits<SecondType>::ConstReference SecondConstReference;
public:

  CompressedPairImpl();

  CompressedPairImpl(FirstParamType x, SecondParamType y);

  CompressedPairImpl(FirstParamType x);

  CompressedPairImpl(SecondParamType y);


  FirstReference First();

  FirstConstReference First() const;

  SecondReference Second();

  SecondConstReference Second() const;

  void Swap(CompressedPair<T1, T2> &y);
};


template<typename T1, typename T2>
class CompressedPairImpl<T1, T2, 5> {
public:
  typedef T1 FirstType;
  typedef T2 SecondType;
  typedef typename CallTraits<FirstType>::ParamType FirstParamType;
  typedef typename CallTraits<SecondType>::ParamType SecondParamType;
  typedef typename CallTraits<FirstType>::Reference FirstReference;
  typedef typename CallTraits<SecondType>::Reference SecondReference;
  typedef typename CallTraits<FirstType>::ConstReference FirstConstReference;
  typedef typename CallTraits<SecondType>::ConstReference SecondConstReference;
public:

  CompressedPairImpl();

  CompressedPairImpl(FirstParamType x, SecondParamType y);

  CompressedPairImpl(FirstParamType x);

  CompressedPairImpl(SecondParamType y);


  FirstReference First();

  FirstConstReference First() const;

  SecondReference Second();

  SecondConstReference Second() const;

  void Swap(CompressedPair<T1, T2> &y);

protected:

  FirstType mFirst;
  SecondType mSecond;
};


template<typename T1, typename T2>
class CompressedPair
: private CompressedPairImpl<T1, T2, CPSwitch<T1, T2, std::is_same<typename std::remove_cv<T1>::type , typename std::remove_cv<T2>::type>::value, std::is_empty<T1>::value, std::is_empty<T2>::value>::Value> {
public:

  typedef CompressedPairImpl<T1, T2, CPSwitch<T1, T2, std::is_same<typename std::remove_cv<T1>::type , typename std::remove_cv<T2>::type>::value, std::is_empty<T1>::value, std::is_empty<T2>::value>::Value> Base;

  typedef T1 FirstType;
  typedef T2 SecondType;
  typedef typename CallTraits<FirstType>::ParamType FirstParamType;
  typedef typename CallTraits<SecondType>::ParamType SecondParamType;
  typedef typename CallTraits<FirstType>::Reference FirstReference;
  typedef typename CallTraits<SecondType>::Reference SecondReference;
  typedef typename CallTraits<FirstType>::ConstReference FirstConstReference;
  typedef typename CallTraits<SecondType>::ConstReference SecondConstReference;

public:
  /**
   * @brief
   * Default constructor.
   */
  CompressedPair();

  CompressedPair(FirstParamType x, SecondParamType y);

  explicit CompressedPair(FirstParamType x);

  explicit CompressedPair(SecondParamType y);

  /**
   * @brief
   * Destructor.
   */
  ~CompressedPair();

  FirstReference First();

  FirstConstReference First() const;

  SecondReference Second();

  SecondConstReference Second() const;

  void Swap(CompressedPair &cSource);
};

template<typename T>
class CompressedPair<T, T>
: private CompressedPairImpl<T, T, CPSwitch<T, T, std::is_same<typename std::remove_cv<T>::type , typename std::remove_cv<T>::type>::value, std::is_empty<T>::value, std::is_empty<T>::value>::Value> {
public:

  typedef CompressedPairImpl<T, T, CPSwitch<T, T, std::is_same<typename std::remove_cv<T>::type , typename std::remove_cv<T>::type>::value, std::is_empty<T>::value, std::is_empty<T>::value>::Value> Base;
  typedef T FirstType;
  typedef T SecondType;
  typedef typename CallTraits<FirstType>::ParamType FirstParamType;
  typedef typename CallTraits<SecondType>::ParamType SecondParamType;
  typedef typename CallTraits<FirstType>::Reference FirstReference;
  typedef typename CallTraits<SecondType>::Reference SecondReference;
  typedef typename CallTraits<FirstType>::ConstReference FirstConstReference;
  typedef typename CallTraits<SecondType>::ConstReference SecondConstReference;

public:
  /**
   * @brief
   * Default constructor.
   */
  CompressedPair();

  CompressedPair(FirstParamType x, SecondParamType y);

  explicit CompressedPair(FirstParamType x);

  /**
   * @brief
   * Destructor.
   */
  ~CompressedPair();

  FirstReference First();

  FirstConstReference First() const;

  SecondReference Second();

  SecondConstReference Second() const;

  void Swap(CompressedPair &cSource);
};

template<typename T1, typename T2>
inline void Swap(CompressedPair<T1, T2> &x, CompressedPair<T1, T2> &y) {
  x.Swap(y);
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Utility/CompressedPair.inl"