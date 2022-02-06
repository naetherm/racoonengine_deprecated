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

#pragma once

#include "RECore/RECore.h"
#include "RECore/Frontend/FrontendOSImpl.h"
#include <map>

#include <X11/Xlib.h>

namespace RECore {

class WindowOS;

/**
 * @class
 * LinuxFrontendOS
 *
 * @brief
 */
class RECORE_API FrontendOSLinux : public FrontendOSImpl {
public:

/**
 * @brief
 * Constructor.
 *
 * @param [in]cFrontendOS
 * Reference to the FrontendOS instance.
 */
  FrontendOSLinux(FrontendOS &cFrontendOS);

/**
 * @brief
 * Destructor.
 */
  virtual ~FrontendOSLinux();

/**
 * @brief
 * Returns the Display instance we are using on Linux platforms.
 *
 * @return
 * The Display instance we are using on Linux platforms.
 */
  ::Display *getDisplay() const;

/**
 * @brief
 * Adds the WindowOS pointer to the list of active windows.
 *
 * @param [in]pWindow
 * Pointer to a newly added window. This can be a nullptr.
 */
  void addWindowHandle(WindowOS *pWindow) override;

/**
 * @brief
 * Removes the WindowOS pointer from the nternaly active windows.
 *
 * @param [in]pWindow
 * Pointer to the window that should be removed.
 */
  void removeWindowHandle(WindowOS *pWindow) override;

/**
 * @brief
 * Responsible for hadling messages.
 */
  void handlePendingMessages() override;

protected:

/**
 * @brief
 * Responsible for handling a single event.
 *
 * @param [in]cEvent
 * The event that should be handled.
 *
 * @return
 * True if everything went fine, false otherwise.
 */
  bool handleEvent(XEvent &cEvent);

/**
 * List of all windows that are currently registered to this frontend.
 */
  std::map<handle, WindowOS *> m_lstWindows;

private:

/**
 * Pointer ot the x11 intern display.
 */
  ::Display *m_pDisplay;
};

}
