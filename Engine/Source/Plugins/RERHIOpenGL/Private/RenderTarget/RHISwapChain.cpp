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
#include "RERHIOpenGL/RenderTarget/RHISwapChain.h"
#include "RERHIOpenGL/RenderTarget/RHIRenderPass.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"
#if defined(LINUX)
#include "RERHIOpenGL/Linux/RHIOpenGLContextLinux.h"
#elif defined(WIN32)
#include "RERHIOpenGL/Windows/RHIOpenGLContextWindows.h"
#endif


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
SwapChain::SwapChain(RERHI::RHIRenderPass& renderPass, RERHI::WindowHandle windowHandle, [[maybe_unused]] bool useExternalContext RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
RHISwapChain(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mNativeWindowHandle(windowHandle.nativeWindowHandle),
#ifdef _WIN32
mOpenGLContext(RHI_NEW(renderPass.getRhi().getContext(), OpenGLContextWindows)(static_cast<const RenderPass&>(renderPass).getDepthStencilAttachmentTextureFormat(), windowHandle.nativeWindowHandle, static_cast<const OpenGLContextWindows*>(&static_cast<RHIDynamicRHI&>(renderPass.getRhi()).getOpenGLContext()))),
#elif defined LINUX
mOpenGLContext(RHI_NEW(renderPass.getRhi().getContext(), OpenGLContextLinux)(static_cast<RHIDynamicRHI&>(renderPass.getRhi()), static_cast<const RenderPass&>(renderPass).getDepthStencilAttachmentTextureFormat(), windowHandle.nativeWindowHandle, useExternalContext, static_cast<const OpenGLContextLinux*>(&static_cast<RHIDynamicRHI&>(renderPass.getRhi()).getOpenGLContext()))),
#else
#error "Unsupported platform"
#endif
mOwnsOpenGLContext(true),
mRenderWindow(windowHandle.renderWindow),
mVerticalSynchronizationInterval(0),
mNewVerticalSynchronizationInterval(0)	// 0 instead of ~0u to ensure that we always set the swap interval at least once to have a known initial setting
{}

SwapChain::~SwapChain()
{
  if (mOwnsOpenGLContext)
  {
    RHI_DELETE(getRhi().getContext(), RHIOpenGLContext, mOpenGLContext);
  }
}

void SwapChain::getWidthAndHeight(uint32_t& width, uint32_t& height) const
{
  // Return stored width and height when both valid
  if (nullptr != mRenderWindow)
  {
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
#elif defined LINUX
  if (NULL_HANDLE != mNativeWindowHandle)
  {
    RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());
    Display* display = static_cast<const OpenGLContextLinux&>(openGLRhi.getOpenGLContext()).getDisplay();

    // Get the width and height...
    ::Window rootWindow = 0;
    int positionX = 0, positionY = 0;
    unsigned int unsignedWidth = 0, unsignedHeight = 0, border = 0, depth = 0;
    if (nullptr != display)
    {
      XGetGeometry(display, mNativeWindowHandle, &rootWindow, &positionX, &positionY, &unsignedWidth, &unsignedHeight, &border, &depth);
    }

    // ... and ensure that none of them is ever zero
    if (unsignedWidth < 1)
    {
      unsignedWidth = 1;
    }
    if (unsignedHeight < 1)
    {
      unsignedHeight = 1;
    }

    // Done
    width = unsignedWidth;
    height = unsignedHeight;
  }
  else
#else
#error "Unsupported platform"
#endif
  {
    // Set known default return values
    width  = 1;
    height = 1;
  }
}

void SwapChain::present()
{
  if (nullptr != mRenderWindow)
  {
    mRenderWindow->present();
    return;
  }
#ifdef _WIN32
  {
				// Set new vertical synchronization interval?
				// -> We do this in here to avoid having to use "wglMakeCurrent()"/"glXMakeCurrent()" to often at multiple places
				if (~0u != mNewVerticalSynchronizationInterval)
				{
					const Extensions& extensions = static_cast<RHIDynamicRHI&>(getRhi()).getExtensions();
					if (extensions.isWGL_EXT_swap_control())
					{
						// Use adaptive vertical synchronization if possible
						wglSwapIntervalEXT((extensions.isWGL_EXT_swap_control_tear() && mNewVerticalSynchronizationInterval > 0) ? -static_cast<int>(mNewVerticalSynchronizationInterval) : static_cast<int>(mNewVerticalSynchronizationInterval));
					}
					mVerticalSynchronizationInterval = mNewVerticalSynchronizationInterval;
					mNewVerticalSynchronizationInterval = ~0u;
				}

				// Swap buffers
				::SwapBuffers(static_cast<OpenGLContextWindows*>(mOpenGLContext)->getDeviceContext());
				if (mVerticalSynchronizationInterval > 0)
				{
					glFinish();
				}
			}
#elif defined LINUX
  // TODO(naetherm) Add support for vertical synchronization and adaptive vertical synchronization: "GLX_EXT_swap_control" and "GLX_EXT_swap_control_tear"
  if (NULL_HANDLE != mNativeWindowHandle)
  {
    glXSwapBuffers(static_cast<const OpenGLContextLinux&>(static_cast<RHIDynamicRHI&>(getRhi()).getOpenGLContext()).getDisplay(), mNativeWindowHandle);
  }
#else
#error "Unsupported platform"
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
