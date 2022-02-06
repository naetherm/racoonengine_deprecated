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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get pointer to current application
*/
inline CoreApplication *CoreApplication::getApplication()
{
	// Return global application pointer
	return g_pApplication;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
inline const ApplicationContext& CoreApplication::getApplicationContext() const {
  return mApplicationContext;
}

/**
*  @brief
*    Get application name
*/
inline String CoreApplication::getName() const
{
	// Return name
	return m_sName;
}

/**
*  @brief
*    Get application title
*/
inline String CoreApplication::getTitle() const
{
	// Return title
	return m_sTitle;
}

/**
*  @brief
*    Set application title
*/
inline void CoreApplication::setTitle(const String &sTitle)
{
	// Set title
	m_sTitle = sTitle;
}

/**
*  @brief
*    Get version of application
*/
inline const Version &CoreApplication::getVersion() const
{
	// Return version
	return m_cVersion;
}

/**
*  @brief
*    Set version of application
*/
inline void CoreApplication::setVersion(const Version &cVersion)
{
	// Set version
	m_cVersion = cVersion;
}

/**
*  @brief
*    Check if application uses multi-user environment
*/
inline bool CoreApplication::getMultiUser() const
{
	// Return multi-user flag
	return m_bMultiUser;
}

/**
*  @brief
*    Set if application uses multi-user environment
*/
inline void CoreApplication::setMultiUser(bool bMultiUser)
{
	// Set multi-user flag
	m_bMultiUser = bMultiUser;
}

/**
*  @brief
*    Get name of config file
*/
inline String CoreApplication::getConfigName() const
{
	// Return config name
	return m_sConfigName;
}

/**
*  @brief
*    Set name of config file
*/
inline void CoreApplication::setConfigName(const String &sConfigName)
{
	// Set config name
	m_sConfigName = sConfigName;
}

/**
*  @brief
*    Get name of log file
*/
inline String CoreApplication::getLogName() const
{
	// Return log name
	return m_sLogName;
}

/**
*  @brief
*    Set name of log file
*/
inline void CoreApplication::setLogName(const String &sLogName)
{
	// Set log name
	m_sLogName = sLogName;
}

/**
*  @brief
*    Get subdirectory for application data files
*/
inline String CoreApplication::getAppDataSubdir() const
{
	// Return application data subdirectory
	return m_sAppDataSubdir;
}

/**
*  @brief
*    Set subdirectory for application data files
*/
inline void CoreApplication::setAppDataSubdir(const String &sSubdir)
{
	// Set application data subdirectory
	m_sAppDataSubdir = sSubdir;
}

/**
*  @brief
*    Returns whether or not the application is currently running
*/
inline bool CoreApplication::isRunning() const
{
	// Set config
	return m_bRunning;
}

/**
*  @brief
*    Exit application
*/
inline void CoreApplication::exit(int nResult)
{
	// Set result
	m_nResult = nResult;

	// Stop application
	m_bRunning = false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
