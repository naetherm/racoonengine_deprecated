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
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
inline CommandLine::CommandLine() :
	m_bError(false)
{
}

/**
*  @brief
*    Destructor
*/
inline CommandLine::~CommandLine()
{
	// Clear options
	clear();
}

/**
*  @brief
*    Get number of registered options
*/
inline uint32 CommandLine::getNumOfOptions() const
{
	// Return number of options stored in our array
	return m_lstOptions.size();
}

/**
*  @brief
*    Get option by index
*/
inline CommandLineOption *CommandLine::getOption(uint32 nIndex) const
{
	// Return option
	return m_lstOptions[nIndex];
}

/**
*  @brief
*    Get option by name
*/
inline CommandLineOption *CommandLine::getOption(const String &sName) const
{
	// Return option
  auto iter = this->m_mapOptions.find(sName);
  if (iter != this->m_mapOptions.end()) {
    return iter->second;
  }
	return nullptr;
}

/**
*  @brief
*    Check if there were any errors parsing the command line arguments
*/
inline bool CommandLine::hasErrors() const
{
	// Return error flag
	return m_bError;
}

/**
*  @brief
*    Get number of additional arguments that have been defined
*/
inline uint32 CommandLine::getNumOfAdditionalArguments() const
{
	// Return number of additional arguments
	return m_lstParameters.size();
}

/**
*  @brief
*    Get additional argument
*/
inline String CommandLine::getAdditionalArgument(uint32 nIndex) const
{
	auto iter = this->m_lstParameters.begin();
  std::advance(iter, nIndex);
  return *iter;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
