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
class Slot {
public:

  Slot();

  Slot(Widget* widget);

  Slot(const Slot& rhs);

  virtual ~Slot();


  Slot& operator=(const Slot& rhs);

  bool operator==(const Slot& rhs) const;

  bool operator!=(const Slot& rhs) const;

  /**
   * @brief
   * Attachs a widget to the slot.
   *
   * @param[in] widget
   * Pointer to the widget to attach.
   *
   * @note
   * Keep in mind that you have to keep track about the pointers by yourself!
   *
   * @return
   * Reference to the attached widget.
   */
  Widget& attachWidget(Widget* widget);

  /**
   * @brief
   * Detaches the widget again.
   *
   * @note
   * Keep in mind that you have to keep track about the pointers by yourself!
   *
   * @return
   * Returns reference to the
   */
  Widget& detachWidget();

  Widget* getWidget() const;

protected:

  Widget* mWidget;
};

template<typename TType>
class TSlot : public Slot {
public:

  TSlot();

  TSlot(const Widget* widget);

  TSlot(const TSlot& rhs);


  TSlot& operator=(const TSlot& rhs);

  bool operator==(const TSlot<TType>& rhs) const;


  TType& operator[](Widget* widget);


  virtual TType& exposeThis(TType*& outType);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "REGui/Layout/Slot.inl"