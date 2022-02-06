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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI
{


class IRenderWindow;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Holds information about an window into which the rendering should be done
*
*  @note
*    - One of those members must be valid
*/
struct WindowHandle final
{
  RECore::handle			nativeWindowHandle;	///< The native window handle
  IRenderWindow*	renderWindow;		///< A pointer to an "Rhi::IRenderWindow"-instance, can be a null pointer
#ifdef LINUX
  wl_surface*	waylandSurface;		///< A Wayland surface can't be put into a handle type, so we store a pointer to the Wayland surface here
#else
  void*		unused;				///< For writing e.g. "Rhi::WindowHandle{nativeWindowHandle, nullptr, nullptr}" for all platforms // TODO(naetherm) We might want to implement another solution like "WindowHandle::fromNativeWindowHandle()", "WindowHandle::fromRenderWindow()", "WindowHandle::fromWaylandSurface()", while there we could add a type and then using a data-union
#endif
};

/**
*  @brief
*    Map types
*
*  @note
*    - These constants directly map to Direct3D 10 & 11 constants, do not change them
*/
// TODO(naetherm) Comments
enum class MapType
{
  READ			   = 1,	///<
  WRITE			   = 2,	///<
  READ_WRITE		   = 3,	///<
  WRITE_DISCARD	   = 4,	///<
  WRITE_NO_OVERWRITE = 5	///<
};

/**
*  @brief
*    Map flags
*
*  @note
*    - These constants directly map to Direct3D 11 constants, do not change them
*/
struct MapFlag final
{
  enum Enum
  {
    DO_NOT_WAIT = 0x100000L	///< In case the resource is currently used when "Rhi::IRhi::map()" is called, let the method return with an error, cannot be used with "Rhi::MapType::WRITE_DISCARD" or "Rhi::MapType::WRITE_NO_OVERWRITE"
  };
};

/**
*  @brief
*    Clear flags
*/
struct ClearFlag final
{
  enum Enum
  {
    COLOR       = 1 << 0,		///< Clear color buffer
    DEPTH       = 1 << 1,		///< Clear depth buffer
    STENCIL     = 1 << 2,		///< Clear stencil buffer
    COLOR_DEPTH = COLOR | DEPTH	///< Clear color and depth buffer
  };
};

/**
*  @brief
*    Comparison function
*
*  @note
*    - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/bb204902%28v=vs.85%29.aspx are used in here
*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
*
*  @see
*    - "D3D12_COMPARISON_FUNC"-documentation for details
*/
enum class ComparisonFunc
{
  NEVER		  = 1,	///< Never pass the comparison
  LESS		  = 2,	///< If the source data is less than the destination data, the comparison passes
  EQUAL		  = 3,	///< If the source data is equal to the destination data, the comparison passes
  LESS_EQUAL	  = 4,	///< If the source data is less than or equal to the destination data, the comparison passes
  GREATER		  = 5,	///< If the source data is greater than the destination data, the comparison passes
  NOT_EQUAL	  = 6,	///< If the source data is not equal to the destination data, the comparison passes
  GREATER_EQUAL = 7,	///< If the source data is greater than or equal to the destination data, the comparison passes
  ALWAYS		  = 8	///< Always pass the comparison
};

/**
*  @brief
*    Color write enable flags
*
*  @note
*    - These constants directly map to Direct3D 10 & 11 constants, do not change them
*/
// TODO(naetherm) Rhi::ColorWriteEnableFlag, document
// TODO(naetherm) A flags-class would be nice to avoid invalid flags
struct ColorWriteEnableFlag final
{
  enum Enum
  {
    RED   = 1,
    GREEN = 2,
    BLUE  = 4,
    ALPHA = 8,
    ALL   = (((RED | GREEN) | BLUE) | ALPHA)
  };
};

/**
*  @brief
*    Mapped subresource
*
*  @note
*    - This structure directly maps to Direct3D 11, do not change it
*/
// TODO(naetherm) Comments
struct MappedSubresource final
{
  void*	 data;
  uint32_t rowPitch;
  uint32_t depthPitch;
};

/**
*  @brief
*    Viewport
*
*  @note
*    - This structure directly maps to Direct3D 11 & 12 as well as Vulkan, do not change it
*
*  @see
*    - "D3D12_VIEWPORT" or "VkViewport" documentation for details
*/
struct Viewport final
{
  float topLeftX;	///< Top left x start position
  float topLeftY;	///< Top left y start position
  float width;	///< Viewport width
  float height;	///< Viewport height
  float minDepth;	///< Minimum depth value, usually 0.0f, between [0, 1]
  float maxDepth;	///< Maximum depth value, usually 1.0f, between [0, 1]
};

/**
*  @brief
*    Scissor rectangle
*
*  @note
*    - This structure directly maps to Direct3D 9 & 10 & 11 & 12, do not change it
*
*  @see
*    - "D3D12_RECT"-documentation for details
*/
struct ScissorRectangle final
{
  long topLeftX;		///< Top left x-coordinate of the scissor rectangle
  long topLeftY;		///< Top left y-coordinate of the scissor rectangle
  long bottomRightX;	///< Bottom right x-coordinate of the scissor rectangle
  long bottomRightY;	///< Bottom right y-coordinate of the scissor rectangle
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
