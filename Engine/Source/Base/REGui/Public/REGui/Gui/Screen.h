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
#include <RECore/Math/Vec2i.h>
#include <RECore/String/String.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Screen
 *
 * @brief
 * This class represents a specific monitor resolution.
 */
class Screen {
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] gui
   * Pointer to platform independent gui implementation.
   */
  REGUI_API Screen(Gui* gui);

  /**
   * @brief
   * Destructor
   */
  REGUI_API ~Screen();


  [[nodiscard]] Gui* getGui() const;

  [[nodiscard]] const RECore::String& getName() const;

  void setName(const RECore::String& name);

  [[nodiscard]] const RECore::Vec2i& getPosition() const;

  void setPosition(const RECore::Vec2i& position);

  [[nodiscard]] const RECore::Vec2i& getSize() const;

  void setSize(const RECore::Vec2i& size);

  void setDefault(bool defaultScreen);

private:

  Gui* mGui;
  RECore::String mName;
  RECore::Vec2i mPosition;
  RECore::Vec2i mSize;
  bool mDefaultScreen;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
