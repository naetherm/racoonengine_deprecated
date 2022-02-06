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
#include "RECore/Log/LogFormatter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get verbose mode (write log entries to console)
*/
inline bool Log::isVerbose() const {
  return m_bVerbose;
}

/**
*  @brief
*    Set verbose mode (write log entries to console)
*/
inline void Log::setVerbose(bool bVerbose, bool bVerboseLogLevelPrefix) {
  m_bVerbose = bVerbose;
  m_bVerboseLogLevelPrefix = bVerboseLogLevelPrefix;
}


/**
*  @brief
*    Write a string into the log
*/
inline bool Log::output(uint8 nLogLevel, const std::string &sText) {
  // Check log level
  return (nLogLevel > Quiet && nLogLevel <= m_nLogLevel && write(nLogLevel, sText));
}

/**
*  @brief
*    Get current log level
*/
inline uint8 Log::getLogLevel() const {
  return m_nLogLevel;
}

/**
*  @brief
*    Set current log level
*/
inline void Log::setLogLevel(uint8 nLogLevel) {
  // Is this log level already set?
  if (m_nLogLevel != nLogLevel) {
    // Set log level
    m_nLogLevel = nLogLevel;

    // Write log message
    write(Debug, "[Enter log level \"" + logLevelToString(nLogLevel) + "\"]");
  }
}

/**
*  @brief
*    Get log level for automatic flush
*/
inline uint8 Log::getFlushLogLevel() const {
  return m_nFlushLogLevel;
}

/**
*  @brief
*    Set log level for automatic flush
*/
inline void Log::setFlushLogLevel(uint8 nFlushLogLevel) {
  // Change only if the new flush log level is not equal to the current one
  if (nFlushLogLevel != m_nFlushLogLevel) {
    // An explicit flush is necessary when the new flush log level is lower then the old one
    if (nFlushLogLevel < m_nFlushLogLevel)
      flush();

    // Set the new flush log level
    m_nFlushLogLevel = nFlushLogLevel;
  }
}

/**
*  @brief
*    Flush output buffer to disk
*/
inline bool Log::flush() {
  // Is there anything to flush?
  if (m_nFlushMessages && m_pLogFormatter) {
    // Flush on disc
    m_nFlushMessages = 0;
    return m_pLogFormatter->flush();
  } else {
    // No need to flush anything
    return false;
  }
}

/**
*  @brief
*    Get current amount of buffered log messages
*/
inline uint32 Log::getBufferCount() const {
  return m_nBufferedMessages;
}

/**
*  @brief
*    Get a copy of the queue which holds the last log messages
*/
inline const std::queue<std::string> &Log::getLastMessages() const {
  return m_qLastMessages;
}

/**
*  @brief
*    Get current log formatter
*/
inline LogFormatter *Log::getLogFormatter() const {
  return m_pLogFormatter;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
