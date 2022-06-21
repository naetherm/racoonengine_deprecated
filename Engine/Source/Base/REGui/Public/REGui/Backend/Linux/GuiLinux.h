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
#include "REGui/Gui/GuiImpl.h"
#include <RECore/Linux/X11Includes.h>


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
 * GuiImpl
 *
 * @brief
 * Linux specific implementation of the gui system.
 */
class GuiLinux : public GuiImpl {
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] gui
   * Pointer to platform independent gui implementation.
   */
  GuiLinux(Gui* gui);

  /**
   * @brief
   * Destructor.
   */
  ~GuiLinux() override;


  /**
   * @brief
   * Returns pointer to X display.
   *
   * @return
   * Pointer to X display.
   */
  [[nodiscard]] ::Display* getDisplay() const;

public:

  /**
   * @brief
   * Determines whether there are any messages to be handled.
   *
   * @return
   * True if there are messages left, false otherwise.
   */
  bool hasPendingMessages() override;

  /**
   * @brief
   * Process all outstanding messages.
   */
  void processMessage() override;

private:

  /**
   * @brief
   * Processes the provided X event.
   *
   * @param[in] event
   * Pointer to X event to process.
   */
  void processXEvent(XEvent* event);

private:

  /**
   * @brief
   * X error handler
   *
   * @param[in] display
   * Display
   * @param[in] error
   * Error descriptor.
   *
   * @return
   * Ignored.
   */
  static int errorHandler(Display* display, XErrorEvent* error);

protected:
  /** Pointer to x11 display, always valid! */
  ::Display* mDisplay;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
