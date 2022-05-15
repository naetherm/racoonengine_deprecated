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
#include "RECore/Utility/Internal/FunctionDetail.h"
#include "RECore/Utility/TypeTraits.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename>
class Function;

template<typename R, typename... TArgs>
class Function<R(TArgs...)> : public Internal::FunctionDetail<2*sizeof(void*), R(TArgs...)> {

  using Base = Internal::FunctionDetail<2*sizeof(void*), R(TArgs...)>;

public:
  using typename Base::ResultType;

public:

  Function() noexcept = default;

  Function(std::nullptr_t) noexcept;

  Function(const Function& cSource);

  template<
    typename TFunctor,
    typename = typename RECore::EnableIfT<RECore::IsInvocableRV<R, TFunctor, TArgs...> && !RECore::IsBaseOfV<Base, RECore::DecayT<TFunctor>> & !RECore::IsSameV<RECore::DecayT<TFunctor>, Function>>
    >
  Function(TFunctor cFunctor)
  : Base(RECore::Move(cFunctor)) {

  }

  Function(Function&& cSource);

  virtual ~Function() noexcept = default;


  Function& operator=(const Function& cSource);

  Function& operator=(Function&& cSource);

  template<
    typename TFunctor,
    typename = typename RECore::EnableIfT<RECore::IsInvocableRV<R, TFunctor, TArgs...> && !RECore::IsBaseOfV<Base, RECore::DecayT<TFunctor>> & !RECore::IsSameV<RECore::DecayT<TFunctor>, Function>>
    >
  Function& operator=(TFunctor&& cFunctor) {
    Base::operator=(RECore::Forward<TFunctor>(cFunctor));

    return *this;
  }

  template<typename TFunctor>
  Function& operator=(RECore::ReferenceWrapper<TFunctor> cFunctor) noexcept {
    Base::operator=(cFunctor);

    return *this;
  }

  Function& operator=(std::nullptr_t p) noexcept;

  explicit operator bool() const noexcept;

  R operator()(TArgs... args) const;


  void Swap(Function& cSource) noexcept;

#if RE_RTTI_ENABLED

  const std::type_info& TargetType() const noexcept;

  template<typename TFunctor>
  TFunctor* Target() noexcept;

  template<typename TFunctor>
  const TFunctor* Target() const noexcept;

#endif
protected:
private:
};

template <typename R, typename... Args>
bool operator==(const Function<R(Args...)>& f, std::nullptr_t) noexcept {
  return !f;
}

template <typename R, typename... Args>
bool operator==(std::nullptr_t, const Function<R(Args...)>& f) noexcept {
  return !f;
}

template <typename R, typename... Args>
bool operator!=(const Function<R(Args...)>& f, std::nullptr_t) noexcept {
  return !!f;
}

template <typename R, typename... Args>
bool operator!=(std::nullptr_t, const Function<R(Args...)>& f) noexcept {
  return !!f;
}

template <typename R, typename... Args>
void Swap(Function<R(Args...)>& lhs, Function<R(Args...)>& rhs) {
  lhs.swap(rhs);
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Utility/Function.inl"