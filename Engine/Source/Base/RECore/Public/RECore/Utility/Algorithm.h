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
//[ Header Guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/Utility/InitializerList.h"
#include "RECore/Utility/TypeTraits.h"
#include "RECore/Utility/Move.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ - AllOf
//[ - AnyOf
//[ - NoneOf
//[ - Copy
//[ - CopyIf
//[ - CopyN
//[ - CopyBackward
//[ - Count
//[ - CountOf
//[ - Equal
//[ - EqualRange
//[ - Fill
//[ - FillN
//[ - Find
//[ - FindEnd
//[ - findFirstOf
//[ - findFirstNotOf
//[ - findLastOf
//[ - FindLastNotIf
//[ - FindIf
//[ - ForEach
//[ - Generate
//[ - Identical
//[ - MinAlt
//[ - MaxAlt
//[ - Min
//[ - Max
//[-------------------------------------------------------]



//
// AllOf
//
template<typename ITER, typename PREDICATE>
inline bool AllOf(
  ITER nFirst,
  ITER nLast,
  PREDICATE cPredicate
) {
  for (; nFirst != nLast; ++nFirst) {
    if (!cPredicate(*nFirst)) {
      return false;
    }
  }
  return true;
}


//
// AnyOf
//
template<typename ITER, typename PREDICATE>
inline bool AnyOf(
  ITER nFirst,
  ITER nLast,
  PREDICATE cPredicate
) {
  for (; nFirst != nLast; ++nFirst) {
    if (cPredicate(*nFirst)) {
      return true;
    }
  }
  return false;
}


//
// ForEach
//
template<typename TIter, typename TFunc>
inline TFunc ForEach(TIter cFirst, TIter cLast, TFunc cFunc) {
  for (; cFirst != cLast; ++cFirst) { cFunc(*cFirst); }
  return cFunc;
}

//
// ForEachN
//
template<typename TIter, typename TSize, typename TFunc>
inline TFunc ForEachN(TIter cFirst, TSize nNum, TFunc cFunc) {
  for (TSize i = 0; i < nNum; ++nNum, ++cFirst) { cFunc(*cFirst); }
  return cFunc;
}


template<typename ITER>
ITER MinElement(
  ITER nFirst,
  ITER nLast
) {
  if (nFirst != nLast) {
    ITER _cMin = nFirst;

    while (++nFirst != nLast) {
      if (*nFirst < _cMin) {
        _cMin = nFirst;
      }
    }

    return _cMin;
  }

  return nFirst;
}

template<typename ITER, typename COMPARE>
ITER MinElement(
  ITER nFirst,
  ITER nLast,
  COMPARE cCompare
) {
  if (nFirst != nLast) {
    ITER _cMin = nFirst;

    while (++nFirst != nLast) {
      if (ccompare(*nFirst, _cMin)) {
        _cMin = nFirst;
      }
    }

    return _cMin;
  }

  return nFirst;
}

template<typename ITER>
ITER MaxElement(
  ITER nFirst,
  ITER nLast
) {
  if (nFirst != nLast) {
    ITER _cMax = nFirst;

    while (++nFirst != nLast) {
      if (_cMax < *nFirst) {
        _cMax = nFirst;
      }
    }

    return _cMax;
  }

  return nFirst;
}

template<typename ITER, typename COMPARE>
ITER MaxElement(
  ITER nFirst,
  ITER nLast,
  COMPARE cCompare
) {
  if (nFirst != nLast) {
    ITER _cMax = nFirst;

    while (++nFirst != nLast) {
      if (ccompare(_cMax, *nFirst)) {
        _cMax = nFirst;
      }
    }

    return _cMax;
  }

  return nFirst;
}


//
// MinAlt
//
template<typename T>
inline constexpr typename EnableIf<IsScalar<T>::Value, T>::Type
MinAlt(T a, T b) {
  return b < a ? b : a;
}

template<typename T>
inline typename EnableIf<!IsScalar<T>::Value, const T &>::Type
MinAlt(const T &a, const T &b) {
  return b < a ? b : a;
}

inline constexpr float MinAlt(float a, float b) {
  return b < a ? b : a;
}

inline constexpr double MinAlt(double a, double b) {
  return b < a ? b : a;
}

inline constexpr long double MinAlt(long double a, long double b) {
  return b < a ? b : a;
}


//
// MinAlt
//
template<typename T>
inline constexpr typename EnableIf<IsScalar<T>::Value, T>::Type
MaxAlt(T a, T b) {
  return b > a ? b : a;
}

template<typename T>
inline typename EnableIf<!IsScalar<T>::Value, const T &>::Type
MaxAlt(const T &a, const T &b) {
  return b > a ? b : a;
}

inline constexpr float MaxAlt(float a, float b) {
  return b > a ? b : a;
}

inline constexpr double MaxAlt(double a, double b) {
  return b > a ? b : a;
}

inline constexpr long double MaxAlt(long double a, long double b) {
  return b > a ? b : a;
}


//
// Equal
//
template<typename TIter1, typename TIter2>
constexpr inline bool Equal(TIter1 first, TIter1 last, TIter2 firsts) {
  for (; first != last; ++first, ++firsts) {
    if (!(*first == *firsts)) {
      return false;
    }
  }
  return true;
}
template<typename TIter1, typename TIter2, typename TBinaryPredicate>
constexpr inline bool Equal(TIter1 first, TIter1 last, TIter2 firsts, TBinaryPredicate predicate) {
  for (; first != last; ++first, ++firsts) {
    if (!predicate(*first, *firsts)) {
      return false;
    }
  }
  return true;
}


//
// Fill
//


//
// FillN
//
template<typename TIter, typename TSize, typename TType>
TIter FillN(TIter iterator, TSize n, const TType& value) {

  for (; n-- > 0; iterator++) {
    *iterator = value;
  }

  return iterator;
}


//
// Find
//
template<typename InputIterator, typename T>
inline InputIterator
Find(InputIterator first, InputIterator last, const T &value) {
  while ((first != last) && !(*first == value)) // Note that we always express value comparisons in terms of < or ==.
    first++;
  return first;
}


// C++ doesn't define a find with predicate, as it can effectively be synthesized via find_if
// with an appropriate predicate. However, it's often simpler to just have find with a predicate.
template<typename InputIterator, typename T, typename Predicate>
inline InputIterator
Find(InputIterator first, InputIterator last, const T &value, Predicate predicate) {
  while ((first != last) && !predicate(*first, value))
    ++first;
  return first;
}


//
// Search
//
template<typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1
Search(ForwardIterator1 first1, ForwardIterator1 last1,
       ForwardIterator2 first2, ForwardIterator2 last2) {
  if (first2 != last2) {
    // We need to make a special case for a pattern of one element,
    // as the logic below prevents one element patterns from working.
    ForwardIterator2 temp2(first2);
    ++temp2;

    if (temp2 != last2) {
      ForwardIterator1 cur1(first1);
      ForwardIterator2 p2;

      while (first1 != last1) {
        // The following loop is the equivalent of eastl::find(first1, last1, *first2)
        while ((first1 != last1) && !(*first1 == *first2))
          ++first1;

        if (first1 != last1) {
          p2 = temp2;
          cur1 = first1;

          if (++cur1 != last1) {
            while (*cur1 == *p2) {
              if (++p2 == last2)
                return first1;

              if (++cur1 == last1)
                return last1;
            }

            ++first1;
            continue;
          }
        }
        return last1;
      }

      // Fall through to the end.
    } else
      return Find(first1, last1, *first2);
  }

  return first1;
}


template<typename ForwardIterator, typename T>
inline void Replace(ForwardIterator first, ForwardIterator last, const T &old_value, const T &new_value) {
  for (; first != last; ++first) {
    if (*first == old_value)
      *first = new_value;
  }
}

template<typename ForwardIterator, typename Predicate, typename T>
inline void ReplaceIf(ForwardIterator first, ForwardIterator last, Predicate predicate, const T &new_value) {
  for (; first != last; ++first) {
    if (predicate(*first))
      *first = new_value;
  }
}

template<typename InputIterator, typename OutputIterator, typename T>
inline OutputIterator RemoveCopy(InputIterator first, InputIterator last, OutputIterator result, const T &value) {
  for (; first != last; ++first) {
    if (!(*first == value)) // Note that we always express value comparisons in terms of < or ==.
    {
      *result = RECore::Move(*first);
      ++result;
    }
  }
  return result;
}

template<typename InputIterator, typename OutputIterator, typename Predicate>
inline OutputIterator
RemoveCopyIf(InputIterator first, InputIterator last, OutputIterator result, Predicate predicate) {
  for (; first != last; ++first) {
    if (!predicate(*first)) {
      *result = RECore::Move(*first);
      ++result;
    }
  }
  return result;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore