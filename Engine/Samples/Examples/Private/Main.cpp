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
#include <RECore/Tools/CommandLineArguments.h>
#include "Examples/ExampleRunner.h"


//[-------------------------------------------------------]
//[ Platform independent program entry point              ]
//[-------------------------------------------------------]
int programEntryPoint(const RECore::CommandLineArguments& commandLineArguments)
{
	return ExampleRunner().run(commandLineArguments);
}


//[-------------------------------------------------------]
//[ Platform dependent program entry point                ]
//[-------------------------------------------------------]
// Windows implementation
#ifdef _WIN32
	#include <RERenderer/Core/Platform/WindowsHeader.h>

	// Global variables
	extern "C"
	{
		// NVIDIA: Force usage of NVidia GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
		// -> See "Enabling High Performance Graphics Rendering on Optimus Systems" http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
		_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

		// AMD: Force usage of AMD GPU in case there is an integrated graphics unit as well, if we don't do this we risk getting the integrated graphics unit and hence a horrible performance
		// -> Named "Dynamic Switchable Graphics", found no official documentation, only https://community.amd.com/message/1307599#comment-1307599 - "Can an OpenGL app default to the discrete GPU on an Enduro system?"
		__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	}

	// Global functions
	#ifdef _CONSOLE
		#ifdef UNICODE
			int wmain(int, wchar_t**)
		#else
			int main(int, char**)
		#endif
			{
				// For memory leak detection
				#ifdef DEBUG
					// "_CrtDumpMemoryLeaks()" reports false positive memory leak with static variables, so use a memory difference instead
					_CrtMemState crtMemState = { };
					_CrtMemCheckpoint(&crtMemState);
				#endif

				// Call the platform independent program entry point
				// -> Uses internally "GetCommandLine()" to fetch the command line arguments
				const int result = programEntryPoint(CommandLineArguments());

				// For memory leak detection
				#ifdef DEBUG
					_CrtMemDumpAllObjectsSince(&crtMemState);
				#endif

				// Done
				return result;
			}
	#else
		#ifdef UNICODE
			int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
		#else
			int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
		#endif
			{
				// For memory leak detection
				#ifdef DEBUG
					// "_CrtDumpMemoryLeaks()" reports false positive memory leak with static variables, so use a memory difference instead
					_CrtMemState crtMemState = { };
					_CrtMemCheckpoint(&crtMemState);
				#endif

				// Call the platform independent program entry point
				// -> Uses internally "GetCommandLine()" to fetch the command line arguments
				const int result = programEntryPoint(CommandLineArguments());

				// For memory leak detection
				#ifdef DEBUG
					_CrtMemDumpAllObjectsSince(&crtMemState);
				#endif

				// Done
				return result;
			}
	#endif

// Linux implementation
#elif LINUX
	#include "Examples/Framework/PlatformTypes.h"
	int main(int argc, char** argv)
	{
		// Call the platform independent program entry point
		return programEntryPoint(RECore::CommandLineArguments(argc, argv));
	}
#endif
