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
#include "RECore/Reflect/ReflectTypes.h"
#include "RECore/Reflect/Event/Event.h"
#include <list>
#include <map>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Url;
class Class;
class Module;
class ClassImpl;
template <typename T> class ModuleID;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Manager providing access to RTTI classes
*/
class ClassManager {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ModuleID<int>;
	friend class ClassReal;
	friend class ClassDummy;


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		Event<const Module*> EventModuleLoaded;		/**< A module has been registered (do not destroy the given instance) */
		Event<const Module*> EventModuleUnloaded;	/**< A module has been removed (do not destroy the given instance) */
		Event<const Class*>	 EventClassLoaded;		/**< A class has been registered. Please note: At the time you receive this event, the class may not yet be fully initialized, this means that even "isDerivedFrom" may not work, yet. (do not destroy the given instance) */
		Event<const Class*>  EventClassUnloaded;	/**< A class has been removed (do not destroy the given instance) */


	//[-------------------------------------------------------]
	//[ Public static RECore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See RECore::Singleton for more details about singletons
	public:
		static RECORE_API ClassManager &instance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get all registered modules
		*
		*  @return
		*    std::list of modules (do not destroy the returned instances)
		*/
		inline const std::list<const Module*> &getModules() const;

		/**
		*  @brief
		*    Get module by ID
		*
		*  @param[in] nModuleID
		*    Module ID
		*
		*  @return
		*    Pointer to module (do not destroy the returned instance, can be a null pointer)
		*/
		inline const Module *getModuleByID(uint32 nModuleID) const;

		/**
		*  @brief
		*    Get module by name
		*
		*  @param[in] sModuleName
		*    Module name
		*
		*  @return
		*    Pointer to module (do not destroy the returned instance, can be a null pointer)
		*
		*  @note
		*    - Whenever possible use "getModuleByID()" instead of "getModuleByName()" because
		*      unlike the module name, the module ID is guaranteed do be unique
		*/
		RECORE_API const Module *getModuleByName(const String &sModuleName) const;

		/**
		*  @brief
		*    Load a module
		*
		*  @param[in] sAbsFilename
		*    Absolute filename of the shared library to load in
		*  @param[in] bForceBuildTypeMatch
		*    'true' to force a build type match, else 'false'
		*
		*  @return
		*    The loaded module, null pointer on error (do not destroy the returned instance)
		*
		*  @note
		*    - If the module was already loaded, this module instance is returned instead
		*/
		RECORE_API const Module *loadModule(const String &sAbsFilename, bool bForceBuildTypeMatch = false);

		/**
		*  @brief
		*    Scan a directory for compatible plugins and load them in
		*
		*  @param[in] sPath
		*    Directory to search in
		*  @param[in] nRecursive
		*    Also take sub-directories into account?
		*  @param[in] bDelayedPluginLoading
		*    'true' if it's allowed to perform delayed shared library loading to speed up the program start, else 'false'
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*     This function scans for '.plugin'-files and registers the plugins if all went fine.
		*/
		RECORE_API bool scanPlugins(const String &sPath, ERecursive nRecursive = Recursive, bool bDelayedPluginLoading = true);

		/**
		*  @brief
		*    Load plugin
		*
		*  @param[in] sFilename
		*    Filename of the plugin (must be a '.plugin'-file!)
		*  @param[in] bDelayedPluginLoading
		*    'true' if it's allowed to perform delayed shared library loading to speed up the program start, else 'false'
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		RECORE_API bool loadPlugin(const String &sFilename, bool bDelayedPluginLoading = true);

		/**
		*  @brief
		*    Unload plugin
		*
		*  @param[in] pModule
		*    Module to unload (must be valid!)
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the module is static)
		*
		*  @note
		*    - You can only unload plugins modules, other modules will be ignored
		*/
		RECORE_API bool unloadPlugin(Module *pModule);

		/**
		*  @brief
		*    Unload all plugins
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		RECORE_API bool unloadAllPlugins();

		/**
		*  @brief
		*    Get all registered classes
		*
		*  @return
		*    std::list of classes (do not destroy the returned instances)
		*/
		inline const std::list<const Class*> &getClasses() const;

