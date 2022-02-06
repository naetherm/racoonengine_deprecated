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
#include "RECore/Tools/Version.h"
#include "RECore/Tools/CommandLine.h"
#include "RECore/Core/AbstractLifecycle.h"
#include "RECore/Application/ApplicationContext.h"
#include "RECore/Application/CoreContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Application base class
*
*  @remarks
*    An application class is a template for all kind of applications.
*    It controls the main loop of the program and provides all typical data that
*    is needed for an application. Derived classes are provided for specific tasks,
*    e.g. GUI or server applications. Please note that derived application classes
*    are allowed to hand over the main loop as well as other logic to e.g. a host
*    in order to become passive.
*
*    This class provides a most basic framework for console applications.
*    It keeps the filename and startup directory of the executable for later use and
*    provides a name, title and version of the application. It also provides an instance
*    of the command line parser to work with command line parameters (see CommandLine
*    for further explanations).
*
*  @note
*    - Implementation of the template method design pattern (although this class is not abstract)
*/
class CoreApplication : public Object, protected AbstractLifecycle {


  //[-------------------------------------------------------]
  //[ RTTI interface                                        ]
  //[-------------------------------------------------------]
re_class_def(RECORE_API)

re_class_def_end


  //[-------------------------------------------------------]
  //[ Public data types                                     ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Signal
  */
  enum ESignal {
    SignalInterrupt = 1,  /**< Application interrupted (caused by ctrl-c) */
    SignalTerm        /**< Exit application */
  };


  //[-------------------------------------------------------]
  //[ Public static functions                               ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Get pointer to current application
  *
  *  @return
  *    Pointer to application
  *
  *  @remarks
  *    The global application pointer is set when the constructor of an Application
  *    is called and reset when it's destructor is called. While this makes it safe
  *    to use two or more Application instances after each other, you should *never*
  *    use more than one Application instance at a time!
  */
  static inline CoreApplication *getApplication();


  //[-------------------------------------------------------]
  //[ Private static data                                   ]
  //[-------------------------------------------------------]
private:
  static RECORE_API CoreApplication *g_pApplication;  /**< Pointer to the current application instance */


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  */
  RECORE_API CoreApplication();

  /**
  *  @brief
  *    Destructor
  */
  RECORE_API virtual ~CoreApplication();

  /**
  *  @brief
  *    Get application name
  *
  *  @return
  *    Name of the application
  */
  inline String getName() const;

  /**
  *  @brief
  *    Set application name
  *
  *  @param[in] sName
  *    Name of the application
  */
  RECORE_API void setName(const String &sName);

  /**
  *  @brief
  *    Get application title
  *
  *  @return
  *    Title of the application
  */
  inline String getTitle() const;

  /**
  *  @brief
  *    Set application title
  *
  *  @param[in] sTitle
  *    Title of the application
  */
  inline void setTitle(const String &sTitle);

  /**
  *  @brief
  *    Get version of application
  *
  *  @return
  *    Version of the program
  */
  inline const Version &getVersion() const;

  /**
  *  @brief
  *    Set version of application
  *
  *  @param[in] cVersion
  *    Version of the program
  */
  inline void setVersion(const Version &cVersion);

  /**
  *  @brief
  *    Check if application uses multi-user environment
  *
  *  @return
  *    'true' if multi-user environment is used, else 'false'
  */
  inline bool getMultiUser() const;

  /**
  *  @brief
  *    Set if application uses multi-user environment
  *
  *  @param[in] bMultiUser
  *    'true' if multi-user environment is used, else 'false'
  *
  *  @remarks
  *    By default, multi-user environment is used.
  *    If on, e.g. config and log files are loaded and saved in the user directory
  */
  inline void setMultiUser(bool bMultiUser);

  /**
  *  @brief
  *    Get name of config file
  *
  *  @return
  *    Config filename (only filename, not a path!)
  */
  inline String getConfigName() const;

  /**
  *  @brief
  *    Set name of config file
  *
  *  @param[in] sConfigName
  *    Config filename (only filename, not a path!)
  *
  *  @remarks
  *    Default is "<appname>.cfg"
  */
  inline void setConfigName(const String &sConfigName);

  /**
  *  @brief
  *    Get name of log file
  *
  *  @return
  *    Log filename (only filename, not a path!)
  */
  inline String getLogName() const;

  /**
  *  @brief
  *    Set name of log file
  *
  *  @param[in] sLogName
  *    Log filename (only filename, not a path!)
  *
  *  @remarks
  *    Default is "<appname>.log"
  */
  inline void setLogName(const String &sLogName);

