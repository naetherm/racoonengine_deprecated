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
#include "RECore/Platform/Platform.h"
//#include "RECore/Tools/Localization.h"
//#include "RECore/Tools/LoadableManager.h"
//#include "RECore/Tools/LocalizationGroup.h"
#include "RECore/Frontend/FrontendContext.h"
#include "RECore/Frontend/FrontendApplication.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(FrontendApplication, "RECore", RECore::CoreApplication, "Frontend application class")
	// Methods
	re_method_0_metadata(getFrontend,	re_ret_type(Frontend&),	"Returns the frontend this application is running in.",	"")
re_class_metadata_end(FrontendApplication)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendApplication::FrontendApplication(Frontend &cFrontend) : CoreApplication(),
	m_pFrontend(&cFrontend)
{
	// Set application name and title
	setName(m_pFrontend->getFrontendContext().getName());
	setTitle(m_pFrontend->getFrontendContext().getName());

	// Set running state
	m_bRunning = true;
}

/**
*  @brief
*    Destructor
*/
FrontendApplication::~FrontendApplication()
{
}

//[-------------------------------------------------------]
//[ Protected virtual AbstractLifecycle functions         ]
//[-------------------------------------------------------]
/**
*  @brief
*    De-initialization function that is called after OnDeInit()
*/
void FrontendApplication::onStop()
{
	{ // Write down display mode information
		int nX = 0;
		int nY = 0;
		uint32 nWidth = 0;
		uint32 nHeight = 0;
		getFrontend().getWindowPositionSize(nX, nY, nWidth, nHeight);
		//GetConfig().SetVar("RECore::FrontendConfig", "X",      String(nX));
		//GetConfig().SetVar("RECore::FrontendConfig", "Y",      String(nY));
		//GetConfig().SetVar("RECore::FrontendConfig", "Width",  String(nWidth));
		//GetConfig().SetVar("RECore::FrontendConfig", "Height", String(nHeight));
	}

	// Call base implementation
	CoreApplication::onStop();
}

bool FrontendApplication::onStart() {
  if (CoreApplication::onStart()) {


    getFrontend().setWindowPositionSize(12, 42, 800, 600);

    return true;
  }

  return false;
}

//[-------------------------------------------------------]
//[ Protected virtual AbstractFrontend functions          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the window size has been changed
*/
void FrontendApplication::onSize()
{
	// No default implementation
}

/**
*  @brief
*    Called when the fullscreen mode was changed
*/
void FrontendApplication::onFullscreenMode()
{
	// No default implementation
}

/**
*  @brief
*    Called to let the frontend draw into it's window
*/
void FrontendApplication::onDraw()
{
	// No default implementation
}

/**
*  @brief
*    Called to let the frontend update it's states
*/
void FrontendApplication::onUpdate()
{
	// No default implementation
}

/**
*  @brief
*    Called when string data has been dropped onto the frontend window
*/
void FrontendApplication::onDrop(const std::vector<String> &lstFiles)
{
	// No default implementation
}


//[-------------------------------------------------------]
//[ Protected virtual CoreApplication functions           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when application should initialize it's configuration
*/
void FrontendApplication::onInitConfig()
{
	// Call base implementation
	CoreApplication::onInitConfig();

  WindowDescription _cDesc;
  _cDesc.setWidthOnScreen(600);
  _cDesc.setHeightOnScreen(480);
  _cDesc.setPositionOnScreenX(12);
  _cDesc.setPositionOnScreenX(42);

	// Read frontend configuration and set frontend window position and size of the previous session
	getFrontend().setWindowPositionSize(12, 42, 800, 600);
}

/**
*  @brief
*    Called when application should load it's plugins
*/
void FrontendApplication::onInitPlugins()
{
  /*
	// The "Scan PL-runtime directory for compatible plugins and load them in"-part is job of a higher being

	// Scan for plugins in the application executable directory, but not recursively, please. This is
	// quite useful for shipping applications and putting all plugins inside the application root directory
	// (which is necessary due to VC manifest policy)
	ClassManager::instance()->ScanPlugins(m_cApplicationContext.GetExecutableDirectory(), NonRecursive);

	// There's no guarantee that the application executable directory is the same as the application startup directory
	// -> If the application executable directory is not the same as the application startup directory, do also scan the application startup directory
	// -> This behavior is quite useful because it allows development of plugins which can be tested within e.g. PLViewer without copying files around,
	//    just set the current directory to your plugin directory when launching the viewer application
	const String sStartupDirectory = m_cApplicationContext.GetStartupDirectory();
	if (m_cApplicationContext.GetExecutableDirectory() != sStartupDirectory) {
		// Scan for plugins in the application startup directory, but not recursively, please
		ClassManager::instance()->ScanPlugins(sStartupDirectory, NonRecursive);
	}
 */
}

/**
*  @brief
*    Called when application should set it's data paths
*/
void FrontendApplication::onInitData()
{
  /*
	// The "Scan PL-runtime directory for compatible data and register it"-part is job of a higher being

	// Is '.' (= the current directory) already a base directory? If not, add it right now...
	LoadableManager *pLoadableManager = LoadableManager::GetInstance();
	if (!pLoadableManager->IsBaseDir('.'))
		pLoadableManager->AddBaseDir('.');

	// Is the application directory already a base directory? If not, add it right now...
	if (!pLoadableManager->IsBaseDir(m_cApplicationContext.GetAppDirectory()))
		pLoadableManager->AddBaseDir(m_cApplicationContext.GetAppDirectory());

	// The application and current directories should have the highest priority of all base directories because the application data is most likely in those directories
	pLoadableManager->SetBaseDirPriority(m_cApplicationContext.GetAppDirectory(), pLoadableManager->GetBaseDir(0));
	pLoadableManager->SetBaseDirPriority('.', pLoadableManager->GetBaseDir(1));

	// Scan for packages in current "Data" directory
	const String sCurrentDir = Platform::GetInstance()->GetCurrentDir();
	pLoadableManager->ScanPackages(sCurrentDir + "/Data");

	// Scan for packages in application's "Data" directory
	if (sCurrentDir != m_cApplicationContext.GetAppDirectory())
		pLoadableManager->ScanPackages(m_cApplicationContext.GetAppDirectory() + "/Data");

	// Get localization language (from config or from default)
	String sLanguage = m_cConfig.GetVar("RECore::CoreConfig", "Language");
	if (!sLanguage.length()) {
		// Use always English instead of the current program locale language so that we have a known default behavior
		sLanguage = "English";
	}

	// Setup localization system
	Localization::GetInstance()->SetLanguage(sLanguage);
	if (sLanguage.length()) {
		LocalizationGroup *pLocalizationGroup = Localization::GetInstance()->AddGroup(Localization::RacoonEngine);
		if (pLocalizationGroup)
			pLocalizationGroup->LoadByFilename("Data/Misc/" + Localization::RacoonEngine + '_' + sLanguage + ".loc");
	}
 */
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
