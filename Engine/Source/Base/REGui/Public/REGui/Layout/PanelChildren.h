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
#include "REGui/Layout/Children.h"
#include <vector>


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
/**
 * @class
 * PanelChildren
 *
 * @tparam TSlotType
 *
 * @brief
 * PanelChildren is a implementation that can contain multiple child widgets.
 */
template<typename TSlotType>
class PanelChildren : public Children {
public:

  /**
   * @brief
   * Default constructor.
   */
  PanelChildren();

  /**
   * @brief
   * Destructor.
   */
  ~PanelChildren() override;


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
  const TSlotType& operator[](RECore::uint32 index) const;

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
  TSlotType& operator[](RECore::uint32 index);

  /**
   * @brief
   * Returns number of child widgets.
   *
   * @return
   * Number of child widgets.
   */
  RECore::int32 getNumOfChildren() const;

  /**
   * @brief
   * Adds another slot to this.
   *
   * @param[in] slot
   * Pointer to slot to add.
   */
  void add(TSlotType* slot);

  /**
   * @brief
   * Clears the internal structured.
   */
  void clear();

  /**
   * @brief
   * Adds another slot at the given position.
   *
   * @param[in] slot
   * Pointer o slot to add.
   * @param[in] index
   * Index position where the slot should be added
   */
  void insert(TSlotType* slot, RECore::int32 index);

  /**
   * @brief
   * Returns pointer to child widget at index position @p index.
   *
   * @param[in] index
   * Index position.
   *
   * @return
   * Pointer to widget.
   */
  const Widget* getChildAtIndex(RECore::uint32 index) const override;

  /**
   * @brief
   * Returns pointer to child widget at index position @p index.
   *
   * @param[in] index
   * Index position.
   *
   * @return
   * Pointer to widget.
   */
  Widget* getChildAtIndex(RECore::uint32 index) override;

  /**
   * @brief
   * Removes child widget at index position @p index.
   *
   * @param[in] index
   * Index position.
   *
   * @return
   * True if element was removed, false otherwise.
   */
  bool removeAtIndex(RECore::int32 index);

private:
  /** Internal container for hilding all slots */
  std::vector<TSlotType> mChildren;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "REGui/Layout/PanelChildren.inl"