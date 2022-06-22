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
#include "RECore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Pipe
 *
 * @brief
 * Class to create and access system pipes (named or unnamed)
 */
class Pipe {


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Default constructor
   */
  inline Pipe();

  /**
   * @brief
   * Copy constructor
   *
   * @param[in] cSource
   * Source to copy
   */
  inline Pipe(const Pipe &rhs);

  /**
   * @brief
   * Destructor
   */
  inline ~Pipe();

  /**
   * @brief
   * Assignment operator
   *
   * @param[in] rhs
   * Source to copy
   *
   * @return
   * Reference to this Pipe
   */
  inline Pipe &operator=(const Pipe &rhs);

  /**
   * @brief
   * Comparison operator
   *
   * @param[in] rhs
   * Pipe to compare with
   *
   * @return
   * 'true', if both Pipe's are equal, else 'false'
   */
  inline bool operator==(const Pipe &rhs) const;

  /**
   * @brief
   * Comparison operator
   *
   * @param[in] rhs
   * Pipe to compare with
   *
   *  @return
   * 'true', if the Pipe's are different, else 'false'
   */
  inline bool operator!=(const Pipe &rhs) const;

  /**
   * @brief
   * Creates a new unnamed pipe
   *
   * @return
   * 'true', if the pipe could be created, else 'false'
   */
  RECORE_API bool create();

  /**
   * @brief
   * Creates a new named pipe
   *
   * @param[in] sName
   * Name of the pipe
   *
   * @return
   * 'true', if the pipe could be created, else 'false'
   */
  RECORE_API bool create(const String &name);

  /**
   * @brief
   * Open a pipe by file handles
   *
   * @param[in] hRead
   * Handle to the read end of the pipe
   * @param[in] hWrite
   * Handle to the write end of the pipe
   *
   * @return
   * 'true', if the pipe could be opened, else 'false'
   */
  inline bool open(handle hRead, handle hWrite);

  /**
   * @brief
   * Close read side of the pipe
   *
   * @return
   * 'true', if the read side could be closed, else 'false'
   */
  RECORE_API bool closeRead();

  /**
   * @brief
   * Close write side of the pipe
   *
   * @return
   * 'true', if the write side could be closed, else 'false'
   */
  RECORE_API bool closeWrite();

  /**
   * @brief
   * Close both sides of the pipe
   *
   * @return
   * 'true', if the pipe could be closed, else 'false'
   */
  inline bool close();

  /**
   * @brief
   * Get name of pipe
   *
   * @return
   * Name of the pipe
   */
  inline String getName() const;

  /**
   * @brief
   * Get read handle for the pipe
   *
   * @return
   * Handle for the read-end of the pipe
   *
   * @note
   * - On Linux, the handle is a file handle of type int
   * - On Windows, the handle is a file handle of type HANDLE
   */
  inline handle getReadHandle() const;

  /**
   * @brief
   * Get write handle for the pipe
   *
   * @return
   * Handle for the write-end of the pipe
   *
   * @see
   * - getReadHandle()
   */
  inline handle getWriteHandle() const;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // Pipe data
  String mName;  /**< Name of the pipe */
#if defined(Win32)
  handle mPipe[2];  /**< Pipe handles (0 = read end, 1 = write end) */
#elif defined(LINUX)
  int mPipe[2];
#endif
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/System/Pipe.inl"