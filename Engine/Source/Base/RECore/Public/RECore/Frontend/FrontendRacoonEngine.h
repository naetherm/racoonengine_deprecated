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
#include "RECore/Frontend/Frontend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class FrontendApplication;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    RacoonEngine frontend
*
*  @remarks
*    This frontend controls an embedded RacoonEngine frontend application, meaning application classes
*    derived from "FrontendApplication". Just think of this frontend class as a puppet master
*    (puppet = application class) controlled by another puppet master (the frontend implementation,
*    e.g. a simple native OS window or a browser such as MS Internet Explorer or Mozilla Firefox).
*/
class FrontendRacoonEngine : public Frontend {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	re_class_def(RECORE_API)
		// Attributes
		re_attribute_directvalue(ApplicationClass,					String,	"Application",	ReadWrite)
		re_attribute_directvalue(ApplicationConstructor,			String,	"",				ReadWrite)
		re_attribute_directvalue(ApplicationConstructorParameters,	String,	"",				ReadWrite)
		re_attribute_directvalue(ApplicationParameters,				String,	"",				ReadWrite)
	re_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFrontendContext
		*    Frontend context to use (just shared, the given instance must stay valid as long as this frontend lives)
		*  @param[in] cFrontendImpl
		*    Frontend implementation instance
		*/
		RECORE_API FrontendRacoonEngine(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl);

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~FrontendRacoonEngine();


	//[-------------------------------------------------------]
	//[ Public virtual Frontend functions                     ]
	//[-------------------------------------------------------]
	public:
		RECORE_API virtual bool isRunning() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual Frontend functions                  ]
	//[-------------------------------------------------------]
	protected:
		RECORE_API virtual void onRun(const String &sExecutableFilename, const std::vector<String> &lstArguments);


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractLifecycle functions         ]
	//[-------------------------------------------------------]
	protected:
		RECORE_API virtual void onCreate() override;
		RECORE_API virtual void onRestart() override;
		RECORE_API virtual bool onStart() override;
		RECORE_API virtual void onResume() override;
		RECORE_API virtual void onPause() override;
		RECORE_API virtual void onStop() override;
		RECORE_API virtual void onDestroy() override;


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
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FrontendApplication *m_pFrontendApplication;			/**< Frontend application instance, can be a null pointer */
		bool				 m_bFrontendApplicationInitialized;	/**< Frontend application successfully initialized? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