		/**
		*  @brief
		*    Get classes based on their base class and/or module (search for classes)
		*
		*  @param[out] lstClasses
		*    std::list that receives the found classes (do not destroy the returned instances)
		*  @param[in] sClass
		*    Name of base class (with namespace)
		*  @param[in] nRecursive
		*    'Recursive' to list all subclasses of the base class, 'NonRecursive' to only list direct subclasses
		*  @param[in] nIncludeBase
		*    'IncludeBase' to include the base class itself, 'NoBase' to exclude it
		*  @param[in] nIncludeAbstract
		*    'IncludeAbstract' to include classes without a constructor, 'NoAbstract' to exclude them
		*  @param[in] nModuleID
		*    ID of module, or 0 to include the classes from all modules
		*
		*  @remarks
		*    The found classes will be added to the list that is passed as the first argument to the function.
		*    The class list is not automatically cleared by this function.
		*/
		RECORE_API void getClasses(std::list<const Class*> &lstClasses, const String &sClass, ERecursive nRecursive = Recursive, EIncludeBase nIncludeBase = IncludeBase, EIncludeAbstract nIncludeAbstract = IncludeAbstract, uint32 nModuleID = 0) const;

		/**
		*  @brief
		*    Get class
		*
		*  @param[in] sClass
		*    Class name
		*
		*  @return
		*    Pointer to the class, or a null pointer on error (do not destroy the returned instance)
		*/
		inline const Class *getClass(const String &sClass) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		RECORE_API ClassManager();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		RECORE_API ClassManager(const ClassManager &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~ClassManager();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		RECORE_API ClassManager &operator =(const ClassManager &cSource);

		/**
		*  @brief
		*    Get a unique module ID
		*
		*  @return
		*    Unique module ID for a new module
		*/
		RECORE_API uint32 getUniqueModuleID();

		/**
		*  @brief
		*    Create module
		*
		*  @param[in] nModuleID
		*    Module ID
		*
		*  @return
		*    Module belonging to that ID
		*
		*  @remarks
		*    If a module with that ID already exists, the function will return a pointer to it.
		*    Otherwise it will create a new module and assign it to the given ID.
		*/
		RECORE_API Module *createModule(uint32 nModuleID);

		/**
		*  @brief
		*    Register module
		*
		*  @param[in] pnModuleID
		*    Pointer to a static module ID variable inside the shared library or executable the module is implemented in, always valid ("memory anchor")
		*  @param[in] sName
		*    Module name
		*  @param[in] sVendor
		*    Vendor name
		*  @param[in] sLicense
		*    Module license
		*  @param[in] sDescription
		*    Module description
		*/
		RECORE_API void registerModule(const uint32 *pnModuleID, const String &sName, const String &sVendor, const String &sLicense, const String &sDescription);

		/**
		*  @brief
		*    Unregister module
		*
		*  @param[in] pModule
		*    Module (must be valid!)
		*/
		RECORE_API void unregisterModule(Module *pModule);

		/**
		*  @brief
		*    Register class
		*
		*  @param[in] nModuleID
		*    Module ID
		*  @param[in] pClassImpl
		*    Pointer to class implementation (must be valid), if it's a dummy the
		*    class manager takes over the control and will destroy it if it's no longer needed
		*
		*  @note
		*    - Called automatically by the RTTI system (see "re_class"-macro)
		*/
		RECORE_API void registerClass(uint32 nModuleID, ClassImpl *pClassImpl);

		/**
		*  @brief
		*    Unregister class
		*
		*  @param[in] nModuleID
		*    Module ID
		*  @param[in] pClassImpl
		*    Pointer to class implementation (must be valid), if it's a dummy the
		*    class manager takes over the control and will destroy it if it's no longer needed
		*
		*  @note
		*    - Called automatically by the RTTI system (see "re_class"-macro)
		*/
		RECORE_API void unregisterClass(uint32 nModuleID, ClassImpl *pClassImpl);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Modules
		uint32							m_nModuleID;	/**< Module ID counter */
		std::list<const Module*>				m_lstModules;	/**< std::list of all modules */
		std::map<uint32, const Module*>	m_mapModules;	/**< Hash-map of module IDs to modules */
		// Classes
		std::list<const Class*>				m_lstClasses;	/**< std::list of all classes */
		std::map<String, const Class*>	m_mapClasses;	/**< Hash-map of class names to classes (with namespace) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Reflect/ClassManager.inl"
