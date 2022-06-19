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
template<typename TSlotType>
PanelChildren<TSlotType>::PanelChildren() {

}

template<typename TSlotType>
PanelChildren<TSlotType>::~PanelChildren() {

}


template<typename TSlotType>
const TSlotType& PanelChildren<TSlotType>::operator[](RECore::uint32 index) const {
  return mChildren[index];
}

template<typename TSlotType>
TSlotType& PanelChildren<TSlotType>::operator[](RECore::uint32 index) {
  return mChildren[index];
}

template<typename TSlotType>
RECore::int32 PanelChildren<TSlotType>::getNumOfChildren() const {
  return mChildren.size();
}

template<typename TSlotType>
void PanelChildren<TSlotType>::add(TSlotType* slot) {
  mChildren.push_back(*slot);
}

template<typename TSlotType>
void PanelChildren<TSlotType>::clear() {
  mChildren.clear();
}

template<typename TSlotType>
void PanelChildren<TSlotType>::insert(TSlotType* slot, RECore::int32 index) {

}

template<typename TSlotType>
const Widget* PanelChildren<TSlotType>::getChildAtIndex(RECore::uint32 index) const {
  return mChildren[index].getWidget();
}

template<typename TSlotType>
Widget* PanelChildren<TSlotType>::getChildAtIndex(RECore::uint32 index) {
  return mChildren[index].getWidget();
}

template<typename TSlotType>
bool PanelChildren<TSlotType>::removeAtIndex(RECore::int32 index) {
  return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
