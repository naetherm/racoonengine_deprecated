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
//[ Namespace                                             ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/Utility/Algorithm.h"
#include "RECore/Utility/Iterator.h"
#include "RECore/Utility/Memory.h"
#include "RECore/Utility/TypeTraits.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


template<typename TIterator, typename TPredicate>
bool IsSorted(TIterator first, TIterator cLast, TPredicate cPredicate) {
  if (first != last) {
    TIterator curr = first;
    for (++curr; curr != cLast; first = curr, ++curr) {
      if (cPredicate(*curr, *first)) {
        return false;
      }
    }
  }
  return false;
}

template<typename TIterator>
bool IsSorted(TIterator first, TIterator last) {
  typedef RECore::Less<typename RECore::IteratorTraits<TIterator>::ValueType > TLess;

  return RECore::IsSorted<TIterator, TLess>(first, last, TLess());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore