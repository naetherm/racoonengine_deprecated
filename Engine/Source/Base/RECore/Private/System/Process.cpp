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
#include "RECore/System/Process.h"
#include "RECore/String/BasicTokenizer.h"
#include <vector>
#if defined(LINUX)
#include <unistd.h>
#include <signal.h>
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Process::Process()
: mProcess(NULL_HANDLE) {

}

Process::~Process() {
#if defined(WIN32)
  if (mProcess) {
    CloseHandle(reinterpret_cast<HANDLE>(mProcess));
  }
#endif
}

void Process::execute(const String &command, const String &arguments) {
#if defined(WIN32)
  // Create process
		PROCESS_INFORMATION piProcInfo;
		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
		STARTUPINFO siStartInfo;
		ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
		siStartInfo.cb = sizeof(STARTUPINFO);
		String cmdLine = command + ' ' + arguments;
		BOOL bResult = CreateProcess(
							nullptr,
							const_cast<LPWSTR>(cmdLine.cstr()),	// Command line
							nullptr,									// Process security attributes
							nullptr,									// Primary thread security attributes
							TRUE,										// Handles are inherited
							0,											// Creation flags
							nullptr,									// Use parent's environment
							nullptr,									// Use parent's current directory
							&siStartInfo,								// STARTUPINFO pointer
							&piProcInfo									// Receives PROCESS_INFORMATION
						);

		// Check result
		if (bResult) {
			mProcess = reinterpret_cast<handle>(piProcInfo.hProcess);
			CloseHandle(piProcInfo.hThread);
		}
#elif defined(LINUX)
  // Create process
  mProcess = fork();
  if (mProcess == 0) {	// Child process
    // Get arguments
    std::vector<String> lstArgs;
    lstArgs.push_back(command);
    CTokenizer tokenizer;
    tokenizer.SetSingleChars("");
    tokenizer.SetSingleLineComment("");
    tokenizer.Start(arguments);
    String sToken = tokenizer.GetNextToken();
    while (sToken.length()) {
      lstArgs.push_back(sToken);
      sToken = tokenizer.GetNextToken();
    }

    // Make array for arguments
    const int nSize = lstArgs.size();
    if (nSize > 0) {
      char **ppszParams = new char*[nSize+1];
      for (int i=0; i<nSize; i++)
        ppszParams[i] = const_cast<char*>(lstArgs[i].cstr());
      ppszParams[nSize] = nullptr;

      // Execute application
      execv(lstArgs[0], ppszParams);
    }
  }
#endif
}

bool Process::isRunning() const {
  // Windows implementation
#if defined(WIN32)
  // Check process handle
		return (mProcess != NULL_HANDLE);
#endif

  // Linux implementation
#if defined(LINUX)
  return (mProcess != 0);
#endif
}

void Process::terminate() {
  // Windows implementation
#if defined(WIN32)
  TerminateProcess(reinterpret_cast<HANDLE>(mProcess), 0);
		CloseHandle(reinterpret_cast<HANDLE>(mProcess));
		mProcess = NULL_HANDLE;
#endif

  // Linux implementation
#if defined(LINUX)
  kill(mProcess, SIGKILL);
  mProcess = NULL_HANDLE;
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
