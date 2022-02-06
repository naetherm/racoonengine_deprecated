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
*  @brief
*    Abstract buffered reader base class
*
*  @remarks
*    A buffered reader is used to access a buffer or stream directly without the need to seek within the
*    stream too often. Therefore the reader stores the read data inside a buffer until it is no longer needed.
*/
template<typename TCharType>
class BasicBufferedReader {


  //[-------------------------------------------------------]
  //[ Public virtual BufferedReader functions               ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Destructor
  */
  virtual ~BasicBufferedReader() = default;

  /**
  *  @brief
  *    Closes the reader
  */
  virtual void close() = 0;

  /**
  *  @brief
  *    Check if the end of the stream has been reached
  *
  *  @return
  *    'true' if the end of the buffer has been reached
  */
  virtual bool isEof() const = 0;

  /**
  *  @brief
  *    Returns a character read at the current position
  *
  *  @return
  *    Character that has been read, '\0' on error
  */
  virtual int getChar() = 0;

  /**
  *  @brief
  *    Returns a string of a given size read at the current position
  *
  *  @param[in] nSize
  *    Number of characters to read
  *
  *  @return
  *    String that has been read, "" on error
  */
  virtual BasicString<TCharType> getString(uint32 nSize) = 0;

  /**
  *  @brief
  *    Reads a character at the current position and moves to the position after that
  *
  *  @return
  *    Character that has been read, '\0' on error
  */
  virtual int readChar() = 0;

  /**
  *  @brief
  *    Reads a string at the current position and moves to the position after that
  *
  *  @param[in] nSize
  *    Number of characters to read
  *
  *  @return
  *    String that has been read, "" on error
  */
  virtual BasicString<TCharType> readString(uint32 nSize) = 0;

  /**
  *  @brief
  *    Checks if the next string equals the given one (case sensitive)
  *
  *  @param[in] sString
  *    String to compare with (length of this string is used to compare)
  *
  *  @return
  *    'true' if the next string equals the given one, else 'false'
  */
  virtual bool isString(const BasicString<TCharType> &sString) = 0;

  /**
  *  @brief
  *    Checks if the next string equals the given one (not case sensitive)
  *
  *  @param[in] sString
  *    String to compare with (length of this string is used to compare)
  *
  *  @return
  *    'true' if the next string equals the given one, else 'false'
  */
  virtual bool isStringNoCase(const BasicString<TCharType> &sString) = 0;

  /**
  *  @brief
  *    Returns the current position
  *
  *  @return
  *    Current position
  */
  virtual uint32 tell() const = 0;

  /**
  *  @brief
  *    Set the current position
  *
  *  @param[in] nPos
  *    New position
  *
  *  @return
  *    'true' if the seek has been successful
  */
  virtual bool seek(uint32 nPos) = 0;


  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Constructor
  */
  BasicBufferedReader() = default;

  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] cSource
  *    Source to copy from
  */
  BasicBufferedReader(const BasicBufferedReader &cSource) = default;

  /**
  *  @brief
  *    Copy operator
  *
  *  @param[in] cSource
  *    Source to copy from
  *
  *  @return
  *    Reference to this instance
  */
  BasicBufferedReader &operator =(const BasicBufferedReader &cSource) = default;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore