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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGLES3/OpenGLES3Context.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {

IOpenGLES3Context::IOpenGLES3Context([[maybe_unused]] RHIDynamicRHI &openGLES3Rhi, RECore::handle nativeWindowHandle,
                                     bool useExternalContext) :
  mNativeWindowHandle(nativeWindowHandle),
#if (defined(LINUX) && !defined(__ANDROID__))
  mX11Display(nullptr),
  mOwnsX11Display(true),
#endif
  mEGLDisplay(EGL_NO_DISPLAY),
  mEGLConfig(nullptr),
  mEGLContext(EGL_NO_CONTEXT),
  mDummyNativeWindow(NULL_HANDLE),
  mDummySurface(EGL_NO_SURFACE),
  mUseExternalContext(useExternalContext) {
#if (defined(LINUX) && !defined(__ANDROID__))
  const RERHI::RHIContext &context = openGLES3Rhi.getContext();

  // If the given RHI context is an X11 context use the display connection object provided by the context
  if (context.getType() == RERHI::RHIContext::ContextType::X11) {
    mX11Display = static_cast<const RERHI::X11Context &>(context).getDisplay();
    mOwnsX11Display = mX11Display == nullptr;
  }

  if (mOwnsX11Display) {
    mX11Display = XOpenDisplay(nullptr);
  }
#endif
}


void IOpenGLES3Context::deinitialize() {
  // Don't touch anything in case we don't have a display
  if (EGL_NO_DISPLAY != mEGLDisplay) {
    // Make "nothing" current
    eglMakeCurrent(mEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    // Destroy the EGL dummy surface
    if (EGL_NO_SURFACE != mDummySurface && eglDestroySurface(mEGLDisplay, mDummySurface) == EGL_FALSE) {
      // Error! Failed to destroy the used EGL dummy surface!
    }
    mDummySurface = EGL_NO_SURFACE;

    // Destroy the EGL context
    if (EGL_NO_CONTEXT != mEGLContext) {
      // Release all resources allocated by the shader compiler
      glReleaseShaderCompiler();

      // Destroy the EGL context
      if (eglDestroyContext(mEGLDisplay, mEGLContext) == EGL_FALSE) {
        // Error! Failed to destroy the used EGL context!
      }
      mEGLContext = EGL_NO_CONTEXT;
    }

    // Return EGL to it's state at thread initialization
    if (eglReleaseThread() == EGL_FALSE) {
      // Error! Failed to release the EGL thread!
    }

    // Terminate the EGL display
    if (eglTerminate(mEGLDisplay) == EGL_FALSE) {
      // Error! Failed to terminate the used EGL display!
    }
    mEGLDisplay = EGL_NO_DISPLAY;
    mEGLConfig = nullptr;

    // Destroy the dummy native window, if required
#ifdef _WIN32
    if (NULL_HANDLE == mNativeWindowHandle && NULL_HANDLE != mDummyNativeWindow)
          {
            ::DestroyWindow(mDummyNativeWindow);
            ::UnregisterClass(TEXT("OpenGLES3DummyNativeWindow"), ::GetModuleHandle(nullptr));
          }
#elif (defined(LINUX) && !defined(__ANDROID__))
    // Destroy the dummy native window
    if (NULL_HANDLE == mNativeWindowHandle && NULL_HANDLE != mDummyNativeWindow) {
      ::XDestroyWindow(mX11Display, mDummyNativeWindow);
    }

    // Close the X server display connection
    if (nullptr != mX11Display && mOwnsX11Display) {
      ::XCloseDisplay(mX11Display);
      mX11Display = nullptr;
    }
#endif
    mDummyNativeWindow = NULL_HANDLE;
  }
}

EGLConfig IOpenGLES3Context::chooseConfig(RECore::uint32 multisampleAntialiasingSamples) const {
  // Try to find a working EGL configuration
  EGLConfig eglConfig = nullptr;
  EGLint numberOfConfigurations = 0;
  bool chooseConfigCapitulated = false;
  EGLint multisampleAntialiasingSampleBuffers = 0;
  EGLint multisampleAntialiasingSamplesCurrent = static_cast<EGLint>(multisampleAntialiasingSamples);
  do {
    // Get the current multisample antialiasing settings
    const bool multisampleAntialiasing = (multisampleAntialiasingSamplesCurrent >
                                          1);  // Multisample antialiasing with just one sample per pixel isn't real multisample, is it? :D
    multisampleAntialiasingSampleBuffers = multisampleAntialiasing ? 1 : 0;

    // Set desired configuration
    const EGLint configAttribs[] =
      {
        EGL_LEVEL, 0,                    // Frame buffer level
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,              // Which types of EGL surfaces are supported
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,          // Which client APIs are supported
        EGL_DEPTH_SIZE, EGL_DONT_CARE,              // Bits of Z in the depth buffer
        EGL_SAMPLE_BUFFERS,
        multisampleAntialiasingSampleBuffers,  // Number of multisample buffers (enable/disable multisample antialiasing)
        EGL_SAMPLES,
        multisampleAntialiasingSamplesCurrent,  // Number of samples per pixel (multisample antialiasing samples)
        EGL_BUFFER_SIZE, 16,
        EGL_NONE
      };

    // Choose exactly one matching configuration
    if (eglChooseConfig(mEGLDisplay, configAttribs, &eglConfig, 1, &numberOfConfigurations) == EGL_FALSE ||
        numberOfConfigurations < 1) {
      // Can we change something on the multisample antialiasing? (may be the cause that no configuration was found!)
      if (multisampleAntialiasing) {
        if (multisampleAntialiasingSamplesCurrent > 8) {
          multisampleAntialiasingSamplesCurrent = 8;
        } else if (multisampleAntialiasingSamplesCurrent > 4) {
          multisampleAntialiasingSamplesCurrent = 4;
        } else if (multisampleAntialiasingSamplesCurrent > 2) {
          multisampleAntialiasingSamplesCurrent = 2;
        } else if (2 == multisampleAntialiasingSamplesCurrent) {
          multisampleAntialiasingSamplesCurrent = 0;
        }
      } else {
        // Don't mind, forget it...
        chooseConfigCapitulated = true;
      }
    }
  } while (numberOfConfigurations < 1 && !chooseConfigCapitulated);

  // Done
  return eglConfig;
}


EGLBoolean IOpenGLES3Context::makeCurrent(EGLSurface eglSurface) {
  // Use the EGL dummy surface?
  if (nullptr == eglSurface) {
    eglSurface = mDummySurface;
  }

  // Make the EGL surface to the current one
  return eglMakeCurrent(mEGLDisplay, eglSurface, eglSurface, mEGLContext);
}

bool IOpenGLES3Context::initialize(RECore::uint32 multisampleAntialiasingSamples) {
  if (mUseExternalContext) {
    return true;
  }

  // Get display
#if (defined(LINUX) && !defined(__ANDROID__))
  mEGLDisplay = eglGetDisplay(static_cast<EGLNativeDisplayType>(mX11Display));
#else
  mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#endif
  if (EGL_NO_DISPLAY != mEGLDisplay) {
    // Initialize EGL
    EGLint eglMajorVersion, eglMinorVersion;
    if (eglInitialize(mEGLDisplay, &eglMajorVersion, &eglMinorVersion) == EGL_TRUE) {
      // Choose a EGL configuration
      mEGLConfig = chooseConfig(multisampleAntialiasingSamples);

      // We can only go on if a EGL configuration was chosen properly
      if (mEGLConfig) {
        // Create context (request an version 3 client)
        // TODO(naetherm) Add support for the "GL_KHR_no_error"-extension
        const EGLint contextAttribs[] = {
          EGL_CONTEXT_CLIENT_VERSION, 3,
#ifdef DEBUG
          EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR, // TODO(sw) make it possible to enable it from outside during runtime
#endif
          EGL_NONE
        };
        mEGLContext = eglCreateContext(mEGLDisplay, mEGLConfig, EGL_NO_CONTEXT, contextAttribs);
        if (EGL_NO_CONTEXT != mEGLContext) {
          // Create a dummy native window?
          if (NULL_HANDLE != mNativeWindowHandle) {
            // There's no need to create a dummy native window, we've got a real native window to work with :D
            mDummyNativeWindow = (EGLNativeWindowType) mNativeWindowHandle;  // Interesting - in here, we have an OS dependent cast issue when using C++ casts: While we would need
            // reinterpret_cast<EGLNativeWindowType>(nativeWindowHandle) under Microsoft Windows ("HWND"), we would need static_cast<EGLNativeWindowType>(nativeWindowHandle)
            // under Linux ("int")... so, to avoid #ifdefs, we just use old school c-style casts in here...

#ifdef __ANDROID__
            // Reconfigure the ANativeWindow buffers to match
                  EGLint format;
                  eglGetConfigAttrib(mEGLDisplay, mEGLConfig, EGL_NATIVE_VISUAL_ID, &format);
                  ANativeWindow_setBuffersGeometry(reinterpret_cast<ANativeWindow*>(mNativeWindowHandle), 0, 0, format);
#endif
          } else {
            // Create the dummy native window
#ifdef _WIN32
            HINSTANCE moduleHandle = ::GetModuleHandle(nullptr);
                  WNDCLASS windowClass;
                  windowClass.hInstance	  = moduleHandle;
                  windowClass.lpszClassName = TEXT("OpenGLES3DummyNativeWindow");
                  windowClass.lpfnWndProc	  = DefWindowProc;
                  windowClass.style		  = 0;
                  windowClass.hIcon		  = nullptr;
                  windowClass.hCursor		  = nullptr;
                  windowClass.lpszMenuName  = nullptr;
                  windowClass.cbClsExtra	  = 0;
                  windowClass.cbWndExtra	  = 0;
                  windowClass.hbrBackground = nullptr;
                  ::RegisterClass(&windowClass);
                  mDummyNativeWindow = ::CreateWindow(TEXT("OpenGLES3DummyNativeWindow"), TEXT("PFormat"), WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 8, 8, HWND_DESKTOP, nullptr, moduleHandle, nullptr);
#elif (defined(LINUX) && !defined(__ANDROID__))
            // Create dummy window
            XSetWindowAttributes xSetWindowAttributes;
            xSetWindowAttributes.event_mask = 0;
            xSetWindowAttributes.border_pixel = 0;
            mDummyNativeWindow = ::XCreateWindow(mX11Display, DefaultRootWindow(mX11Display), 0, 0, 300, 300, 0,
                                                 CopyFromParent, InputOutput, CopyFromParent,
                                                 CWBorderPixel | CWEventMask, &xSetWindowAttributes);
#endif
          }

          // Create an EGL dummy surface
          mDummySurface = eglCreateWindowSurface(mEGLDisplay, mEGLConfig, mDummyNativeWindow, nullptr);
          if (EGL_NO_SURFACE == mDummySurface) {
            // Error! Failed to create EGL dummy surface!
          }

          // Make the internal dummy surface to the currently used one
          if (makeCurrent(EGL_NO_SURFACE) == EGL_FALSE) {
            // Error! Failed to make the EGL dummy surface to the current one!
          }

          // Done
          return true;
        } else {
          // Error! Failed to create EGL context!
        }
      } else {
        // Error! Failed to choose EGL configuration! (OpenGL ES 3 not supported?)
      }
    } else {
      // Error! Failed to initialize EGL!
    }
  } else {
    // Error! Failed to get EGL default display!
  }

  // Error!
  return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
