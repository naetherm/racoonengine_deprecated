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
#include "Examples/Framework/Linux/ApplicationImplLinux.h"
#include "Examples/Framework/Linux/X11Application.h"
#include "Examples/Framework/Linux/X11Window.h"
#include "Examples/Framework/IApplication.h"
#include "Examples/Framework/IApplicationRenderer.h"

#ifdef RENDERER_IMGUI
	#include <RERenderer/IRenderer.h>
	#include <RERenderer/DebugGui/Detail/DebugGuiManagerLinux.h>
#endif

#include <X11/Xutil.h>
#include <X11/Xlib.h>

#include <unordered_map>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class ApplicationWindow : public X11Window
{
	std::unordered_map<RECore::uint32, RECore::uint32> mX11KeySymToKeyMap {
												{XK_a, 'A'}, 
												{XK_A, 'A'},
												{XK_w, 'W'}, 
												{XK_W, 'W'},
												{XK_s, 'S'}, 
												{XK_S, 'S'},
												{XK_d, 'D'}, 
												{XK_D, 'D'}
											};
public:
	inline explicit ApplicationWindow(IApplication& application) :
		mApplication(application)
	{
		// Nothing here
	}

	virtual bool HandleEvent(XEvent& event)
	{
		X11Window::HandleEvent(event);

		if (isDestroyed())
		{
			return true;
		}

		switch (event.type)
		{
			case Expose:
				// There could be more then one expose event currently in the event loop.
				// To avoid too many redraw calls, call onDrawRequest only when the current processed expose event is the last one.
				if (!event.xexpose.count)
				{
					mApplication.onDrawRequest();
				}
				break;

			// Window configuration changed
			case ConfigureNotify:
				mApplication.onResize();
				break;

			case KeyPress:
			{
				const RECore::uint32 key = XLookupKeysym(&event.xkey, 0);
				auto keyIterator = mX11KeySymToKeyMap.find(key);
				if (keyIterator != mX11KeySymToKeyMap.end() && 27 == keyIterator->second)
				{
					mApplication.onEscapeKey();
				}
				break;
			}
		}

		#ifdef RENDERER_IMGUI
		{
			// TODO(naetherm) Evil cast ahead. Maybe simplify the example application framework? After all, it's just an example framework for RacoonEngine and nothing too generic.
			const IApplicationRenderer* applicationRenderer = dynamic_cast<IApplicationRenderer*>(&mApplication);
			if (nullptr != applicationRenderer)
			{
				const RERenderer::IRenderer* renderer = applicationRenderer->getRenderer();
				if (nullptr != renderer)
				{
					RERenderer::DebugGuiManagerLinux& debugGuiLinux = static_cast<RERenderer::DebugGuiManagerLinux&>(renderer->getDebugGuiManager());
					switch(event.type)
					{
						case ConfigureNotify:
						{
							debugGuiLinux.onWindowResize(event.xconfigure.width, event.xconfigure.height);
							break;
						}
						case KeyPress:
						case KeyRelease:
						{
							const int buffer_size = 2;
							char buffer[buffer_size + 1];
							KeySym keySym;
							int count = XLookupString(&event.xkey, buffer, buffer_size, &keySym, nullptr);
							buffer[count] = 0;

							debugGuiLinux.onKeyInput(keySym, buffer[0], event.type == KeyPress);
							break;
						}
						case ButtonRelease:
						case ButtonPress:
						{
							const bool isPressed = (ButtonPress == event.type);
							if (isPressed && (event.xbutton.button == 4 || event.xbutton.button == 5)) // Wheel buttons
							{
								debugGuiLinux.onMouseWheelInput(event.xbutton.button == 4);
							}
							else
							{
								debugGuiLinux.onMouseButtonInput(event.xbutton.button, isPressed);
							}
							break;
						}

						case MotionNotify:
						{
							debugGuiLinux.onMouseMoveInput(event.xmotion.x, event.xmotion.y);
							break;
						}
					}
				}
			}
		}
		#endif
		return false;
	}
private:
	IApplication& mApplication;
};


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
ApplicationImplLinux::ApplicationImplLinux(IApplication& application, const char* windowTitle) :
	IApplicationImpl(application),
	mApplication(&application),
	mX11EventLoop(nullptr),
	mMainWindow(nullptr)
{
	// Copy the given window title
	if (nullptr != windowTitle)
	{
		strncpy(mWindowTitle, windowTitle, 64);
	}
	else
	{
		mWindowTitle[0] = '\0';
	}
}


//[-------------------------------------------------------]
//[ Public virtual IApplicationImpl methods               ]
//[-------------------------------------------------------]
void ApplicationImplLinux::onInitialization()
{
	mX11EventLoop = new X11Application();
	mMainWindow = new ApplicationWindow(*mApplication);

	mMainWindow->setTitle(mWindowTitle);
	mMainWindow->show();

	XSync(X11Application::instance()->getDisplay(), False);
}

void ApplicationImplLinux::onDeinitialization()
{
	// Destroy the OS window instance, in case there's one
	if (nullptr != mMainWindow)
	{
		// Destroy the OpenGL dummy window
		delete mMainWindow;
		mMainWindow = nullptr;
	}

	if (nullptr != mX11EventLoop)
	{
		delete mX11EventLoop;
		mX11EventLoop = nullptr;
	}
}

bool ApplicationImplLinux::processMessages()
{
	return mX11EventLoop->handlePendingEvents();
}

void ApplicationImplLinux::getWindowSize(int& width, int& height) const
{
	// Is there a valid OS window?
	if (nullptr != mMainWindow)
	{
		mMainWindow->getWindowSize(width, height);
	}
	else
	{
		// There's no valid OS window, return known values
		width  = 0;
		height = 0;
	}
}

handle ApplicationImplLinux::getNativeWindowHandle() const
{
	return mMainWindow->winId();
}

void ApplicationImplLinux::redraw()
{
	// Is there a valid OS window?
	if (nullptr != mMainWindow)
	{
		mMainWindow->refresh();
	}
}

void ApplicationImplLinux::showUrgentMessage([[maybe_unused]] const char* message, [[maybe_unused]] const char* title) const
{
	// TODO(naetherm) Implement me
}

Display* ApplicationImplLinux::getX11Display() const
{
	return mX11EventLoop->getDisplay();
}
