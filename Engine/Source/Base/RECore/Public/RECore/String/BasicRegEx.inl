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
#include <pcre.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
// Maximum number of arguments we can set
static const int MaxGroups	= 11;
static const int VecSize	= (MaxGroups+1) * 3;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TCharType>
BasicString<TCharType> BasicRegEx<TCharType>::WildcardToRegEx(const BasicString<TCharType> &sWildcard) {
  BasicString<TCharType> sString = '^';

  // Loop through all characters
  for (uint32 i=0, nLength=sWildcard.length(); i<nLength; i++) {
    const char szCharacter = sWildcard[i];
    switch (szCharacter) {
      case '*':
        sString += ".*";
        break;

      case '?':
        sString += '.';
        break;

        // Escape special regex-characters
      case '(': case ')': case '[': case ']': case '$':
      case '^': case '.': case '{': case '}': case '|':
      case '\\':
        sString += '\\';
        sString += szCharacter;
        break;

      default:
        sString += szCharacter;
        break;
    }
  }

  // End
  sString += '$';

  // Return the regular expression
  return sString;
}

template<typename TCharType>
BasicRegEx<TCharType>::BasicRegEx() :
m_pPCRE(nullptr),
m_pExtra(nullptr),
m_nFlags(MatchCaseSensitive | MatchGreedy),
m_nPosition(0) {

}

template<typename TCharType>
BasicRegEx<TCharType>::BasicRegEx(const BasicString<TCharType> &sExpression, uint32 nFlags) :
m_pPCRE(nullptr),
m_pExtra(nullptr),
m_nFlags(nFlags),
m_nPosition(0) {
  // Set expression
  setExpression(sExpression, nFlags);
}

template<typename TCharType>
BasicRegEx<TCharType>::BasicRegEx(const BasicRegEx &cRegEx) :
m_pPCRE(nullptr),
m_pExtra(nullptr),
m_nFlags(cRegEx.m_nFlags),
m_nPosition(0) {
  // Copy expression
  setExpression(cRegEx.m_sExpression, cRegEx.m_nFlags);
}

template<typename TCharType>
BasicRegEx<TCharType>::~BasicRegEx() {
  // Release expression
  freePCRE();
}

template<typename TCharType>
BasicRegEx<TCharType> &BasicRegEx<TCharType>::operator=(const BasicRegEx &cRegEx) {
  // Copy expression
  setExpression(cRegEx.m_sExpression, cRegEx.m_nFlags);

  // Return this instance
  return *this;
}

template<typename TCharType>
BasicString<TCharType> BasicRegEx<TCharType>::getExpression() const {
  // Return expression
  return m_sExpression;
}

template<typename TCharType>
void BasicRegEx<TCharType>::setExpression(const BasicString<TCharType> &sExpression, uint32 nFlags) {
  // Save expression and options
  m_sExpression	= sExpression;
  m_nFlags		= nFlags;

  // Any encoding flags set by the user?
  const bool bEncodingASCII = (m_nFlags & EncodingASCII) != 0;
  const bool bEncodingUTF8  = (m_nFlags & EncodingUTF8)  != 0;

  // ASCII and UTF8 flags set at the same time?
  if (bEncodingASCII && bEncodingUTF8)
    m_nFlags |= ~EncodingASCII; // Remove the ASCII flag!

    // Get the encoding automatically?
  else if (!bEncodingASCII && !bEncodingUTF8)
    m_nFlags |= EncodingASCII;

  // Compile expression
  compilePCRE();
}

template<typename TCharType>
bool BasicRegEx<TCharType>::isValid() const {
  // Return valid flag
  return (m_pPCRE != nullptr);
}

template<typename TCharType>
uint32 BasicRegEx<TCharType>::getFlags() const {
  return m_nFlags;
}

template<typename TCharType>
bool BasicRegEx<TCharType>::isCaseSensitive() const {
  return ((m_nFlags & MatchCaseSensitive) != 0);
}

template<typename TCharType>
bool BasicRegEx<TCharType>::isGreedy() const {
  return ((m_nFlags & MatchGreedy) != 0);
}

template<typename TCharType>
bool BasicRegEx<TCharType>::isASCII() const {
  return ((m_nFlags & EncodingASCII) != 0);
}

template<typename TCharType>
bool BasicRegEx<TCharType>::isUTF8() const {
  return ((m_nFlags & EncodingUTF8) != 0);
}

template<typename TCharType>
void BasicRegEx<TCharType>::study() {
  if (m_pPCRE) {
    const char *pszError;
    m_pExtra = pcre_study(m_pPCRE, 0, &pszError);
  }
}

