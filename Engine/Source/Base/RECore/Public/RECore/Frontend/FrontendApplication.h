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
#include "RECore/Frontend/Frontend.h"
#include "RECore/Application/CoreApplication.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Frontend;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Frontend application class
*
*  @remarks
*    As the name of this class indicates, this is the base class for applications running
*    within a frontend (the application host). The main loop as well as other logic is handed
*    over to a host in order to become passive. Although a frontend application is always
*    running within a frontend, it shouldn't care about the frontend. Just write our application
*    as if there wouldn't be any "outer world". By following this philosophy as close as possible
*    and only accessing frontend features when really necessary, you enhance the chance that your
*    application can be embedded within other applications like browsers such as MS Internet Explorer
*    or Mozilla Firefox without problems.
*/
class FrontendApplication : public CoreApplication, protected AbstractFrontend {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FrontendRacoonEngine;


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
		*    Returns the frontend this application is running in
		*
		*  @return
		*    Frontend this application instance is running in
		*
		*  @note
		*    - Try to avoid to access the frontend whenever possible (Hollywood Principle: "Don't call us, we'll call you")
		*/
		[[nodiscard]] inline Frontend& getFrontend() const;

	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFrontend
		*    Frontend this application instance is running in
		*
		*  @note
		*    - This constructor sets the default application name and title by using the name of the frontend executable ("GetFrontend().GetContext().GetName()")
		*/
		RECORE_API FrontendApplication(Frontend &cFrontend);

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~FrontendApplication();


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractLifecycle functions         ]
	//[-------------------------------------------------------]
	protected:
  [[nodiscard]] RECORE_API bool onStart() override;

		/**
		*  @brief
		*    De-initialization function that is called after OnDeInit()
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Get frontend position and size of the current session and write frontend configuration
		*    - Everything that CoreApplication::OnStop() does
		*/
		RECORE_API void onStop() override;


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractFrontend functions          ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when the window size has been changed
		*
		*  @note
		*    - The default implementation is empty
		*/
		RECORE_API void onSize() override;

		/**
		*  @brief
		*    Called when the fullscreen mode was changed
		*
		*  @remarks
		*    This method just says "something related to fullscreen mode has been changed". Whether we
		*    changed from window mode into fullscreen mode or changed e.g. the resolution used in
		*    fullscreen mode is not really interesting in here.
		*
		*  @note
		*    - The default implementation is empty
		*/
		RECORE_API void onFullscreenMode() override;

		/**
		*  @brief
		*    Called to let the frontend draw into it's window
		*
		*  @note
		*    - The default implementation is empty
		*/
		RECORE_API void onDraw() override;

		/**
		*  @brief
		*    Called to let the frontend update it's states
		*
		*  @remarks
		*    You can use this method to do work you have to perform on a regular basis. It's
		*    recommended to keep the work done within the implementation as compact as possible.
		*    Don't use this function to perform 'polling'-everything, use events or if required
		*    for example timers or threads instead.
		*
		*  @note
		*    - The default implementation is empty
		*/
		RECORE_API void onUpdate() override;

		/**
		*  @brief
		*    Called when string data has been dropped onto the frontend window
		*
		*  @param[in] lstFiles
		*    List of file names
		*
		*  @note
		*    - The default implementation is empty
		*/
		RECORE_API void onDrop(const std::vector<String> &lstFiles) override;


	//[-------------------------------------------------------]
	//[ Protected virtual CoreApplication functions           ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when application should initialize it's configuration
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Everything that CoreApplication::OnInitConfig() does
		*    - Read frontend configuration and set frontend position and size of the previous session
		*
		*  @note
		*    - Part of the application framework initialization function "OnStart()"
		*/
		RECORE_API void onInitConfig() override;

		/**
		*  @brief
		*    Called when application should load it's plugins
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Scan for plugins in application executable directory non-recursively
		*    - If the application executable directory is not the same as the application startup directory, scan for plugins in application startup directory non-recursively
		*
		*  @note
		*    - Part of the application framework initialization function "OnStart()"
		*/
		RECORE_API void onInitPlugins() override;

		/**
		*  @brief
		*    Called when application should set it's data paths
		*
		*  @remarks
		*    The default implementation does the following tasks:
		*    - Set '.' as base path in LoadableManager
		*    - Scan for packages in "Data/" directory
		*    - Set application directory as base path in LoadableManager
		*    - Scan for packages in application directory "Data/" directory
		*    - Get current language and load RacoonEngine localization file, if no language is defined, English is used as default
		*
		*  @note
		*    - Part of the application framework initialization function "OnStart()"
		*/
		RECORE_API void onInitData() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Frontend *m_pFrontend;	/**< Frontend the application is running in, always valid! */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Frontend/FrontendApplication.inl"
