#////////////////////////////////////////////////////////////////////////////////////////////////////
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
#ifdef LINUX
	#include <signal.h>
#endif
#include "RECore/Runtime.h"
#include "RECore/Log/Log.h"
#include "RECore/Asset/AssetManager.h"
#include "RECore/Resource/ResourceStreamer.h"
#include "RECore/File/IFileManager.h"
#include "RECore/Platform/Platform.h"
#include "RECore/Platform/Console.h"
#include "RECore/Time/TimeManager.h"
#include "RECore/Application/CoreApplication.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
re_class_metadata(CoreApplication, "RECore", RECore::Object, "Application class")
	// Constructors
	re_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	re_method_1_metadata(exit,					re_ret_type(void),						int,	"Exit application. Return code for application as first parameter (usually 0 means no error).",	"")
re_class_metadata_end(CoreApplication)


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
CoreApplication *CoreApplication::g_pApplication = nullptr;	/**< Pointer to the current application instance */


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
CoreApplication::CoreApplication() :
	m_bMultiUser(true),
	m_bRunning(false),
	m_nResult(0)
{
	// Set global application pointer
	g_pApplication = this;

	// Set default application name and title
	//m_sName  = Url(Url(Platform::instance().getExecutableFilename()).GetFilename()).CutExtension();
	m_sTitle = m_sName;

	// Set default config and log filenames
	m_sConfigName = m_sName + ".cfg";
	m_sLogName = m_sName + ".log";

	// Set default application data subdir to "RacoonEngine" (Windows) respectively ".pixellight" (Linux)
	// -> Why this setting? At first, this was "m_sAppDataSubdir = System::instance()->GetDataDirName(m_sName);" resulting in e.g.
	//    "C:\Users\COfenberg\AppData\Roaming\50RendererTriangleD", "C:\Users\COfenberg\AppData\Roaming\50RendererTriangle", "C:\Users\COfenberg\AppData\Roaming\XTest42"
	//    and so on. Worst case is, that in the end, "Roaming" is full of spam files and it's hard to figure out what's trash and what's important. So, by default, everything
	//    RacoonEngine based will be thrown into a common "RacoonEngine"-directory. More experienced users may then, explicitly, change this into an individual directory.
	//m_sAppDataSubdir = Platform::instance().getDataDirName("RacoonEngine");

	// Add standard command line options
	m_cCommandLine.addFlag(		"Help",			"-h", "--help",			"Display help");
	m_cCommandLine.addFlag(		"About",		"-V", "--version",		"Display version");
	m_cCommandLine.addFlag(		"Verbose",		"-v", "--verbose",		"Write log messages onto the console");
	m_cCommandLine.addParameter("LogFile",		"-l", "--logfile",		"Log filename",		"");
	m_cCommandLine.addParameter("ConfigFile",	"-c", "--configfile",	"Config filename",	"");
}

/**
*  @brief
*    Destructor
*/
CoreApplication::~CoreApplication()
{
	// Reset global application pointer
	g_pApplication = nullptr;
}

/**
*  @brief
*    Set application name
*/
void CoreApplication::setName(const String &sName)
{
	// Semi-hack: Adjust config and log filenames, if they are set to their default values
	if (m_sConfigName == m_sName + ".cfg")
		m_sConfigName = sName + ".cfg";
	if (m_sLogName == m_sName + ".log")
		m_sLogName = sName + ".log";

	// Set new name
	m_sName = sName;
}

const CoreContext &CoreApplication::getCoreContext() const {
  return this->mcCoreContext;
}

CoreContext &CoreApplication::getCoreContext() {
  return this->mcCoreContext;
}

