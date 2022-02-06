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
#include "RECore/REMain.h"
#include "RECore/Runtime.h"
#include "RECore/ModuleMain.h"
#include "RECore/Frontend/Frontend.h"
#include "RECore/Frontend/FrontendContext.h"


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Defines a default frontend for a module inside a plugin library
*
*  @param[in] ModuleName
*    Module name
*  @param[in] ApplicationClass
*    Name of the application RTTI class to use (must be derived from "RECore::FrontendApplication")
*
*  @remarks
*    Use this when you don't want to care about the program entry point.
*/
#define re_module_application(ModuleName, ApplicationClass) \
	int REMain(const RECore::String &sExecutableFilename, const std::vector<RECore::String> &lstArguments) \
	{ \
		/* Scan PL-runtime directory for compatible plugins and load them in as well as scan for compatible data and register it */ \
		/* if (RECore::Runtime::ScanDirectoryPluginsAndData()) */ { \
			/* Setup the frontend context */ \
			RECore::FrontendContext cFrontendContext; \
			cFrontendContext.setExecutableFilename(sExecutableFilename); \
			cFrontendContext.setArguments(lstArguments); \
			cFrontendContext.setName(ModuleName); \
			cFrontendContext.setFrontendParameters(RECore::String("ApplicationClass=\"") + ApplicationClass + '\"'); \
	\
			/* Run the frontend */ \
			return RECore::Frontend::run(cFrontendContext); \
		} \
	} \
	\
	re_module_plugin(ModuleName)

/**
*  @brief
*    Defines a frontend for a module inside a plugin library
*
*  @param[in] ModuleName
*    Module name
*  @param[in] ApplicationClass
*    Name of the application RTTI class to use (must be derived from "RECore::FrontendApplication")
*  @param[in] FrontendClass
*    Name of the frontend implementation RTTI class to use (e.g. "RECore::FrontendOS")
*
*  @remarks
*    Use this when you don't want to care about the program entry point.
*/
#define re_module_application_frontend(ModuleName, ApplicationClass, FrontendClass) \
	int REMain(const RECore::String &sExecutableFilename, const std::vector<RECore::String> &lstArguments) \
	{ \
		/* Scan PL-runtime directory for compatible plugins and load them in as well as scan for compatible data and register it */ \
		/*if (RECore::Runtime::ScanDirectoryPluginsAndData()) */ { \
			/* Setup the frontend context */ \
			RECore::FrontendContext cFrontendContext; \
			cFrontendContext.setExecutableFilename(sExecutableFilename); \
			cFrontendContext.setArguments(lstArguments); \
			cFrontendContext.setName(ModuleName); \
			cFrontendContext.setFrontendParameters(RECore::String("ApplicationClass=\"") + ApplicationClass + '\"'); \
			cFrontendContext.setFrontendImplementation(FrontendClass); \
	\
			/* Run the frontend */ \
			return RECore::Frontend::run(cFrontendContext); \
		} else { \
			/* Error! */ \
			return -1; \
		} \
	} \
	\
	re_module_plugin(ModuleName)
