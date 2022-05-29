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


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType>
Value<TType>::Value()
: mValue() {

}

template<typename TType>
Value<TType>::Value(const TType& rhs)
: mValue(rhs) {

}

template<typename TType>
Value<TType>::Value(TType&& rhs)
: mValue(rhs) {

}

template<typename TType>
Value<TType>::Value(const Value<TType>& rhs)
: mValue(rhs.mValue) {

}

template<typename TType>
Value<TType>::~Value() {

}


template<typename TType>
Value<TType>& Value<TType>::operator=(const TType& rhs) {
  mValue = rhs;
  return *this;
}

template<typename TType>
Value<TType>& Value<TType>::operator=(const Value<TType>& rhs) {
  mValue = rhs.mValue;
  return *this;
}

template<typename TType>
bool Value<TType>::operator==(const Value<TType>& rhs) const {
  return (mValue == rhs.mValue);
}

template<typename TType>
bool Value<TType>::operator!=(const Value<TType>& rhs) const {
  return !operator==(rhs);
}


template<typename TType>
bool Value<TType>::isSet() const {
  return true;
}

template<typename TType>
const TType Value<TType>::get() const {
  return mValue;
}

template<typename TType>
TType Value<TType>::get() {
  return mValue;
}

template<typename TType>
void Value<TType>::set(TType value) {
  mValue = value;
}



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore