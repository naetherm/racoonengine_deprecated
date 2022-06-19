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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "REGui/REGui.h"
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Widget;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class MultiSlot {
public:

  MultiSlot();

  MultiSlot(const MultiSlot& rhs);

  virtual ~MultiSlot();


  MultiSlot& operator=(const MultiSlot& rhs);

  bool operator==(const MultiSlot& rhs) const;

  bool operator!=(const MultiSlot& rhs) const;
};

template<typename TType, typename TContainer = std::vector<Widget*>>
class TMultiSlot : public MultiSlot {
public:

  TMultiSlot();

  TMultiSlot(const TMultiSlot<TType, TContainer>& rhs);


  TMultiSlot& operator=(const TMultiSlot<TType, TContainer>& rhs);

  bool operator==(const TMultiSlot<TType, TContainer>& rhs) const;


  TType& operator[](Widget* widget);


  Widget& attachWidget(Widget* widget);

  Widget* getWidget(RECore::uint32 index = 0) const;

  virtual TType& exposeThis(TType*& outType);

protected:

  TContainer mWidgets;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "REGui/Layout/MultiSlot.inl"