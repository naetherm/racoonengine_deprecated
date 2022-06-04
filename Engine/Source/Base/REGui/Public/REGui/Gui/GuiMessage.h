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
#include "REGui/Gui/GuiTypes.h"
#include <RECore/Math/Vec2i.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERHI {
class RHICommandBuffer;
}


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
/**
 * @class
 * GuiMessage
 *
 * @brief
 * Gui message implementation for the platform independent communication.
 */
class GuiMessage {
public:

  static REGUI_API GuiMessage onDraw(NativeWindow* nativeWindow);

  static REGUI_API GuiMessage onShow(NativeWindow* nativeWindow);

  static REGUI_API GuiMessage onHide(NativeWindow* nativeWindow);

  static REGUI_API GuiMessage onMove(NativeWindow* nativeWindow, const RECore::Vec2i& position);

  static REGUI_API GuiMessage onSize(NativeWindow* nativeWindow, const RECore::Vec2i& size);

  static REGUI_API GuiMessage onMouseEnter(NativeWindow* nativeWindow);

  static REGUI_API GuiMessage onMouseLeave(NativeWindow* nativeWindow);

  static REGUI_API GuiMessage onMouseMove(NativeWindow* nativeWindow, const RECore::Vec2i& position);

  static REGUI_API GuiMessage onMouseButtonDown(NativeWindow* nativeWindow, EMouseButton button);

  static REGUI_API GuiMessage onMouseButtonUp(NativeWindow* nativeWindow, EMouseButton button);

  static REGUI_API GuiMessage onMouseWheel(NativeWindow* nativeWindow, int delta);

  static REGUI_API GuiMessage onKeyDown(NativeWindow* nativeWindow, RECore::uint32 key, RECore::uint32 modifiers);

  static REGUI_API GuiMessage onKeyUp(NativeWindow* nativeWindow, RECore::uint32 key, RECore::uint32 modifiers);

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
  ~GuiMessage();


  REGUI_API bool operator==(const GuiMessage& rhs) const;

  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * The object to copy.
   *
   * @return
   * Reference to this instance.
   */
  REGUI_API GuiMessage& operator=(const GuiMessage& rhs);

  /**
   * @brief
   * Returns pointer to native window.
   *
   * @return
   * Pointer to native window.
   */
  inline NativeWindow* getWindow() const;

  /**
   * @brief
   * Returns the gui message type.
   *
   * @return
   * Gui message type.
   */
  inline EMessageType getType() const;

  /**
   * @brief
   * Returns data.
   *
   * @return
   * Data.
   */
  inline RECore::uint32 getData() const;

  /**
   * @brief
   * Returns delta value.
   *
   * @return
   * Delta value.
   */
  inline int getDelta() const;

  /**
   * @brief
   * Returns the mouse button.
   *
   * @return
   * Mouse button.
   */
  inline EMouseButton getMouseButton() const;

  /**
   * @brief
   * Returns key.
   *
   * @return
   * Key.
   */
  inline RECore::uint32 getKey() const;

  /**
   * @brief
   * Returns data pointer.
   *
   * @return
   * Data pointer.
   */
  inline void* getDataPointer() const;

  /**
   * @brief
   * Returns ext data.
   *
   * @return
   * Ext data.
   */
  inline RECore::uint32 getExtData() const;

  /**
   * @brief
   * Returns modifiers.
   *
   * @return
   * Modifiers
   */
  inline RECore::uint32 getModifiers() const;

  /**
   * @brief
   * Returns position or size.
   *
   * @return
   * Position or size.
   */
  inline const RECore::Vec2i& getPositionSize() const;

  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  /** Window target pointer */
  NativeWindow* mNativeWindow;
  /** Message type */
  EMessageType mMessageType;

  /** First data block */
  union {
    RECore::int32 mData;
    int mDelta;
    EMouseButton mMouseButton;
    RECore::uint32 mKey;
  } mDataBlock1;

  /** Second data block for additional material */
  union {
    void* mDataPtr;
    RECore::uint32 mExtData;
    RECore::uint32 mModifiers;
  } mDataBlock2;

  /** Store for position or size */
  RECore::Vec2i mPositionSize;
};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "REGui/Gui/GuiMessage.inl"