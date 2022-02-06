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
#include "RECore/Log/Log.h"
#include "RECore/File/DefaultFileManager.h"
#include "RECore/Platform/Platform.h"
#include "RECore/System/DynLib.h"
#include "RECore/Loader/Loader.h"
#include "RECore/Log/Log.h"
#include "RECore/Time/Stopwatch.h"
#include "RECore/Reflect/Module.h"
#include "RECore/Reflect/Rtti.h"
#include "RECore/Reflect/Class.h"
#include "RECore/Reflect/ClassImpl.h"
#include "RECore/Reflect/ClassDummy.h"
#include "RECore/Reflect/ClassManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
// Plugin API functions
typedef bool (*PLIsPluginDebugBuildFunc) ();
typedef int  (*PLGetPluginInfoFunc) ();


//[-------------------------------------------------------]
//[ Public static RECore::Singleton functions             ]
//[-------------------------------------------------------]
ClassManager &ClassManager::instance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	static ClassManager SInstance;
  return SInstance;
}



//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get module by name
*/
const Module *ClassManager::getModuleByName(const String &sModuleName) const
{
	// Have a look into our module list
	for (const Module* pModule: m_lstModules) {
		if (pModule->getName() == sModuleName)
			return pModule;
	}

	// Module not found
	return nullptr;
}

/**
*  @brief
*    Load a module
*/
const Module *ClassManager::loadModule(const String &sAbsFilename, bool bForceBuildTypeMatch)
{
	// Check if that library is already loaded
	for (const Module* pModule: m_lstModules) {
		if (pModule->getFilename() == sAbsFilename && (!pModule->isPlugin() || pModule->getDynLib()))
			return pModule;
	}

	// Start the stopwatch
	Stopwatch cStopwatch(true);

  /*

	// Is the library existent?
	Module *pModule = nullptr;
	File cFile(sAbsFilename);
	if (cFile.Exists() && cFile.IsFile()) {
		// Load library
		DynLib *pDynLib = new DynLib();
		if (pDynLib->Load(sAbsFilename)) {
			bool bUseLibrary = true;

			// Check build type match
			if (bForceBuildTypeMatch) {
				PLIsPluginDebugBuildFunc pIsPluginDebugBuild = reinterpret_cast<PLIsPluginDebugBuildFunc>(pDynLib->getSymbol("PLIsPluginDebugBuild"));
				if (pIsPluginDebugBuild) {
					if (RECORE_IS_DEBUGMODE != pIsPluginDebugBuild()) {
						// Error!
						bUseLibrary = false;
						RE_LOG(Error, "Module '" + sAbsFilename + "': release/debug mismatch")
					}
				} else {
					// Error!
					bUseLibrary = false;
					RE_LOG(Error, "Module '" + sAbsFilename + "': Forced build type match is active, but there's no 'PLIsPluginDebugBuild' function!")
				}
			}

			// Check if it is a valid PL plugin library
			if (bUseLibrary) {
				bUseLibrary = false;
				const String sLibraryFilename = Url(sAbsFilename).getFilename();
				PLGetPluginInfoFunc pGetPluginInfo = reinterpret_cast<PLGetPluginInfoFunc>(pDynLib->getSymbol("PLGetPluginInfo"));
				if (pGetPluginInfo) {
					// Get plugin info
					const int nModuleID = pGetPluginInfo();
					if (nModuleID > 0) {
						// Library successfully loaded
						RE_LOG(Info, "Module '" + sLibraryFilename + "' successfully loaded (required time: " + cStopwatch.getSeconds() + " sec)")
						pModule = createModule(nModuleID);
						if (pModule) {
							// Set plugin information
							pModule->m_bPlugin   = true;
							pModule->m_pDynLib   = pDynLib;	// The module now takes over the control of the dynamic library instance
							pModule->m_sFilename = sAbsFilename;
							bUseLibrary = true;
						} else {
							// Error!
							RE_LOG(Error, "Module '" + sLibraryFilename + "': failed to create the module")
						}
					} else {
						// Error!
						RE_LOG(Error, "Module '" + sLibraryFilename + "': release/debug mismatch")
					}
				} else {
					// Error!
					RE_LOG(Error, "Module '" + sLibraryFilename + "' is no valid RacoonEngine plugin library")
				}
			}

			// Unload library if it is not used
			if (!bUseLibrary)
				delete pDynLib;
		} else {
			// Error!
			RE_LOG(Error, "Can't load the module '" + sAbsFilename + '\'')
			delete pDynLib;
		}
	} else {
		// Error!
		RE_LOG(Error, "Can't find the module '" + sAbsFilename + '\'')
	}

	// Return the loaded module
	return pModule;
   */
  return nullptr;
}