/**
*  @brief
*    Run the application
*/
int CoreApplication::run(const String &sExecutableFilename, const std::vector<String> &lstArguments)
{
	// Connect Linux signals
	#ifdef LINUX
		signal(SIGINT,  CoreApplication::signalHandler);
		signal(SIGTERM, CoreApplication::signalHandler);
	#endif

  // Fill application context
  mApplicationContext.setArguments(lstArguments);

	// Do the life cycle thing - let the world know that we have been created (must be done in here instead of within the constructor)
	onCreate();

	// The application is now running
	m_bRunning = true;

	// Initialize application
	m_nResult  = 0;
	if (onStart()) {
		onResume();

		// Call application-specific initialization routine
		onInit();

		// Run application
		main();

		// Call application-specific de-initialization routine
		onDeInit();

		// Do the frontend life cycle thing - de-initialize
		onPause();
		onStop();
	}

	// The application is no longer running
	m_bRunning = false;

	// Do the life cycle thing - let the world know that we're going to die (should be done in here instead of within the destructor)
	onDestroy();

	// Exit
	return m_nResult;
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractLifecycle functions         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called directly after the object has been created
*/
void CoreApplication::onCreate()
{
	// No default implementation
}

/**
*  @brief
*    Called directly before a stopped object is going to start again (always followed by "OnStart()")
*/
void CoreApplication::onRestart()
{
	// No default implementation
}

/**
*  @brief
*    Initialization function that is called prior to OnInit()
*/
bool CoreApplication::onStart()
{

  // Initialize the filesystem
  this->onInitFilesystem();

	// Parse command line
	m_cCommandLine.parseCommandLine(getApplicationContext().getArguments());


	// Create log
  onInitLog();
	if (!m_bRunning)
		return false;

	// Check command line
  onInitCmdLine();
	if (!m_bRunning)
		return false;

	// Load configuration
  onInitConfig();
	if (!m_bRunning)
		return false;

	// Init plugins
  onInitPlugins();
	if (!m_bRunning)
		return false;

	// Init data
  onInitData();
	if (!m_bRunning)
		return false;

	// No error
	return true;
}

/**
*  @brief
*    Called when the object has the focus (keep the implementation lightweight)
*/
void CoreApplication::onResume()
{
	// No default implementation
}

/**
*  @brief
*    Called when the object has no longer the focus (keep the implementation lightweight)
*/
void CoreApplication::onPause()
{
	// No default implementation
}

/**
*  @brief
*    De-initialization function that is called after OnDeInit()
*/
void CoreApplication::onStop()
{
	// Save configuration
  //const String sFilename = m_cApplicationContext.GetConfigFilename();
  //if (sFilename.length())
  //	m_cConfig.SaveByFilename(sFilename);

	// Close log
	//Log::instance()->close();
}

/**
*  @brief
*    Called before the object is going to be finally destroyed
*/
void CoreApplication::onDestroy()
{
	// No default implementation
}


//[-------------------------------------------------------]
//[ Protected virtual CoreApplication functions           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Main function
*/
void CoreApplication::main()
{

}

void CoreApplication::onInitFilesystem() {
  RE_LOG(Info, "CoreApplication::onInitFilesystem")

  // The root directory of the current application is automatically added as the absolute root directory
  //this->mcCoreContext.getFileManager().mountDirectory(Url(Platform::instance().GetCurrentDir() + "/Content").GetNativePath(), "content");
  //this->mcCoreContext.getFileManager().mountDirectory(Url(Platform::instance().GetCurrentDir() + "/Plugins").GetNativePath(), "plugins");
  //this->mcCoreContext.getFileManager().mountDirectory(Url(Platform::instance().GetCurrentDir() + "/Configs").GetNativePath(), "configs");
  //this->mcCoreContext.getFileManager().mountDirectory(Url(Platform::instance().GetCurrentDir() + "/Logs").GetNativePath(), "logs");
}

/**
*  @brief
*    Called when application should initialize it's log
*/
void CoreApplication::onInitLog()
{
  /*
	{ // Write some general information into the log
		// Create the log header:
		RE_LOG(Info, "Log-system started")
		RE_LOG(Quiet, '\n')
		RE_LOG(Info, "< " + Runtime::getVersion().ToString() + " >")
		RE_LOG(Info, "RECore build: " __DATE__ " " __TIME__ "")
		RE_LOG(Info, "Application start time: " + Platform::instance().GetTime().ToString())
		RE_LOG(Info, "\nRECore information:\n" + Platform::instance().GetInfo() + '\n')
		RE_LOG(Quiet, '\n')

		{ // Print out directory information:
			// System directories
			RE_LOG(Info, "System directories:")
			RE_LOG(Info, "- Executable: " + Platform::instance().GetExecutableFilename())
			RE_LOG(Info, "- Home:       " + Platform::instance().GetUserHomeDir())
			RE_LOG(Info, "- Data:       " + Platform::instance().GetUserDataDir())
			RE_LOG(Info, "- Current:    " + Platform::instance().GetCurrentDir())
			RE_LOG(Quiet, '\n')

      // Mount directories
      RE_LOG(Info, "Mounted directories:")
      RE_LOG(Info, "- LocalData: " + this->mcCoreContext.GetFileManager().GetMountPoint("LocalData"));

			// RacoonEngine directories
			RE_LOG(Info, "RacoonEngine directories:")
			RE_LOG(Info, "- Runtime:    " + Runtime::getDirectory())
			RE_LOG(Quiet, '\n')

			// Application context
			RE_LOG(Info, "Application context:")
			RE_LOG(Info, "- Executable: " + GetApplicationContext().GetExecutableFilename())
			RE_LOG(Info, "- Startup:    " + GetApplicationContext().GetStartupDirectory())
			RE_LOG(Info, "- AppDir:     " + GetApplicationContext().GetAppDirectory())
			RE_LOG(Quiet, '\n')
		}

		// Print out the general system info:
		RE_LOG(Info, String().appendFormat("CPU: %d MHz", Platform::instance().GetCPUMhz()))

		// Start enumerate messages
		RE_LOG(Quiet, "\n\n")
		RE_LOG(Info, "Messages:")
	}
 */
}

/**
*  @brief
*    Called when application should check command line options
*/
void CoreApplication::onInitCmdLine()
{
	// Check command line
	if (m_cCommandLine.hasErrors() || m_cCommandLine.isValueSet("--help")) {
		// Display help and exit
		onPrintHelp();
		exit(1);
	} else if (m_cCommandLine.isValueSet("--version")) {
		// Display about and exit
		onPrintVersion();
		exit(0);
	}
}

/**
*  @brief
*    Called when application should initialize it's configuration
*/
void CoreApplication::onInitConfig()
{
  /*
	// Check if a config file has been given on the command line
  String sFilename = m_cCommandLine.GetValue("--configfile");
	if (sFilename.length()) {
		// Try to load, do nothing if that fails - command line is command line ;-)
		m_cConfig.LoadByFilename(sFilename);
	} else {
		// Could not open config file, try standard locations
		if (m_sConfigName.length()) {
			// Create user data directory, if it does not yet exist
			Directory cDir(Platform::instance().GetUserDataDir() + '/' + m_sAppDataSubdir);
			if (!cDir.Exists())
				cDir.Create();

			// Try user directory first, then use application (!) directory
			sFilename = Platform::instance().GetUserDataDir() + '/' + m_sAppDataSubdir + '/' + m_sConfigName;
			if (!(m_bMultiUser && m_cConfig.LoadByFilename(sFilename))) {
				// Not successful, so try application directory instead
				sFilename = m_cApplicationContext.GetAppDirectory() + '/' + m_sConfigName;
				if (!m_cConfig.LoadByFilename(sFilename)) {
					// Error: Could not open config file
					sFilename = "";
				}
			}
		}
	}

	// If no config file could be found, set a filename anyway so that the config can be saved later
	if (!sFilename.length()) {
		// Only if we want a config at all
		if (m_sConfigName.length()) {
			// Try user directory first, then use application directory
			Directory cDir(Platform::instance().GetUserDataDir() + '/' + m_sAppDataSubdir);
			if (m_bMultiUser && cDir.Exists())
				sFilename = Platform::instance().GetUserDataDir() + '/' + m_sAppDataSubdir + '/' + m_sConfigName;
			else
				sFilename = m_cApplicationContext.GetAppDirectory() + '/' + m_sConfigName;
		}
	}

	// Save config filename in application context
	m_cApplicationContext.SetConfigFilename(sFilename);

	// Analyze configuration
	if (m_sConfigName.length()) {
		// Is this the first program start?
		if (m_cConfig.GetVar("RECore::CoreConfig", "FirstRun").getBool()) {
			// Call virtual function
			OnFirstProgramStart();

			// Reset flag
			m_cConfig.SetVar("RECore::CoreConfig", "FirstRun", "0");
		}
	}
 */
}

/**
*  @brief
*    Called when application should load it's plugins
*/
void CoreApplication::onInitPlugins()
{
  /*
	// Scan PL-runtime directory for compatible plugins and load them in
	Runtime::ScanDirectoryPlugins();

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
void CoreApplication::onInitData()
{
  /*
	// Scan PL-runtime directory for compatible data and register it
	Runtime::ScanDirectoryData();

	// Is '.' (= the current directory) already a base directory? If not, add it right now...
	LoadableManager *pLoadableManager = LoadableManager::instance();
	if (!pLoadableManager->IsBaseDir('.'))
		pLoadableManager->AddBaseDir('.');

	// Is the application directory already a base directory? If not, add it right now...
	if (!pLoadableManager->IsBaseDir(m_cApplicationContext.GetAppDirectory()))
		pLoadableManager->AddBaseDir(m_cApplicationContext.GetAppDirectory());

	// The application and current directories should have the highest priority of all base directories because the application data is most likely in those directories
	pLoadableManager->SetBaseDirPriority(m_cApplicationContext.GetAppDirectory(), pLoadableManager->GetBaseDir(0));
	pLoadableManager->SetBaseDirPriority('.', pLoadableManager->GetBaseDir(1));

	// Scan for packages in current "Data" directory
	const String sCurrentDir = Platform::instance().GetCurrentDir();
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
	Localization::instance()->SetLanguage(sLanguage);
	if (sLanguage.length()) {
		LocalizationGroup *pLocalizationGroup = Localization::instance()->AddGroup(Localization::RacoonEngine);
		if (pLocalizationGroup)
			pLocalizationGroup->LoadByFilename("Data/Misc/" + Localization::RacoonEngine + '_' + sLanguage + ".loc");
	}
 */
}

/**
*  @brief
*    Called when application should initialize itself
*/
void CoreApplication::onInit()
{
	// Reset timing class
	//TimeManager::instance()->Reset();
}

/**
*  @brief
*    Called when application should de-initialize itself
*/
void CoreApplication::onDeInit()
{
	// No default implementation
}

/**
*  @brief
*    Function that is called when the program has been started for the first time
*/
void CoreApplication::onFirstProgramStart()
{
	// Write message into log
	RE_LOG(Info, "First application start detected")
}

/**
*  @brief
*    Function that is called to display a help message about the application
*/
void CoreApplication::onPrintHelp()
{
	// Print application title
	Platform::instance().getConsole().print(m_sTitle + "\n\n");

	// Print available command line options
	m_cCommandLine.printHelp(m_sName);
}

/**
*  @brief
*    Function that is called to display version information of the application
*/
void CoreApplication::onPrintVersion()
{
	// Print application title and version
	Platform::instance().getConsole().print(m_sTitle + " - V" + m_cVersion.toString() + '\n');
}

/**
*  @brief
*    Function that is called when a signal has arrive
*/
bool CoreApplication::onSignal(ESignal nSignal)
{
	// Catch signal
	switch (nSignal) {
		// Interrupt (exit application by ctrl-c)
		case SignalInterrupt:
			// Exit application gracefully
			m_bRunning = false;
			return false;

		// Terminate (exit application)
		case SignalTerm:
			// Exit application gracefully
			m_bRunning = false;
			return false;
	}

	// Signal not handled, go on with default action
	return true;
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Signal handler callback
*/
void CoreApplication::signalHandler(int nSignal)
{
	// Linux implementation
	#ifdef LINUX
		// Catch signal
		switch (nSignal) {
			// Interrupt (exit application by ctrl-c)
			case SIGINT:
				// Send signal to application
				if (!CoreApplication::getApplication()->onSignal(SignalInterrupt)) {
					// Ignore signal and restore handler
					signal(SIGINT, CoreApplication::signalHandler);
				} else {
					// Signal handler has done it's job, re-raise signal
					signal(nSignal, SIG_DFL);
					raise(nSignal);
				}
				break;

			// Terminate (exit application)
			case SIGTERM:
				// Send signal to application
				if (!CoreApplication::getApplication()->onSignal(SignalTerm)) {
					// Ignore signal and restore handler
					signal(SIGTERM, CoreApplication::signalHandler);
				} else {
					// Signal handler has done it's job, re-raise signal
					signal(nSignal, SIG_DFL);
					raise(nSignal);
				}
				break;
		}
	#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
