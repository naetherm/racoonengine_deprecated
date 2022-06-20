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
/**
 * @class
 * Slot
 *
 * @brief
 * Foundation implementation of a slot containing a multiple widgets.
 */
class MultiSlot {
public:

  /**
   * @brief
   * Default constructor.
   */
  MultiSlot();

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * Object to copy.
   */
  MultiSlot(const MultiSlot& rhs);

  /**
   * @brief
   * Destructor.
   */
  virtual ~MultiSlot();


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
  MultiSlot& operator=(const MultiSlot& rhs);

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
  bool operator==(const MultiSlot& rhs) const;

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
  bool operator!=(const MultiSlot& rhs) const;
};

/**
 * @class
 * TMultiSlot
 *
 * @tparam TType
 *
 * @brief
 * A TMultiSlot contains multiple widgets and describes how that children should be arranged on the screen.
 */
template<typename TType, typename TContainer = std::vector<Widget*>>
class TMultiSlot : public MultiSlot {
public:

  /**
   * @brief
   * Default constructor.
   */
  TMultiSlot();

  /**
   * @brief
   * Constructor.
   *
   * @param[in] widget
   * Pointer to widget.
   */
  TMultiSlot(const TMultiSlot<TType, TContainer>& rhs);


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
  TMultiSlot& operator=(const TMultiSlot<TType, TContainer>& rhs);

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
  bool operator==(const TMultiSlot<TType, TContainer>& rhs) const;


  /**
   * @brief
   * Assign operator.
   *
   * @param[in] widget
   * Pointer to widget to assign to this slot.
   * @return
   */
  TType& operator[](Widget* widget);


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
   * Returns pointer to the widget at index position @p index.
   *
   * @param[in] index
   * Index position.
   *
   * @return
   * Pointer to widget.
   */
  Widget* getWidget(RECore::uint32 index = 0) const;

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