  /**
  *  @brief
  *    Get subdirectory for application data files
  *
  *  @return
  *    Subdirectory (relative path)
  */
  inline String getAppDataSubdir() const;

  /**
  *  @brief
  *    Set subdirectory for application data files
  *
  *  @param[in] sSubdir
  *    Subdirectory (relative path)
  *
  *  @remarks
  *    Default is "RacoonEngine" (Windows) respectively ".pixellight" (Linux).
  *    If you change this, you should use System::GetDataDirName(), to convert your name
  *    into the typical style for the used OS. Example:
  *      SetAppDataSubdir(System::GetInstance()->GetDataDirName("MyApplication"))
  */
  inline void setAppDataSubdir(const String &sSubdir);


  [[nodiscard]] inline const ApplicationContext& getApplicationContext() const;

  const CoreContext &getCoreContext() const;

  CoreContext &getCoreContext();

  /**
  *  @brief
  *    Returns whether or not the application is currently running
  *
  *  @return
  *    'true' if the application is currently running, else 'false'
  */
  inline bool isRunning() const;

  /**
  *  @brief
  *    Exit application
  *
  *  @param[in] nResult
  *    Return code for application (usually 0 means no error)
  *
  *  @remarks
  *    This will set the application return code and set m_bRunning to 'false', so that the application
  *    should exit as soon as possible. To immediately terminate the execution, use System::Exit()
  */
  inline void exit(int nResult);

  /**
  *  @brief
  *    Run the application
  *
  *  @param[in] sExecutableFilename
  *    Absolute application executable filename
  *  @param[in] lstArguments
  *    List of arguments to the program
  *
  *  @remarks
  *    The implementation does the following tasks:
  *    - Connect Linux signals
  *    - Fill application context
  *    - Call OnStart()
  *    - Call OnInit()
  *    - Call Main()
  *    - Call OnDeInit()
  *    - Call OnStop()
  *
  *  @return
  *    Exit code (usually 0 means no error)
  */
  RECORE_API int run(const String &sExecutableFilename, const std::vector<String> &lstArguments);


  //[-------------------------------------------------------]
  //[ Protected virtual AbstractLifecycle functions         ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Called directly after the object has been created
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - none (implement in derived classes)
  */
  RECORE_API virtual void onCreate() override;

  /**
  *  @brief
  *    Called directly before a stopped object is going to start again (always followed by "OnStart()")
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - none (implement in derived classes)
  */
  RECORE_API virtual void onRestart() override;

  /**
  *  @brief
  *    Initialization function that is called prior to OnInit()
  *
  *  @return
  *    'true' if all went fine, else 'false' which will stop the application
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Parse the command line (m_cCommandLine), set your options in the constructor
  *    - Call OnInitLog()
  *    - Call OnInitCmdLine()
  *    - Call OnInitConfig()
  *    - Call OnInitPlugins()
  *    - Call OnInitData()
  *    - Return and go on with OnInit()
  */
  RECORE_API virtual bool onStart() override;

  /**
  *  @brief
  *    Called when the object has the focus (keep the implementation lightweight)
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - none (implement in derived classes)
  */
  RECORE_API virtual void onResume() override;

  /**
  *  @brief
  *    Called when the object has no longer the focus (keep the implementation lightweight)
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - none (implement in derived classes)
  */
  RECORE_API virtual void onPause() override;

  /**
  *  @brief
  *    De-initialization function that is called after OnDeInit()
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Save configuration
  *    - Close log
  */
  RECORE_API virtual void onStop() override;

  /**
  *  @brief
  *    Called before the object is going to be finally destroyed
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - none (implement in derived classes)
  */
  RECORE_API virtual void onDestroy() override;


  //[-------------------------------------------------------]
  //[ Protected virtual CoreApplication functions           ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Main function
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - none (implement in derived classes)
  */
  RECORE_API virtual void main();

  /**
   * @brief
   * Initializes the file system.
   *
   * @remorks
   * The default implementation creates the file manager instance
   */
  RECORE_API virtual void onInitFilesystem();

  /**
  *  @brief
  *    Called when application should initialize it's log
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Open log file according to parameter '--logfile' (default: <appname>.log)
  *    - Set verbose mode according to parameter '--verbose'
  *    - Write some general information into the log
  *
  *  @note
  *    - Part of the application framework initialization function "OnStart()"
  */
  RECORE_API virtual void onInitLog();

