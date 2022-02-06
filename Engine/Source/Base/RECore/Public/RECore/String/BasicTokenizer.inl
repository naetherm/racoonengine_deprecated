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
//#include "RECore/String/BasicBufferedReaderFile.h"
#include "RECore/String/BasicBufferedReaderString.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
template<typename TCharType>
const BasicString<TCharType> BasicTokenizer<TCharType>::EndOfLine = '\n';


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TCharType>
BasicTokenizer<TCharType>::BasicTokenizer():
m_sDelimiters(" \t\r\n"),
m_sSingleChars("{}()[]<*>/=,;\""),
m_sQuotes("\"\'"),
m_sCommentStartTag("/*"),
m_sCommentEndTag("*/"),
m_sSingleLineComment("//"),
m_bCaseSensitive(false),
m_pStream(nullptr),
m_nPosition(0),
m_nLine(0),
m_nParseMode(eEatGarbage) {

}

template<typename TCharType>
BasicTokenizer<TCharType>::~BasicTokenizer() {
  // Stop tokenizer
  Stop();
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::GetDelimiters() const {
  return m_sDelimiters;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::SetDelimiters(const BasicString<TCharType> &sDelimiters) {
  m_sDelimiters = sDelimiters;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::GetSingleChars() const {
  return m_sSingleChars;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::SetSingleChars(const BasicString<TCharType> &sSingleChars) {
  m_sSingleChars = sSingleChars;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::GetQuotes() const {
  return m_sQuotes;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::SetQuotes(const BasicString<TCharType> &sQuotes) {
  m_sQuotes = sQuotes;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::GetCommentStartTag() const {
  return m_sCommentStartTag;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::SetCommentStartTag(const BasicString<TCharType> &sCommentStartTag) {
  m_sCommentStartTag = sCommentStartTag;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::GetCommentEndTag() const {
  return m_sCommentEndTag;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::SetCommentEndTag(const BasicString<TCharType> &sCommentEndTag) {
  m_sCommentEndTag = sCommentEndTag;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::GetSingleLineComment() const {
  return m_sSingleLineComment;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::SetSingleLineComment(const BasicString<TCharType> &sSingleLineComment) {
  m_sSingleLineComment = sSingleLineComment;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::isCaseSensitive() const {
  return m_bCaseSensitive;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::SetCaseSensitive(bool bCaseSensitive) {
  m_bCaseSensitive = bCaseSensitive;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::Start(const BasicString<TCharType> &sBuffer) {
  // Start tokenizer
  Stop();
  m_pStream = new BasicBufferedReaderString<TCharType>(sBuffer);
}
/*
template<typename TCharType>
void BasicTokenizer<TCharType>::Start(File &cFile) {
  // Start tokenizer
  Stop();
  m_pStream = new BasicBufferedReaderFile<TCharType>(cFile);
}
*/

template<typename TCharType>
void BasicTokenizer<TCharType>::Stop() {
  // Stop tokenizer
  if (m_pStream) {
    delete m_pStream;
    m_pStream = nullptr;
  }
  m_sToken     = "";
  m_nPosition  = 0;
  m_nLine      = 0;
  m_nParseMode = eEatGarbage;
  m_sEndTag    = "";
  while (!m_cStateStack.empty()) {
    m_cStateStack.pop();
  }
}

template<typename TCharType>
std::vector<BasicString<TCharType>> BasicTokenizer<TCharType>::GetTokens() {
  // Read tokens
  std::vector<BasicString<TCharType> > cTokens;
  BasicString<TCharType> sToken = GetNextToken();
  while (sToken.length()) {
    // Add token
    cTokens.push_back(sToken);

    // Next token
    sToken = GetNextToken();
  }

  // Return array of tokens
  return cTokens;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::GetNextToken() {
  // Clear the current token
  m_sToken = "";

  // Valid stream?
  if (m_pStream) {
    // Read ASCII input
    while (!m_pStream->isEof()) {
      // Get character
      const char nChar = static_cast<char>(m_pStream->getChar());

      // Do the work
      switch (m_nParseMode) {
        // Eat garbage until the first character of a word has been found
        case eEatGarbage:
          if (m_sCommentStartTag.length() && StreamIsString(m_sCommentStartTag)) {
            // The beginning of a comment has been found
            StreamRead(m_sCommentStartTag.length());
            m_sEndTag	 = m_sCommentEndTag;
            m_nParseMode = eSkipComment;
          } else if (m_sSingleLineComment.length() && StreamIsString(m_sSingleLineComment)) {
            // The beginning of a comment has been found
            StreamRead(m_sSingleLineComment.length());
            m_sEndTag	 = EndOfLine;
            m_nParseMode = eSkipComment;
          } else if (m_sQuotes.Find(nChar) != BasicString<TCharType>::NPOS) {
            // The beginning of a quote has been found
            StreamRead(1);
            m_sEndTag	 = nChar;
            m_nParseMode = eReadQuote;
          } else if (m_sDelimiters.Find(nChar) != BasicString<TCharType>::NPOS) {
            // Only more garbage found
            StreamRead(1);
          } else if (m_sSingleChars.Find(nChar) != BasicString<TCharType>::NPOS) {
            // A single character word has been found
            m_nParseMode = eSingleChar;
          } else {
            // The beginning of a word has been found
            m_nParseMode = eReadWord;
          }
          break;

          // Read a word until a delimiter has been found
        case eReadWord:
          if (m_sCommentStartTag.length() && StreamIsString(m_sCommentStartTag)) {
            // The beginning of a comment has been found
            StreamRead(m_sCommentStartTag.length());
            m_sEndTag	 = m_sCommentEndTag;
            m_nParseMode = eSkipComment;
          } else if (m_sSingleLineComment.length() && StreamIsString(m_sSingleLineComment)) {
            // The beginning of a comment has been found
            StreamRead(m_sSingleLineComment.length());
            m_sEndTag	 = EndOfLine;
            m_nParseMode = eSkipComment;
          } else if (m_sQuotes.Find(nChar) != BasicString<TCharType>::NPOS) {
            // The beginning of a quote has been found
            StreamRead(1);
            m_sEndTag	 = nChar;
            m_nParseMode = eReadQuote;
          } else if (m_sDelimiters.Find(nChar) != BasicString<TCharType>::NPOS) {
            // A delimiter has been found
            StreamRead(1);
            m_nParseMode = eEatGarbage;
          } else if (m_sSingleChars.Find(nChar) != BasicString<TCharType>::NPOS) {
            // A single character word has been found
            m_nParseMode = eSingleChar;
          } else {
            // Read word
            m_sToken += nChar;
            StreamRead(1);
          }

          // Return word if one has been found
          if (m_nParseMode != eReadWord && m_sToken.length())
            return m_sToken;
          break;

          // Read a single character word
        case eSingleChar:
          // Return the single character word
          StreamRead(1);
          m_nParseMode = eEatGarbage;
          m_sToken	 = nChar;
          return m_sToken;

          // Read the beginning of a quote
        case eReadQuote:
          m_nParseMode = eReadQuoteInside;
          m_sToken	 = m_sEndTag;
          return m_sEndTag;

          // Read until the end quote has been found
        case eReadQuoteInside:
          if (StreamIsString(m_sEndTag)) {
            // Return string inside the quote
            m_nParseMode = eReadQuoteEnd;
            if (m_sToken.length())
              return m_sToken;
          } else {
            m_sToken += nChar;
            StreamRead(1);
          }
          break;

          // Read the end of a quote
        case eReadQuoteEnd:
          StreamRead(m_sEndTag.length());
          m_nParseMode = eEatGarbage;
          m_sToken	 = m_sEndTag;
          return m_sEndTag;

          // Skip a comment until the comment end-tag has been found
        case eSkipComment:
          if (StreamIsString(m_sEndTag)) {
            StreamRead(m_sEndTag.length());
            m_nParseMode = eEatGarbage;
          } else {
            StreamRead(1);
          }
          break;
      }
    }
  }

  // Return token
  return m_sToken;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ExpectToken(const BasicString<TCharType> &sExpected) {
  PushState();
  const BasicString<TCharType> sToken = GetNextToken();
  if (!sToken.length() || !CompareToken(sExpected)) {
    PopState();

    // The next token is not equal to the string
    return false;
  } else {
    DropState();

    // The next token is equal to the string
    return true;
  }
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::FindToken(const BasicString<TCharType> &sExpected) {
  // Check expected string
  if (sExpected.length()) {
    // Read first token
    PushState();
    BasicString<TCharType> sToken = GetNextToken();

    // Read tokens
    while (sToken.length()) {
      // Token found?
      if (CompareToken(sExpected)) {
        PopState();
        return true;
      }

      // Next token
      DropState();
      PushState();
      sToken = GetNextToken();
    }
    DropState();
  }

  // Not found
  return false;
}

template<typename TCharType>
BasicString<TCharType> BasicTokenizer<TCharType>::GetToken() const {
  // Return current token
  return m_sToken;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::CompareToken(const BasicString<TCharType> &sExpected) {
  return (m_bCaseSensitive ? m_sToken.compare(sExpected) : m_sToken.comparei(sExpected));
}

template<typename TCharType>
uint32 BasicTokenizer<TCharType>::getPosition() const {
  // Return current position
  return m_nPosition;
}

template<typename TCharType>
uint32 BasicTokenizer<TCharType>::GetLine() const {
  // Return current line
  return m_nLine;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::PushState() {
  // Push current state onto the stack
  STokenizerState sState;
  sState.m_sToken		= m_sToken;
  sState.m_nPosition	= m_nPosition;
  sState.m_nLine		= m_nLine;
  sState.m_nParseMode	= m_nParseMode;
  sState.m_sEndTag	= m_sEndTag;
  m_cStateStack.Push(sState);
}

template<typename TCharType>
void BasicTokenizer<TCharType>::PopState() {
  // Is the stack empty?
  if (m_cStateStack.size() > 0) {
    // Restore state
    STokenizerState sState = m_cStateStack.Pop();
    m_sToken	 = sState.m_sToken;
    m_nPosition	 = sState.m_nPosition;
    m_nLine		 = sState.m_nLine;
    m_nParseMode = sState.m_nParseMode;
    m_sEndTag	 = sState.m_sEndTag;
    if (m_pStream)
      m_pStream->seek(m_nPosition);
  }
}

template<typename TCharType>
void BasicTokenizer<TCharType>::DropState() {
  // Is the stack empty?
  if (m_cStateStack.size() > 0) {
    // Pop saved state from stack
    m_cStateStack.Pop();
  }
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseNumber(int &nNumber) {
  const BasicString<TCharType> sToken = GetNextToken();
  if (sToken.length()) {
    nNumber = sToken.getInt();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseNumber(float &fNumber) {
  const BasicString<TCharType> sToken = GetNextToken();
  if (sToken.length()) {
    fNumber = sToken.getFloat();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseNumber(double &dNumber) {
  const BasicString<TCharType> sToken = GetNextToken();
  if (sToken.length()) {
    dNumber = sToken.getDouble();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
bool
BasicTokenizer<TCharType>::ParseVector(std::vector<BasicString<TCharType>> &cVector, const BasicString<TCharType> &sStart,
                                       const BasicString<TCharType> &sEnd, const BasicString<TCharType> &sSeparator) {
  // Start parsing
  PushState();

  // Expect open bracket
  if (ExpectToken(sStart)) {
    // Read elements
    bool bFirst = true;
    while (!ExpectToken(sEnd)) {
      // Expect separator
      if (bFirst) {
        bFirst = false;
      } else if (sSeparator.length() && !ExpectToken(sSeparator)) {
        PopState();

        // Error!
        return false;
      }

      // Expect an element
      BasicString<TCharType> sElement = GetNextToken();
      if (!sElement.length() || CompareToken(sSeparator) || CompareToken(sEnd)) {
        // Error!
        PopState();
        return false;
      }
      cVector.Add(sElement);
    }

    // Success
    DropState();
    return true;
  } else {
    PopState();

    // Error!
    return false;
  }
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseVector(std::vector<int> &cVector, const BasicString<TCharType> &sStart,
                                            const BasicString<TCharType> &sEnd,
                                            const BasicString<TCharType> &sSeparator) {
  // Parse string vector
  std::vector<BasicString<TCharType> > cStringVector;
  if (!ParseVector(cStringVector, sStart, sEnd, sSeparator))
    return false; // Error!

  // Create int vector
  for (uint32 i=0; i<cStringVector.size(); i++)
    cVector.push_back(cStringVector[i].getInt());

  // Done
  return true;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseVector(std::vector<float> &cVector, const BasicString<TCharType> &sStart,
                                            const BasicString<TCharType> &sEnd,
                                            const BasicString<TCharType> &sSeparator) {
  // Parse string vector
  std::vector<BasicString<TCharType> > cStringVector;
  if (!ParseVector(cStringVector, sStart, sEnd, sSeparator))
    return false; // Error!

  // Create int vector
  for (uint32 i=0; i<cStringVector.size(); i++)
    cVector.push_back(cStringVector[i].getFloat());

  // Done
  return true;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseVector(std::vector<double> &cVector, const BasicString<TCharType> &sStart,
                                            const BasicString<TCharType> &sEnd,
                                            const BasicString<TCharType> &sSeparator) {
  // Parse string vector
  std::vector<BasicString<TCharType> > cStringVector;
  if (!ParseVector(cStringVector, sStart, sEnd, sSeparator))
    return false; // Error!

  // Create int vector
  for (uint32 i=0; i<cStringVector.size(); i++)
    cVector.push_back(cStringVector[i].getDouble());

  // Done
  return true;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseEquation(BasicString<TCharType> &sName, BasicString<TCharType> &sValue,
                                              const BasicString<TCharType> &sEquation) {
  // Start parsing
  PushState();

  // Expect name
  BasicString<TCharType> sToken = GetNextToken();
  if (sToken.length()) {
    // Store name
    sName = sToken;

    // Expect equation
    sToken = GetNextToken();
    if (sToken.length()) {
      if (!sEquation.length() || CompareToken(sEquation)) {
        // Read value
        sToken = GetNextToken();
        if (sToken.length()) {
          // Store value
          sValue = sToken;
          DropState();

          // Done
          return true;
        }
      }
    }
  }

  // An error has occurred
  PopState();
  return false;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseEquation(BasicString<TCharType> &sName, int &nValue,
                                              const BasicString<TCharType> &sEquation) {
  BasicString<TCharType> sValue;
  if (ParseEquation(sName, sValue, sEquation)) {
    nValue = sValue.getInt();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseEquation(BasicString<TCharType> &sName, float &fValue,
                                              const BasicString<TCharType> &sEquation) {
  BasicString<TCharType> sValue;
  if (ParseEquation(sName, sValue, sEquation)) {
    fValue = sValue.getFloat();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::ParseEquation(BasicString<TCharType> &sName, double &dValue,
                                              const BasicString<TCharType> &sEquation) {
  BasicString<TCharType> sValue;
  if (ParseEquation(sName, sValue, sEquation)) {
    dValue = sValue.getDouble();

    // Done
    return true;
  }

  // Error!
  return false;
}

template<typename TCharType>
BasicTokenizer<TCharType>::BasicTokenizer(const BasicTokenizer<TCharType> &cSource) :
  m_sDelimiters(" \t\r\n"),
  m_sSingleChars("{}()[]<*>/=,;\""),
  m_sQuotes("\"\'"),
  m_sCommentStartTag("/*"),
  m_sCommentEndTag("*/"),
  m_sSingleLineComment("//"),
  m_bCaseSensitive(false),
  m_pStream(nullptr),
  m_nPosition(0),
  m_nLine(0),
  m_nParseMode(eEatGarbage) {

}

template<typename TCharType>
BasicTokenizer<TCharType> &BasicTokenizer<TCharType>::operator=(const BasicTokenizer<TCharType> &cSource) {
  return *this;
}

template<typename TCharType>
bool BasicTokenizer<TCharType>::StreamIsString(const BasicString<TCharType> &sString) {
  if (!m_pStream->isEof())
    return (m_bCaseSensitive ? m_pStream->isString(sString) : m_pStream->isStringNoCase(sString));
  else
    return false;
}

template<typename TCharType>
void BasicTokenizer<TCharType>::StreamRead(uint32 nSize) {
  // Read next character(s)
  for (uint32 i=0; i<nSize; i++) {
    if (m_pStream->readChar() == '\n')
      m_nLine++;
  }
  m_nPosition += nSize;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]