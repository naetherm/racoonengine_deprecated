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
#include "RERHIOpenGLES3/RenderTarget/RHISwapChain.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


SwapChain::SwapChain(RERHI::RHIRenderPass &renderPass, RERHI::WindowHandle windowHandle
                     RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHISwapChain(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mNativeWindowHandle(windowHandle.nativeWindowHandle),
  mRenderWindow(windowHandle.renderWindow),
  mNewVerticalSynchronizationInterval(
    0)  // 0 instead of ~0u to ensure that we always set the swap interval at least once to have a known initial setting
{}

void SwapChain::getWidthAndHeight(uint32_t &width, uint32_t &height) const {
// TODO(naetherm) Use "eglQuerySurface()"
//	EGLint renderTargetWidth  = 1;
//	EGLint renderTargetHeight = 1;
//	eglQuerySurface(mOpenGLES3Context->getEGLDisplay(), mOpenGLES3Context->getEGLDummySurface(), EGL_HEIGHT, &renderTargetHeight);
// Return stored width and height when both valid
  if (nullptr != mRenderWindow) {
    mRenderWindow->getWidthAndHeight(width, height);
    return;
  }
#ifdef _WIN32
  // Is there a valid native OS window?
          if (NULL_HANDLE != mNativeWindowHandle)
          {
            // Get the width and height
            long swapChainWidth  = 1;
            long swapChainHeight = 1;
            {
              // Get the client rectangle of the native output window
              // -> Don't use the width and height stored in "DXGI_SWAP_CHAIN_DESC" -> "DXGI_MODE_DESC"
              //    because it might have been modified in order to avoid zero values
              RECT rect;
              ::GetClientRect(reinterpret_cast<HWND>(mNativeWindowHandle), &rect);

              // Get the width and height...
              swapChainWidth  = rect.right  - rect.left;
              swapChainHeight = rect.bottom - rect.top;

              // ... and ensure that none of them is ever zero
              if (swapChainWidth < 1)
              {
                swapChainWidth = 1;
              }
              if (swapChainHeight < 1)
              {
                swapChainHeight = 1;
              }
            }

            // Write out the width and height
            width  = static_cast<UINT>(swapChainWidth);
            height = static_cast<UINT>(swapChainHeight);
          }
          else
#elif defined(__ANDROID__)
  if (NULL_HANDLE != mNativeWindowHandle)
          {
            //TODO(sw) get size on Android
            width = height = 1;
          }
          else
#elif defined LINUX
  if (NULL_HANDLE != mNativeWindowHandle) {
    IOpenGLES3Context &openGLES3Context = static_cast<RHIDynamicRHI &>(getRhi()).getOpenGLES3Context();

// TODO(sw) Reuse X11 display from "Frontend" -> for now reuse it from the OpenGL ES 3 context
    Display *display = openGLES3Context.getX11Display();

// Get the width and height...
    ::Window rootWindow = 0;
    int positionX = 0, positionY = 0;
    unsigned int unsignedWidth = 0, unsignedHeight = 0, border = 0, depth = 0;
    XGetGeometry(display, mNativeWindowHandle, &rootWindow, &positionX, &positionY, &unsignedWidth, &unsignedHeight,
                 &border, &depth);

// ... and ensure that none of them is ever zero
    if (unsignedWidth < 1) {
      unsignedWidth = 1;
    }
    if (unsignedHeight < 1) {
      unsignedHeight = 1;
    }

// Done
    width = unsignedWidth;
    height = unsignedHeight;
  } else
#else
#error "Unsupported platform"
#endif
  {
// Set known default return values
    width = 1;
    height = 1;
  }
}

void SwapChain::present() {
  if (nullptr != mRenderWindow) {
    mRenderWindow->present();
  } else {
    const IOpenGLES3Context &openGLES3Context = static_cast<RHIDynamicRHI &>(getRhi()).getOpenGLES3Context();
    const EGLDisplay eglDisplay = openGLES3Context.getEGLDisplay();

// Set new vertical synchronization interval?
// -> We do this in here to avoid having to use "eglMakeCurrent()" to often at multiple places
    if (~0u != mNewVerticalSynchronizationInterval) {
      eglSwapInterval(eglDisplay, static_cast<EGLint>(mNewVerticalSynchronizationInterval));
      mNewVerticalSynchronizationInterval = ~0u;
    }

// Swap buffers
    eglSwapBuffers(eglDisplay, openGLES3Context.getEGLDummySurface());
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