template<typename TCharType>
bool BasicRegEx<TCharType>::match(const BasicString<TCharType> &sSubject, uint32 nPosition) {
  if (m_pPCRE) {
    // Clear results of last operation
    clearResults();

    // Early escape test: In case we try matching when we are already at the end of the string, we know the result without doing any matches
    if (nPosition < sSubject.length()) {
      // Match pattern
      int nMatches[VecSize];
      int nMatchCount = pcre_exec(
        m_pPCRE,								// PCRE expression
        m_pExtra,								// Extra data
        sSubject.data(),					// Subject string
        sSubject.size(),	// Subject length in bytes
        nPosition,								// Start at byte position
        0,										// Default options
        nMatches,								// Output vector with offsets in bytes
        VecSize									// Size of output vector
      );

      // Output vector too small?
      if (nMatchCount == 0) {
        // Allocate big enough output vector and retry
        // [TODO]
        nMatchCount = MaxGroups;
      }

      // Check result
      if (nMatchCount > 0) {
        // Save new byte position within the string
        m_nPosition = nMatches[1];

        // Get pointer to subject string
        const char *pszSubject = sSubject.cstr();

        // The first two elements in nMatches describe the result string which was matched by the expression
        // e.g. source string is: "Hallo World!", the expression is "Hal+o" -> first element = "Hallo"
        m_sMatchResult.copy(&pszSubject[nMatches[0]], nMatches[1] - nMatches[0]);

        // Save matching substrings which have a group description in the expression by index
        m_lstGroups.reserve(nMatchCount-1);
        for (int i=1; i<nMatchCount; i++)
          m_lstGroups[i-1].copy(&pszSubject[nMatches[i*2]], nMatches[i*2+1] - nMatches[i*2]);

        // Get named groups
        int nNameCount;
        pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMECOUNT, &nNameCount);
        const char *pszNameTable;
        pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMETABLE, &pszNameTable);
        int nNameEntrySize;
        pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMEENTRYSIZE, &nNameEntrySize);

        // Return matching substrings by name
        for (int i=0; i<nNameCount; i++) {
          // Get pointer to current entry
          const char *pszEntry = &pszNameTable[i*nNameEntrySize];

          // Get number
          const int nNum = (pszEntry[0] << 8) | pszEntry[1];

          // Get name
          const BasicString<TCharType> sName = &pszEntry[2];

          // Get substring
          const int nIndex0 = nMatches[nNum*2];
          const int nIndex1 = nMatches[nNum*2+1];

          // Add name->substring to map
          if (nIndex0 >= 0 && nIndex1 >= 0)
            m_mapGroups.emplace(sName, String(&pszSubject[nIndex0], true, nIndex1 - nIndex0));
          else
            m_mapGroups.emplace(sName, "");
        }

        // Done
        return true;
      }
    }
  }

  // Error!
  return false;
}

template<typename TCharType>
int BasicRegEx<TCharType>::getPosition() const {
  return m_nPosition;
}

template<typename TCharType>
const std::vector<BasicString<TCharType>> &BasicRegEx<TCharType>::getResults() const {
  return this->m_lstGroups;
}

template<typename TCharType>
BasicString<TCharType> BasicRegEx<TCharType>::getResult(uint32 nIndex) const {
  return m_lstGroups[nIndex];
}

template<typename TCharType>
const std::map<BasicString<TCharType>, BasicString<TCharType>> &BasicRegEx<TCharType>::getNameResults() const {
  return this->m_mapGroups;
}

template<typename TCharType>
BasicString<TCharType> BasicRegEx<TCharType>::getNameResult(const BasicString<TCharType> &sName) const {
  return m_mapGroups.Get(sName);
}

template<typename TCharType>
BasicString<TCharType> BasicRegEx<TCharType>::getMatchResult() const {
  return m_sMatchResult;
}

template<typename TCharType>
void BasicRegEx<TCharType>::clearResults() {
  // Clear result string
  m_sMatchResult = "";

  // Clear lists for groups
  m_lstGroups.clear();
  m_mapGroups.clear();

  // Reset byte position
  m_nPosition = 0;
}

template<typename TCharType>
void BasicRegEx<TCharType>::compilePCRE() {
  // Free former expression
  freePCRE();

  // Set options
  int nOptions = (isCaseSensitive() ? 0 : PCRE_CASELESS) | (isGreedy() ? 0 : PCRE_UNGREEDY);
  if (m_nFlags & Multiline)
    nOptions |= PCRE_MULTILINE;
  if (m_nFlags & DotAll)
    nOptions |= PCRE_DOTALL;

  const char *pszError;
  int nErrorOffset;

  // Compile regular expression
  m_pPCRE = pcre_compile(m_sExpression.cstr(), nOptions, &pszError, &nErrorOffset, nullptr);


  if (!m_pPCRE) {
    // Error!
  }
}

template<typename TCharType>
void BasicRegEx<TCharType>::freePCRE() {
  // Free PCRE expression
  if (m_pPCRE) {
    pcre_free(m_pPCRE);
    m_pPCRE  = nullptr;
    m_pExtra = nullptr;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]