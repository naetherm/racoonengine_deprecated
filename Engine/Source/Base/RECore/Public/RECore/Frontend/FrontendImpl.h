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
#include "RECore/RECore.h"
#include "RECore/Reflect/Object.h"
#include "RECore/Frontend/AbstractFrontend.h"
#include "RECore/Core/AbstractLifecycle.h"
#include "RECore/Frontend/WindowDescription.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Frontend;
class FrontendContext;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract frontend implementation base class
*
*  @remarks
*    This base class provides the backend interface for concrete implementations.
*    Just think of the frontend implementation as a puppet master, while the puppet
*    is the "Frontend"-class. The frontend implementation is e.g. a simple native OS
*    window or a browser such as MS Internet Explorer or Mozilla Firefox.
*/
class FrontendImpl : public Object, protected AbstractLifecycle, protected AbstractFrontend {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Frontend;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	re_class_def(RECORE_API)
	re_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		RECORE_API FrontendImpl();
		
		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~FrontendImpl();

		/**
		*  @brief
		*    Returns the frontend instance
		*
		*  @return
		*    The frontend instance, can be a null pointer
		*/
		inline Frontend *getFrontend() const;


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractLifecycle functions         ]
	//[-------------------------------------------------------]
	protected:
		RECORE_API virtual void onCreate() override;	// Automatically called from within "Frontend::Run()"
		RECORE_API virtual void onRestart() override;	// Don't forget to call this within derived frontend implementations
		RECORE_API virtual bool onStart() override;		// Don't forget to call this within derived frontend implementations
		RECORE_API virtual void onResume() override;	// Don't forget to call this within derived frontend implementations
		RECORE_API virtual void onPause() override;		// Don't forget to call this within derived frontend implementations
		RECORE_API virtual void onStop() override;		// Don't forget to call this within derived frontend implementations
		RECORE_API virtual void onDestroy() override;	// Automatically called from within "Frontend::Run()"


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractFrontend functions          ]
	//[-------------------------------------------------------]
	protected:
		RECORE_API virtual void onSize() override;
		RECORE_API virtual void onFullscreenMode() override;
		RECORE_API virtual void onDraw() override;
		RECORE_API virtual void onUpdate() override;
		RECORE_API virtual void onDrop(const std::vector<String> &lstFiles) override;


	//[-------------------------------------------------------]
	//[ Protected virtual FrontendImpl functions              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when the frontend is run
		*
		*  @param[in] sExecutableFilename
		*    Absolute application executable filename
		*  @param[in] lstArguments
		*    List of arguments to the program
		*
		*  @remarks
		*    This frontend method is called just before the frontend calls it's run-method in order to
		*    enter it's main-loop. This means that this method is called between "AbstractLifecycle::OnCreate()"
		*    and "AbstractLifecycle::OnStart()". Use this method for instance to pre-process command line arguments.
		*
		*  @note
		*    - Automatically called from within "Frontend::Run()"
		*/
		RECORE_API virtual void onRun(const String &sExecutableFilename, const std::vector<String> &lstArguments);

		/**
		*  @brief
		*    Called when the frontend should run
		*
		*  @param[in] sExecutableFilename
		*    Absolute application executable filename
		*  @param[in] lstArguments
		*    List of arguments to the program
		*
		*  @return
		*    Exit code (usually 0 means no error), usually <0 when there was an error
		*    (e.g. an embedded frontend implementation is run and controlled by another application and can't be run by using this method)
		*
		*  @note
		*    - The default implementation does nothing at all
		*    - Automatically called from within "Frontend::Run()"
		*/
		RECORE_API virtual int run(const String &sExecutableFilename, const std::vector<String> &lstArguments);

		/**
		*  @brief
		*    Get native window handle
		*
		*  @return
		*    Native window handle for the frontend window, can be a null pointer
		*/
		virtual handle getNativeWindowHandle() const = 0;

		/**
		*  @brief
		*    Redraw frontend
		*
		*  @remarks
		*    There are situations were an application may do some heavy work without letting
		*    the frontend a chance to redraw. In such situations, it may be wise
		*    to call this method from time to time to give the frontend a chance to do redraw
		*    itself.
		*
		*  @note
		*    - Whenever possible, don't use this method, do heavy work within e.g. threads
		*    - Depending on the frontend implementation, the redraw may not be immediate
		*    - Doesn't include "Ping()"
		*/
		virtual void redraw() = 0;

		/**
		*  @brief
		*    Give the frontend a chance to process OS messages
		*
		*  @remarks
		*    There are situations were an application may do some heavy work without letting
		*    the frontend a chance to process OS messages. In such situations, it may be wise
		*    to call this method from time to time to give the frontend a chance to do some
		*    message processing.
		*
		*  @note
		*    - Whenever possible, don't use this method, do heavy work within e.g. threads
		*/
		virtual void ping() = 0;

		/**
		*  @brief
		*    Get frontend title
		*
		*  @return
		*    Frontend title
		*
		*  @remarks
		*    When the frontend has a window, this title can be seen within the window
		*    title bar. Please note that there's no guarantee that there's a window
		*    title bar or even a window. By default, the title is set to the frontend
		*    context name ("GetContext().GetName()") which is usually sufficient. So,
		*    unless you have a good reason to explicitly set an individual frontend
		*    title, just use the default setting and don't touch the frontend.
		*/
		virtual String getTitle() const = 0;

		/**
		*  @brief
		*    Set frontend title
		*
		*  @param[in] sTitle
		*    Frontend title
		*
		*  @see
		*    - getTitle()
		*/
		virtual void setTitle(const String &sTitle) = 0;

		//[-------------------------------------------------------]
		//[ Position and size                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the x position of the frontend (in screen coordinates)
		*
		*  @return
		*    X position of the frontend
		*/
		virtual int getX() const = 0;

		/**
		*  @brief
		*    Get the y position of the frontend (in screen coordinates)
		*
		*  @return
		*    Y position of the frontend
		*/
		virtual int getY() const = 0;

		/**
		*  @brief
		*    Get frontend width
		*
		*  @return
		*    Width of the frontend
		*/
		virtual uint32 getWidth() const = 0;

		/**
		*  @brief
		*    Get frontend height
		*
		*  @return
		*    Height of the frontend
		*/
		virtual uint32 getHeight() const = 0;

		/**
		*  @brief
		*    Returns frontend window position and size
		*
		*  @param[out] nX
		*    Receives the x position of the frontend window (in screen coordinates)
		*  @param[out] nY
		*    Receives the y position of the frontend window (in screen coordinates)
		*  @param[out] nWidth
		*    Receives the width of the frontend window
		*  @param[out] nHeight
		*    Receives the height of the frontend window
		*
		*  @remarks
		*    The primary argument to allow the user to request a frontend window position and size change is,
		*    that it should be possible to restore the frontend window position and size of a previous session
		*    (may be important for the usability). Do not misuse this method to frequently manipulate
		*    the frontend window appearance. Please note that, as for all other frontend methods, this is only
		*    considered to be a request. A frontend implementation may deny the request in general or
		*    just improper settings (e.g. a too small size, position outside the visible screen etc.).
		*/
		virtual void getWindowPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight) const = 0;

		/**
		*  @brief
		*    Set frontend window position and size
		*
		*  @param[in] nX
		*    X position of the frontend window (in screen coordinates)
		*  @param[in] nY
		*    Y position of the frontend window (in screen coordinates)
		*  @param[in] nWidth
		*    Width of the frontend window
		*  @param[in] nHeight
		*    Height of the frontend window
		*
		*  @see
		*    - "GetWindowPositionSize"()
		*/
		virtual void setWindowPositionSize(int nX, int nY, uint32 nWidth, uint32 nHeight) = 0;

		//[-------------------------------------------------------]
		//[ Fullscreen                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Gets whether it's allowed to toggle the fullscreen mode using hotkeys
		*
		*  @return
		*    'true' if it's possible to toggle the fullscreen mode using hotkeys, else 'false'
		*/
		virtual bool getToggleFullscreenMode() const = 0;

		/**
		*  @brief
		*    Sets whether it's allowed to toggle the fullscreen mode using hotkeys
		*
		*  @param[in] bToggleFullscreenMode
		*    Is it allowed to toggle the fullscreen mode using hotkeys?
		*
		*  @note
		*    - By default, it's allowed to switch widgets into fullscreen mode using Alt-Return or AltGr-Return
		*/
		virtual void setToggleFullscreenMode(bool bToggleFullscreenMode) = 0;

		/**
		*  @brief
		*    Gets whether it's allowed to use Alt-Tab if fullscreen mode is used
		*
		*  @return
		*    'true' if it's possible to use Alt-Tab if fullscreen mode is used, else 'false'
		*
		*  @note
		*    - Widgets only
		*/
		virtual bool getFullscreenAltTab() const = 0;

		/**
		*  @brief
		*    Sets whether it's allowed to use Alt-Tab if fullscreen mode is used
		*
		*  @param[in] bAllowed
		*    Is it allowed to use Alt-Tab within fullscreen mode?
		*
		*  @note
		*    - By default, it's allowed to use Alt-Tab
		*
		*  @see
		*    - GetFullscreenAltTab()
		*/
		virtual void setFullscreenAltTab(bool bAllowed) = 0;

		/**
		*  @brief
		*    Returns whether the frontend is in fullscreen mode or not
		*
		*  @return
		*    'true' if the frontend is in fullscreen mode, else 'false'
		*/
		virtual bool isFullscreen() const = 0;

		/**
		*  @brief
		*    Sets the frontend's fullscreen mode
		*
		*  @param[in] bFullscreen
		*    'true' if the frontend should be in fullscreen mode, else 'false'
		*/
		virtual void setFullscreen(bool bFullscreen) = 0;

		/**
		*  @brief
		*    Something related to fullscreen mode has been changed (e.g. the display resolution)
		*/
		virtual void refreshFullscreen() = 0;

		//[-------------------------------------------------------]
		//[ Mouse                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if the mouse is currently over the frontend
		*
		*  @return
		*    'true' if mouse-over, else 'false'
		*/
		virtual bool isMouseOver() const = 0;

		/**
		*  @brief
		*    Get current mouse cursor X position inside the frontend
		*
		*  @return
		*    Current mouse cursor X position inside the frontend, negative value if the mouse cursor isn't currently over the frontend
		*/
		virtual int getMousePositionX() const = 0;

		/**
		*  @brief
		*    Get current mouse cursor Y position inside the frontend
		*
		*  @return
		*    Current mouse cursor Y position inside the frontend, negative value if the mouse cursor isn't currently over the frontend
		*/
		virtual int getMousePositionY() const = 0;

		/**
		*  @brief
		*    Check if the mouse cursor is visible
		*
		*  @return
		*    'true' if the mouse cursor is visible, else 'false'
		*
		*  @note
		*    - If the mouse cursor is visible in general, it's still possible that it's
		*      invisible over some special widgets.
		*    - If the mouse cursor is invisible in general, it will NEVER be visible!
		*/
		virtual bool isMouseVisible() const = 0;

		/**
		*  @brief
		*    Set mouse cursor visibility
		*
		*  @param[in] bVisible
		*    Shall the mouse cursor be visible?
		*
		*  @see
		*    - IsMouseVisible()
		*/
		virtual void setMouseVisible(bool bVisible) = 0;

		/**
		*  @brief
		*    Trap mouse inside the frontend
		*
		*  @param[in] bTrap
		*    'true' if the mouse should be trapped inside the frontend, else 'false'
		*/
		virtual void setTrapMouse(bool bTrap) = 0;


	//[-------------------------------------------------------]
	//[ Protected static functions                            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Creates a frontend instance
		*
		*  @param[in] cFrontendContext
		*    Frontend context to use
		*  @param[in] cFrontendImpl
		*    Frontend implementation instance
		*
		*  @return
		*    Frontend instance, null pointer on error
		*/
		static RECORE_API Frontend *createFrontend(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl);

		/**
		*  @brief
		*    Correct frontend position and size settings
		*
		*  @param[in, out] nX
		*    X position of the frontend (in screen coordinates)
		*  @param[in, out] nY
		*    Y position of the frontend (in screen coordinates)
		*  @param[in, out] nWidth
		*    Width of the frontend
		*  @param[in, out] nHeight
		*    Height of the frontend
		*  @param[in]     nScreenLeft
		*    Screen left side
		*  @param[in]     nScreenTop
		*    Screen top side
		*  @param[in]     nScreenWidth
		*    Screen width
		*  @param[in]     nScreenHeight
		*    Screen height
		*  @param[in]     nMinWidth
		*    Minimum allowed frontend width
		*  @param[in]     nMinHeight
		*    Minimum allowed frontend height
		*
		*  @note
		*    - Corrects: The frontend position shouldn't be negative
		*    - Corrects: The frontend position shouldn't be outside the visible screen
		*    - Corrects: The size of the frontend shouldn't leave the visible screen
		*/
		static RECORE_API void correctPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight, int nScreenLeft, int nScreenTop, int nScreenWidth, int nScreenHeight, uint32 nMinWidth = 200, uint32 nMinHeight = 200);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Frontend *m_pFrontend;	/**< Pointer to frontend, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Frontend/FrontendImpl.inl"