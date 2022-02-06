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
*    Windows OpenGL context class
*/
class OpenGLContextWindows : public IOpenGLContext
{


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class OpenGLRhi;


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] depthStencilAttachmentTextureFormat
  *    Depth stencil attachment texture format
  *  @param[in] nativeWindowHandle
  *    Optional native main window handle, can be a null handle
  *  @param[in] shareContextWindows
  *    Optional share context, can be a null pointer
  */
  inline OpenGLContextWindows(RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::handle nativeWindowHandle, const OpenGLContextWindows* shareContextWindows = nullptr) :
    OpenGLContextWindows(nullptr, depthStencilAttachmentTextureFormat, nativeWindowHandle, shareContextWindows)
  {}

  /**
  *  @brief
  *    Destructor
  */
  virtual ~OpenGLContextWindows() override
  {
    // Release the device context of the OpenGL window
    if (NULL_HANDLE != mWindowDeviceContext)
    {
      // Is the device context of the OpenGL window is the currently active OpenGL device context?
      if (wglGetCurrentDC() == mWindowDeviceContext)
      {
        wglMakeCurrent(nullptr, nullptr);
      }

      // Destroy the render context of the OpenGL window
      if (NULL_HANDLE != mWindowRenderContext && mOwnsRenderContext)
      {
        wglDeleteContext(mWindowRenderContext);
      }

      // Release the device context of the OpenGL window
      if (NULL_HANDLE != mNativeWindowHandle)
      {
        ::ReleaseDC(reinterpret_cast<HWND>(mNativeWindowHandle), mWindowDeviceContext);
      }
    }

    // Destroy the OpenGL dummy window, in case there's one
    if (NULL_HANDLE != mDummyWindow)
    {
      // Destroy the OpenGL dummy window
      ::DestroyWindow(reinterpret_cast<HWND>(mDummyWindow));

      // Unregister the window class for the OpenGL dummy window
      ::UnregisterClass(TEXT("OpenGLDummyWindow"), ::GetModuleHandle(nullptr));
    }
  }

  /**
  *  @brief
  *    Return the primary device context
  *
  *  @return
  *    The primary device context, null pointer on error
  */
  [[nodiscard]] inline HDC getDeviceContext() const
  {
    return mWindowDeviceContext;
  }

  /**
  *  @brief
  *    Return the primary render context
  *
  *  @return
  *    The primary render context, null pointer on error
  */
  [[nodiscard]] inline HGLRC getRenderContext() const
  {
    return mWindowRenderContext;
  }


  //[-------------------------------------------------------]
  //[ Public virtual OpenGLRhi::RHIOpenGLContext methods      ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual bool isInitialized() const override
  {
    return (nullptr != mWindowRenderContext);
  }

  inline virtual void makeCurrent() const override
  {
    wglMakeCurrent(mWindowDeviceContext, mWindowRenderContext);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit OpenGLContextWindows(const OpenGLContextWindows& source) = delete;
  OpenGLContextWindows& operator =(const OpenGLContextWindows& source) = delete;

  /**
  *  @brief
  *    Constructor for primary context
  *
  *  @param[in] openGLRuntimeLinking
  *    OpenGL runtime linking instance, if null pointer this isn't a primary context
  *  @param[in] depthStencilAttachmentTextureFormat
  *    Depth stencil attachment texture format
  *  @param[in] nativeWindowHandle
  *    Optional native main window handle, can be a null handle
  *  @param[in] shareContextWindows
  *    Optional share context, can be a null pointer
  */
  OpenGLContextWindows(OpenGLRuntimeLinking* openGLRuntimeLinking, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::handle nativeWindowHandle, const OpenGLContextWindows* shareContextWindows = nullptr) :
    IOpenGLContext(openGLRuntimeLinking),
    mNativeWindowHandle(nativeWindowHandle),
    mDummyWindow(NULL_HANDLE),
    mWindowDeviceContext(NULL_HANDLE),
    mWindowRenderContext(NULL_HANDLE),
    mOwnsRenderContext(true)
  {
    // Create a OpenGL dummy window?
    // -> Under Microsoft Windows, a OpenGL context is always coupled to a window... even if we're not going to render into a window at all...
    if (NULL_HANDLE == mNativeWindowHandle)
    {
      // Setup and register the window class for the OpenGL dummy window
      WNDCLASS windowDummyClass;
      windowDummyClass.hInstance		= ::GetModuleHandle(nullptr);
      windowDummyClass.lpszClassName	= TEXT("OpenGLDummyWindow");
      windowDummyClass.lpfnWndProc	= DefWindowProc;
      windowDummyClass.style			= 0;
      windowDummyClass.hIcon			= nullptr;
      windowDummyClass.hCursor		= nullptr;
      windowDummyClass.lpszMenuName	= nullptr;
      windowDummyClass.cbClsExtra		= 0;
      windowDummyClass.cbWndExtra		= 0;
      windowDummyClass.hbrBackground	= nullptr;
      ::RegisterClass(&windowDummyClass);

      // Create the OpenGL dummy window
      mNativeWindowHandle = mDummyWindow = reinterpret_cast<RECore::handle>(::CreateWindow(TEXT("OpenGLDummyWindow"), TEXT("PFormat"), WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 8, 8, HWND_DESKTOP, nullptr, ::GetModuleHandle(nullptr), nullptr));
    }

    // Is there a valid window handle?
    if (NULL_HANDLE != mNativeWindowHandle)
    {
      // Get the device context of the OpenGL window
      mWindowDeviceContext = ::GetDC(reinterpret_cast<HWND>(mNativeWindowHandle));
      if (NULL_HANDLE != mWindowDeviceContext)
      {
        // Get the color depth of the desktop
        int bits = 32;
        {
          HDC deskTopDC = ::GetDC(nullptr);
          bits = ::GetDeviceCaps(deskTopDC, BITSPIXEL);
          ::ReleaseDC(nullptr, deskTopDC);
        }

        // Get the first best pixel format
        // TODO(naetherm) Use more detailed color and depth/stencil information from render pass
        const BYTE depthBufferBits = (RERHI::TextureFormat::Enum::UNKNOWN == depthStencilAttachmentTextureFormat) ? 0u : 24u;
        const PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
          {
            sizeof(PIXELFORMATDESCRIPTOR),	// Size of this pixel format descriptor
            1,								// Version number
            PFD_DRAW_TO_WINDOW |			// Format must support window
            PFD_SUPPORT_OPENGL |			// Format must support OpenGL
            PFD_DOUBLEBUFFER,				// Must support double buffering
            PFD_TYPE_RGBA,					// Request an RGBA format
            static_cast<UCHAR>(bits),		// Select our color depth
            0, 0, 0, 0, 0, 0,				// Color bits ignored
            0,								// No alpha buffer
            0,								// Shift bit ignored
            0,								// No accumulation buffer
            0, 0, 0, 0,						// Accumulation bits ignored
            depthBufferBits,				// Z-buffer (depth buffer)
            0,								// No stencil buffer
            0,								// No auxiliary buffer
            PFD_MAIN_PLANE,					// Main drawing layer
            0,								// Reserved
            0, 0, 0							// Layer masks ignored
          };
        const int pixelFormat = ::ChoosePixelFormat(mWindowDeviceContext, &pixelFormatDescriptor);
        if (0 != pixelFormat)
        {
          // Set the pixel format
          ::SetPixelFormat(mWindowDeviceContext, pixelFormat, &pixelFormatDescriptor);

          // Lookout! OpenGL context sharing chaos: https://www.opengl.org/wiki/OpenGL_Context
          // "State" objects are not shared between contexts, including but not limited to:
          // - Vertex Array Objects (VAOs)
          // - Framebuffer Objects (FBOs)
          // -> Keep away from "wglShareLists()" and the share context parameter of "wglCreateContextAttribsARB()" and just share the OpenGL render context instead
          if (nullptr != shareContextWindows)
          {
            mWindowRenderContext = shareContextWindows->getRenderContext();
            mOwnsRenderContext = false;
          }
          else
          {
            // Create a legacy OpenGL render context
            HGLRC legacyRenderContext = wglCreateContext(mWindowDeviceContext);
            if (NULL_HANDLE != legacyRenderContext)
            {
              // Make the legacy OpenGL render context to the current one
              wglMakeCurrent(mWindowDeviceContext, legacyRenderContext);

              // Load the >= OpenGL 3.0 entry points
              if (loadOpenGL3EntryPoints())
              {
                // Create the render context of the OpenGL window
                mWindowRenderContext = createOpenGLContext(nullptr);

                // Destroy the legacy OpenGL render context
                wglMakeCurrent(nullptr, nullptr);
                wglDeleteContext(legacyRenderContext);

                // If there's an OpenGL context, do some final initialization steps
                if (NULL_HANDLE != mWindowRenderContext)
                {
                  // Make the OpenGL context to the current one
                  // TODO(naetherm) Review this, might cause issues when creating a context while a program is running
                  wglMakeCurrent(mWindowDeviceContext, mWindowRenderContext);
                }
              }
              else
              {
                // Error, failed to load >= OpenGL 3 entry points!
              }
            }
            else
            {
              // Error, failed to create a legacy OpenGL render context!
            }
          }
        }
        else
        {
          // Error, failed to choose a pixel format!
        }
      }
      else
      {
        // Error, failed to obtain the device context of the OpenGL window!
      }
    }
    else
    {
      // Error, failed to create the OpenGL window!
    }
  }

  /**
  *  @brief
  *    Create a OpenGL context
  *
  *  @param[in] shareContextWindows
  *    Optional share context, can be a null pointer
  *
  *  @return
  *    The created OpenGL context, null pointer on error
  */
  [[nodiscard]] HGLRC createOpenGLContext(const OpenGLContextWindows* shareContextWindows)
  {
    // Disable the following warning, we can't do anything to resolve this warning
    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_DISABLE_MSVC(4191)	// warning C4191: 'reinterpret_cast' : unsafe conversion from 'PROC' to '<x>'

    // Get the OpenGL extension wglGetExtensionsStringARB function pointer, we need it to check for further supported OpenGL extensions
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARBLocal = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(wglGetProcAddress("wglGetExtensionsStringARB"));
    if (nullptr != wglGetExtensionsStringARBLocal)
    {
      // Get the available WGL extensions as string
      const char* extensions = wglGetExtensionsStringARBLocal(mWindowDeviceContext);

      // Check whether or not "WGL_ARB_create_context" is a substring of the WGL extension string meaning that this OpenGL extension is supported
      if (nullptr != strstr(extensions, "WGL_ARB_create_context"))
      {
        // Get the OpenGL extension "wglCreateContextAttribsARB" function pointer
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
        if (nullptr != wglCreateContextAttribsARB)
        {
          // Create the OpenGL context
          // -> OpenGL 4.1 (the best OpenGL version Mac OS X 10.11 supports, so lowest version we have to support)
          static const int ATTRIBUTES[] =
            {
              // We want an OpenGL context
              WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
              WGL_CONTEXT_MINOR_VERSION_ARB, 1,
              WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef DEBUG
              // "WGL_CONTEXT_DEBUG_BIT_ARB" comes from the "GL_ARB_debug_output"-extension
									WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#else
              // "WGL_ARB_create_context_no_error" and "GL_KHR_no_error"-extension
              ((nullptr != strstr(extensions, "WGL_ARB_create_context_no_error")) ? WGL_CONTEXT_OPENGL_NO_ERROR_ARB : 0), 1,
#endif
              // Done
              0
            };

          // Lookout! OpenGL context sharing chaos: https://www.opengl.org/wiki/OpenGL_Context
          // "State" objects are not shared between contexts, including but not limited to:
          // - Vertex Array Objects (VAOs)
          // - Framebuffer Objects (FBOs)
          // -> Practically, this makes a second OpenGL context only useful for resource background loading
          const HGLRC hglrc = wglCreateContextAttribsARB(mWindowDeviceContext, (nullptr != shareContextWindows) ? shareContextWindows->getRenderContext() : nullptr, ATTRIBUTES);
          if (nullptr != hglrc)
          {
            // Done
            return hglrc;
          }
          else
          {
            // Error, context creation failed!
            return NULL_HANDLE;
          }
        }
        else
        {
          // Error, failed to obtain the "wglCreateContextAttribsARB" function pointer (wow, something went terrible wrong!)
          return NULL_HANDLE;
        }
      }
      else
      {
        // Error, the OpenGL extension "WGL_ARB_create_context" is not supported... as a result we can't create an OpenGL context!
        return NULL_HANDLE;
      }
    }
    else
    {
      // Error, failed to obtain the "wglGetExtensionsStringARB" function pointer (wow, something went terrible wrong!)
      return NULL_HANDLE;
    }

    // Restore the previous warning configuration
    PRAGMA_WARNING_POP
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RECore::handle mNativeWindowHandle;	///< OpenGL window, can be a null pointer (HWND)
  RECore::handle mDummyWindow;			///< OpenGL dummy window, can be a null pointer (HWND)
  HDC			mWindowDeviceContext;	///< The device context of the OpenGL dummy window, can be a null pointer
  HGLRC		mWindowRenderContext;	///< The render context of the OpenGL dummy window, can be a null pointer
  bool		mOwnsRenderContext;		///< Does this context owns the OpenGL render context?


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
