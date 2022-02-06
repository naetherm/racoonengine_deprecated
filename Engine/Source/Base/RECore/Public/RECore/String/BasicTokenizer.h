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
#include <stack>
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template<typename TCharType> class BasicBufferedReader;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tokenizer class for parsing texts (also called 'scanner' or 'lexer' -> lexical analysis)
*
*  @remarks
*    The tokenizer parses an input stream (e.g. a file or string) and produces
*    a subsequent list of tokens by filtering out whitespace, comments and such.
*    There are many settings that can be manipulated to serve your needs, like choosing
*    the type of comment tags or the characters that shall be considered whitespace.
*    In general, the tokenizer should only be used to produce a list of tokens,
*    after that a real parser is used to check the syntax and semantics of the
*    parsed code. In addition to this basic functionality the class also provides
*    some advanced functions to parse typical syntax like equations (a=b) or vectors
*    (a b c). These functions can be used if no strict syntax is needed, e.g. for
*    config files. But they are not intended to replace a decent parser.
*
*  @verbatim
*    Usage example:
*    Tokenizer cTokenizer;                         // Tokenizer instance
*    String sText = "This is a test";              // Text to be parsed
*    cTokenizer.Start(sText);                      // Start tokenizer
*    String sToken;                                // String for current token
*    sToken = cTokenizer.GetNextToken();           // Will return 'This'
*    sToken = cTokenizer.GetNextToken();           // Will return 'is'
*    sToken = cTokenizer.GetNextToken();           // Will return 'a'
*    sToken = cTokenizer.GetNextToken();           // Will return 'test'
*  @endverbatim
*/
template<typename TCharType>
class BasicTokenizer {


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  */
  RECORE_API BasicTokenizer();

  /**
  *  @brief
  *    Destructor
  */
  inline ~BasicTokenizer();

  /**
  *  @brief
  *    Get delimiters (characters that a treated as whitespace)
  *
  *  @return
  *    Delimiter characters
  */
  inline BasicString<TCharType> GetDelimiters() const;

  /**
  *  @brief
  *    Set delimiters (characters that a treated as whitespace)
  *
  *  @param[in] sDelimiters
  *    Delimiter characters
  *
  *  @remarks
  *    Default: " \t\r\n"
  */
  inline void SetDelimiters(const BasicString<TCharType> &sDelimiters);

  /**
  *  @brief
  *    Get single characters
  *
  *  @return
  *    Single characters
  */
  inline BasicString<TCharType> GetSingleChars() const;

  /**
  *  @brief
  *    Set single characters
  *
  *  @param[in] sSingleChars
  *    Single characters
  *
  *  @remarks
  *    Default: "{}()[]<*>/=,;\""
  */
  inline void SetSingleChars(const BasicString<TCharType> &sSingleChars);

  /**
  *  @brief
  *    Get characters that are used for quotes
  *
  *  @return
  *    Quote characters
  */
  inline BasicString<TCharType> GetQuotes() const;

  /**
  *  @brief
  *    Set characters that are used for quotes
  *
  *  @param[in] sQuotes
  *    Quote characters
  *
  *  @remarks
  *    Default: "\"\'"
  */
  inline void SetQuotes(const BasicString<TCharType> &sQuotes);

  /**
  *  @brief
  *    Get the string that starts a multi-line comment
  *
  *  @return
  *    Comment start tag
  */
  inline BasicString<TCharType> GetCommentStartTag() const;

  /**
  *  @brief
  *    Set the string that starts a multi-line comment
  *
  *  @param[in] sCommentStartTag
  *    Comment start tag
  *
  *  @remarks
  *    Default is slash-star ("/ *" without the space in the middle, just written with a space to keep C++ compilers happy).
  */
  inline void SetCommentStartTag(const BasicString<TCharType> &sCommentStartTag);

  /**
  *  @brief
  *    Get the string that ends a multi-line comment
  *
  *  @return
  *    Comment end tag
  */
  inline BasicString<TCharType> GetCommentEndTag() const;

  /**
  *  @brief
  *    Set the string that ends a multi-line comment
  *
  *  @param[in] sCommentEndTag
  *    Comment end tag
  *
  *  @remarks
  *    Default: "*\/"
  */
  inline void SetCommentEndTag(const BasicString<TCharType> &sCommentEndTag);

  /**
  *  @brief
  *    Get the string that starts a single-line comment
  *
  *  @return
  *    Comment start tag
  */
  inline BasicString<TCharType> GetSingleLineComment() const;