  /**
  *  @brief
  *    Called when application should check command line options
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Check for command line errors or the parameters --help or --version
  *      - On '--help' or on error, call OnPrintHelp() and exit
  *      - On '--version', call OnPrintVersion() and exit
  *
  *  @note
  *    - Part of the application framework initialization function "OnStart()"
  *    - To end the application here, use Application::Exit()
  */
  RECORE_API virtual void onInitCmdLine();

  /**
  *  @brief
  *    Called when application should initialize it's configuration
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Load configuration from file <appname>.cfg
  *    - Save config file name to ApplicationContext
  *    - If a config has been loaded or created:
  *      - Read 'FirstRun' and call OnFirstRun() if set
  *
  *  @note
  *    - Part of the application framework initialization function "OnStart()"
  */
  RECORE_API virtual void onInitConfig();

  /**
  *  @brief
  *    Called when application should load it's plugins
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Scan for plugins in RacoonEngine runtime directory non-recursively
  *    - Scan for plugins in application executable directory non-recursively
  *    - If the application executable directory is not the same as the application startup directory, scan for plugins in application startup directory non-recursively
  *
  *  @note
  *    - Part of the application framework initialization function "OnStart()"
  */
  RECORE_API virtual void onInitPlugins();

  /**
  *  @brief
  *    Called when application should set it's data paths
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Set RacoonEngine runtime directory as base path in LoadableManager
  *    - Scan for data packages in RacoonEngine runtime directory
  *    - Set '.' as base path in LoadableManager
  *    - Scan for packages in "Data/" directory
  *    - Set application directory as base path in LoadableManager
  *    - Scan for packages in application directory "Data/" directory
  *    - Get current language and load RacoonEngine localization file, if no language is defined, English is used as default
  *
  *  @note
  *    - Part of the application framework initialization function "OnStart()"
  */
  RECORE_API virtual void onInitData();

  /**
  *  @brief
  *    Called when application should initialize itself
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Reset timing class
  *    - Return and go on with Main()
  */
  RECORE_API virtual void onInit();

  /**
  *  @brief
  *    Called when application should de-initialize itself
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - none (implement in derived classes)
  *    - Return and go on with DeInit()
  */
  RECORE_API virtual void onDeInit();

  /**
  *  @brief
  *    Function that is called when the program has been started for the first time
  *
  *  @remarks
  *    The default implementation does the following tasks:
  *    - Write message into log
  */
  RECORE_API virtual void onFirstProgramStart();

  /**
  *  @brief
  *    Function that is called to display a help message about the application
  *
  *  @remarks
  *    The default implementation prints the application title and it's command line
  *    options onto the screen
  */
  RECORE_API virtual void onPrintHelp();

  /**
  *  @brief
  *    Function that is called to display version information of the application
  *
  *  @remarks
  *    The default implementation prints the application title and version that is
  *    stored in Application
  */
  RECORE_API virtual void onPrintVersion();

  /**
  *  @brief
  *    Function that is called when a signal has arrive
  *
  *  @param[in] nSignal
  *    Signal
  *  @return
  *    'true' to go on with the signal (e.g. terminate application), 'false' to cancel
  *
  *  @remarks
  *    This function is called when the operation system has sent a signal to the process.
  *    Use this to make your application exit gracefully, e.g. set a flag that lets your main function exit
  *    after finishing the current task and cleaning up the application. Otherwise, your process is likely to
  *    be killed by the system.
  *
  *  @remarks
  *    The default implementation sets m_bRunning to 'false', be sure to react to this flag!
  */
  RECORE_API virtual bool onSignal(ESignal nSignal);


  //[-------------------------------------------------------]
  //[ Private static functions                              ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Signal handler callback
  *
  *  @param[in] nSignal
  *    Signal
  */
  static void signalHandler(int nSignal);


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  ApplicationContext mApplicationContext;
  CoreContext mcCoreContext; /** Core context */
  String m_sName;        /**< Name of application */
  String m_sTitle;        /**< Title of application */
  Version m_cVersion;        /**< Version of application */
  bool m_bMultiUser;      /**< Use multi-user environment? */
  String m_sConfigName;      /**< File name (not path) of config */
  String m_sLogName;        /**< File name (not path) of log */
  String m_sAppDataSubdir;    /**< Subdirectory for application data */
  CommandLine m_cCommandLine;      /**< Command line arguments */
  bool m_bRunning;        /**< Is the application currently running? */
  int m_nResult;        /**< Return code */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Application/CoreApplication.inl"
