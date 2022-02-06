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
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <asm/byteorder.h>


//[-------------------------------------------------------]
//[ Public definitions                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    _NET_WM state actions
*/
#define _NET_WM_STATE_REMOVE			0    /**< Remove/unset property */
#define _NET_WM_STATE_ADD				1    /**< Add/set property */
#define _NET_WM_STATE_TOGGLE			2    /**< Toggle property  */

/**
*  @brief
*    XEMBED messages
*/
#define XEMBED_EMBEDDED_NOTIFY					 0	/**< Window has been embedded */
#define XEMBED_WINDOW_ACTIVATE					 1	/**< Window has been activated */
#define XEMBED_WINDOW_DEACTIVATE  				 2	/**< Window has been de-activated */
#define XEMBED_REQUEST_FOCUS		 			 3	/**< Window requests to get the focus */
#define XEMBED_FOCUS_IN 	 					 4	/**< Window has got the focus */
#define XEMBED_FOCUS_OUT  						 5	/**< Window has lost the focus */
#define XEMBED_FOCUS_NEXT 						 6	/**< Window has reached the end of it's logical tab (tabbing forward) */
#define XEMBED_FOCUS_PREV 						 7	/**< Window has reached the beginning of it's logical tab (tabbing backwards) */
#define XEMBED_MODALITY_ON 						10	/**< Window becomes shadowed by a modal dialog */
#define XEMBED_MODALITY_OFF 					11	/**< Window has been released from a modal dialog */
#define XEMBED_REGISTER_ACCELERATOR				12	/**< Register a key accelerator */
#define XEMBED_UNREGISTER_ACCELERATOR			13	/**< Unregister a key accelerator */
#define XEMBED_ACTIVATE_ACCELERATOR				14	/**< An accelerator has been activated */

/**
*  @brief
*    XEMBED flags
*/
#define XEMBED_MAPPED                     (1 << 0)	/**< Client is visible */

/**
*  @brief
*    XEMBED options for XEMBED_FOCUS_IN
*/
#define XEMBED_FOCUS_CURRENT					 0	/**< Do not move logical focus */
#define XEMBED_FOCUS_FIRST 						 1	/**< Activate first item in list */
#define XEMBED_FOCUS_LAST						 2	/**< Activate last item in list */

/**
*  @brief
*    XEMBED modifier keys for XEMBED_FOCUS_IN
*/
#define XEMBED_MODIFIER_SHIFT			  (1 << 0)	/**< Shift key */
#define XEMBED_MODIFIER_CONTROL			  (1 << 1)	/**< Control key ("Ctrl") */
#define XEMBED_MODIFIER_ALT				  (1 << 2)	/**< ALT key */
#define XEMBED_MODIFIER_SUPER			  (1 << 3)	/**< Super key */
#define XEMBED_MODIFIER_HYPER			  (1 << 4)	/**< Hyper key */

/**
*  @brief
*    System tray messages
*/
#define SYSTEM_TRAY_REQUEST_DOCK				 0	/**< Dock into systray */
#define SYSTEM_TRAY_BEGIN_MESSAGE				 1	/**< Show message */
#define SYSTEM_TRAY_CANCEL_MESSAGE				 2	/**< Hide message */

/**
*  @brief
*    Mouse buttons
*/
#ifdef Button1
namespace REInputLinuxIncludes {
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
  Button1 = REInputLinuxIncludes::X11_Button1,
  Button2 = REInputLinuxIncludes::X11_Button2,
  Button3 = REInputLinuxIncludes::X11_Button3,
  Button4 = REInputLinuxIncludes::X11_Button4,
  Button5 = REInputLinuxIncludes::X11_Button5
};
}
#endif


/**
*  @brief
*    Misc
*/
#ifdef None
namespace REInputLinuxIncludes {
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
  None = REInputLinuxIncludes::X11_None,
  Always = REInputLinuxIncludes::X11_Always,
  Above = REInputLinuxIncludes::X11_Above,
  Success = REInputLinuxIncludes::X11_Success
};
}
#endif


//[-------------------------------------------------------]
//[ Bluetooth definitions                                 ]
//[-------------------------------------------------------]
#include <asm/byteorder.h>
#include <bits/sockaddr.h>

#define htobs(a)        __cpu_to_le16(a)
#define BTPROTO_L2CAP   0

typedef struct {
  unsigned char b[6];
} __attribute__((packed)) bdaddr_t;

struct sockaddr_l2 {
  sa_family_t     l2_family;
  unsigned short  l2_psm;
  bdaddr_t        l2_bdaddr;
};


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
static constexpr int TIMEOUT = 10;	///< Timeout for detecting devices

/**
*  @brief
*    Bluetooth transport types
*/
enum ETrans {
  TransHandshake				= 0x00,		// Handshake request
  TransSetReport				= 0x50,		// Report request
  TransData					= 0xA0		// Data request
};

/**
*  @brief
*    Bluetooth parameters
*/
enum EParam {
  ParamInput					= 0x01,		// Input
  ParamOutput					= 0x02,		// Output
  ParamFeature				= 0x03		// Feature
};

/**
*  @brief
*    Bluetooth handshake responses
*/
enum EResult {
  ResultSuccess				= 0x00,		// Handshake OK
  ResultNotReady				= 0x01,		// Device not ready
  ResultErrInvalidReportID	= 0x02,		// Error: Invalid report ID
  ResultErrUnsupportedRequest	= 0x03,		// Error: Unsupported Request
  ResultErrInvalidParameter	= 0x04,		// Error: Invalid parameter
  ResultErrUnknown			= 0x0E,		// Error: Unknown error
  ResultErrFatal				= 0x0F		// Error: Fatal error
};

// Internals
static constexpr int BufferSize	= 128;		// Size of temporary read/write buffer