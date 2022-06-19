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
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TType, typename TContainer>
TMultiSlot<TType, TContainer>::TMultiSlot() {

}

template<typename TType, typename TContainer>
TMultiSlot<TType, TContainer>::TMultiSlot(const TMultiSlot<TType, TContainer>& rhs)
: MultiSlot(rhs)
, mWidgets(rhs.mWidgets) {

}
template<typename TType, typename TContainer>
TMultiSlot<TType, TContainer>& TMultiSlot<TType, TContainer>::operator=(const TMultiSlot<TType, TContainer> &rhs) {
  mWidgets = rhs.mWidgets;

  return *this;
}

template<typename TType, typename TContainer>
bool TMultiSlot<TType, TContainer>::operator==(const TMultiSlot<TType, TContainer> &rhs) const {
  return (mWidgets == rhs.mWidgets);
}

template<typename TType, typename TContainer>
TType& TMultiSlot<TType, TContainer>::operator[](Widget *widget) {
  attachWidget(widget);

  return (TType&)(*this);
}

template<typename TType, typename TContainer>
Widget& TMultiSlot<TType, TContainer>::attachWidget(Widget *widget) {
  mWidgets.push_back(widget);

  return *widget;
}

template<typename TType, typename TContainer>
Widget* TMultiSlot<TType, TContainer>::getWidget(RECore::uint32 index) const {
  return mWidgets[index];
}

template<typename TType, typename TContainer>
TType& TMultiSlot<TType, TContainer>::exposeThis(TType *&outType) {
  outType = (TType*)(this);

  return (TType&)(*this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
