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
#include "RECore/Runtime.h"
#include "RECore/Log/Log.h"
#include "RECore/Reflect/Class.h"
#include "RECore/Platform/Platform.h"
#include "RECore/Frontend/FrontendImpl.h"
#include "RECore/Frontend/FrontendContext.h"
#include "RECore/Frontend/Frontend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(Frontend, "RECore", RECore::Object, "Abstract frontend base class")
	// Methods
	re_method_0_metadata(redraw,					re_ret_type(void),						"Redraw frontend.",																																				"")
	re_method_0_metadata(ping,						re_ret_type(void),						"Give the frontend a chance to process OS messages.",																											"")
	re_method_0_metadata(redrawAndPing,				re_ret_type(void),						"Redraw frontend and give the frontend a chance to process OS messages.",																						"")
	re_method_0_metadata(getTitle,					re_ret_type(String),					"Returns the frontend title.",																																	"")
	re_method_1_metadata(setTitle,					re_ret_type(void),		const String&,	"Sets the frontend title.",																																		"")
	// Position and size methods
	re_method_0_metadata(getX,						re_ret_type(int),						"Returns the x position of the frontend (in screen coordinates).",																								"")
	re_method_0_metadata(getY,						re_ret_type(int),						"Returns the y position of the frontend (in screen coordinates).",																								"")
	re_method_0_metadata(getWidth,					re_ret_type(uint32),					"Returns the frontend width.",																																	"")
	re_method_0_metadata(getHeight,					re_ret_type(uint32),					"Returns the frontend height.",																																	"")
	// Fullscreen methods
	re_method_0_metadata(getToggleFullscreenMode,	re_ret_type(bool),						"Returns whether it's allowed to toggle the fullscreen mode using hotkeys. 'true' if it's possible to toggle the fullscreen mode using hotkeys, else 'false'.",	"")
	re_method_1_metadata(setToggleFullscreenMode,	re_ret_type(void),		bool,			"Sets whether it's allowed to toggle the fullscreen mode using hotkeys. 'true' as first parameter to allow it, else 'false'.",									"")
	re_method_0_metadata(getFullscreenAltTab,		re_ret_type(bool),						"Returns whether it's allowed to use Alt-Tab if fullscreen mode is used. 'true' if it's possible to use Alt-Tab if fullscreen mode is used, else 'false'.",		"")
	re_method_1_metadata(setFullscreenAltTab,		re_ret_type(void),		bool,			"Sets whether it's allowed to use Alt-Tab if fullscreen mode is used. 'true' as first parameter to allow it, else 'false'.",									"")
	re_method_0_metadata(isFullscreen,				re_ret_type(bool),						"Returns whether or not the frontend is currently fullscreen or not. Returns 'true' if the frontend is currently fullscreen, else 'false'.",					"")
	re_method_1_metadata(setFullscreen,				re_ret_type(void),		bool,			"Sets whether or not the frontend is currently fullscreen or not. 'true' as first parameter if the frontend is currently fullscreen, else 'false'.",			"")
	// Mouse methods
	re_method_0_metadata(isMouseOver,				re_ret_type(bool),						"Returns whether or not the mouse cursor is currently over the frontend. Returns 'true' if the mouse cursor is currently over the frontend, else 'false'.",		"")
	re_method_0_metadata(getMousePositionX,			re_ret_type(int),						"Returns the current mouse cursor X position inside the frontend, negative value if the mouse cursor isn't currently over the frontend",						"")
	re_method_0_metadata(getMousePositionY,			re_ret_type(int),						"Returns the current mouse cursor Y position inside the frontend, negative value if the mouse cursor isn't currently over the frontend",						"")
	re_method_0_metadata(isMouseVisible,			re_ret_type(bool),						"Returns whether or not the mouse cursor is currently visible. Returns 'true' if the mouse cursor is currently visible, else 'false'.",							"")
	re_method_1_metadata(setMouseVisible,			re_ret_type(void),		bool,			"Set the mouse cursor visibility. 'true' as first parameter if the mouse cursor shall be visible.",																"")
	re_method_1_metadata(setTrapMouse,				re_ret_type(void),		bool,			"Trap the mouse inside the frontend. 'true' as first parameter if the mouse should be trapped inside the frontend, else 'false'.",								"")
re_class_metadata_end(Frontend)


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Run the frontend
*/
int Frontend::run(const FrontendContext &cFrontendContext, bool bUrgentMessageAllowed)
{
	int nResult = -1;	// Error by default

	// Create a frontend implementation instance
	FrontendImpl *pFrontendImpl = createFrontendImplementation(cFrontendContext);
	if (pFrontendImpl) {
		// Create a frontend instance
		Frontend *pFrontend = FrontendImpl::createFrontend(cFrontendContext, *pFrontendImpl);
		if (pFrontend) {
			// Do the frontend life cycle thing - let the world know that we have been created
			pFrontendImpl->onCreate();

			// Let the world know that this frontend is now going to run
			pFrontendImpl->onRun(cFrontendContext.getExecutableFilename(), cFrontendContext.getArguments());

			// Let the frontend implementation run
			nResult = pFrontendImpl->run(cFrontendContext.getExecutableFilename(), cFrontendContext.getArguments());

			// Do the frontend life cycle thing - let the world know that we're going to die
			pFrontendImpl->onDestroy();

			// Destroy the frontend
			delete pFrontend;
		} else {
			// Error!
			if (bUrgentMessageAllowed)
				Platform::instance().urgentMessage("Failed to create the frontend instance (\"" + cFrontendContext.getFrontend() + "\")");
		}

		// Destroy the frontend implementation
		delete pFrontendImpl;
	} else {
		// Error!
		if (bUrgentMessageAllowed)
			Platform::instance().urgentMessage("Failed to create the frontend implementation instance \"" + cFrontendContext.getFrontendImplementation() + "\".\n\n" \
												 "-> Is the RacoonEngine runtime registered correctly? (see e.g. \"RacoonEngineBuild.pdf\" for details)\n" \
												 "-> If yes, is there a RTTI class with the given name?\n" \
												 "-> If yes, if the RTTI class within a plugin, is this plugin active and is it possible to load it?");
	}

	// Done
	return nResult;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Frontend::Frontend(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl) :
	m_cFrontendContext(cFrontendContext),
	m_pFrontendImpl(&cFrontendImpl)
{
	// Set frontend
	m_pFrontendImpl->m_pFrontend = this;
}

/**
*  @brief
*    Destructor
*/
Frontend::~Frontend()
{
}


//[-------------------------------------------------------]
//[ Protected virtual Frontend functions                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the frontend is run
*/
void Frontend::onRun(const String &sExecutableFilename, const std::vector<String> &lstArguments)
{
	// No default implementation
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a implementation frontend instance
*/
FrontendImpl *Frontend::createFrontendImplementation(const FrontendContext &cFrontendContext)
{
	// Get the frontend implementation RTTI class
	const Class *pClass = ClassManager::instance().getClass(cFrontendContext.getFrontendImplementation());
	if (pClass && pClass->isDerivedFrom("RECore::FrontendImpl")) {
		// Create the frontend RTTI class instance
		Object *pObject = cFrontendContext.getFrontendImplementationConstructor().length() ? pClass->create(cFrontendContext.getFrontendImplementationConstructor(), cFrontendContext.getFrontendImplementationConstructorParameters()) : pClass->create();
		if (pObject) {
			// Write down a log message
			RE_LOG(Info, "Using frontend implementation '" + pClass->getClassName() + "': " + pClass->getDescription())

			// Set parameters for the instanced frontend implementation RTTI class
			if (cFrontendContext.getFrontendImplementationParameters().length())
				pObject->setValues(cFrontendContext.getFrontendImplementationParameters());

			// Done
			return static_cast<FrontendImpl*>(pObject);
		} else {
			// Error!
			RE_LOG(Error, "Failed instancing frontend implementation '" + pClass->getClassName() + "': " + pClass->getDescription())
		}
	} else {
		// Error!
		RE_LOG(Error, "Frontend '" + cFrontendContext.getFrontendImplementation() + "' is no valid frontend implementation RTTI class")
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
