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
#include <RECore/REMain.h>
#include "Application.h"


//[-------------------------------------------------------]
//[ Program entry point                                   ]
//[-------------------------------------------------------]
// This is the real entry point for any RacoonEngine executable. You must define this function if you
// include <PLCore/Main.h> in your project. The parameters passed by the engine into this function
// are self-explanatory.
int REMain(const RECore::String &sExecutableFilename, const std::vector<RECore::String> &lstArguments)
{
  // We just create an instance of our very basic application class and run it. This will result
  // in the engine being initialized into usable state and the above Application::Main method
  // being called. Note that only the very essential engine systems are intialized, such as log
  // and plugin system. We will explore the initialization of more advanced systems (such as renderer)
  // in future excercises.
  Application cApplication;
  return cApplication.run(sExecutableFilename, lstArguments);
}