/**
*  @brief
*    Scan a directory for compatible plugins and load them in
*/
bool ClassManager::scanPlugins(const String &sPath, ERecursive nRecursive, bool bDelayedPluginLoading)
{
  /*
	// Get URL from path
	Url cUrl(sPath);
	if (!cUrl.IsDirectory())
		cUrl = sPath + '/';
	const String sUrl = cUrl.GetUrl();

	// Open directory
	Directory cDir(sUrl);
	if (cDir.Exists() && cDir.IsDirectory()) {
		// List files
		FileSearch cSearch(sUrl);
		while (cSearch.HasNextFile()) {
			// Get filename
			const String sFilename = cSearch.GetNextFile();
			if (sFilename != "." && sFilename != "..") {
				// Get absolute filename
				const String sFile = sUrl + sFilename;

				// Check extension
				const String sExtension = Url(sFile).GetExtension();
				if (sExtension == "plugin") {
					// Try to load plugin
					loadPlugin(sFile, bDelayedPluginLoading);

				// Scan recursively?
				} else if (nRecursive == Recursive) {
					// Check if this is a directory
					File cFile(sFile);
					if (cFile.Exists() && cFile.IsDirectory()) {
						// Scan sub-directory
						scanPlugins(sFile, nRecursive, bDelayedPluginLoading);
					}
				}
			}
		}

		// Return success
		return true;
	}
   */

	// Error! Couldn't open directory
	return false;
}

/**
*  @brief
*    Load plugin
*/
bool ClassManager::loadPlugin(const String &sFilename, bool bDelayedPluginLoading)
{
  /*
	// Get URL
	const Url cUrl = Url(sFilename);

	// Check file extension
	const String sExtension = cUrl.GetExtension();
	if (sExtension == "plugin") {
		// Load XML document
		XmlDocument cDocument;
		if (cDocument.Load(sFilename)) {
			// Get plugin element
			const XmlElement *pPluginElement = cDocument.GetFirstChildElement("Plugin");
			if (pPluginElement) {
				// Get format version
				const int nVersion = pPluginElement->getAttribute("Version").getInt();

				// Unknown
				if (nVersion > 1) {
					RE_LOG(Error, cDocument.GetValue() + ": " + LoaderImpl::UnknownFormatVersion)

				// 1 (current) or ""/0 (same format as 1)
				} else if (nVersion == 1 || nVersion == 0) {
					// [DEPRECATED]
					if (nVersion == 0)
						RE_LOG(Warning, cDocument.GetValue() + ": " + LoaderImpl::DeprecatedFormatVersion)

					// Load in the plugin
					return loadPluginV1(cUrl, *pPluginElement, bDelayedPluginLoading);

				// No longer supported format version
				} else if (nVersion >= 0) {
					RE_LOG(Warning, cDocument.GetValue() + ": " + LoaderImpl::NoLongerSupportedFormatVersion)

				// Invalid format version (negative!)
				} else {
					RE_LOG(Error, cDocument.GetValue() + ": " + LoaderImpl::InvalidFormatVersion)
				}
			} else {
				RE_LOG(Error, cDocument.GetValue() + ": Can't find 'Plugin' element")
			}
		} else {
			RE_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
		}
	}
*/
	// Error!
	return true;
}

