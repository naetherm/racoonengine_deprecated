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
#include "RECore/Xml/Xml.h"
#include "RECore/Reflect/Tools/ParamsParserXml.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Parse parameters from XML
*/
bool ParamsParserXml::parseXml(const XmlElement &cElement)
{
	// Save pointer to XML element
	m_pElement = &cElement;
	if (m_pElement) {
		// Get first attribute
		m_pAttribute = m_pElement->GetFirstAttribute();
		if (m_pAttribute) {
			m_sName  = m_pAttribute->getName();
			m_sValue = m_pAttribute->getValue();
			return true;
		}
	}

	// Parameters are empty
	return false;
}

/**
*  @brief
*    Parse next parameter
*/
bool ParamsParserXml::next()
{
	// Is attribute valid?
	if (m_pAttribute) {
		// Get the next attribute
		m_pAttribute = m_pAttribute->GetNext();
		if (m_pAttribute) {
			// Get name and value
			m_sName  = m_pAttribute->getName();
			m_sValue = m_pAttribute->getValue();
			return true;
		}
	}

	// Error, could not parse next expression
	m_pAttribute = nullptr;
	m_sName		 = "";
	m_sValue	 = "";
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
