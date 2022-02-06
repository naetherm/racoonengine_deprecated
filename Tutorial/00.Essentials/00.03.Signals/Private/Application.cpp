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
#include <RECore/Platform/Platform.h>
#include <RECore/Platform/Console.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
Application::Application()
: CoreApplication()
, mExit(false) {

}

Application::~Application() {

}

void Application::main() {
  int i = 1;
  while (!mExit || i > 1) {
    RECore::Platform::instance().sleep(500);
    RECore::Platform::instance().getConsole().print(RECore::String("Counting ") + i + '\n');
    i++;
    if (i > 10)
      i = 1;
  }
}

bool Application::onSignal(ESignal nSignal) {
  // Termination
  if (nSignal == SignalTerm) {
    // Start counter
    RECore::Platform::instance().getConsole().print("<Exit>: Caught TERM signal, will continue counting to 10 and then exit.\n");
    mExit = true;

    // Cancel signal
    return false;
  }

  // Go on with default signal handler
  return true;
}