  /**
  *  @brief
  *    Set the string that starts a single-line comment
  *
  *  @param[in] sSingleLineComment
  *    Comment start tag
  *
  *  @remarks
  *    Default: "//"
  */
  inline void SetSingleLineComment(const BasicString<TCharType> &sSingleLineComment);

  /**
  *  @brief
  *    Get case sensitivity flag
  *
  *  @return
  *    'true' if the text is parsed case sensitive
  */
  inline bool isCaseSensitive() const;

  /**
  *  @brief
  *    Set case sensitivity flag
  *
  *  @param[in] bCaseSensitive
  *    'true' if the text is parsed case sensitive (default is false)
  *
  *  @remarks
  *    Default: false
  */
  inline void SetCaseSensitive(bool bCaseSensitive);

  /**
  *  @brief
  *    Starts the tokenizer on a string
  *
  *  @param[in] sBuffer
  *    String buffer
  */
  RECORE_API void Start(const BasicString<TCharType> &sBuffer);

  /**
  *  @brief
  *    Starts the tokenizer on a file
  *
  *  @param[in] cFile
  *    File to read, released automatically if Stop() is called
  */
  //RECORE_API void Start(File &cFile);

  /**
  *  @brief
  *    Stops the tokenizer
  */
  RECORE_API void Stop();

  /**
  *  @brief
  *    Reads all tokens until the end of the stream
  *
  *  @return
  *    Vector of all tokens of the stream
  */
  RECORE_API std::vector<BasicString<TCharType>> GetTokens();

  /**
  *  @brief
  *    Reads the next token from the stream
  *
  *  @return
  *    Next token
  *
  *  @note
  *    - After the token has been read this function goes to the next token in the stream
  *    - To get the read token again, use GetToken()
  *
  *  @see
  *    - GetToken()
  */
  RECORE_API BasicString<TCharType> GetNextToken();

  /**
  *  @brief
  *    Expects the next token to be equal to a given string
  *
  *  @return
  *    'true' if the next token is equal to the string
  *
  *  @note
  *    - If the expected token has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  *    - The comparison of strings is done according to the settings given in SetCaseSensitive()
  *
  *  @see
  *    - isCaseSensitive()
  *    - SetCaseSensitive()
  */
  RECORE_API bool ExpectToken(const BasicString<TCharType> &sExpected);

  /**
  *  @brief
  *    Finds a given token in the stream
  *
  *  @return
  *    'true' if the token has been found
  *
  *  @note
  *    - Reads the next token until the expected token has been found or the end of the stream has been reached
  *    - If the function has succeed, the next call of GetNextToken() will return the desired token
  *    - The comparison of strings is done according to the settings given in SetCaseSensitive()
  *
  *  @see
  *    - isCaseSensitive()
  *    - SetCaseSensitive()
  */
  RECORE_API bool FindToken(const BasicString<TCharType> &sExpected);

  /**
  *  @brief
  *    Returns the current token
  *
  *  @return
  *    Current token
  *
  *  @note
  *    - Does not go to the next token in the stream, so multiple calls of this function will
  *      always return the same token.
  */
  inline BasicString<TCharType> GetToken() const;

  /**
  *  @brief
  *    Compares the current token with a given string
  *
  *  @return
  *    'true' if the token is equal to the string
  *
  *  @note
  *    - Does not go to the next token in the stream, so multiple calls of this function will
  *      always return the same token
  *    - The comparison of strings is done according to the settings given in SetCaseSensitive()
  *
  *  @see
  *    - isCaseSensitive()
  *    - SetCaseSensitive()
  */
  inline bool CompareToken(const BasicString<TCharType> &sExpected);

  /**
  *  @brief
  *    Returns the current position in the stream
  *
  *  @return
  *    Position in the stream
  */
  inline uint32 getPosition() const;

  /**
  *  @brief
  *    Returns the current line (counted by '\n' occurrences)
  *
  *  @return
  *    Line in the file
  */
  inline uint32 GetLine() const;

  /**
  *  @brief
  *    Saves the current state of the tokenizer on a state stack
  */
  RECORE_API void PushState();

  /**
  *  @brief
  *    Restores the last saved state from the stack
  */
  RECORE_API void PopState();

  /**
  *  @brief
  *    Deletes the last saved state from the stack
  */
  inline void DropState();

  /**
  *  @brief
  *    Expects the next token to be a number and returns it as an integer value
  *
  *  @param[out] nNumber
  *    Receives the number
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @note
  *    - If a number has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  inline bool ParseNumber(int &nNumber);

  /**
  *  @brief
  *    Expects the next token to be a floating point number and returns it as a float value
  *
  *  @param[out] fNumber
  *    Receives the number
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @note
  *    - If a number has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  inline bool ParseNumber(float &fNumber);

  /**
  *  @brief
  *    Expects the next token to be a floating point number and returns it as a double value
  *
  *  @param[out] dNumber
  *    Receives the number
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @note
  *    - If a number has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  inline bool ParseNumber(double &dNumber);

  /**
  *  @brief
  *    Expects the next tokens to be a vector and returns it as an array of strings
  *
  *  @param[out] cVector
  *    Receives the vector elements
  *  @param[in]  sStart
  *    Open bracket (e.g. "[")
  *  @param[in]  sEnd
  *    Closed bracket (e.g. "]")
  *  @param[in]  sSeparator
  *    Separator between the elements (e.g. ","). Can also be ""
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @remarks
  *    Example: [one, two, three]
  *
  *  @note
  *    - If a vector has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  RECORE_API bool ParseVector(std::vector<BasicString<TCharType>> &cVector, const BasicString<TCharType> &sStart = "[", const BasicString<TCharType> &sEnd = "]", const BasicString<TCharType> &sSeparator = ",");

  /**
  *  @brief
  *    Expects the next tokens to be a vector and returns it as an array of ints
  *
  *  @param[out] cVector
  *    Receives the vector elements
  *  @param[in]  sStart
  *    Open bracket (e.g. "[")
  *  @param[in]  sEnd
  *    Closed bracket (e.g. "]")
  *  @param[in]  sSeparator
  *    Separator between the elements (e.g. ","). Can also be ""
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @remarks
  *    Example: [1, 2, 3]
  *
  *  @note
  *    - If a vector has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  RECORE_API bool ParseVector(std::vector<int> &cVector, const BasicString<TCharType> &sStart = "[", const BasicString<TCharType> &sEnd = "]", const BasicString<TCharType> &sSeparator = ",");

  /**
  *  @brief
  *    Expects the next tokens to be a vector and returns it as an array of floats
  *
  *  @param[out] cVector
  *    Receives the vector elements
  *  @param[in]  sStart
  *    Open bracket (e.g. "[")
  *  @param[in]  sEnd
  *    Closed bracket (e.g. "]")
  *  @param[in]  sSeparator
  *    Separator between the elements (e.g. ","). Can also be ""
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @remarks
  *    Example: [1.0, 2.1, 3.2]
  *
  *  @note
  *    - If a vector has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  RECORE_API bool ParseVector(std::vector<float> &cVector, const BasicString<TCharType> &sStart = "[", const BasicString<TCharType> &sEnd = "]", const BasicString<TCharType> &sSeparator = ",");

  /**
  *  @brief
  *    Expects the next tokens to be a vector and returns it as an array of doubles
  *
  *  @param[out] cVector
  *    Receives the vector elements
  *  @param[in]  sStart
  *    Open bracket (e.g. "[")
  *  @param[in]  sEnd
  *    Closed bracket (e.g. "]")
  *  @param[in]  sSeparator
  *    Separator between the elements (e.g. ","). Can also be ""
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @remarks
  *    Example: [1.0, 2.1, 3.2]
  *
  *  @note
  *    - If a vector has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  RECORE_API bool ParseVector(std::vector<double> &cVector, const BasicString<TCharType> &sStart = "[", const BasicString<TCharType> &sEnd = "]", const BasicString<TCharType> &sSeparator = ",");

  /**
  *  @brief
  *    Expects the next tokens to be an equation and returns it
  *
  *  @param[out] sName
  *    Name of the element
  *  @param[out] sValue
  *    Value as a string
  *  @param[in]  sEquation
  *    Equation sign (e.g. "=")
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @remarks
  *    Example: Console = On
  *
  *  @note
  *    - If an equation has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  RECORE_API bool ParseEquation(BasicString<TCharType> &sName, BasicString<TCharType> &sValue, const BasicString<TCharType> &sEquation = "");

  /**
  *  @brief
  *    Expects the next tokens to be an equation and returns it
  *
  *  @param[out] sName
  *    Name of the element
  *  @param[out] nValue
  *    Value as an int
  *  @param[in]  sEquation
  *    Equation sign (e.g. "=")
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @remarks
  *    Example: Health = 100
  *
  *  @note
  *    - If an equation has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  inline bool ParseEquation(BasicString<TCharType> &sName, int &nValue, const BasicString<TCharType> &sEquation = "");

  /**
  *  @brief
  *    Expects the next tokens to be an equation and returns it
  *
  *  @param[out] sName
  *    Name of the element
  *  @param[out] fValue
  *    Value as a float
  *  @param[in]  sEquation
  *    Equation sign (e.g. "=")
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @remarks
  *    Example: Gamma = 2.1
  *
  *  @note
  *    - If an equation has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  inline bool ParseEquation(BasicString<TCharType> &sName, float &fValue, const BasicString<TCharType> &sEquation = "");

  /**
  *  @brief
  *    Expects the next tokens to be an equation and returns it
  *
  *  @param[out] sName
  *    Name of the element
  *  @param[out] dValue
  *    Value as a double
  *  @param[in]  sEquation
  *    Equation sign (e.g. "=")
  *
  *  @return
  *    'true' on success, else 'false'
  *
  *  @remarks
  *    Example: Speed = 3.25
  *
  *  @note
  *    - If an equation has been found, the tokenizer goes to the next token in the stream,
  *      otherwise it stays at the current token
  */
  inline bool ParseEquation(BasicString<TCharType> &sName, double &dValue, const BasicString<TCharType> &sEquation = "");


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] cSource
  *    Source to copy from
  */
  BasicTokenizer(const BasicTokenizer &cSource);

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
  BasicTokenizer &operator =(const BasicTokenizer &cSource);

  /**
  *  @brief
  *    Checks if the next string in the stream equals the given one
  *
  *  @param[in] sString
  *    String to compare with
  *
  *  @return
  *    'true' if the next string in the stream equals the given one, else 'false'
  *
  *  @note
  *    - 'm_pStream' must be valid!
  */
  inline bool StreamIsString(const BasicString<TCharType> &sString);

  /**
  *  @brief
  *    Reads the next characters of the stream
  *
  *  @param[in] nSize
  *    Number of characters to read from the stream
  *
  *  @note
  *    - 'm_pStream' must be valid!
  */
  void StreamRead(uint32 nSize);


  //[-------------------------------------------------------]
  //[ Private static data                                   ]
  //[-------------------------------------------------------]
