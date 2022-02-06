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

template<typename R, typename... TArgs>
Function<R(TArgs...)>::Function(std::nullptr_t p) noexcept
: Base(p) {

}

template<typename R, typename... TArgs>
Function<R(TArgs...)>::Function(const Function<R(TArgs...)> &cSource)
: Base(cSource) {

}

template<typename R, typename... TArgs>
Function<R(TArgs...)>::Function(Function<R(TArgs...)> &&cSource)
: Base(Move(cSource)) {

}

template<typename R, typename... TArgs>
Function<R(TArgs...)> &Function<R(TArgs...)>::operator=(const Function<R(TArgs...)> &cSource) {

  Base::operator=(cSource);
  return *this;
}

template<typename R, typename... TArgs>
Function<R(TArgs...)> &Function<R(TArgs...)>::operator=(Function<R(TArgs...)> &&cSource) {

  Base::operator=(cSource);
  return *this;
}

template<typename R, typename... TArgs>
Function<R(TArgs...)> &Function<R(TArgs...)>::operator=(std::nullptr_t p) noexcept {
  Base::operator=(p);
  return *this;
}

template<typename R, typename... TArgs>
Function<R(TArgs...)>::operator bool() const noexcept {
  return Base::operator bool();
}

template<typename R, typename... TArgs>
R Function<R(TArgs...)>::operator()(TArgs... args) const {
  return Base::operator()(RECore::Forward<TArgs>(args)...);
}

template<typename R, typename... TArgs>
void Function<R(TArgs...)>::Swap(Function<R(TArgs...)> &cSource) noexcept {
  Base::Swap(cSource);
}

#if PL_RTTI_ENABLED

template<typename R, typename... TArgs>
const std::type_info& Function<R(TArgs...)>::TargetType() const noexcept {
  return Base::TargetType();
}

template<typename R, typename... TArgs>
template<typename TFunctor>
TFunctor* Function<R(TArgs...)>::Target() noexcept {
  return Base::Target();
}

template<typename R, typename... TArgs>
template<typename TFunctor>
const TFunctor* Function<R(TArgs...)>::Target() const noexcept {
  return Base::Target();
}

#endif

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore
