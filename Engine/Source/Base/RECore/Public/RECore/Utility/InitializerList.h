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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * InitializerList
 *
 * @brief
 * InitializerList is an initializer_list as it is specified through the
 * C++11 standard.
 */
template<class T>
class InitializerList {
public:

  //
  // Type declarations
  //
  typedef T ValueType;
  typedef const T &Reference;
  typedef const T &ConstReference;
  typedef int32 SizeType;
  typedef const T *Iterator;
  typedef const T *ConstIterator;

public:

  /**
   * @brief
   * Default constructor.
   */
  InitializerList()
    : m_pData(nullptr), m_nSize(0) {

  }

  InitializerList(ValueType cData[])
    : m_pData(cData), m_nSize(sizeof(cData) / sizeof(ValueType *)) {

  }

  InitializerList(const InitializerList<T> &cSource)
    : m_pData(cSource.m_pData), m_nSize(cSource.m_nSize) {

  }


  /**
   * @brief
   * Returns the size in elements of the InitializerList.
   *
   * @return
   * The size in elements of the InitializerList.
   */
  SizeType GetSize() const {
    return this->m_nSize;
  }

  /**
   * @brief
   * Returns the begin of the data that is hold by the InitializerList.
   *
   * @return
   * Pointer to the begin of the data hold by the InitializerList.
   */
  ConstIterator GetBegin() const {
    return this->m_pData;
  }

  /**
   * @brief
   * Returns the end of the data that is hold by the InitializerList.
   *
   * @return
   * Pointer to the end of the data hold by the InitializerList.
   */
  ConstIterator GetEnd() const {
    return this->m_pData + this->m_nSize;
  }

protected:

private:

  /**
   * @brief
   * Constructor.
   *
   * @param [in]pData
   * Pointer to the data that should be represented by the InitializerList.
   * @param [in]nSize
   * The number of elements.
   */
  InitializerList(
    ConstIterator pData,
    SizeType nSize
  )
    : m_pData(pData), m_nSize(nSize) {

  }

  /**
   * The data which is hold by the InitializerList.
   */
  Iterator m_pData;

  /**
   * The size of the InitializerList.
   */
  SizeType m_nSize;
};


template<typename TType>
const TType* Begin(InitializerList<TType> cList) noexcept {
  return cList.GetBegin();
}

template<typename TType>
const TType* End(InitializerList<TType> cList) noexcept {
  return cList.GetEnd();
}

template<typename TType>
const TType* begin(InitializerList<TType> cList) noexcept {
  return cList.GetBegin();
}

template<typename TType>
const TType* end(InitializerList<TType> cList) noexcept {
  return cList.GetEnd();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore