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
#include "RECore/Reflect/Tools/ParamsParser.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Parse parameter string
*/
bool ParamsParser::parseString(const String &sParameters)
{
	// Save parameter string
	m_sParameters = sParameters;
	if (sParameters.length()) {
		// Get first expression
		m_nParsePos = 0;
		return next();
	}

	// Parameter string is empty
	return false;
}

/**
*  @brief
*    Parse next parameter
*/
bool ParamsParser::next()
{
	// Parse next expression
	if (m_nParsePos > -1 && m_cRegEx.match(m_sParameters, m_nParsePos)) {
		// Get expression and new parse position
		m_nParsePos			= m_cRegEx.getPosition();
		String sExpression	= m_cRegEx.getResult(0);

		// Process the found expression
		if (sExpression[sExpression.length()-1] == '\'') {
			static CRegEx cRegEx("\\s*(\\w*)\\s*=\\s*'?\\s*([^\']*)\\s*'?");
			if (cRegEx.match(sExpression)) {
				// Get name and value
				m_sName  = cRegEx.getResult(0);
				m_sValue = cRegEx.getResult(1);
				return true;
			}
		} else {
			static CRegEx cRegEx("\\s*(\\w*)\\s*=\\s*\"?\\s*([^\"]*)\\s*\"?");
			if (cRegEx.match(sExpression)) {
				// Get name and value
				m_sName  = cRegEx.getResult(0);
				m_sValue = cRegEx.getResult(1);
				return true;
			}
		}
	}

	// Error, could not parse next expression
	m_sName		= "";
	m_sValue	= "";
	m_nParsePos	= -1;
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
