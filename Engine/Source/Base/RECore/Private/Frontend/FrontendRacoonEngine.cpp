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
#include "RECore/Reflect/Class.h"
#include "RECore/Platform/Platform.h"
#include "RECore/Frontend/FrontendImpl.h"
#include "RECore/Frontend/FrontendApplication.h"
#include "RECore/Frontend/FrontendRacoonEngine.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(FrontendRacoonEngine, "RECore", RECore::Frontend, "RacoonEngine frontend")
	// Constructors
	re_constructor_2_metadata(ParameterConstructor,	const FrontendContext&, FrontendImpl&,	"Parameter constructor. Frontend context this frontend is using as first parameter, frontend implementation this frontend is using as second parameter.",	"")
	// Attributes
	re_attribute_metadata(ApplicationClass,					String,	"Application",	ReadWrite,	"Name of the frontend application RTTI class to use",				"")
	re_attribute_metadata(ApplicationConstructor,			String,	"",				ReadWrite,	"Name of the frontend application RTTI class constructor to use",	"")
	re_attribute_metadata(ApplicationConstructorParameters,	String,	"",				ReadWrite,	"Parameters for the frontend application RTTI class constructor",	"")
	re_attribute_metadata(ApplicationParameters,			String,	"",				ReadWrite,	"Parameters for the frontend application RTTI class instance",		"")
re_class_metadata_end(FrontendRacoonEngine)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendRacoonEngine::FrontendRacoonEngine(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl) : Frontend(cFrontendContext, cFrontendImpl),
	ApplicationClass(this),
	ApplicationConstructor(this),
	ApplicationConstructorParameters(this),
	ApplicationParameters(this),
	m_pFrontendApplication(nullptr),
	m_bFrontendApplicationInitialized(false)
{
}

/**
*  @brief
*    Destructor
*/
FrontendRacoonEngine::~FrontendRacoonEngine()
{
}


//[-------------------------------------------------------]
//[ Public virtual Frontend functions                     ]
//[-------------------------------------------------------]
bool FrontendRacoonEngine::isRunning() const
{
	return (m_pFrontendApplication && m_bFrontendApplicationInitialized && m_pFrontendApplication->isRunning());
}


//[-------------------------------------------------------]
//[ Protected virtual Frontend functions                  ]
//[-------------------------------------------------------]
void FrontendRacoonEngine::onRun(const String &sExecutableFilename, const std::vector<String> &lstArguments)
{
	// Fill application context
	if (m_pFrontendApplication) {
		ApplicationContext &cApplicationContext = m_pFrontendApplication->mApplicationContext;
		//cApplicationContext.SetStartupDirectory(Platform::GetInstance()->GetCurrentDir());
		//cApplicationContext.SetExecutableFilename(sExecutableFilename);
		cApplicationContext.setArguments(lstArguments);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractLifecycle functions         ]
//[-------------------------------------------------------]
void FrontendRacoonEngine::onCreate()
{
	// Get the frontend application RTTI class
	const Class *pClass = ClassManager::instance().getClass(ApplicationClass.get());
	if (pClass && pClass->isDerivedFrom("RECore::FrontendApplication")) {
		// Create the frontend application RTTI class instance
		if (ApplicationConstructor.get().length())
			m_pFrontendApplication = static_cast<FrontendApplication*>(pClass->create(ApplicationConstructor.get(), "Frontend=\"" + Type<Frontend&>::convertToString(*this) + "\" " + ApplicationConstructorParameters.get()));
		else
			m_pFrontendApplication = static_cast<FrontendApplication*>(pClass->create(Params<Object*, Frontend&>(*this)));
		if (m_pFrontendApplication) {
			// Set parameters for the instanced frontend application RTTI class
			if (ApplicationParameters.get().length())
				m_pFrontendApplication->setValues(ApplicationParameters.get());

			// Do the frontend life cycle thing - let the world know that we have been created
			m_pFrontendApplication->onCreate();
		}
	}
}

void FrontendRacoonEngine::onRestart()
{
	// Do the frontend life cycle thing
	if (m_pFrontendApplication)
		m_pFrontendApplication->onRestart();
}

bool FrontendRacoonEngine::onStart()
{
	// Do the frontend life cycle thing
	if (m_pFrontendApplication && m_pFrontendApplication->onStart()) {
		// Frontend application successfully initialized (the following "onInit()" is just application-specific, not framework specific)
		m_bFrontendApplicationInitialized = true;

		// Call application-specific initialization routine
		m_pFrontendApplication->onInit();

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

void FrontendRacoonEngine::onResume()
{
	// Do the frontend life cycle thing
	if (m_pFrontendApplication)
		m_pFrontendApplication->onResume();
}

void FrontendRacoonEngine::onPause()
{
	// Do the frontend life cycle thing
	if (m_pFrontendApplication)
		m_pFrontendApplication->onPause();
}

void FrontendRacoonEngine::onStop()
{
	if (m_pFrontendApplication && m_bFrontendApplicationInitialized) {
		// Call application-specific de-initialization routine
		m_pFrontendApplication->onDeInit();

		// Do the frontend life cycle thing
		m_pFrontendApplication->onStop();

		// Frontend application is no longer initialized
		m_bFrontendApplicationInitialized = false;
	}
}

void FrontendRacoonEngine::onDestroy()
{
	// Destroy the frontend application instance
	if (m_pFrontendApplication) {
		// Do the frontend life cycle thing - let the world know that we're going to die
		m_pFrontendApplication->onDestroy();

		// Destroy the frontend application instance
		delete m_pFrontendApplication;
		m_pFrontendApplication = nullptr;
	}
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractFrontend functions          ]
//[-------------------------------------------------------]
void FrontendRacoonEngine::onSize()
{
	// Call virtual function from application (don't check "m_bFrontendApplicationInitialized" in here because it's valid that this method is called during the initialization process)
	if (m_pFrontendApplication)
		m_pFrontendApplication->onSize();
}

void FrontendRacoonEngine::onFullscreenMode()
{
	// Call virtual function from application (don't check "m_bFrontendApplicationInitialized" in here because it's valid that this method is called during the initialization process)
	if (m_pFrontendApplication)
		m_pFrontendApplication->onFullscreenMode();
}

void FrontendRacoonEngine::onDraw()
{
	// Call virtual function from application
	if (m_pFrontendApplication && m_bFrontendApplicationInitialized)
		m_pFrontendApplication->onDraw();
}

void FrontendRacoonEngine::onUpdate()
{
	// Call virtual function from application
	if (m_pFrontendApplication && m_bFrontendApplicationInitialized)
		m_pFrontendApplication->onUpdate();
}

void FrontendRacoonEngine::onDrop(const std::vector<String> &lstFiles)
{
	// Call virtual function from application
	if (m_pFrontendApplication && m_bFrontendApplicationInitialized)
		m_pFrontendApplication->onDrop(lstFiles);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
