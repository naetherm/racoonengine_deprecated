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
#include "RERHI/RERHI.h"
#include "RERHI/RenderTarget/RHIRenderTarget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI
{


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract render window interface which is used to implement some platform specific functionality regarding render window needed by the swap chain implementation
*
*  @remarks
*    This interface can be used to implement the needed platform specific functionality for a platform which isn't known by the RHI implementation
*    e.g. the user uses a windowing library (e.g. SDL2) which abstracts the window handling on different windowing platforms (e.g. Win32 or Linux/Wayland)
*    and the application should run on a windowing platform which isn't supported by the swap chain implementation itself.
*/
class IRenderWindow
{

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~IRenderWindow()
  {}

  // Public virtual Rhi::IRendererWindow methods
public:
  /**
  *  @brief
  *    Return the width and height of the render window
  *
  *  @param[out] width
  *    Width of the render window
  *  @param[out] height
  *    Height of the render window
  */
  virtual void getWidthAndHeight(RECore::uint32& width, RECore::uint32& height) const = 0;

  /**
  *  @brief
  *    Present the content of the current back buffer
  *
  *  @note
  *    - Swap of front and back buffer
  */
  virtual void present() = 0;

  // Protected methods
protected:
  inline IRenderWindow()
  {}

  explicit IRenderWindow(const IRenderWindow& source) = delete;
  IRenderWindow& operator =(const IRenderWindow& source) = delete;

};

/**
*  @brief
*    Abstract swap chain interface
*/
class RHISwapChain : public RHIRenderTarget
{

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHISwapChain() override
  {
#ifdef RHI_STATISTICS
    // Update the statistics
				--getRhi().getStatistics().currentNumberOfSwapChains;
#endif
  }

  // Public virtual Rhi::RHISwapChain methods
public:
  /**
  *  @brief
  *    Return the native window handle
  *
  *  @return
  *    Native window handle the swap chain is using as output window, can be a null handle
  */
  [[nodiscard]] virtual RECore::handle getNativeWindowHandle() const = 0;

  /**
  *  @brief
  *    Set vertical synchronization interval
  *
  *  @param[in] synchronizationInterval
  *    Synchronization interval, >0 if vertical synchronization should be used, else zero
  */
  virtual void setVerticalSynchronizationInterval(RECore::uint32 synchronizationInterval) = 0;

  /**
  *  @brief
  *    Present the content of the current back buffer
  *
  *  @note
  *    - Swap of front and back buffer
  */
  virtual void present() = 0;

  /**
  *  @brief
  *    Call this method whenever the size of the native window was changed
  */
  virtual void resizeBuffers() = 0;

  /**
  *  @brief
  *    Return the current fullscreen state
  *
  *  @return
  *    "true" if fullscreen, else "false"
  */
  [[nodiscard]] virtual bool getFullscreenState() const = 0;

  /**
  *  @brief
  *    Set the current fullscreen state
  *
  *  @param[in] fullscreen
  *    "true" if fullscreen, else "false"
  */
  virtual void setFullscreenState(bool fullscreen) = 0;

  /**
  *  @brief
  *    Set an render window instance
  *
  *  @param[in] renderWindow
  *    The render window interface instance, can be a null pointer, if valid the instance must stay valid as long as it's connected to the swap chain instance
  *
  *  @remarks
  *    This method can be used to override the platform specific handling for retrieving window size and doing an buffer swap on the render window (aka present).
  *    An instance can be set when the user don't want that the swap chain itself tempers with the given window handle (the handle might be invalid but non zero)
  *    e.g. the user uses a windowing library (e.g. SDL2) which abstracts the window handling on different windowing platforms (e.g. Win32 or Linux/Wayland) and
  *    the application should run on a windowing platform which isn't supported by the swap chain implementation itself.
  */
  virtual void setRenderWindow(IRenderWindow* renderWindow) = 0;

  // Protected methods
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] renderPass
  *    Render pass to use, the swap chain keeps a reference to the render pass
  */
  inline explicit RHISwapChain(RHIRenderPass& renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHIRenderTarget(ResourceType::SWAP_CHAIN, renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
  {
#ifdef RHI_STATISTICS
    // Update the statistics
				++getRhi().getStatistics().numberOfCreatedSwapChains;
				++getRhi().getStatistics().currentNumberOfSwapChains;
#endif
  }

  explicit RHISwapChain(const RHISwapChain& source) = delete;
  RHISwapChain& operator =(const RHISwapChain& source) = delete;

};

typedef RECore::SmartRefCount<RHISwapChain> RHISwapChainPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
