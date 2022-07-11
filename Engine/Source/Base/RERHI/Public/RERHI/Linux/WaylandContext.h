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
#include "RERHI/RERHI.h"
#include "RERHI/RHIContext.h"


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
*    Wayland version of the context class
*/
class RERHI_API WaylandContext final : public RHIContext
{

  // Public methods
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] log
  *    Log instance to use, the log instance must stay valid as long as the RHI instance exists
  *  @param[in] assert
  *    Assert instance to use, the assert instance must stay valid as long as the RHI instance exists
  *  @param[in] allocator
  *    Allocator instance to use, the allocator instance must stay valid as long as the RHI instance exists
  *  @param[in] display
  *    The Wayland display connection
  *  @param[in] surface
  *    The Wayland surface
  *  @param[in] useExternalContext
  *    Indicates if an external RHI context is used; in this case the RHI itself has nothing to do with the creation/managing of an RHI context
  */
  inline WaylandContext(wl_display* display, wl_surface* surface = 0, bool useExternalContext = false) :
    RHIContext(1, useExternalContext, RHIContext::ContextType::WAYLAND),	// Under Wayland the surface (aka window) handle is not an integer, but the RHI implementation expects an integer as window handle so we give here an value != 0 so that a swap chain is created
    mDisplay(display),
    mSurface(surface)
  {}

  /**
  *  @brief
  *    Return the Wayland display connection
  *
  *  @return
  *    The Wayland display connection
  */
  [[nodiscard]] inline wl_display* getDisplay() const
  {
    return mDisplay;
  }

  /**
  *  @brief
  *    Return the Wayland surface
  *
  *  @return
  *    The Wayland surface
  */
  [[nodiscard]] inline wl_surface* getSurface() const
  {
    return mSurface;
  }

  // Private data
private:
  wl_display* mDisplay;
  wl_surface* mSurface;

};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
