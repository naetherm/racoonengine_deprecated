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
#include "REGui/Gui/MessageTypes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class NativeWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class GuiMessage {
  //[-------------------------------------------------------]
  //[ Public named constructors                             ]
  //[-------------------------------------------------------]
public:

  static REGUI_API GuiMessage onClose(NativeWindow* nativeWindow);

  static REGUI_API GuiMessage onCreate(NativeWindow* nativeWindow);

  static REGUI_API GuiMessage onDestroy(NativeWindow* nativeWindow);

  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:

  /**
   * @brief
   * Constructor.
   *
   * @param[in] nativeWindow
   * Pointer to native window.
   * @param[in] messageType
   * Message type.
   */
  REGUI_API GuiMessage(NativeWindow* nativeWindow, EMessageType messageType = MessageOnUnknown);

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * Object to copy.
   */
  REGUI_API GuiMessage(const GuiMessage& rhs);

  /**
   * @brief
   * Destructor.
   */
  REGUI_API ~GuiMessage();


  REGUI_API bool operator==(const GuiMessage& rhs) const;

  REGUI_API GuiMessage& operator=(const GuiMessage& rhs);

  NativeWindow* getWindow() const;

  EMessageType getType() const;

  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  // Window target
  NativeWindow* mNativeWindow;
  // Message type
  EMessageType mMessageType;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui
