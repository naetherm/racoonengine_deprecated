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
#include "REGui/REGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REGui {


//[-------------------------------------------------------]
//[ MS Windows definitions                                ]
//[-------------------------------------------------------]
#ifdef WIN32

//           Virtual code            Hex Value      Key
#define		 REGUIMOD_ALT				0x0001		/**< alt */
#define		 REGUIMOD_CONTROL			0x0002		/**< control */
#define		 REGUIMOD_ALTGR				0x0003		/**< alt-gr */
#define		 REGUIMOD_SHIFT				0x0004		/**< shift */
#define		 REGUIMOD_WIN				0x0008		/**< Windows key */

#define 	 REGUIKEY_LBUTTON			0x01		/**< left mouse button */
#define 	 REGUIKEY_RBUTTON			0x02		/**< right mouse button */
#define 	 REGUIKEY_CANCEL			0x03		/**< control-break processing */
#define 	 REGUIKEY_MBUTTON			0x04		/**< middle mouse button (three-button mouse) */

#define 	 REGUIKEY_BACK				0x08		/**< backspace */
#define 	 REGUIKEY_TAB				0x09		/**< tab */

#define 	 REGUIKEY_CLEAR				0x0C		/**< clear */
#define 	 REGUIKEY_RETURN			0x0D		/**< return (often the same as "enter") */

#define 	 REGUIKEY_SHIFT				0x10		/**< shift */
#define 	 REGUIKEY_CONTROL			0x11		/**< ctrl */
#define 	 REGUIKEY_MENU				0x12		/**< alt */
#define 	 REGUIKEY_PAUSE				0x13		/**< pause */
#define 	 REGUIKEY_CAPITAL			0x14		/**< caps lock */

#define 	 REGUIKEY_ESCAPE			0x1B		/**< esc */
#define 	 REGUIKEY_SPACE				0x20		/**< spacebar */
#define 	 REGUIKEY_PRIOR				0x21		/**< page up */
#define 	 REGUIKEY_NEXT				0x22		/**< page down */
#define 	 REGUIKEY_END				0x23		/**< end key */
#define 	 REGUIKEY_HOME				0x24		/**< home key */
#define 	 REGUIKEY_LEFT				0x25		/**< left arrow */
#define 	 REGUIKEY_UP				0x26		/**< up arrow */
#define 	 REGUIKEY_RIGHT				0x27		/**< right arrow */
#define 	 REGUIKEY_DOWN				0x28		/**< down arrow */
#define 	 REGUIKEY_SELECT			0x29		/**< select key */

#define 	 REGUIKEY_EXECUTE			0x2B		/**< execute key */
#define 	 REGUIKEY_SNAPSHOT			0x2C		/**< print screen key */
#define 	 REGUIKEY_INSERT			0x2D		/**< insert key */
#define 	 REGUIKEY_DELETE			0x2E		/**< delete key */
#define 	 REGUIKEY_HELP				0x2F		/**< help key */
#define 	 REGUIKEY_0					0x30		/**< 0 */
#define 	 REGUIKEY_1					0x31		/**< 1 */
#define 	 REGUIKEY_2					0x32		/**< 2 */
#define 	 REGUIKEY_3					0x33		/**< 3 */
#define 	 REGUIKEY_4					0x34		/**< 4 */
#define 	 REGUIKEY_5					0x35		/**< 5 */
#define 	 REGUIKEY_6					0x36		/**< 6 */
#define 	 REGUIKEY_7					0x37		/**< 7 */
#define 	 REGUIKEY_8					0x38		/**< 8 */
#define 	 REGUIKEY_9					0x39		/**< 9 */
											
