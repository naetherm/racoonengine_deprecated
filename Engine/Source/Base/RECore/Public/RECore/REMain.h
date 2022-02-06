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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/String/String.h"
#include "RECore/Platform/Platform.h"
#if defined(WIN32)
#endif

#if defined(LINUX)
#include <locale.h>
#include "RECore/Linux/Linux.h"
#endif
#include <vector>


//[-------------------------------------------------------]
//[ RE main function                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Program entry point
*
*  @param[in] sExecutableFilename
*    Absolute executable filename
*  @param[in] lstArguments
*    List of program arguments
*
*  @return
*    Exit code of the application (usually 0 means no error)
*/
int REMain(const RECore::String& executableFilename, const std::vector<RECore::String>& arguments);


//[-------------------------------------------------------]
//[ Program entry point                                   ]
//[-------------------------------------------------------]
// Do only include the program entry point if the current build target is no shared library
// (On e.g. Linux those definitions are set by our build system -> we just use the MS Windows
//  names so that we don't have to invent a new definition for this purpose)
#if !defined(_WINDLL) && !defined(_USRDLL)
// Windows implementation
#ifdef WIN32
  #ifdef _CONSOLE
			#ifdef UNICODE
				int wmain(int argc, wchar_t **argv)
			#else
				int main(int argc, char **argv)
			#endif
				{
					std::vector<RECore::String> lstArguments;
					for (int i=1; i<argc; i++)
						lstArguments.push_back(argv[i]);
					return REMain(RECore::Platform::instance().getExecutableFilename(), lstArguments);
				}
		#else
			#ifdef UNICODE
				int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR pszCmdLine, int nShow)
			#else
				int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pszCmdLine, int nShow)
			#endif
				{
					std::vector<RECore::String> lstArguments;
					for (int i=1; i<argc; i++)
						lstArguments.push_back(argv[i]);
					return REMain(RECore::Platform::instance().getExecutableFilename(), lstArguments);
				}
		#endif

	// Linux implementation
#elif LINUX
int main(int argc, char **argv) {
  std::vector<RECore::String> lstArguments;
  for (int i=1; i<argc; i++)
    lstArguments.push_back(RECore::String(argv[i]));
  return REMain(RECore::Platform::instance().getExecutableFilename(), lstArguments);
}
#endif
#endif