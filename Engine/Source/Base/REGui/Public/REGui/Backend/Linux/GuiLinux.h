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
class Screen;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * GuiLinux
 *
 * @brief
 */
class GuiLinux : public GuiImpl {

  friend class NativeWindowLinux;
public:

  REGUI_API GuiLinux(Gui* gui);

  /**
   * @brief
   * Destructor
   */
  REGUI_API ~GuiLinux() override;


  [[nodiscard]] ::Display* getDisplay() const;

  [[nodiscard]] ::Window getWindowHandle() const;

public:

  bool hasPendingMessages() override;

  void processMessage() override;

  void postMessage(const GuiMessage& guiMessage) override;

  void enumerateScreens(std::vector<Screen*>& screens) override;

  RECore::Vec2i getScreenSize() const override;

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

private:

  /**
   * @brief
   * X event procedure.
   *
   * @param[in] event
   * Pointer to an x-event.
   */
  REGUI_API void processXEvent(XEvent* event);


  void onDraw();

  void onResize(RECore::uint32 width, RECore::uint32 height);

  void onKeyInput(uint32_t keySym, char character, bool pressed);

  void onMouseMoveInput(int x, int y);

  void onMouseButtonInput(uint32_t button, bool pressed);

  void onMouseWheelInput(bool scrollUp);

private:

  // System handles
  /** X display */
  ::Display* mDisplay;
  /** Hidden window for internal messages */
  ::Window mHiddenWindow;

  // Used client protocols
  struct {
    ::Atom Close;							/** Message for closing the window */
    ::Atom Focus;							/** Message for taking the focus */
    ::Atom Timer;							/** Message for timer events */
    ::Atom Exit;							/** Message for quitting the GUI */
    ::Atom Wakeup;							/** Message for waking up the message loop */
    ::Atom DestroyWidget;					/** Message for destroying a widget (delayed) */
    ::Atom ThemeChanged;					/** Message for changing the theme of a widget */
  } mClientProtocols;

  // Other used atoms
  struct {
    ::Atom UTF8_STRING;						/** Atom for the type of a window title */
    ::Atom WM_NAME;							/** Window title (old?) */
    ::Atom _NET_WM_NAME;					/** Window title */
    ::Atom _NET_WM_VISIBLE_NAME;			/** Window title (visible title, can be different) */
    ::Atom _NET_WM_STATE;					/** Window state */
    ::Atom _NET_WM_STATE_FULLSCREEN;		/** Fullscreen mode */
    ::Atom _NET_WM_STATE_MAXIMIZED_HORZ;	/** Maximize horizontally */
    ::Atom _NET_WM_STATE_MAXIMIZED_VERT;	/** Maximize vertically */
    ::Atom _NET_WM_STATE_HIDDEN;			/** Minimize window */
    ::Atom _NET_WM_STATE_SKIP_TASKBAR;		/** Do not show window in taskbar */
    ::Atom _NET_WM_STATE_ABOVE;				/** Topmost state */
    ::Atom _XEMBED;							/** XEMBED protocol */
    ::Atom _XEMBED_INFO;					/** XEMBED information */
    ::Atom _NET_SYSTEM_TRAY_OPCODE;			/** System tray protocol */
    ::Atom _NET_SYSTEM_TRAY_MESSAGE_DATA;	/** System tray message data */
  } mAtoms;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