private:
  static const BasicString<TCharType> EndOfLine;	/**< End of line ("\n") */


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Internal parse mode of the state machine
  */
  enum EParseMode {
    eEatGarbage,							/**< Eat garbage until the first character of a word has been found */
    eReadWord,								/**< Read a word until a delimiter has been found */
    eSingleChar,							/**< Read a single character word */
    eReadQuote,								/**< read the beginning of a quote */
    eReadQuoteInside,						/**< Read until the end quote has been found */
    eReadQuoteEnd,							/**< Read the end of a quote */
    eSkipComment							/**< Skip a comment until the comment end-tag has been found */
  };

  /**
  *  @brief
  *    Data structure to store the state of the tokenizer
  */
  struct STokenizerState {
    BasicString<TCharType>		m_sToken;					/**< The current token */
    uint32		m_nPosition;				/**< The current position */
    uint32		m_nLine;					/**< The current line */
    EParseMode	m_nParseMode;				/**< Current parse mode */
    BasicString<TCharType>		m_sEndTag;					/**< End tag of the current mode, used for quotes and comments */
  };

  // Tokenizer options
  BasicString<TCharType> m_sDelimiters;						/**< List of delimiters (white space) */
  BasicString<TCharType> m_sSingleChars;						/**< List of characters that count as words (e.g. : or ,) */
  BasicString<TCharType> m_sQuotes;							/**< List of characters that start quotes (e.g. " or ') */
  BasicString<TCharType> m_sCommentStartTag;					/**< Start tag for multi line comments (e.g. "(*") */
  BasicString<TCharType> m_sCommentEndTag;					/**< End tag for multi line comments (e.g. "*)") */
  BasicString<TCharType> m_sSingleLineComment;				/**< Tag for single line comments (e.g. "//") */
  bool   m_bCaseSensitive;					/**< Are comparisons case sensitive? */

  // Tokenizer status
  BasicBufferedReader<TCharType>			*m_pStream;			/**< Character stream to read from, can be a null pointer */
  BasicString<TCharType>					 m_sToken;			/**< The current token */
  uint32					 m_nPosition;		/**< The current position */
  uint32					 m_nLine;			/**< The current line */
  EParseMode				 m_nParseMode;		/**< Current parse mode */
  BasicString<TCharType>					 m_sEndTag;			/**< End tag of the current mode, used for quotes and comments */
  std::stack<STokenizerState>	 m_cStateStack;		/**< Stack of tokenizer states */
};

typedef BasicTokenizer<char> CTokenizer;
typedef BasicTokenizer<wchar_t> WTokenizer;
typedef BasicTokenizer<char16_t> U16Tokenizer;
typedef BasicTokenizer<char32_t> U32Tokenizer;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/String/BasicTokenizer.inl"