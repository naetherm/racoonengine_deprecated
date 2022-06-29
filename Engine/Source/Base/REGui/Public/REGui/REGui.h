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
#include <RECore/RECore.h>
#include <RERHI/RERHI.h>
#include <RERHI/Rhi.h>
#include <RERenderer/RERenderer.h>
#include <mutex>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <cassert>

#ifdef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#undef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#endif

#if defined(LINUX)
#include <RECore/Linux/X11Includes.h>

/**
*  @brief
*    Mouse buttons
*/
#ifdef Button1
namespace PLGuiLinuxIncludes {
		enum {
			X11_Button1 = Button1,
			X11_Button2 = Button2,
			X11_Button3 = Button3,
			X11_Button4 = Button4,
			X11_Button5 = Button5
		};
	}

	#undef Button1
	#undef Button2
	#undef Button3
	#undef Button4
	#undef Button5

	namespace XLib {
		enum {
			Button1 = PLGuiLinuxIncludes::X11_Button1,
			Button2 = PLGuiLinuxIncludes::X11_Button2,
			Button3 = PLGuiLinuxIncludes::X11_Button3,
			Button4 = PLGuiLinuxIncludes::X11_Button4,
			Button5 = PLGuiLinuxIncludes::X11_Button5
		};
	}
#endif


/**
*  @brief
*    Misc
*/
#ifdef None
namespace PLGuiLinuxIncludes {
		enum {
			X11_None = None,
			X11_Always = Always,
			X11_Above = Above,
			X11_Success = Success
		};
	}

	#undef None
	#undef Always
	#undef Above
	#undef Success

	namespace XLib {
		enum {
			None = PLGuiLinuxIncludes::X11_None,
			Always = PLGuiLinuxIncludes::X11_Always,
			Above = PLGuiLinuxIncludes::X11_Above,
			Success = PLGuiLinuxIncludes::X11_Success
		};
	}
#endif
#endif

//[-------------------------------------------------------]
//[ Import/Export                                         ]
//[-------------------------------------------------------]
#ifdef REGUI_EXPORTS
// To export classes, methods and variables
  #define REGUI_API			RE_GENERIC_API_EXPORT
#else
// To import classes, methods and variables
#define REGUI_API			RE_GENERIC_API_IMPORT
#endif