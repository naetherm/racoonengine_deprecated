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
//[ Header Guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"
#include "RECore/String/String.h"
#include "RECore/String/BasicBufferedReader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Buffered reader that reads from a string or character buffer
*/
template<typename TCharType>
class BasicBufferedReaderString : public BasicBufferedReader<TCharType> {


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] sBuffer
  *    String to read from
  */
  BasicBufferedReaderString(const BasicString<TCharType> &sBuffer);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~BasicBufferedReaderString();


  //[-------------------------------------------------------]
  //[ Public virtual BufferedReader functions               ]
  //[-------------------------------------------------------]
public:
  virtual void close() override;
  virtual bool isEof() const override;
  virtual int getChar() override;
  virtual BasicString<TCharType> getString(uint32 nSize) override;
  virtual int readChar() override;
  virtual BasicString<TCharType> readString(uint32 nSize) override;
  virtual bool isString(const BasicString<TCharType> &sString) override;
  virtual bool isStringNoCase(const BasicString<TCharType> &sString) override;
  virtual uint32 tell() const override;
  virtual bool seek(uint32 nPos) override;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  BasicString<TCharType> m_sBuffer;	/**< String that holds the character buffer */
  uint32 m_nCurrent;	/**< The current character index */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/String/BasicBufferedReaderString.inl"