/**
*  @brief
*    Unload plugin
*/
bool ClassManager::unloadPlugin(Module *pModule)
{

	// Is the module valid and a plugin?
	if (pModule && std::find(m_lstModules.begin(), m_lstModules.end(), pModule) != m_lstModules.end() && pModule->isPlugin()) {
		// Remove module
		unregisterModule(pModule);

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Unload all plugins
*/
bool ClassManager::unloadAllPlugins()
{
	// Iterate through modules
	bool bError = false;
	for (const Module * pConstModule: m_lstModules) {
		// Is that module a plugin?
		Module *pModule = const_cast<Module*>(pConstModule);
		if (pModule->isPlugin()) {
			// Unload plugin
			if (!unloadPlugin(pModule))
				bError = true;

			// As the module has been removed from the list, adjust counter variable
		}
	}

	// Done
	return !bError;
}

/**
*  @brief
*    Get classes based on their base class and/or module (search for classes)
*/
void ClassManager::getClasses(std::list<const Class*> &lstClasses, const String &sClass, ERecursive nRecursive, EIncludeBase nIncludeBase, EIncludeAbstract nIncludeAbstract, uint32 nModuleID) const
{
	// Iterate through classes
	for (const Class* pClass: m_lstClasses) {

		// Check base class
		if ( (sClass.length() == 0) ||											// All classes wanted
			 (pClass->getClassName() == sClass && nIncludeBase == IncludeBase) ||	// Base class itself
			 (pClass->getBaseClassName() == sClass) ||								// Directly derived class
			 (nRecursive == Recursive && pClass->isDerivedFrom(sClass))				// Derived class (recursive)
		   ) {
			// Only add classes of a specific module?
			if (nModuleID == 0 || (pClass->getModule()->getModuleID() == nModuleID)) {
				// Only add classes with a constructor?
				if (nIncludeAbstract == IncludeAbstract || pClass->hasConstructor()) {
					// Add class to list
					lstClasses.push_back(pClass);
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ClassManager::ClassManager() :
	m_nModuleID(10000)	// Any value to start with
{
}

/**
*  @brief
*    Copy constructor
*/
ClassManager::ClassManager(const ClassManager &cSource) :
	m_nModuleID(10000)	// Any value to start with
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Destructor
*/
ClassManager::~ClassManager()
{
	// Iterate through all modules and destroy them
	for (const Module* pModule: m_lstModules) {

		// At this point, all real class instances may already have been destroyed automatically by the RTTI system (see "re_class" -> "__re_guard")
		// ... so, one might think that only the dummy classes should be left in here... but due to the random static de-initialization order we can't be sure...

		{ // Remove all classes from that module
			// Get list of classes (make a copy!)
			std::list<const Class*> lstClasses = pModule->getClasses();

			// Remove all classes from that module
			for (const Class *cClassIterator: lstClasses) {
				// Get the class implementation
				ClassImpl *pClassImpl = const_cast<Class*>(cClassIterator)->m_pClassImpl;

				// Unregister the class
				unregisterClass(pModule->getModuleID(), pClassImpl);
			}
		}

		// Module has been unloaded (emit event)
		EventModuleUnloaded(pModule);

		// Destroy the module instance
		delete pModule;
	}
}

/**
*  @brief
*    Copy operator
*/
ClassManager &ClassManager::operator =(const ClassManager &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Get a unique module ID
*/
uint32 ClassManager::getUniqueModuleID()
{
	// Return next module ID
	return m_nModuleID++;
}

/**
*  @brief
*    Create module
*/
Module *ClassManager::createModule(uint32 nModuleID)
{
	// Check if module is already there
	Module *pModule = nullptr;
	if (m_mapModules.find(nModuleID) == m_mapModules.end()) {
		// No, create new module
		pModule = new Module(nModuleID);

		// Add module to lists
		m_lstModules.push_back(pModule);
		m_mapModules.emplace(nModuleID, pModule);
	} else {
    pModule = const_cast<Module*>(m_mapModules.at(nModuleID));
  }

	// Return module
	return pModule;
}

/**
*  @brief
*    Register module
*/
void ClassManager::registerModule(const uint32 *pnModuleID, const String &sName, const String &sVendor, const String &sLicense, const String &sDescription)
{
	// Get module
	Module *pModule = createModule(*pnModuleID);	// "pnModuleID" is guaranteed to be valid
	if (pModule) {
		// Set module info
		pModule->setModuleInfo(sName, sVendor, sLicense, sDescription);

		// Set module filename, or at least try it
		/// TODO(naetherm): pModule->m_sFilename = Platform::instance().getModuleFilenameByMemoryAddress(pnModuleID);

		// Module has been registered (emit event)
		EventModuleLoaded(pModule);
	}
}

/**
*  @brief
*    Unregister module
*/
void ClassManager::unregisterModule(Module *pModule)
{
	// Check if module is valid
	if (pModule) {
		if (std::find(m_lstModules.begin(), m_lstModules.end(), pModule) != m_lstModules.end()) {
			// Get list of classes (make a copy!)
			std::list<const Class*> lstClasses = pModule->getClasses();

			// Remove all classes from that module
			for (const Class* pConstClass: lstClasses)
				unregisterClass(pModule->getModuleID(), const_cast<Class*>(pConstClass)->m_pClassImpl);
		}

		// Module has been unloaded (emit event)
		EventModuleUnloaded(pModule);

		// Remove module
		m_lstModules.remove(pModule);
		m_mapModules.erase(pModule->getModuleID());
		delete pModule;
	}
}

/**
*  @brief
*    Register class
*/
void ClassManager::registerClass(uint32 nModuleID, ClassImpl *pClassImpl)
{
	// Check for real/dummy class or duplicate class name
	Module *pModule = nullptr;
	const Class *pOldClass = nullptr;
	if (m_mapClasses.find(pClassImpl->getClassName()) != m_mapClasses.end()) {
    pOldClass = m_mapClasses.at(pClassImpl->getClassName());
		// Is the already registered implementation of the class currently a dummy?
		if (pOldClass->m_pClassImpl->isDummy()) {
			// Is the new given implementation also a dummy?
			if (pClassImpl->isDummy()) {
				// We only need one dummy, destroy the given one because we're now responsible for the given object
				delete pClassImpl;
			} else {
				// The current class implementation is a worthless dummy, replace it through the real thing right now!

				// The dummy and the real class are in different modules (this is just a security check)
				if (pOldClass->m_pClassImpl->m_nModuleID != nModuleID) {
					// Add the real class to the real module...
					createModule(nModuleID)->addClass(pOldClass);

					// ... and remove the dummy class from the dummy module because a class can only be within
					// one module at the same time! Please note that in the end, there may be still dummy
					// classes left in the dummy module - e.g. if there are classes for other target platforms
					// within the plugin file (those aren't linked into the binary but PLProject still lists them).
					createModule(pOldClass->m_pClassImpl->m_nModuleID)->removeClass(pOldClass);
				}

				// Destroy the dummy class implementation
				delete pOldClass->m_pClassImpl;

				// Set the real class implementation
				pOldClass->m_pClassImpl = pClassImpl;

				// Tell the class implementation about the class instance wrapping it
				pClassImpl->m_pClass = const_cast<Class*>(pOldClass);
			}

			// We're done, get us out of this method right now!
			return;
		} else {
			// It's no dummy, is the new class to register a dummy?
			if (pClassImpl->isDummy()) {
				// Ok, the new class to register is a worthless dummy because we already have the real thing, so just ignore it!

				// Destroy the given dummy because we're now responsible for the given object
				delete pClassImpl;

				// We're done, get us out of this method right now!
				return;
			} else {
				// Ok, the old and new classes are not dummies, this means that there's a class name conflict!

				// Get module
				pModule = createModule(nModuleID);

				// The class has a name that is already used by another class
				const String sClass		= pClassImpl->getClassName();
				const String sModule	= pModule->getName();
				const String sOldModule	= pOldClass->getModule()->getName();

				// Add warning to log that the class will not be available through e.g. getClass()
				/// TODO(naetherm): RE_LOG(Warning, "Class '" + sClass + "' [module '" + sModule + "']: Name conflict with already registered class '" + sClass + "' [module '" + sOldModule + "']")
			}
		}
	} else {
		// Get module
		pModule = createModule(nModuleID);
	}

	// Create an class instance wrapping the class implementation?
	Class *pClass = new Class(*pClassImpl);

	// Add class to list
	m_lstClasses.push_back(pClass);
	if (!pOldClass)
		m_mapClasses.emplace(pClass->getClassName(), pClass);

	// Add class to module
	pModule->addClass(pClass);

	// Class has been registered (emit event)
	EventClassLoaded(pClass);
}

/**
*  @brief
*    Unregister class
*/
void ClassManager::unregisterClass(uint32 nModuleID, ClassImpl *pClassImpl)
{
	// Get the class instance wrapping the class implementation
	Class *pClass = pClassImpl->getClass();
	if (pClass) {
		// Get module
		Module *pModule = createModule(nModuleID);

		// Remove class from list
		m_lstClasses.remove(pClass);
		m_mapClasses.erase(pClass->getClassName());

		// Remove class from module
		pModule->removeClass(pClass);

		// Class has been unloaded (emit event)
		EventClassUnloaded(pClass);

		// If it's a dummy, destroy the class implementation
		if (pClassImpl->isDummy()) {
			delete pClassImpl;
		} else {
			// The class implementation is no longer wrapped by a class
			pClassImpl->m_pClass = nullptr;

			// -> The real class instance is destroyed automatically by the RTTI system (see "re_class" -> "__re_guard")
		}

		// Destroy the class instance wrapping the class implementation
		delete pClass;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