#define 	 REGUIKEY_A					0x41		/**< a */
#define 	 REGUIKEY_B					0x42		/**< b */
#define 	 REGUIKEY_C					0x43		/**< c */
#define 	 REGUIKEY_D					0x44		/**< d */
#define 	 REGUIKEY_E					0x45		/**< e */
#define 	 REGUIKEY_F					0x46		/**< f */
#define 	 REGUIKEY_G					0x47		/**< g */
#define 	 REGUIKEY_H					0x48		/**< h */
#define 	 REGUIKEY_I					0x49		/**< i */
#define 	 REGUIKEY_J					0x4A		/**< j */
#define 	 REGUIKEY_K					0x4B		/**< k */
#define 	 REGUIKEY_L					0x4C		/**< l */
#define 	 REGUIKEY_M					0x4D		/**< m */
#define 	 REGUIKEY_N					0x4E		/**< n */
#define		 REGUIKEY_O					0x4F		/**< o */
#define 	 REGUIKEY_P					0x50		/**< p */
#define		 REGUIKEY_Q					0x51		/**< q */
#define 	 REGUIKEY_R					0x52		/**< r */
#define 	 REGUIKEY_S					0x53		/**< s */
#define 	 REGUIKEY_T					0x54		/**< t */
#define 	 REGUIKEY_U					0x55		/**< u */
#define 	 REGUIKEY_V					0x56		/**< v */
#define 	 REGUIKEY_W					0x57		/**< w */
#define 	 REGUIKEY_X					0x58		/**< x */
#define 	 REGUIKEY_Y					0x59		/**< y */
#define 	 REGUIKEY_Z					0x5A		/**< z */
#define  	 REGUIKEY_NUMPAD0			0x60		/**< Numeric keypad 0 key */
#define 	 REGUIKEY_NUMPAD1			0x61		/**< Numeric keypad 1 key */
#define 	 REGUIKEY_NUMPAD2			0x62		/**< Numeric keypad 2 key */
#define 	 REGUIKEY_NUMPAD3			0x63		/**< Numeric keypad 3 key */
#define 	 REGUIKEY_NUMPAD4			0x64		/**< Numeric keypad 4 key */
#define 	 REGUIKEY_NUMPAD5			0x65		/**< Numeric keypad 5 key */
#define 	 REGUIKEY_NUMPAD6			0x66		/**< Numeric keypad 6 key */
#define 	 REGUIKEY_NUMPAD7			0x67		/**< Numeric keypad 7 key */
#define 	 REGUIKEY_NUMPAD8			0x68		/**< Numeric keypad 8 key */
#define 	 REGUIKEY_NUMPAD9			0x69		/**< Numeric keypad 9 key */
#define 	 REGUIKEY_MULTIPLY			0x6A		/**< Multiply key */
#define 	 REGUIKEY_ADD				0x6B		/**< Add key */
#define 	 REGUIKEY_SEPARATOR			0x6C		/**< Separator key */
#define 	 REGUIKEY_SUBTRACT			0x6D		/**< Subtract key */
#define 	 REGUIKEY_DECIMAL			0x6E		/**< Decimal key */
#define 	 REGUIKEY_DIVIDE			0x6F		/**< Divide key */
#define 	 REGUIKEY_F1				0x70		/**< f1 key */
#define 	 REGUIKEY_F2				0x71		/**< f2 key */
#define 	 REGUIKEY_F3				0x72		/**< f3 key */
#define 	 REGUIKEY_F4				0x73		/**< f4 key */
#define 	 REGUIKEY_F5				0x74		/**< f5 key */
#define 	 REGUIKEY_F6				0x75		/**< f6 key */
#define 	 REGUIKEY_F7				0x76		/**< f7 key */
#define 	 REGUIKEY_F8				0x77		/**< f8 key */
#define 	 REGUIKEY_F9				0x78		/**< f9 key */
#define 	 REGUIKEY_F10				0x79		/**< f10 key */
#define 	 REGUIKEY_F11				0x7A		/**< f11 key */
#define 	 REGUIKEY_F12				0x7B		/**< f12 key */
	 	   
#define 	 REGUIKEY_NUMLOCK			0x90		// num lock key */
#define 	 REGUIKEY_SCROLL			0x91		// scroll lock key */  

#define 	 REGUIKEY_CIRCUMFLEX		0xDC		// ^ circumflex */


//[-------------------------------------------------------]
//[ Android definitions                                   ]
//[-------------------------------------------------------]
#elif defined(ANDROID)

// Do not include this header in here or we get thousands of compiler errors because this header is messing up the global namespace
//#include <android/keycodes.h>

//           Virtual code               Hex Value                   Key
#define		 REGUIMOD_ALT				57							// alt										- AKEYCODE_ALT_LEFT
#define		 REGUIMOD_CONTROL			0xF000						// control									- not supported
#define		 REGUIMOD_ALTGR				58							// alt-gr									- AKEYCODE_ALT_RIGHT
#define		 REGUIMOD_SHIFT				59							// shift									- AKEYCODE_SHIFT_LEFT
#define		 REGUIMOD_WIN				0xF001						// Windows key								- not supported

#define 	 REGUIKEY_LBUTTON			0xF002						// left mouse button						- not supported
#define 	 REGUIKEY_RBUTTON			0xF003						// right mouse button						- not supported
#define 	 REGUIKEY_CANCEL			0xF004	 					// control-break processing					- not supported
#define 	 REGUIKEY_MBUTTON			0xF005						// middle mouse button (three-button mouse)	- not supported
#define 	 REGUIKEY_BACK				4							// backspace								- AKEYCODE_BACK
#define 	 REGUIKEY_TAB				61							// tab										- AKEYCODE_TAB
#define 	 REGUIKEY_CLEAR				28							// clear									- AKEYCODE_CLEAR
#define 	 REGUIKEY_RETURN			66							// return (often the same as "enter")		- AKEYCODE_ENTER
#define 	 REGUIKEY_SHIFT				59							// shift									- AKEYCODE_SHIFT_LEFT
#define 	 REGUIKEY_CONTROL			0xF006						// ctrl										- not supported
#define 	 REGUIKEY_MENU				82							// alt										- AKEYCODE_MENU
#define 	 REGUIKEY_PAUSE				85							// pause									- AKEYCODE_MEDIA_PLAY_PAUSE
#define 	 REGUIKEY_CAPITAL			0xF007						// caps lock								- not supported
#define 	 REGUIKEY_ESCAPE			0xF008						// esc										- not supported
#define 	 REGUIKEY_SPACE				62							// spacebar									- AKEYCODE_SPACE
#define 	 REGUIKEY_PRIOR				88							// page up									- AKEYCODE_MEDIA_PREVIOUS
#define 	 REGUIKEY_NEXT				87							// page down								- AKEYCODE_MEDIA_NEXT
#define 	 REGUIKEY_END				0xF009						// end key									- not supported
#define 	 REGUIKEY_HOME				3							// home key									- AKEYCODE_HOME
#define 	 REGUIKEY_LEFT				21							// left arrow								- AKEYCODE_DPAD_LEFT
#define 	 REGUIKEY_UP				19							// up arrow									- AKEYCODE_DPAD_UP
#define 	 REGUIKEY_RIGHT				22							// right arrow								- AKEYCODE_DPAD_RIGHT
#define 	 REGUIKEY_DOWN				20							// down arrow								- AKEYCODE_DPAD_DOWN
#define 	 REGUIKEY_SELECT			109							// select key								- AKEYCODE_BUTTON_SELECT	
#define 	 REGUIKEY_EXECUTE			0xF00A						// execute key								- not supported
#define 	 REGUIKEY_SNAPSHOT			0xF00B						// print screen key							- not supported
#define 	 REGUIKEY_INSERT			0xF00C						// insert key								- not supported
#define 	 REGUIKEY_DELETE			67							// delete key								- AKEYCODE_DEL
#define 	 REGUIKEY_HELP				0xF00D						// help key									- not supported
#define 	 REGUIKEY_0					7							// 0										- AKEYCODE_0
#define 	 REGUIKEY_1					8							// 1										- AKEYCODE_1
#define 	 REGUIKEY_2					9							// 2										- AKEYCODE_2
#define 	 REGUIKEY_3					10							// 3										- AKEYCODE_3
#define 	 REGUIKEY_4					11							// 4										- AKEYCODE_4
#define 	 REGUIKEY_5					12							// 5										- AKEYCODE_5
#define 	 REGUIKEY_6					13							// 6										- AKEYCODE_6
#define 	 REGUIKEY_7					14							// 7										- AKEYCODE_7
#define 	 REGUIKEY_8					15							// 8										- AKEYCODE_8
#define 	 REGUIKEY_9					16							// 9										- AKEYCODE_9
#define 	 REGUIKEY_A					29							// a										- AKEYCODE_A
#define 	 REGUIKEY_B					30							// b										- AKEYCODE_B
#define 	 REGUIKEY_C					31							// c										- AKEYCODE_C
#define 	 REGUIKEY_D					32							// d										- AKEYCODE_D
#define 	 REGUIKEY_E					33							// e										- AKEYCODE_E
#define 	 REGUIKEY_F					34							// f										- AKEYCODE_F
#define 	 REGUIKEY_G					35							// g										- AKEYCODE_G
#define 	 REGUIKEY_H					36							// h										- AKEYCODE_H
#define 	 REGUIKEY_I					37							// i										- AKEYCODE_I
#define 	 REGUIKEY_J					38							// j										- AKEYCODE_J
#define 	 REGUIKEY_K					39							// k										- AKEYCODE_K
#define 	 REGUIKEY_L					40							// l										- AKEYCODE_L
#define 	 REGUIKEY_M					41							// m										- AKEYCODE_M
#define 	 REGUIKEY_N					42							// n										- AKEYCODE_N
#define		 REGUIKEY_O					43							// o										- AKEYCODE_O
#define 	 REGUIKEY_P					44							// p										- AKEYCODE_P
#define		 REGUIKEY_Q					45							// q										- AKEYCODE_Q
#define 	 REGUIKEY_R					46							// r										- AKEYCODE_R
#define 	 REGUIKEY_S					47							// s										- AKEYCODE_S
#define 	 REGUIKEY_T					48							// t										- AKEYCODE_T
#define 	 REGUIKEY_U					49							// u										- AKEYCODE_U
#define 	 REGUIKEY_V					50							// v										- AKEYCODE_V
#define 	 REGUIKEY_W					51							// w										- AKEYCODE_W
#define 	 REGUIKEY_X					52							// x										- AKEYCODE_X
#define 	 REGUIKEY_Y					53							// y										- AKEYCODE_Y
#define 	 REGUIKEY_Z					54							// z										- AKEYCODE_Z
#define 	 REGUIKEY_NUMPAD0			0xF00E						// Numeric keypad 0 key						- not supported
#define 	 REGUIKEY_NUMPAD1			0xF00F						// Numeric keypad 1 key						- not supported
#define 	 REGUIKEY_NUMPAD2			0xF010						// Numeric keypad 2 key						- not supported
#define 	 REGUIKEY_NUMPAD3			0xF011						// Numeric keypad 3 key						- not supported
#define 	 REGUIKEY_NUMPAD4			0xF012						// Numeric keypad 4 key						- not supported
#define 	 REGUIKEY_NUMPAD5			0xF013						// Numeric keypad 5 key						- not supported
#define 	 REGUIKEY_NUMPAD6			0xF014						// Numeric keypad 6 key						- not supported
#define 	 REGUIKEY_NUMPAD7			0xF015						// Numeric keypad 7 key						- not supported
#define 	 REGUIKEY_NUMPAD8			0xF016						// Numeric keypad 8 key						- not supported
#define 	 REGUIKEY_NUMPAD9			0xF017						// Numeric keypad 9 key						- not supported
#define 	 REGUIKEY_MULTIPLY			0xF018						// Multiply key								- not supported
#define 	 REGUIKEY_ADD				0xF019						// Add key									- not supported
#define 	 REGUIKEY_SEPARATOR			0xF01A						// Separator key							- not supported
#define 	 REGUIKEY_SUBTRACT			69							// Subtract key								- AKEYCODE_MINUS
#define 	 REGUIKEY_DECIMAL			0xF01B						// Decimal key								- not supported
#define 	 REGUIKEY_DIVIDE			0xF01C						// Divide key								- not supported
#define 	 REGUIKEY_F1				0xF01D						// F1 key									- not supported
#define 	 REGUIKEY_F2				0xF01E						// F2 key									- not supported
#define 	 REGUIKEY_F3				0xF01F						// F3 key									- not supported
#define 	 REGUIKEY_F4				0xF020						// F4 key									- not supported
#define 	 REGUIKEY_F5				0xF021						// F5 key									- not supported
#define 	 REGUIKEY_F6				0xF022						// F6 key									- not supported
#define 	 REGUIKEY_F7				0xF023						// F7 key									- not supported
#define 	 REGUIKEY_F8				0xF024						// F8 key									- not supported
#define 	 REGUIKEY_F9				0xF025						// F9 key									- not supported
#define 	 REGUIKEY_F10				0xF026						// F10 key									- not supported
#define 	 REGUIKEY_F11				0xF027						// F11 key									- not supported
#define 	 REGUIKEY_F12				0xF028						// F12 key									- not supported
#define 	 REGUIKEY_NUMLOCK			0xF029						// num lock key								- not supported
#define 	 REGUIKEY_SCROLL			0xF030						// scroll lock key							- not supported
#define 	 REGUIKEY_CIRCUMFLEX		0xF031						// ^ circumflex								- not supported


