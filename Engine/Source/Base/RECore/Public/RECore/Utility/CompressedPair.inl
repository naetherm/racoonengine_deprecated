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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 0>::CompressedPairImpl() {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 0>::CompressedPairImpl(
  CompressedPairImpl<T1, T2, 0>::FirstParamType x,
  CompressedPairImpl<T1, T2, 0>::SecondParamType y)
  : mFirst(x), mSecond(y) {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 0>::CompressedPairImpl(CompressedPairImpl<T1, T2, 0>::FirstParamType x)
  : mFirst(x) {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 0>::CompressedPairImpl(CompressedPairImpl<T1, T2, 0>::SecondParamType y)
  : mSecond(y) {

}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 0>::FirstReference CompressedPairImpl<T1, T2, 0>::First() {
  return this->mFirst;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 0>::FirstConstReference CompressedPairImpl<T1, T2, 0>::First() const {
  return this->mFirst;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 0>::SecondReference CompressedPairImpl<T1, T2, 0>::Second() {
  return this->mSecond;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 0>::SecondConstReference CompressedPairImpl<T1, T2, 0>::Second() const {
  return this->mSecond;
}

template<typename T1, typename T2>
void CompressedPairImpl<T1, T2, 0>::Swap(CompressedPair <T1, T2> &y) {
  CPSwap(this->mFirst, y.First());
  CPSwap(this->mSecond, y.Second());
}


template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 1>::CompressedPairImpl() {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 1>::CompressedPairImpl(
  CompressedPairImpl<T1, T2, 1>::FirstParamType x,
  CompressedPairImpl<T1, T2, 1>::SecondParamType y)
  : FirstType(x), mSecond(y) {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 1>::CompressedPairImpl(CompressedPairImpl<T1, T2, 1>::FirstParamType x)
  : FirstType(x) {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 1>::CompressedPairImpl(CompressedPairImpl<T1, T2, 1>::SecondParamType y)
  : mSecond(y) {

}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 1>::FirstReference CompressedPairImpl<T1, T2, 1>::First() {
  return *this;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 1>::FirstConstReference CompressedPairImpl<T1, T2, 1>::First() const {
  return *this;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 1>::SecondReference CompressedPairImpl<T1, T2, 1>::Second() {
  return this->mSecond;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 1>::SecondConstReference CompressedPairImpl<T1, T2, 1>::Second() const {
  return this->mSecond;
}

template<typename T1, typename T2>
void CompressedPairImpl<T1, T2, 1>::Swap(CompressedPair <T1, T2> &y) {
  CPSwap(this->mSecond, y.Second());
}


template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 2>::CompressedPairImpl() {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 2>::CompressedPairImpl(
  CompressedPairImpl<T1, T2, 2>::FirstParamType x,
  CompressedPairImpl<T1, T2, 2>::SecondParamType y)
  : SecondType(y), mFirst(x) {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 2>::CompressedPairImpl(CompressedPairImpl<T1, T2, 2>::FirstParamType x)
  : mFirst(x) {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 2>::CompressedPairImpl(CompressedPairImpl<T1, T2, 2>::SecondParamType y)
  : SecondType(y) {

}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 2>::FirstReference CompressedPairImpl<T1, T2, 2>::First() {
  return this->mFirst;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 2>::FirstConstReference CompressedPairImpl<T1, T2, 2>::First() const {
  return this->mFirst;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 2>::SecondReference CompressedPairImpl<T1, T2, 2>::Second() {
  return *this;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 2>::SecondConstReference CompressedPairImpl<T1, T2, 2>::Second() const {
  return *this;
}

template<typename T1, typename T2>
void CompressedPairImpl<T1, T2, 2>::Swap(CompressedPair <T1, T2> &y) {
  CPSwap(this->mFirst, y.First());
}


template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 3>::CompressedPairImpl() {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 3>::CompressedPairImpl(
  CompressedPairImpl<T1, T2, 3>::FirstParamType x,
  CompressedPairImpl<T1, T2, 3>::SecondParamType y)
  : FirstType(x), SecondType(y) {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 3>::CompressedPairImpl(CompressedPairImpl<T1, T2, 3>::FirstParamType x)
  : FirstType(x) {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 3>::CompressedPairImpl(CompressedPairImpl<T1, T2, 3>::SecondParamType y)
  : SecondType(y) {

}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 3>::FirstReference CompressedPairImpl<T1, T2, 3>::First() {
  return *this;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 3>::FirstConstReference CompressedPairImpl<T1, T2, 3>::First() const {
  return *this;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 3>::SecondReference CompressedPairImpl<T1, T2, 3>::Second() {
  return *this;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 3>::SecondConstReference CompressedPairImpl<T1, T2, 3>::Second() const {
  return *this;
}

template<typename T1, typename T2>
void CompressedPairImpl<T1, T2, 3>::Swap(CompressedPair <T1, T2> &y) {

}


template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 4>::CompressedPairImpl() {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 4>::CompressedPairImpl(
  CompressedPairImpl<T1, T2, 4>::FirstParamType x,
  CompressedPairImpl<T1, T2, 4>::SecondParamType y)
  : FirstType(x) {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 4>::CompressedPairImpl(CompressedPairImpl<T1, T2, 4>::FirstParamType x)
  : FirstType(x) {

}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 4>::FirstReference CompressedPairImpl<T1, T2, 4>::First() {
  return *this;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 4>::FirstConstReference CompressedPairImpl<T1, T2, 4>::First() const {
  return *this;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 4>::SecondReference CompressedPairImpl<T1, T2, 4>::Second() {
  return *this;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 4>::SecondConstReference CompressedPairImpl<T1, T2, 4>::Second() const {
  return *this;
}

template<typename T1, typename T2>
void CompressedPairImpl<T1, T2, 4>::Swap(CompressedPair <T1, T2> &y) {

}


template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 5>::CompressedPairImpl() {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 5>::CompressedPairImpl(
  CompressedPairImpl<T1, T2, 5>::FirstParamType x,
  CompressedPairImpl<T1, T2, 5>::SecondParamType y)
  : mFirst(x), mSecond(y) {

}

template<typename T1, typename T2>
CompressedPairImpl<T1, T2, 5>::CompressedPairImpl(CompressedPairImpl<T1, T2, 5>::FirstParamType x)
  : mFirst(x), mSecond(x) {

}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 5>::FirstReference CompressedPairImpl<T1, T2, 5>::First() {
  return this->mFirst;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 5>::FirstConstReference CompressedPairImpl<T1, T2, 5>::First() const {
  return this->mFirst;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 5>::SecondReference CompressedPairImpl<T1, T2, 5>::Second() {
  return this->mSecond;
}

template<typename T1, typename T2>
typename CompressedPairImpl<T1, T2, 5>::SecondConstReference CompressedPairImpl<T1, T2, 5>::Second() const {
  return this->mSecond;
}

template<typename T1, typename T2>
void CompressedPairImpl<T1, T2, 5>::Swap(CompressedPair <T1, T2> &y) {

}


template<typename T1, typename T2>
CompressedPair<T1, T2>::CompressedPair() {

}

template<typename T1, typename T2>
CompressedPair<T1, T2>::CompressedPair(
  CompressedPair<T1, T2>::FirstParamType x,
  CompressedPair<T1, T2>::SecondParamType y)
  : CompressedPair<T1, T2>::Base(x, y) {

}

template<typename T1, typename T2>
CompressedPair<T1, T2>::CompressedPair(CompressedPair<T1, T2>::FirstParamType x)
  : CompressedPair<T1, T2>::Base(x) {

}

template<typename T1, typename T2>
CompressedPair<T1, T2>::CompressedPair(CompressedPair<T1, T2>::SecondParamType y)
  : CompressedPair<T1, T2>::Base(y) {

}

template<typename T1, typename T2>
CompressedPair<T1, T2>::~CompressedPair() {

}

template<typename T1, typename T2>
typename CompressedPair<T1, T2>::FirstReference CompressedPair<T1, T2>::First() {
  return Base::First();
}

template<typename T1, typename T2>
typename CompressedPair<T1, T2>::FirstConstReference CompressedPair<T1, T2>::First() const {
  return Base::First();
}

template<typename T1, typename T2>
typename CompressedPair<T1, T2>::SecondReference CompressedPair<T1, T2>::Second() {
  return Base::Second();
}

template<typename T1, typename T2>
typename CompressedPair<T1, T2>::SecondConstReference CompressedPair<T1, T2>::Second() const {
  return Base::Second();
}

template<typename T1, typename T2>
void CompressedPair<T1, T2>::Swap(CompressedPair &cSource) {
  Base::Swap(cSource);
}


template<typename T>
CompressedPair<T, T>::CompressedPair() {

}

template<typename T>
CompressedPair<T, T>::CompressedPair(
  CompressedPair<T, T>::FirstParamType x,
  CompressedPair<T, T>::SecondParamType y)
  : CompressedPair<T, T>::Base(x, y) {

}

template<typename T>
CompressedPair<T, T>::CompressedPair(CompressedPair<T, T>::FirstParamType x)
  : CompressedPair<T, T>::Base(x) {

}

template<typename T>
CompressedPair<T, T>::~CompressedPair() {

}

template<typename T>
typename CompressedPair<T, T>::FirstReference CompressedPair<T, T>::First() {
  return CompressedPair<T, T>::Base::First();
}

template<typename T>
typename CompressedPair<T, T>::FirstConstReference CompressedPair<T, T>::First() const {
  return CompressedPair<T, T>::Base::First();
}

template<typename T>
typename CompressedPair<T, T>::SecondReference CompressedPair<T, T>::Second() {
  return CompressedPair<T, T>::Base::Second();
}

template<typename T>
typename CompressedPair<T, T>::SecondConstReference CompressedPair<T, T>::Second() const {
  return CompressedPair<T, T>::Base::Second();
}

template<typename T>
void CompressedPair<T, T>::Swap(CompressedPair &cSource) {
  CompressedPair<T, T>::Base::Swap(cSource);
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore