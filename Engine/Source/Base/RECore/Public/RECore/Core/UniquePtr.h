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
 * UniquePtr
 *
 * @brief
 * UniquePtr
 */
template<typename TYPE>
class UniquePtr {

  template<typename OTHER> friend
  class UniquePtr;

public:

  typedef UniquePtr<TYPE> ThisType;

  /**
   * @brief
   * Create a new object of type TYPE with the given arguments ARGS and
   * returns it as a UniquePtr.
   */
  template<typename... ARGS>
  inline static UniquePtr<TYPE>
  MakeUnique(ARGS&&... args);

  /**
   * @brief
   * Default constructor.
   */
  UniquePtr();

  /**
   * @brief
   * Constructor.
   */
  explicit inline UniquePtr(TYPE *pData);

  /**
   * @brief
   * Constructor.
   */
  inline UniquePtr(UniquePtr &&cSource);

  /**
   * @brief
   * Constructor.
   */
  template<typename OTHER>
  inline UniquePtr(UniquePtr<OTHER> &&cSource);

  /**
   * @brief
   * Destructor.
   */
  ~UniquePtr();


  /**
   * @brief
   * Assignment operator.
   */
  inline UniquePtr& operator=(UniquePtr &&cSource);

  /**
   * @brief
   * Assignment operator.
   */
  template<typename OTHER>
  inline UniquePtr& operator=(UniquePtr<OTHER> &&cSource);


  /**
   * @brief
   * operator bool
   */
  inline explicit operator bool() const;

  /**
   * @brief
   * Logical not operator.
   */
  inline bool operator!() const;

  /**
   * @brief
   * Dereferenceing operator.
   */
  inline TYPE* operator->() const;

  /**
   * @brief
   * Dereferenceing operator.
   */
  inline TYPE& operator*() const;

  /**
   * @brief
   * Returns the raw pointer of this ScopedPtr.
   */
  inline TYPE* Get() const;

  /**
   * @brief
   * Resets the internal raw pointer. Thereby the old value will be erased and replace
   * by the pointer 'pData'.
   */
  inline TYPE* Release();

  /**
   * @brief
   * Resets the raw pointer.
   */
  inline void Reset(TYPE *pData = nullptr);

  inline void swap(ThisType &cSource) {
    ///TODO: Implement me
  }

protected:
private:

  /**
   * @brief
   * Copy constructor.
   */
  UniquePtr(const UniquePtr &);

  /**
   * @brief
   * Copy operator.
   */
  UniquePtr &operator=(const UniquePtr &);


  TYPE *m_pData;
};


template<typename L, typename R>
inline bool operator==(
  const UniquePtr<L> &l,
  const UniquePtr<R> &r
) {
  return (l.Get() == r.Get());
}

template<typename L, typename R>
inline bool operator!=(
  const UniquePtr<L> &l,
  const UniquePtr<R> &r
) {
  return (l.Get() != r.Get());
}

template<typename T>
inline bool operator==(
  const UniquePtr<T> &l,
  const UniquePtr<T> &r
) {
  return (l.Get() == r.Get());
}

template<typename T>
inline bool operator!=(
  const UniquePtr<T> &l,
  const UniquePtr<T> &r
) {
  return (l.Get() != r.Get());
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Core/UniquePtr.inl"