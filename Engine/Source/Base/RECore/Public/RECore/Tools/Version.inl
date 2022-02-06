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
*    Destructor
*/
inline Version::~Version()
{
}

/**
*  @brief
*    Get project name
*/
inline String Version::getName() const
{
	return m_sName;
}

/**
*  @brief
*    Set project name
*/
inline void Version::setName(const String &sName)
{
	m_sName = sName;
}

/**
*  @brief
*    Get codename
*/
inline String Version::getCodename() const
{
	return m_sCodename;
}

/**
*  @brief
*    Set codename
*/
inline void Version::setCodename(const String &sCodename)
{
	m_sCodename = sCodename;
}

/**
*  @brief
*    Get major version number
*/
inline uint16 Version::getMajor() const
{
	return m_nMajor;
}

/**
*  @brief
*    Set major version number
*/
inline void Version::setMajor(uint16 nMajor)
{
	m_nMajor = nMajor;
}

/**
*  @brief
*    Get minor version number
*/
inline uint16 Version::getMinor() const
{
	return m_nMinor;
}

/**
*  @brief
*    Set minor version number
*/
inline void Version::setMinor(uint16 nMinor)
{
	m_nMinor = nMinor;
}

/**
*  @brief
*    Get patch number
*/
inline uint16 Version::getPatch() const
{
	return m_nPatch;
}

/**
*  @brief
*    Set patch number
*/
inline void Version::setPatch(uint16 nPatch)
{
	m_nPatch = nPatch;
}

/**
*  @brief
*    Get release name
*/
inline String Version::getRelease() const
{
	return m_sName;
}

/**
*  @brief
*    Set release name
*/
inline void Version::setRelease(const String &sRelease)
{
	m_sRelease = sRelease;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
