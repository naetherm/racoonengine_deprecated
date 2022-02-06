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
#include "RECore/Reflect/Rtti.h"


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Defines a module inside an application or library
*
*  @param[in] ModuleName
*    Module name
*
*  @remarks
*    Use this in applications or libraries that are linked
*    directly to the main project (statically or dynamically),
*    but not inside a dynamically loaded plugin library.
*/
#define re_module(ModuleName) \
	class ModuleInfo { \
		public: \
			ModuleInfo() { \
				ModuleInfo::registerThis(); \
			} \
		private: \
			static void registerThis() \
			{ \
				RECore::String sName        = ModuleName; \
				RECore::String sVendor      = "Unknown"; \
				RECore::String sLicense     = "Unknown"; \
				RECore::String sDescription = "Unknown module"; \

/**
*  @brief
*    Defines a module inside a plugin library
*
*  @param[in] ModuleName
*    Module name
*
*  @remarks
*    Use this in a library that is dynamically loaded as a plugin
*/
#define re_module_plugin(ModuleName) \
	extern "C" RECORE_PLUGIN_API bool PLIsPluginDebugBuild() \
	{ \
		return RECORE_IS_DEBUGMODE; \
	} \
	\
	extern "C" RECORE_PLUGIN_API int PLGetPluginInfo() \
	{ \
		return RECore::ModuleID<int>::getModuleID(); \
	} \
	\
	re_module(ModuleName) \

/**
*  @brief
*    Sets the module vendor name
*
*  @param[in] Vendor
*    Vendor name
*/
#define re_module_vendor(Vendor) \
			sVendor = Vendor; \

/**
*  @brief
*    Sets the module license
*
*  @param[in] License
*    License
*/
#define re_module_license(License) \
			sLicense = License; \

/**
*  @brief
*    Sets the module description
*
*  @param[in] Description
*    Module description
*/
#define re_module_description(Description) \
			sDescription = Description; \

/**
*  @brief
*    Sets the module version
*
*  @param[in] Version
*    Version string
*/
#define re_module_version(Version)

/**
*  @brief
*    Sets the module dependencies for Windows 32/64 bit release/debug mode
*
*  @param[in] Dependencies
*    Module dependencies
*/
#define re_module_dependencies_windows_64_release(Dependencies)
#define re_module_dependencies_windows_32_release(Dependencies)
#define re_module_dependencies_windows_64_debug(Dependencies)
#define re_module_dependencies_windows_32_debug(Dependencies)

/**
*  @brief
*    Sets the module dependencies for Linux 32/64 bit release/debug mode
*
*  @param[in] Dependencies
*    Module dependencies
*/
#define re_module_dependencies_linux_64_release(Dependencies)
#define re_module_dependencies_linux_32_release(Dependencies)
#define re_module_dependencies_linux_64_debug(Dependencies)
#define re_module_dependencies_linux_32_debug(Dependencies)

/**
*  @brief
*    Sets the module dependencies for Android 32/64 bit release/debug mode
*
*  @param[in] Dependencies
*    Module dependencies
*/
#define re_module_dependencies_android_64_release(Dependencies)
#define re_module_dependencies_android_32_release(Dependencies)
#define re_module_dependencies_android_64_debug(Dependencies)
#define re_module_dependencies_android_32_debug(Dependencies)

/**
*  @brief
*    Sets the module dependencies for Mac OS X 32/64 bit release/debug mode
*
*  @param[in] Dependencies
*    Module dependencies
*/
#define re_module_dependencies_macosx_64_release(Dependencies)
#define re_module_dependencies_macosx_32_release(Dependencies)
#define re_module_dependencies_macosx_64_debug(Dependencies)
#define re_module_dependencies_macosx_32_debug(Dependencies)

/**
*  @brief
*    Sets the module's default active state
*
*  @param[in] Active
*    Default active state (1 or 0)
*
*  @note
*    - If not other defined, modules are active by default
*/
#define re_module_active(Active)

/**
*  @brief
*    Sets the module's default delayed shared library loading state
*
*  @param[in] Delayed
*    Default delayed shared library loading state (1 or 0)
*
*  @note
*    - If not other defined, modules are loading their shared libraries delayed by default
*/
#define re_module_delayed(Delayed)

/**
*  @brief
*    Ends the module definition
*/
#define re_module_end \
			RECore::ModuleID<int>::registerModule(sName, sVendor, sLicense, sDescription); \
		} \
	}; \
	\
	ModuleInfo __ModuleInfo;
