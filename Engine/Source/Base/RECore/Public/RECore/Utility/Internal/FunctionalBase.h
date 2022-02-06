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
#include "RECore/Utility/Memory.h"
#include "RECore/Utility/Move.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

template<typename TArg, typename TRes>
struct UnaryFunction {
  typedef TArg ArgumentType;
  typedef TRes ResultType;
};

template<typename TArg1, typename TArg2, typename TRes>
struct BinaryFunction {
  typedef TArg1 FirstArgumentType;
  typedef TArg2 SecondArgumentType;
  typedef TRes ResultType;
};



template<typename TRes, typename TClass, typename TType, typename... TArgs>
auto InvokeImpl(TRes TClass::*func, TType&& cObj, TArgs&&... args) ->
  typename EnableIf<IsBaseOf<TClass, DecayT<decltype(cObj)>>::Value, decltype((RECore::Forward<TType>(cObj).*func)(RECore::Forward<TArgs>(args)...))>::Type {
  return (RECore::Forward<TType>(cObj).*func)(RECore::Forward<TArgs>(args)...);
}
template<typename TFunc, typename... TArgs>
auto InvokeImpl(TFunc&& cFunc, TArgs&&... args) -> decltype(RECore::Forward<TFunc>(cFunc)(RECore::Forward<TArgs>(args)...)) {
  return RECore::Forward<TFunc>(cFunc)(RECore::Forward<TArgs>(args)...);
}
template<typename TRes, typename TClass, typename TType, typename... TArgs>
auto InvokeImpl(TRes TClass::*func, TType&& cObj, TArgs&&... args) -> decltype(((*RECore::Forward<TType>(cObj)).*func)(RECore::Forward<TArgs>(args)...)) {
  return ((*RECore::Forward<TType>(cObj)).*func)(RECore::Forward<TArgs>(args)...);
}
template<typename TMember, typename TClass, typename TType>
auto InvokeImpl(TMember TClass::*member, TType&& cObj) -> typename EnableIf<IsBaseOf<TClass, DecayT<decltype(cObj)>>::Value, decltype(cObj.*member)>::Type {
  return cObj.*member;
}
template<typename TMember, typename TClass, typename TType>
auto InvokeImpl(TMember TClass::*member, TType&& cObj) -> decltype((*RECore::Forward<TType>(cObj)).*member) {
  return (*RECore::Forward<TType>(cObj)).*member;
}

template<typename TFunc, typename... TArgs>
inline decltype(auto) Invoke(TFunc&& cFunc, TArgs&&... args) {
  return InvokeImpl(RECore::Forward<TFunc>(cFunc), RECore::Forward<TArgs>(args)...);
}



template<typename TFunc, typename=void, typename... TArgs>
struct InvokeResultImpl {
};
template<typename TFunc, typename... TArgs>
struct InvokeResultImpl<TFunc, VoidT<decltype(InvokeImpl(RECore::DeclVal<DecayT<TFunc>>(), RECore::DeclVal<TArgs>()...))>, TArgs...> {
  typedef decltype(InvokeImpl(RECore::DeclVal<DecayT<TFunc>>(), RECore::DeclVal<TArgs>()...)) Type;
};
template<typename TFunc, typename... TArgs>
struct InvokeResult : public InvokeResultImpl<TFunc, void, TArgs...> {};
template<typename TFunc, typename... TArgs>
using InvokeResultT = typename InvokeResult<TFunc, TArgs...>::Type;


template<typename TFunc, typename=void, typename... TArgs>
struct IsInvocableImpl : public FalseType {};
template<typename TFunc, typename... TArgs>
struct IsInvocableImpl<TFunc, VoidT<typename InvokeResult<TFunc, TArgs...>::Type>, TArgs...> : public TrueType {};
template<typename TFunc, typename... TArgs>
struct IsInvocable : public IsInvocable<TFunc, void, TArgs...> {};
template<typename TFunc, typename... TArgs>
constexpr bool IsInvocableV = IsInvocable<TFunc, TArgs...>::Value;


template<typename TRes, typename TFunc, typename=void, typename... TArgs>
struct IsInvocableRImpl : public FalseType {};
template<typename TRes, typename TFunc, typename... TArgs>
struct IsInvocableRImpl<TRes, TFunc, VoidT<typename InvokeResult<TFunc, TArgs...>::Type>, TArgs...> : public IsConvertible<typename InvokeResult<TFunc, TArgs...>::Type, TRes> {};
template<typename TRes, typename TFunc, typename... TArgs>
struct IsInvocableR : public IsInvocableRImpl<TRes, TFunc, void, TArgs...> {};
template<typename TRes, typename TFunc, typename... TArgs>
constexpr bool IsInvocableRV = IsInvocableR<TRes, TFunc, TArgs...>::Value;


template<typename T>
class ReferenceWrapper {
public:
  typedef T Type;
public:
  ReferenceWrapper(T& cVal) noexcept
  : mpVal(AddressOf(cVal)) {
  }

  ReferenceWrapper(T&& cVal) = delete;

  ReferenceWrapper(const ReferenceWrapper<T>& cSource) noexcept
  : mpVal(cSource.mpVal) {
  }

  ReferenceWrapper& operator=(const ReferenceWrapper<T>& cSource) noexcept {
    this->mpVal = cSource.mpVal;
    return *this;
  }

  operator T&() const noexcept {
    return *this->mpVal;
  }
  T& Get() const noexcept {
    return *this->mpVal;
  }

  template<typename... TArgs>
  typename ResultOf<T&(TArgs&&...)>::Type operator()(TArgs&&... args) const {
    return RECore::Invoke(*this->mpVal, RECore::Forward<TArgs>(args)...);
  }

private:
  T* mpVal;
};


template<typename T>
ReferenceWrapper<T> Ref(T& cRef) noexcept {
  return ReferenceWrapper<T>(cRef);
}
template<typename T>
void Ref(const T&&) = delete;
template<typename T>
ReferenceWrapper<T> Ref(ReferenceWrapper<T> cSource) noexcept {
  return RECore::Ref(cSource.Get());
}
template<typename T>
ReferenceWrapper<const T> CRef(const T& cRef) noexcept {
  return ReferenceWrapper<const T>(cRef);
}
template<typename T>
void CRef(const T&&) = delete;
template<typename T>
ReferenceWrapper<const T> CRef(ReferenceWrapper<T> cSource) noexcept {
  return RECore::CRef(cSource.Get());
}



template<typename T>
struct IsReferenceWrapperHelper : public FalseType {};
template<typename T>
struct IsReferenceWrapperHelper<ReferenceWrapper<T>> : public TrueType {};
template<typename T>
struct IsReferenceWrapper : public IsReferenceWrapperHelper<typename RemoveCV<T>::Type> {};



template<typename T>
struct RemoveReferenceWrapper {
  typedef T Type;
};
template<typename T>
struct RemoveReferenceWrapper<ReferenceWrapper<T>> {
  typedef T& Type;
};
template<typename T>
struct RemoveReferenceWrapper<const ReferenceWrapper<T>> {
  typedef T& Type;
};

template<typename TRes, typename TClass, typename TType, typename... TArgs>
auto InvokeImpl(TRes (TClass::*func)(TArgs...), TType&& cObj, TArgs&&... args) ->
  typename EnableIf<IsReferenceWrapper<typename RemoveReference<TType>::Type>::Value,
                    decltype((cObj.Get().*func)(RECore::Forward<TArgs>(args)...))>::Type {
  return (cObj.Get().*func)(RECore::Forward<TArgs>(args)...);
}
template<typename TMember, typename TClass, typename TType>
auto InvokeImpl(TMember(TClass::*member), TType&& cObj) ->
  typename EnableIfT <IsReferenceWrapper<typename RemoveReference<TType>::Type>::Value,
                      decltype(cObj.Get().*member)>::Type {
  return cObj.Get().*member;
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore