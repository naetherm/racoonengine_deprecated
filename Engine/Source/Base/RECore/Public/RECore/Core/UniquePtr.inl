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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/Utility/Move.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

template<typename TYPE>
template<typename... ARGS>
UniquePtr <TYPE> UniquePtr<TYPE>::MakeUnique(ARGS &&... args) {
  return UniquePtr<TYPE>(new TYPE(RECore::Forward<ARGS>(args)...));
}

template<typename TYPE>
UniquePtr<TYPE>::UniquePtr()
  : m_pData(nullptr) {

}

template<typename TYPE>
UniquePtr<TYPE>::UniquePtr(TYPE *pData)
  : m_pData(pData) {

}

template<typename TYPE>
UniquePtr<TYPE>::UniquePtr(RECore::UniquePtr<TYPE> &&cSource)
  : m_pData(cSource.m_pData) {
  cSource.m_pData = nullptr;
}

template<typename TYPE>
template<typename OTHER>
UniquePtr<TYPE>::UniquePtr(RECore::UniquePtr<OTHER> &&cSource)
  : m_pData(cSource.m_pData) {
  cSource.m_pData = nullptr;
}

template<typename TYPE>
UniquePtr<TYPE>::~UniquePtr() {
  if (this->m_pData) {
    REDelete(this->m_pData);
  }
}


template<typename TYPE>
UniquePtr <TYPE> &UniquePtr<TYPE>::operator=(UniquePtr <TYPE> &&cSource) {
  if (&cSource == this) {
    TYPE *_pTemp = this->m_pData;
    this->m_pData = cSource.m_pData;
    cSource.m_pData = nullptr;

    if (_pTemp) {
      REDelete(_pTemp);
    }
  }

  return (*this);
}

template<typename TYPE>
template<typename OTHER>
UniquePtr <TYPE> &UniquePtr<TYPE>::operator=(RECore::UniquePtr<OTHER> &&cSource) {
  if (&cSource != this) {
    TYPE *_pTemp = this->m_pData;
    this->m_pData = cSource.m_pData;
    cSource.m_pData = nullptr;

    if (_pTemp) {
      REDelete(_pTemp);
    }
  }

  return (*this);
}


template<typename TYPE>
UniquePtr<TYPE>::operator bool() const {
  return !!this->m_pData;
}


template<typename TYPE>
bool UniquePtr<TYPE>::operator!() const {
  return !this->m_pData;
}


template<typename TYPE>
TYPE *UniquePtr<TYPE>::operator->() const {
  return this->m_pData;
}


template<typename TYPE>
TYPE &UniquePtr<TYPE>::operator*() const {
  return *this->m_pData;
}


template<typename TYPE>
TYPE *UniquePtr<TYPE>::Get() const {
  return this->m_pData;
}


template<typename TYPE>
TYPE *UniquePtr<TYPE>::Release() {
  TYPE *_pResult = this->m_pData;
  this->m_pData = nullptr;

  return _pResult;
}


template<typename TYPE>
void UniquePtr<TYPE>::Reset(TYPE *pData) {
  TYPE *_pTemp = this->m_pData;
  this->m_pData = pData;

  if (_pTemp) {
    REDelete(_pTemp);
  }
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore