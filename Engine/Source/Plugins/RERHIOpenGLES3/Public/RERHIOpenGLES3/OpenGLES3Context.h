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
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class IExtensions;


/**
*  @brief
*    Abstract OpenGL ES 3 context base interface
*/
class IOpenGLES3Context
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~IOpenGLES3Context()
  {
    // Everything must be done in "deinitialize()" because when we're in here, the shared libraries
    // are already unloaded and we are no longer allowed to use EGL or OpenGL ES 3 functions
  }

  /**
  *  @brief
  *    Return whether or not the context is properly initialized
  */
  [[nodiscard]] inline bool isInitialized() const
  {
    return (mUseExternalContext || EGL_NO_CONTEXT != getEGLContext());
  }

  /**
  *  @brief
  *    Return the handle of a native OS window which is valid as long as the RHI instance exists
  *
  *  @return
  *    The handle of a native OS window which is valid as long as the RHI instance exists, "NULL_HANDLE" if there's no such window
  */
  [[nodiscard]] inline RECore::handle getNativeWindowHandle() const
  {
    return mNativeWindowHandle;
  }

  /**
  *  @brief
  *    Return the used EGL display
  *
  *  @return
  *    The used EGL display, "EGL_NO_DISPLAY" on error
  */
  [[nodiscard]] inline EGLDisplay getEGLDisplay() const
  {
    return mEGLDisplay;
  }

  /**
  *  @brief
  *    Return the used EGL configuration
  *
  *  @return
  *    The used EGL configuration, null pointer on error
  */
  [[nodiscard]] inline EGLConfig getEGLConfig() const
  {
    return mEGLConfig;
  }

  /**
  *  @brief
  *    Return the used EGL context
  *
  *  @return
  *    The used EGL context, "EGL_NO_CONTEXT" on error
  */
  [[nodiscard]] inline EGLContext getEGLContext() const
  {
    return mEGLContext;
  }

  /**
  *  @brief
  *    Return the used EGL dummy surface
  *
  *  @return
  *    The used EGL dummy surface, "EGL_NO_SURFACE" on error
  */
  [[nodiscard]] inline EGLSurface getEGLDummySurface() const
  {
    return mDummySurface;
  }

  /**
  *  @brief
  *    Makes a given EGL surface to the currently used one
  *
  *  @param[in] eglSurface
  *    EGL surface to make to the current one, can be a null pointer, in this case an internal dummy surface is set
  *
  *  @return
  *    "EGL_TRUE" if all went fine, else "EGL_FALSE"
  */
  [[nodiscard]] EGLBoolean makeCurrent(EGLSurface eglSurface);


  //[-------------------------------------------------------]
  //[ Platform specific                                     ]
  //[-------------------------------------------------------]
#if defined(LINUX) && !defined(__ANDROID__)
  [[nodiscard]] inline ::Display* getX11Display() const
  {
    return mX11Display;
  }
#endif


  //[-------------------------------------------------------]
  //[ Public virtual RHIDynamicRHI::RHIOpenGLES3Context methods ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Initialize the context
  *
  *  @param[in] multisampleAntialiasingSamples
  *    Multisample antialiasing samples per pixel, <=1 means no antialiasing
  *
  *  @return
  *    "true" if all went fine, else "false"
  */
  [[nodiscard]] virtual bool initialize(uint32_t multisampleAntialiasingSamples);

  /**
  *  @brief
  *    Return the available extensions
  *
  *  @return
  *    The available extensions
  */
  [[nodiscard]] virtual const IExtensions& getExtensions() const = 0;


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLES3Rhi
  *    Owner OpenGL ES 3 RHI instance
  *  @param[in] nativeWindowHandle
  *    Handle of a native OS window which is valid as long as the RHI instance exists, "NULL_HANDLE" if there's no such window
  *  @param[in] useExternalContext
  *    When true an own OpenGL ES context won't be created
  */
  IOpenGLES3Context([[maybe_unused]] RHIDynamicRHI& openGLES3Rhi, RECore::handle nativeWindowHandle, bool useExternalContext);

  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] source
  *    Source to copy from
  */
  explicit IOpenGLES3Context(const IOpenGLES3Context& source) = delete;

  /**
  *  @brief
  *    De-initialize the context
  */
  void deinitialize();

  /**
  *  @brief
  *    Copy operator
  *
  *  @param[in] source
  *    Source to copy from
  *
  *  @return
  *    Reference to this instance
  */
  IOpenGLES3Context& operator =(const IOpenGLES3Context&) = delete;


  //[-------------------------------------------------------]
  //[ Protected virtual RHIDynamicRHI::RHIOpenGLES3Context methods ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Choose a EGL configuration
  *
  *  @param[in] multisampleAntialiasingSamples
  *    Multisample antialiasing samples per pixel
  *
  *  @return
  *    The chosen EGL configuration, a null pointer on error
  *
  *  @note
  *    - Automatically tries to find fallback configurations
  */
  [[nodiscard]] virtual EGLConfig chooseConfig(uint32_t multisampleAntialiasingSamples) const;


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  RECore::handle	mNativeWindowHandle;	///< Handle of a native OS window which is valid as long as the RHI instance exists, "NULL_HANDLE" if there's no such window
  // X11
#if (defined(LINUX) && !defined(__ANDROID__))
  ::Display	   *mX11Display;
  bool 			mOwnsX11Display;
#endif
  // EGL
  EGLDisplay			mEGLDisplay;
  // EGL
  EGLConfig			mEGLConfig;
  EGLContext			mEGLContext;
  EGLNativeWindowType	mDummyNativeWindow;	///< Native dummy window handle, can be identical to "mNativeWindowHandle" if it's in fact no dummy at all, can be "NULL_HANDLE"
  EGLSurface			mDummySurface;
  bool				mUseExternalContext;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
