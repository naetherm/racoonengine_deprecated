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
#if defined(WIN32)

#elif defined(LINUX)
#include <unistd.h>
#endif
#include "RECore/System/Pipe.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
bool Pipe::create()
{
  // Unnamed pipe
  mName = "";

  // Windows implementation
#ifdef WIN32
  // Set security attributes
		SECURITY_ATTRIBUTES sSecAttr;
		sSecAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		sSecAttr.bInheritHandle = TRUE;
		sSecAttr.lpSecurityDescriptor = nullptr;

		// Create pipe
		return (CreatePipe((HANDLE*)&mPipe[0], (HANDLE*)&mPipe[1], &sSecAttr, 0) != 0);
#endif

  // Linux implementation
#ifdef LINUX
  // Create a pipe
  return (pipe(reinterpret_cast<int*>(mPipe)) == 0);
#endif
}

bool Pipe::create(const String &name)
{
  // Save name
  mName = name;

  // Windows implementation
#ifdef WIN32
  // Set security attributes
		SECURITY_ATTRIBUTES sSecAttr;
		sSecAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		sSecAttr.bInheritHandle = TRUE;
		sSecAttr.lpSecurityDescriptor = nullptr;

		// Create write end of the pipe
		if (name.GetFormat() == String::ASCII)
			mPipe[1] = (handle)CreateNamedPipeA(name.GetASCII(), PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, &sSecAttr);
		else
			mPipe[1] = (handle)CreateNamedPipeW(name.GetUnicode(), PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, &sSecAttr);

		// Create read end of the pipe
		if (name.GetFormat() == String::ASCII)
			mPipe[0] = (handle)CreateFileA(name.GetASCII(), GENERIC_READ, 0, &sSecAttr, OPEN_EXISTING, 0, nullptr);
		else
			mPipe[0] = (handle)CreateFileW(name.GetUnicode(), GENERIC_READ, 0, &sSecAttr, OPEN_EXISTING, 0, nullptr);

		// Return result
		return ((HANDLE)mPipe[0] != INVALID_HANDLE_VALUE && (HANDLE)mPipe[1] != INVALID_HANDLE_VALUE);
#endif

  // Linux implementation
#ifdef LINUX
  // [TODO]
  if (::pipe(mPipe) != -1) {

  }
  return false;
#endif
}

bool Pipe::closeRead()
{
  // Check if handle has already been closed
  if (mPipe[0] == INVALID_HANDLE) {
    // Error!
    return false;
  }

  // Windows implementation
#ifdef WIN32
  const BOOL bResult = CloseHandle((HANDLE)mPipe[0]);
		mPipe[0] = INVALID_HANDLE;
		return (bResult != FALSE);
#endif

  // Linux implementation
#ifdef LINUX
  ::close(mPipe[0]);
  mPipe[0] = INVALID_HANDLE;
  return true;
#endif
}

bool Pipe::closeWrite()
{
  // Check if handle has already been closed
  if (mPipe[1] == INVALID_HANDLE) {
    // Error!
    return false;
  }

  // Windows implementation
#ifdef WIN32
  const BOOL bResult = CloseHandle((HANDLE)mPipe[1]);
		mPipe[1] = INVALID_HANDLE;
		return (bResult != FALSE);
#endif

  // Linux implementation
#ifdef LINUX
  ::close(mPipe[1]);
  mPipe[1] = INVALID_HANDLE;
  return true;
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
