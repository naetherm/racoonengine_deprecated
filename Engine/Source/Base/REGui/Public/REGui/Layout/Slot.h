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
/**
 * @class
 * Slot
 *
 * @brief
 * Foundation implementation of a slot containing a single widget.
 */
class Slot {
public:

  /**
   * @brief
   * Default constructor.
   */
  Slot();

  /**
   * @brief
   * Constructor.
   *
   * @param[in] widget
   * Pointer to widget.
   */
  Slot(Widget* widget);

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * Object to copy.
   */
  Slot(const Slot& rhs);

  /**
   * @brief
   * Destructor.
   */
  virtual ~Slot();


  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * Object to copy.
   *
   * @return
   * Reference to this.
   */
  Slot& operator=(const Slot& rhs);

  /**
   * @brief
   * Equality operator.
   *
   * @param[in] rhs
   * Object to compare to.
   *
   * @return
   * True if both are equal, false otherwise.
   */
  bool operator==(const Slot& rhs) const;

  /**
   * @brief
   * Equality operator.
   *
   * @param[in] rhs
   * Object to compare to.
   *
   * @return
   * True if both are not equal, false otherwise.
   */
  bool operator!=(const Slot& rhs) const;

  /**
   * @brief
   * Attaches a widget to the slot.
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

  /**
   * @brief
   * Returns pointer to the widget of the slot. This can be a nullptr.
   *
   * @return
   * Pointer to widget of slot.
   */
  Widget* getWidget() const;

protected:

  /** Pointer to widget, can be a nullptr */
  Widget* mWidget;
};

/**
 * @class
 * TSlot
 *
 * @tparam TType
 *
 * @brief
 * A TSlot contains one child widget and describes how that child should be arranged on the screen.
 */
template<typename TType>
class TSlot : public Slot {
public:

  /**
   * @brief
   * Default constructor.
   */
  TSlot();

  /**
   * @brief
   * Constructor.
   *
   * @param[in] widget
   * Pointer to widget.
   */
  TSlot(const Widget* widget);

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * Object to copy.
   */
  TSlot(const TSlot& rhs);


  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * Object to copy.
   *
   * @return
   * Reference to this.
   */
  TSlot& operator=(const TSlot& rhs);

  /**
   * @brief
   * Equality operator.
   *
   * @param[in] rhs
   * Object to compare to.
   *
   * @return
   * True if both are equal, false otherwise.
   */
  bool operator==(const TSlot<TType>& rhs) const;


  /**
   * @brief
   * Assign operator.
   *
   * @param[in] widget
   * Pointer to widget to assign to this slot.
   *
   * @return
   * Reference to this slot implementation.
   */
  TType& operator[](Widget* widget);


  /**
   * @brief
   * Helper method for exposing the slot.
   *
   * @param[in,out] outType
   * The output variable.
   *
   * @return
   * Reference to this, casted to TType.
   */
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