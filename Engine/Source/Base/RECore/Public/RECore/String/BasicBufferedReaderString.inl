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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template<typename TCharType>
BasicBufferedReaderString<TCharType>::BasicBufferedReaderString(const BasicString <TCharType> &sBuffer) :
  m_sBuffer(sBuffer),
  m_nCurrent(0) {
}

/**
*  @brief
*    Destructor
*/
template<typename TCharType>
BasicBufferedReaderString<TCharType>::~BasicBufferedReaderString() {
  // Nothing to do in here :)
}


//[-------------------------------------------------------]
//[ Public virtual BufferedReader functions               ]
//[-------------------------------------------------------]
template<typename TCharType>
void BasicBufferedReaderString<TCharType>::close() {
  // Close stream
  m_sBuffer = "";
  m_nCurrent = 0;
}

template<typename TCharType>
bool BasicBufferedReaderString<TCharType>::isEof() const {
  // Return if the stream is valid and there is more data to read
  return (m_nCurrent >= m_sBuffer.length());
}

template<typename TCharType>
int BasicBufferedReaderString<TCharType>::getChar() {
  // Return the current character, the string class performs the range check for us :)
  return m_sBuffer[m_nCurrent];
}

template<typename TCharType>
BasicString <TCharType> BasicBufferedReaderString<TCharType>::getString(uint32 nSize) {
  // Check whether we can read the requested number of characters
  if (m_nCurrent + nSize <= m_sBuffer.length()) {
    // Get the requested substring
    return m_sBuffer.substr(m_nCurrent, nSize);
  }

  // Error!
  return "";
}

template<typename TCharType>
int BasicBufferedReaderString<TCharType>::readChar() {
  // Check whether we can read a character
  if (m_nCurrent + 1 <= m_sBuffer.length()) {
    // Get the requested character and update the current character index
    return m_sBuffer[m_nCurrent++];
  }

  // Error!
  return '\0';
}

template<typename TCharType>
BasicString <TCharType> BasicBufferedReaderString<TCharType>::readString(uint32 nSize) {
  // Check whether we can read the requested number of characters
  if (m_nCurrent + nSize <= m_sBuffer.length()) {
    // Update the current character index
    m_nCurrent += nSize;

    // Get the requested substring
    return m_sBuffer.substr(m_nCurrent - nSize, nSize);
  }

  // Error!
  return "";
}

template<typename TCharType>
bool BasicBufferedReaderString<TCharType>::isString(const BasicString <TCharType> &sString) {
  return m_sBuffer.compare(m_nCurrent, sString.length(), sString) == 0;
}

template<typename TCharType>
bool BasicBufferedReaderString<TCharType>::isStringNoCase(const BasicString<TCharType> &sString) {
  return m_sBuffer.comparei(m_nCurrent, sString.length(), sString) == 0;
}

template<typename TCharType>
uint32 BasicBufferedReaderString<TCharType>::tell() const {
  return m_nCurrent;
}

template<typename TCharType>
bool BasicBufferedReaderString<TCharType>::seek(uint32 nPos) {
  // Is the given position valid?
  if (nPos < m_sBuffer.length()) {
    // Set the new position
    m_nCurrent = nPos;

    // Done
    return true;
  }

  // Error!
  return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]