/*********************************************************\
 * Copyright (c) 2012-2022 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGL/RERHIOpenGL.h"
#include "RERHIOpenGL/RHIOpenGLContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux OpenGL context class
*/
class OpenGLContextLinux final : public RHIOpenGLContext
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class RHIDynamicRHI;


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  *  @param[in] depthStencilAttachmentTextureFormat
  *    Depth stencil attachment texture format
  *  @param[in] nativeWindowHandle
  *    Optional native main window handle, can be a null handle
  *  @param[in] useExternalContext
  *    When true an own OpenGL context won't be created and the context pointed by "shareContextLinux" is ignored
  *  @param[in] shareContextLinux
  *    Optional share context, can be a null pointer
  */
  OpenGLContextLinux(RHIDynamicRHI& openGLRhi, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::handle nativeWindowHandle, bool useExternalContext, const OpenGLContextLinux* shareContextLinux = nullptr);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~OpenGLContextLinux() override;

  /**
  *  @brief
  *    Return the primary device context
  *
  *  @return
  *    The primary device context, null pointer on error
  */
  [[nodiscard]] Display* getDisplay() const;

  /**
  *  @brief
  *    Return the primary render context
  *
  *  @return
  *    The primary render context, null pointer on error
  */
  [[nodiscard]] GLXContext getRenderContext() const;


  //[-------------------------------------------------------]
  //[ Public virtual OpenGLRhi::RHIOpenGLContext methods      ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual bool isInitialized() const override;

  virtual void makeCurrent() const override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit OpenGLContextLinux(const OpenGLContextLinux& source) = delete;
  OpenGLContextLinux& operator =(const OpenGLContextLinux& source) = delete;

  /**
  *  @brief
  *    Constructor for primary context
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  *  @param[in] openGLRuntimeLinking
  *    OpenGL runtime linking instance, if null pointer this isn't a primary context
  *  @param[in] depthStencilAttachmentTextureFormat
  *    Depth stencil attachment texture format
  *  @param[in] nativeWindowHandle
  *    Optional native main window handle, can be a null handle
  *  @param[in] useExternalContext
  *    When true an own OpenGL context won't be created and the context pointed by "shareContextLinux" is ignored
  *  @param[in] shareContextLinux
  *    Optional share context, can be a null pointer
  */
  OpenGLContextLinux(RHIDynamicRHI& openGLRhi, OpenGLRuntimeLinking* openGLRuntimeLinking, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::handle nativeWindowHandle, bool useExternalContext, const OpenGLContextLinux* shareContextLinux = nullptr);

  /**
  *  @brief
  *    Create a OpenGL context
  *
  *  @param[in] depthStencilAttachmentTextureFormat
  *    Depth stencil attachment texture format
  *
  *  @return
  *    The created OpenGL context, null pointer on error
  */
  [[nodiscard]] GLXContext createOpenGLContext(RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat);


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RHIDynamicRHI&	mOpenGLRhi;				///< Owner OpenGL RHI instance
  RECore::handle mNativeWindowHandle;	///< OpenGL window, can be a null pointer (Window)
  Display*	mDisplay;				///< The X11 display connection, can be a null pointer
  bool		mOwnsX11Display;		///< Indicates if this instance owns the X11 display
  GLXContext	mWindowRenderContext;	///< The render context of the OpenGL dummy window, can be a null pointer
  bool		mUseExternalContext;
  bool		mOwnsRenderContext;		///< Does this context own the OpenGL render context?


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
