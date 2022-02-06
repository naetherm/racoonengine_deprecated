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
#include "RECore/Utility/TypeTraits.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename T, T... Ints>
class IntegerSequence {
public:
  template<T N> using Append = IntegerSequence<T, Ints..., N>;
  typedef T ValueType;
  static_assert(IsIntegral<T>::Value, "IntegerSequence can only be instantiated with an integral type");

  static constexpr sizeT Size() {
    return sizeof...(Ints);
  }

  using Next = Append<sizeof...(Ints)>;
};

template<sizeT N, typename IndexSeq>
struct MakeIndexSequenceImpl;

template<sizeT N, sizeT... Is>
struct MakeIndexSequenceImpl<N, IntegerSequence<sizeT, Is...>> {
  typedef typename MakeIndexSequenceImpl<N - 1, IntegerSequence<sizeT, N - 1, Is...>>::Type Type;
};

template<sizeT... Is>
struct MakeIndexSequenceImpl<0, IntegerSequence<sizeT, Is...>> {
  typedef IntegerSequence<sizeT, Is...> Type;
};

template<sizeT... Is> using IndexSequence = IntegerSequence<sizeT, Is...>;

template<sizeT N> using MakeIndexSequence = typename MakeIndexSequenceImpl<N, IntegerSequence<sizeT>>::Type;

template<typename Target, typename Seq>
struct IntegerSequenceConvertImpl;

template<typename Target, sizeT... Is>
struct IntegerSequenceConvertImpl<Target, IntegerSequence<sizeT, Is...>> {
  typedef IntegerSequence<Target, Is...> Type;
};

template<typename T, sizeT N>
struct MakeIntegerSequenceImpl {
  typedef typename IntegerSequenceConvertImpl<T, MakeIndexSequence<N>>::Type Type;
};

template<typename T, sizeT N> using MakeIntegerSequence = typename MakeIntegerSequenceImpl<T, N>::Type;

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore