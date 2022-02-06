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
#include "Application.h"
#include <RECore/String/String.h>
#include <RECore/Platform/Platform.h>
#include <RECore/Platform/Console.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Application::Application()
: RECore::CoreApplication() {
  m_cCommandLine.addParameter	("Prefix",    "-p", "--prefix",		"Text prefix",						"",	false);
	m_cCommandLine.addFlag		("Uppercase", "-u", "--uppercase",	"Convert the text to upper case",		false);
	m_cCommandLine.addArgument	("Text",							"Text string",						"",	true);
	m_cCommandLine.addArgument	("Postfix",							"Text postfix",						"",	false);
}

Application::~Application() {

}

void Application::main() {
  // Get text, prefix and postfix
	RECore::String sText    = m_cCommandLine.getValue("Text");
  RECore::String sPrefix  = m_cCommandLine.getValue("Prefix");
  RECore::String sPostfix = m_cCommandLine.getValue("Postfix");
	if (sPrefix .length() > 0)
		sText = sPrefix + ' ' + sText;
	if (sPostfix.length() > 0)
		sText = sText   + ' ' + sPostfix;

	// Get upper case option
	if (m_cCommandLine.isValueSet("-u"))
		sText.ToUpper();

	// Print text
  RECore::Platform::instance().getConsole().print("Text: '" + sText + "'\n");

	// Print additional arguments
	if (m_cCommandLine.getNumOfAdditionalArguments()) {
    RECore::Platform::instance().getConsole().print("\nAddition arguments:\n");
		for (RECore::uint32 i=0; i<m_cCommandLine.getNumOfAdditionalArguments(); i++)
      RECore::Platform::instance().getConsole().print("- '" + m_cCommandLine.getAdditionalArgument(i) + "'\n");
	}
}
