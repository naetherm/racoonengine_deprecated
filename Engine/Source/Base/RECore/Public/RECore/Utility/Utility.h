
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
#include "RECore/Utility/Move.h"
#include "RECore/Utility/TypeListTraits.h"
#include "RECore/Utility/TypeTraits.h"
#include "RECore/Core/UniquePtr.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------
template<typename T>
inline void Swap(T& a, T& b) {
  T t(RECore::Move(a));
  a = RECore::Move(b);
  b = RECore::Move(t);
}



template<class T> struct _UniqueIf {
  using _SingleObject = RECore::UniquePtr<T>;
};

template<class T> struct _UniqueIf<T[]> {
  using _UnknownBound = RECore::UniquePtr<T[]>;
};

template<class T, size_t N> struct _UniqueIf<T[N]> {
  using _KnownBound = void;
};

template<class T, class... TArgs>
typename _UniqueIf<T>::_SingleObject
MakeUnique(TArgs&&... args) {
  return RECore::UniquePtr<T>(new T(RECore::Forward<TArgs>(args)...));
}

template<class T>
typename _UniqueIf<T>::_UnknownBound
MakeUnique(sizeT n) {
  using U = typename RECore::RemoveExtent<T>::Type;
  return RECore::UniquePtr<T>(new U[n]());
}

template<class T, class... TArgs>
typename _UniqueIf<T>::_KnownBound
MakeUnique(TArgs&&...) = delete;



template<class T>
constexpr inline RECore::AddConstT<T>& AsConst(T& cValue) noexcept {
  return cValue;
}


template<typename T>
inline typename RECore::EnableIf<RECore::IsPointer<T>::Value, void*>::Type AsVoidPtr(const T& cObj) {
  return const_cast<void*>(reinterpret_cast<const volatile void*>(cObj));
}
template<typename T>
inline typename RECore::EnableIf<!RECore::IsPointer<T>::Value, void*>::Type AsVoidPtr(const T& cObj) {
  return const_cast<void*>(reinterpret_cast<const volatile void*>(&cObj));
}



template<typename T>
using ReturnTypeNormal = RECore::AddPointerT< RECore::RemovePointerT<T> >;

template<typename T, typename Tp = RemoveReferenceT<T>>
using RawAddressOfReturnType = RECore::Conditional< RECore::IsFunctionPtr< RemovePointersExceptOneT<Tp> >::Value,
  RECore::AddPointerT< RemovePointersExceptOneT<Tp> >,
  ReturnTypeNormal<Tp> >;


template<typename T>
using RawAddressOfReturnTypeT = typename RawAddressOfReturnType<T>::Type;

template<typename T, typename Enable = void>
struct RawAddressOfImpl
{
  static inline RawAddressOfReturnTypeT<T> Get(T& data)
  {
    return RECore::AddressOf(data);
  }
};

template<typename T>
using IsRawVoidPointer = RECore::IsSame<void*, AddPointerT< RawTypeT<T> > >;

template<typename T>
using IsVoidPointer = RECore::IntegralConstant<bool, RECore::IsPointer<T>::Value && IsRawVoidPointer<T>::Value && PointerCount<T>::Value == 1>;

template<typename T>
struct RawAddressOfImpl<T, RECore::EnableIfT<(RECore::IsPointer<T>::Value && PointerCount<T>::Value >= 1 && !IsVoidPointer<T>::Value) ||
                             (RECore::PointerCount<T>::Value == 1 && RECore::IsMemberPointer<RECore::RemovePointerT<T> >::Value)
> > {
  static inline RawAddressOfReturnTypeT<T> Get(T& data) {
    return RawAddressOfImpl< RECore::RemovePointerT<T> >::Get(*data);
  }
};

template<typename T>
struct RawAddressOfImpl<T, RECore::EnableIfT <RECore::IsVoidPointer<T>::Value> > {
  static inline RawAddressOfReturnTypeT<T> Get(T& data) {
    return data; // void pointer cannot be dereferenced to type "void"
  }
};

/*!
 * \brief This function will return from its raw type \p T
 *        its address as pointer.
 *
 * \see detail::raw_type
 *
 * \return The address of the raw type from the given object \p data as pointer.
 */
template<typename T>
static inline RawAddressOfReturnTypeT<T> RawAddressOf(T& data) {
  return RawAddressOfImpl<T>::Get(data);
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore
