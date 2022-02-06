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
//#include <type_traits>
#include "RECore/Utility/TypeTraits.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
//
// CallTraits
//

template<typename T, bool TIsSmall>
struct CTImp2 {
  typedef const T &ParamType;
};
template<typename T>
struct CTImp2<T, true> {
  typedef const T ParamType;
};
template<typename T, bool TISP, bool TBl>
struct CTImp {
  typedef const T &ParamType;
};
template<typename T, bool TISP>
struct CTImp<T, TISP, true> {
  typedef typename CTImp2<T, sizeof(T) <= sizeof(void *)>::ParamType ParamType;
};
template<typename T, bool Tbl>
struct CTImp<T, true, Tbl> {
  typedef T const ParamType;
};

template<typename T>
struct CallTraits {
  typedef T ValueType;
  typedef T &Reference;
  typedef const T &ConstReference;
  typedef typename CTImp<T, RECore::IsPointer<T>::Value, RECore::IsArithmetic<T>::Value>::ParamType ParamType;
};

template<typename T>
struct CallTraits<T &> {
  typedef T &ValueType;
  typedef T &Reference;
  typedef const T &ConstReference;
  typedef T &ParamType;
};

template<typename T, sizeT N>
struct CallTraits<T[N]> {
  typedef T ArrayType[N];
  typedef const T *ValueType;
  typedef ArrayType &Reference;
  typedef const ArrayType &ConstReference;
  typedef const T *const ParamType;
};

template<typename T, sizeT N>
struct CallTraits<const T[N]> {
  typedef const T ArrayType[N];
  typedef const T *ValueType;
  typedef ArrayType &Reference;
  typedef const ArrayType &ConstReference;
  typedef const T *const ParamType;
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore