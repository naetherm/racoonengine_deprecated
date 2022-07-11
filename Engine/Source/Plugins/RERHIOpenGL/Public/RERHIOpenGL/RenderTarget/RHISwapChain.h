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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class RHIOpenGLContext;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL swap chain class
*/
class SwapChain final : public RERHI::RHISwapChain
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] renderPass
  *    Render pass to use, the swap chain keeps a reference to the render pass
  *  @param[in] windowHandle
  *    Information about the window to render into
  *  @param[in] useExternalContext
  *    Indicates if an external RHI context is used; in this case the RHI itself has nothing to do with the creation/managing of an RHI context
  */
  SwapChain(RERHI::RHIRenderPass& renderPass, RERHI::WindowHandle windowHandle, [[maybe_unused]] bool useExternalContext RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  ~SwapChain() override;

  /**
  *  @brief
  *    Return the OpenGL context
  *
  *  @return
  *    The OpenGL context
  */
  [[nodiscard]] inline RHIOpenGLContext& getOpenGLContext() const
  {
    return *mOpenGLContext;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIRenderTarget methods             ]
  //[-------------------------------------------------------]
public:
  virtual void getWidthAndHeight(RECore::uint32& width, RECore::uint32& height) const override;


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHISwapChain methods                ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual RECore::handle getNativeWindowHandle() const override
  {
    return mNativeWindowHandle;
  }

  inline virtual void setVerticalSynchronizationInterval(RECore::uint32 synchronizationInterval) override
  {
    mNewVerticalSynchronizationInterval = synchronizationInterval;
  }

  virtual void present() override;

  inline virtual void resizeBuffers() override
  {
    // Nothing here
  }

  [[nodiscard]] inline virtual bool getFullscreenState() const override
  {
    // TODO(naetherm) Implement me
    return false;
  }

  inline virtual void setFullscreenState(bool) override
  {
    // TODO(naetherm) Implement me
  }

  inline virtual void setRenderWindow(RERHI::IRenderWindow* renderWindow) override
  {
    mRenderWindow = renderWindow;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), SwapChain, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit SwapChain(const SwapChain& source) = delete;
  SwapChain& operator =(const SwapChain& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RECore::handle			mNativeWindowHandle;	///< Native window handle window, can be a null handle
  RHIOpenGLContext*		mOpenGLContext;			///< OpenGL context, must be valid
  bool				mOwnsOpenGLContext;		///< Does this swap chain own the OpenGL context?
  RERHI::IRenderWindow* mRenderWindow;			///< Render window instance, can be a null pointer, don't destroy the instance since we don't own it
  RECore::uint32			mVerticalSynchronizationInterval;
  RECore::uint32			mNewVerticalSynchronizationInterval;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