//[-------------------------------------------------------]
//[ Linux definitions                                     ]
//[-------------------------------------------------------]
#elif defined(LINUX)

#include <X11/keysym.h>
#ifdef None
#undef None
#endif

//           Virtual code            Hex Value      Key
// [TODO] Check modification keys
#define		 REGUIMOD_ALT				0x0001				// alt
#define		 REGUIMOD_CONTROL			0x0002				// control
#define		 REGUIMOD_ALTGR				0x0003				// alt-gr
#define		 REGUIMOD_SHIFT				0x0004				// shift
#define		 REGUIMOD_WIN				0x0008				// Windows key

#define 	 REGUIKEY_LBUTTON			0x01				// left mouse button  
#define 	 REGUIKEY_RBUTTON			0x02				// right mouse button  
#define 	 REGUIKEY_CANCEL			XK_Cancel 			// control-break processing (0x03?)  
#define 	 REGUIKEY_MBUTTON			0x04				// middle mouse button (three-button mouse)  
#define 	 REGUIKEY_BACK				XK_BackSpace		// backspace  
#define 	 REGUIKEY_TAB				XK_Tab				// tab  
#define 	 REGUIKEY_CLEAR				XK_Clear			// clear  
#define 	 REGUIKEY_RETURN			XK_Return			// return (often the same as "enter")
#define 	 REGUIKEY_SHIFT				XK_Shift_L			// shift
#define 	 REGUIKEY_CONTROL			XK_Control_L		// ctrl
#define 	 REGUIKEY_MENU				XK_Menu				// alt
#define 	 REGUIKEY_PAUSE				XK_Pause			// pause
#define 	 REGUIKEY_CAPITAL			XK_Caps_Lock		// caps lock
#define 	 REGUIKEY_ESCAPE			XK_Escape			// esc  
#define 	 REGUIKEY_SPACE				XK_space			// spacebar  
#define 	 REGUIKEY_PRIOR				XK_Prior			// page up   
#define 	 REGUIKEY_NEXT				XK_Next				// page down  
#define 	 REGUIKEY_END				XK_End				// end key  
#define 	 REGUIKEY_HOME				XK_Home				// home key  
#define 	 REGUIKEY_LEFT				XK_Left				// left arrow  
#define 	 REGUIKEY_UP				XK_Up				// up arrow  
#define 	 REGUIKEY_RIGHT				XK_Right			// right arrow  
#define 	 REGUIKEY_DOWN				XK_Down				// down arrow  
#define 	 REGUIKEY_SELECT			XK_Select			// select key  
#define 	 REGUIKEY_EXECUTE			XK_Execute			// execute key  
#define 	 REGUIKEY_SNAPSHOT			XK_Print			// print screen key  
#define 	 REGUIKEY_INSERT			XK_Insert			// insert key  
#define 	 REGUIKEY_DELETE			XK_Delete			// delete key  
#define 	 REGUIKEY_HELP				XK_Help				// help key  
#define 	 REGUIKEY_0					XK_0				// 0   
#define 	 REGUIKEY_1					XK_1				// 1   
#define 	 REGUIKEY_2					XK_2				// 2   
#define 	 REGUIKEY_3					XK_3				// 3   
#define 	 REGUIKEY_4					XK_4				// 4   
#define 	 REGUIKEY_5					XK_5				// 5   
#define 	 REGUIKEY_6					XK_6				// 6   
#define 	 REGUIKEY_7					XK_7				// 7   
#define 	 REGUIKEY_8					XK_8				// 8   
#define 	 REGUIKEY_9					XK_9				// 9   
#define 	 REGUIKEY_A					XK_a				// a   
#define 	 REGUIKEY_B					XK_b				// b   
#define 	 REGUIKEY_C					XK_c				// c   
#define 	 REGUIKEY_D					XK_d				// d   
#define 	 REGUIKEY_E					XK_e				// e   
#define 	 REGUIKEY_F					XK_f				// f   
#define 	 REGUIKEY_G					XK_g				// g   
#define 	 REGUIKEY_H					XK_h				// h   
#define 	 REGUIKEY_I					XK_i				// i   
#define 	 REGUIKEY_J					XK_j				// j   
#define 	 REGUIKEY_K					XK_k				// k   
#define 	 REGUIKEY_L					XK_l				// l   
#define 	 REGUIKEY_M					XK_m				// m   
#define 	 REGUIKEY_N					XK_n				// n   
#define		 REGUIKEY_O					XK_o				// o   
#define 	 REGUIKEY_P					XK_p				// p
#define		 REGUIKEY_Q					XK_q				// q   
#define 	 REGUIKEY_R					XK_r				// r   
#define 	 REGUIKEY_S					XK_s				// s   
#define 	 REGUIKEY_T					XK_t				// t   
#define 	 REGUIKEY_U					XK_u				// u   
#define 	 REGUIKEY_V					XK_v				// v   
#define 	 REGUIKEY_W					XK_w				// w   
#define 	 REGUIKEY_X					XK_x				// x   
#define 	 REGUIKEY_Y					XK_y				// y   
#define 	 REGUIKEY_Z					XK_z				// z   
#define  	 REGUIKEY_NUMPAD0			XK_KP_0				// Numeric keypad 0 key  
#define 	 REGUIKEY_NUMPAD1			XK_KP_1				// Numeric keypad 1 key  
#define 	 REGUIKEY_NUMPAD2			XK_KP_2				// Numeric keypad 2 key  
#define 	 REGUIKEY_NUMPAD3			XK_KP_3				// Numeric keypad 3 key  
#define 	 REGUIKEY_NUMPAD4			XK_KP_4				// Numeric keypad 4 key  
#define 	 REGUIKEY_NUMPAD5			XK_KP_5				// Numeric keypad 5 key  
#define 	 REGUIKEY_NUMPAD6			XK_KP_6				// Numeric keypad 6 key  
#define 	 REGUIKEY_NUMPAD7			XK_KP_7				// Numeric keypad 7 key  
#define 	 REGUIKEY_NUMPAD8			XK_KP_8				// Numeric keypad 8 key  
#define 	 REGUIKEY_NUMPAD9			XK_KP_9				// Numeric keypad 9 key  
#define 	 REGUIKEY_MULTIPLY			XK_KP_Multiply		// Multiply key  
#define 	 REGUIKEY_ADD				XK_KP_Add			// Add key  
#define 	 REGUIKEY_SEPARATOR			XK_KP_Separator		// Separator key  
#define 	 REGUIKEY_SUBTRACT			XK_KP_Subtract		// Subtract key  
#define 	 REGUIKEY_DECIMAL			XK_KP_Decimal		// Decimal key  
#define 	 REGUIKEY_DIVIDE			XK_KP_Divide		// Divide key  
#define 	 REGUIKEY_F1				XK_F1				// F1 key  
#define 	 REGUIKEY_F2				XK_F2				// F2 key  
#define 	 REGUIKEY_F3				XK_F3				// F3 key  
#define 	 REGUIKEY_F4				XK_F4				// F4 key  
#define 	 REGUIKEY_F5				XK_F5				// F5 key  
#define 	 REGUIKEY_F6				XK_F6				// F6 key  
#define 	 REGUIKEY_F7				XK_F7				// F7 key  
#define 	 REGUIKEY_F8				XK_F8				// F8 key  
#define 	 REGUIKEY_F9				XK_F9				// F9 key  
#define 	 REGUIKEY_F10				XK_F10				// F10 key  
#define 	 REGUIKEY_F11				XK_F11				// F11 key  
#define 	 REGUIKEY_F12				XK_F12				// F12 key  
#define 	 REGUIKEY_NUMLOCK			XK_Num_Lock			// num lock key  
#define 	 REGUIKEY_SCROLL			XK_Scroll_Lock		// scroll lock key  
#define 	 REGUIKEY_CIRCUMFLEX		XK_asciicircum		// ^ circumflex

#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REGui