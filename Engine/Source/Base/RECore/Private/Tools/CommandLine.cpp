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
#include "RECore/String/BasicTokenizer.h"
#include "RECore/String/BasicRegEx.h"
#include "RECore/Platform/Platform.h"
#include "RECore/Platform/Console.h"
#include "RECore/Tools/CommandLineOption.h"
#include "RECore/Tools/CommandLine.h"


namespace {

namespace detail {

static constexpr RECore::uint32 LINE_LENGTH = 79;

}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Global helper functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Add spaces to a string to ensure that it has a certain length
*
*  @param[in] sString
*    Input string
*  @param[in] nLength
*    Desired length of output string
*
*  @return
*    String that has been extended to the given length
*/
String SetStringLength(const String &sString, uint32 nLength)
{
	String sAdd;
	for (uint32 i=sString.length(); i<nLength; i++)
		sAdd = sAdd + ' ';
	return sString + sAdd;
}

/**
*  @brief
*    Get maximum of two values
*
*  @param[in] nA
*    First value
*  @param[in] nB
*    Second value
*
*  @return
*    The greater of the two values
*/
uint32 Max(uint32 nA, uint32 nB)
{
	return (nA > nB ? nA : nB);
}

/**
*  @brief
*    Get minimum of two values
*
*  @param[in] nA
*    First value
*  @param[in] nB
*    Second value
*
*  @return
*    The lower of the two values
*/
uint32 Min(uint32 nA, uint32 nB)
{
	return (nA < nB ? nA : nB);
}

/**
*  @brief
*    Print text with automatic line breaks
*
*  @param[in] nIndent
*    Column at which the text output starts
*  @param[in] sText
*    Text to print
*/
void PrintDescription(uint32 nIndent, const String &sText)
{
	// Print description
	if (nIndent + sText.length() < detail::LINE_LENGTH) {
		// Description still fits on the line, just print it out
		Platform::instance().getConsole().print(sText + '\n');
	} else {
		// Description is too long to fit on the line
			  uint32 nPos   = nIndent;
		const String sEmpty = SetStringLength("", nPos);

		// Get text word for word
    RECore::uint32 nStartPos = 0;
    RECore::uint32 nIndex = 0;
    while (nIndex != RECore::String::NPOS) {
      nIndex = sText.findFirstOf(" ", nStartPos);
      String word = sText.substr(nStartPos, nIndex);

      if (nStartPos > nIndent) {
        Platform::instance().getConsole().print(' ');
        nStartPos++;
      }

      // Break line when necessary
      nStartPos += word.length();
      if (nStartPos >= 100) {
        Platform::instance().getConsole().print('\n');
        Platform::instance().getConsole().print(sEmpty);
        nStartPos = nIndent + word.length();
      }

      // Print word
      Platform::instance().getConsole().print(word);

      //nStartPos = nIndex;
    }

    /*
		static CRegEx cRegEx("\\s*([^\\s]+)");
		uint32 nParsePos = 0;
		while (cRegEx.match(sText, nParsePos)) {
			// Get next word
						 nParsePos = cRegEx.getPosition();
			const String sWord     = cRegEx.getResult(0);
			if (nPos > nIndent) {
				Platform::instance().getConsole().print(' ');
				nPos++;
			}

			// Break line when necessary
			nPos += sWord.length();
			if (nPos >= 79) {
				Platform::instance().getConsole().print('\n');
				Platform::instance().getConsole().print(sEmpty);
				nPos = nIndent + sWord.length();
			}

			// Print word
			Platform::instance().getConsole().print(sWord);
		}
    */
		// Break line for next option to be consistent to the other if-branch
		Platform::instance().getConsole().print('\n');
	}
}


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Convert an command array to a string
*/
String CommandLine::argumentsToString(const std::vector<String> &lstArray)
{
	// Are there any elements within the given array?
	if (lstArray.size()) {
		String sString;

		// Loop through all elements of the given array
		for (uint32 i=0; i<lstArray.size(); i++) {
			if (i > 0)
				sString += ' ';
			if (sString.Find(' ') != String::NPOS) {
				sString += '\"';
				sString += lstArray[i];
				sString += '\"';
			} else {
				sString += lstArray[i];
			}
		}

		// Return the constructed string
		return sString;
	} else {
		// Return the constructed string - which is astoundingly empty :D
		return "";
	}
}

/**
*  @brief
*    Convert a command string to an array
*/
std::vector<String> CommandLine::stringToArguments(const String &sCmdLine)
{
	// Initialize the tokenizer
	CTokenizer cTokenizer;
	cTokenizer.Start(sCmdLine);

	// Loop through all tokens
  std::vector<String> lstArray;
	String sToken = cTokenizer.GetNextToken();
	while (sToken.length()) {
		// Separator found?
		if (sToken == '\'' || sToken == '\"') {
			sToken = cTokenizer.GetNextToken();
			lstArray.push_back(sToken);
			sToken = cTokenizer.GetNextToken();
		} else {
			lstArray.push_back(sToken);
		}

		// Next token, please
		sToken = cTokenizer.GetNextToken();
	}
	cTokenizer.Stop();

	// Return the constructed string array
	return lstArray;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Delete all options
*/
void CommandLine::clear()
{
	// Delete options
	for (uint32 i=0; i<m_lstOptions.size(); i++)
		delete m_lstOptions[i];

	// Clear lists
	m_lstOptions.clear();
	m_mapOptions.clear();
	m_lstParameters.clear();
	m_bError = false;
}

/**
*  @brief
*    Add parameter
*/
bool CommandLine::addParameter(const String &sName, const String &sShort, const String &sLong, const String &sDescription, const String &sDefault, bool bRequired)
{
	// Check that a name and at least one flag name is given and the names have not been used before
	if ( (sName .length() && m_mapOptions.find(sName) == this->m_mapOptions.end()) && (sShort.length() || sLong.length()) &&
		 (sShort.length() == 0 || m_mapOptions.find(sShort) == this->m_mapOptions.end()) &&
		 (sLong .length() == 0 || m_mapOptions.find(sLong)  == this->m_mapOptions.end()) )
	{
		// Create option
		CommandLineOption *pOption = new CommandLineOption();
		pOption->setType(CommandLineOption::OptionParam);
		pOption->setRequired(bRequired);
		pOption->setName(sName);
		pOption->setShortName(sShort);
		pOption->setLongName(sLong);
		pOption->setDescription(sDescription);
		pOption->setDefault(sDefault);

		// Add option to lists
		m_lstOptions.push_back(pOption);
		m_mapOptions.emplace(sName, pOption);
		if (sShort.length())
			m_mapOptions.emplace(sShort, pOption);
		if (sLong .length())
			m_mapOptions.emplace(sLong,  pOption);
		return true;
	}

	// Error, wrong arguments given
	return false;
}

/**
*  @brief
*    Add flag (on/off)
*/
bool CommandLine::addFlag(const String &sName, const String &sShort, const String &sLong, const String &sDescription, bool bRequired)
{
	// Check that a name and at least one flag name is given and the names have not been used before
	if ( (sName .length() && m_mapOptions.find(sName) == this->m_mapOptions.end()) && (sShort.length() || sLong.length()) &&
		 (sShort.length() == 0 || m_mapOptions.find(sShort) == this->m_mapOptions.end()) &&
		 (sLong .length() == 0 || m_mapOptions.find(sLong)  == this->m_mapOptions.end()) )
	{
		// Create option
		CommandLineOption *pOption = new CommandLineOption();
		pOption->setType(CommandLineOption::OptionFlag);
		pOption->setRequired(bRequired);
		pOption->setName(sName);
		pOption->setShortName(sShort);
		pOption->setLongName(sLong);
		pOption->setDescription(sDescription);

		// Add option to lists
		m_lstOptions.push_back(pOption);
		m_mapOptions.emplace(sName, pOption);
		if (sShort.length())
			m_mapOptions.emplace(sShort, pOption);
		if (sLong .length())
			m_mapOptions.emplace(sLong,  pOption);
		return true;
	}

	// Error, wrong arguments given
	return false;
}

/**
*  @brief
*    Add argument
*/
bool CommandLine::addArgument(const String &sName, const String &sDescription, const String &sDefault, bool bRequired)
{
	// Check that a name is given and has not been used before
	if (sName.length() && m_mapOptions.find(sName) == this->m_mapOptions.end()) {
		// Create option
		CommandLineOption *pOption = new CommandLineOption();
		pOption->setType(CommandLineOption::OptionArgument);
		pOption->setRequired(bRequired);
		pOption->setName(sName);
		pOption->setDescription(sDescription);
		pOption->setDefault(sDefault);

		// Add option to lists
		m_lstOptions.push_back(pOption);
		if (sName.length())
			m_mapOptions.emplace(sName, pOption);
		return true;
	}

	// Error, wrong arguments given
	return false;
}

/**
*  @brief
*    Parse command line arguments
*/
bool CommandLine::parseCommandLine(const std::vector<String> &lstArgs)
{
	// Clear parameters
	m_lstParameters.clear();

	// Set default values and get enumerate arguments
	std::list<CommandLineOption*> lstArguments;
	for (uint32 i=0; i<m_lstOptions.size(); i++) {
		// Set default value
		m_lstOptions[i]->setValue(m_lstOptions[i]->getDefault());

		// Add arguments to list
		if (m_lstOptions[i]->getType() == CommandLineOption::OptionArgument)
			lstArguments.push_back(m_lstOptions[i]);
	}

	// Loop through arguments
	m_bError = false;
	uint32 nArgument = 0;
	CommandLineOption *pLastOption = nullptr;
	String sLastOption;
	for (uint32 i=0; i<lstArgs.size(); i++) {
		// Get argument
		String sArg = lstArgs[i];
		String sOption  = "";
		String sOptions = "";
		String sValue   = "";
		if (sArg.substr(0, 2) == "--") {
			// Long option found
			sOptions = ""; 
			sOption  = sArg;
			sValue   = "";
		} else if (sArg.substr(0, 1) == '-') {
			// Short option(s) found
			sOptions = sArg.substr(2);
			sOption  = sArg.substr(0, 2);
			sValue   = "";
		} else {
			sOptions = "";
			sOption  = "";
			sValue   = sArg;
		}

		// Get options
		while (sOption.length()) {
			// Process option
			if (pLastOption) {
				// Error: There was still an option waiting for it's value!
				Platform::instance().getConsole().print("Parameter " + sLastOption + " needs a value!\n");
				m_bError = true;
				return m_bError;
			} else {
				// Get option
				pLastOption = getOption(sOption);
				sLastOption = sOption;
				if (!pLastOption) {
					// Error: Unknown option!
					Platform::instance().getConsole().print("Unknown parameter " + sOption + "!\n");
					m_bError = true;
					return m_bError;
				}

				// Set 'true' for boolean options or wait for a value
				if (pLastOption->getType() == CommandLineOption::OptionFlag) {
					pLastOption->setValue("true");
					pLastOption = nullptr;
					sLastOption = "";
				}
			}

			// Next option available?
			if (sOptions.length()) {
				// Yes, get next option
				sOption  = '-' + sOptions.substr(0, 1);
				sOptions = sOptions.substr(1);
			} else {
				// No more options
				sOption = "";
			}
		}

		// Get value
		if (sValue.length()) {
			// Process value
			if (pLastOption) {
				// Set option value
				pLastOption->setValue(sValue);
				pLastOption = nullptr;
				sLastOption = "";
			} else {
				// Argument
				if (nArgument < lstArguments.size()) {
					// Set value of defined argument
          auto iter = lstArguments.begin();
          std::advance(iter, nArgument);
					(*iter)->setValue(sValue);
					nArgument++;
				} else {
					// Add additional argument
					m_lstParameters.push_back(sValue);
				}
			}
		}
	}

	// Produce an error if not all required options have been set
	for (uint32 i=0; i<m_lstOptions.size() && !m_bError; i++) {
		// Check if option has been set
		if (m_lstOptions[i]->isRequired() && !m_lstOptions[i]->isSet())
			m_bError = true;
	}

	// Return error-status
	return m_bError;
}

/**
*  @brief
*    Check if an option value is set ('true' for boolean options or any other than "" for string values)
*/
bool CommandLine::isValueSet(const String &sName) const
{
	// Get option
	const CommandLineOption *pOption = getOption(sName);
	return pOption ? pOption->isSet() : false;
}

/**
*  @brief
*    Get option value
*/
String CommandLine::getValue(const String &sName) const
{
	// Get option
	const CommandLineOption *pOption = getOption(sName);
	return pOption ? pOption->getValue() : "";
}

/**
*  @brief
*    Display a help text with all available options on the console
*/
void CommandLine::printHelp(const String &sProgramName) const
{
	// Print synopsis
	Platform::instance().getConsole().print("Usage: " + sProgramName + " [OPTIONS]");
	for (uint32 i=0; i<m_lstOptions.size(); i++) {
		const CommandLineOption *pOption = m_lstOptions[i];
		if (pOption->getType() == CommandLineOption::OptionArgument) {
			if (pOption->isRequired())
				Platform::instance().getConsole().print(" <" + pOption->getName() + ">");
			else
				Platform::instance().getConsole().print(" [" + pOption->getName() + "]");
		}
	}
	Platform::instance().getConsole().print("\n\n");

	// Do not print anything, if no options are available
	if (m_lstOptions.size() > 0) {
		// Get maximum size of option names
		uint32 nMaxName  = 1;
		uint32 nMaxShort = 2;
		uint32 nMaxLong  = 4;
		uint32 nMaxAll   = 1;
		for (uint32 i=0; i<m_lstOptions.size(); i++) {
			const CommandLineOption *pOption = m_lstOptions[i];
			if (pOption->getType() == CommandLineOption::OptionFlag) {
				nMaxShort = Max(pOption->getShortName().length(), nMaxShort);
				nMaxLong  = Max(pOption->getLongName() .length(), nMaxLong );
			} else if (pOption->getType() == CommandLineOption::OptionParam) {
				nMaxShort = Max(pOption->getShortName().length(), nMaxShort);
				nMaxLong  = Max(pOption->getLongName() .length(), nMaxLong );
				nMaxName  = Max(pOption->getName().length(),		 nMaxName);
			} else if (pOption->getType() == CommandLineOption::OptionArgument) {
				nMaxAll   = Max(pOption->getName().length(),		 nMaxAll);
			}
		}
		nMaxAll = Max(nMaxShort + nMaxLong + nMaxName + 5, nMaxAll);

		// Print arguments
		Platform::instance().getConsole().print("Arguments:\n");
		for (uint32 i=0; i<m_lstOptions.size(); i++) {
			// Get option
			CommandLineOption *pOption = m_lstOptions[i];
			if (pOption->getType() == CommandLineOption::OptionArgument) {
				// Print option
				String sInfo = "  " + SetStringLength(pOption->getName(), nMaxAll) + "  ";
				Platform::instance().getConsole().print(sInfo);

				// Print description
				PrintDescription(sInfo.length(), pOption->getDescription());
			}
		}
		Platform::instance().getConsole().print('\n');

		// Print options
		Platform::instance().getConsole().print("Available options:\n");
		for (uint32 i=0; i<m_lstOptions.size(); i++) {
			// Get option
			CommandLineOption *pOption = m_lstOptions[i];
			if (pOption->getType() == CommandLineOption::OptionFlag || pOption->getType() == CommandLineOption::OptionParam) {
				// Print option
				String sName  = pOption->getName();
				bool bComma = (pOption->getShortName().length() > 0 && pOption->getLongName().length() > 0);
				String sShort = SetStringLength(pOption->getShortName(), nMaxShort);
				String sLong  = SetStringLength(pOption->getLongName (), nMaxLong );
				String sInfo  = sShort + (bComma ? ", " : "  ") + sLong;
				if (pOption->getType() == CommandLineOption::OptionParam)
					sInfo += " <" + sName + ">";
				sInfo = "  " + SetStringLength(sInfo, nMaxAll) + "  ";
				Platform::instance().getConsole().print(sInfo);

				// Print description
				PrintDescription(sInfo.length(), pOption->getDescription());
			}
		}
		Platform::instance().getConsole().print('\n');
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
