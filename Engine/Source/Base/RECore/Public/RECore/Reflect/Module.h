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
#include <list>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Class;
class DynLib;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Description and interface for modules
*
*  @remarks
*    A module is a unit like for example an executable or a shared library that contains RTTI content.
*    There can only be one RTTI module per executable or a shared library. A module can either be a
*    plugin or not. The term plugin means in this case that a module is not explicitly linked to the
*    executable or to a used shared library and therefore not loaded automatically by the operation
*    system on startup.
*/
class Module {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ClassDummy;
	friend class ClassManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get module ID
		*
		*  @return
		*    Module ID
		*/
		inline uint32 getModuleID() const;

		/**
		*  @brief
		*    Check if module is loaded as a plugin
		*
		*  @return
		*    'true' if module is a plugin, else 'false'
		*/
		inline bool isPlugin() const;

		/**
		*  @brief
		*    Get dynamic library that contains the plugin
		*
		*  @return
		*    Pointer to dynamic library (can be a null pointer, if the module is not a plugin, do NOT destroy the returned instance!)
		*
		*  @remarks
		*    This function will only return a dynamic library, if the module is a plugin
		*/
		inline DynLib *getDynLib() const;

		/**
		*  @brief
		*    Get absolute filename of dynamic library or executable that contains the plugin
		*
		*  @return
		*    Absolute filename of dynamic library or executable that contains the plugin (native path style, can be empty in case it was impossible to determine the filename)
		*/
		inline String getFilename() const;

		/**
		*  @brief
		*    Get module name
		*
		*  @return
		*    Name
		*/
		inline String getName() const;

		/**
		*  @brief
		*    Get module vendor
		*
		*  @return
		*    Vendor name
		*/
		inline String getVendor() const;

		/**
		*  @brief
		*    Get module license
		*
		*  @return
		*    License
		*/
		inline String getLicense() const;

		/**
		*  @brief
		*    Get module description
		*
		*  @return
		*    Description
		*/
		inline String getDescription() const;

		/**
		*  @brief
		*    Get classes of module
		*
		*  @return
		*    List of classes
		*
		*  @remarks
		*    This method always returns all classes of a module.
		*    If you want to search for classes with more specific search criteria,
		*    have a look at ClassManager::getClasses().
		*/
		inline const std::list<const Class*> &getClasses() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nModuleID
		*    Module ID
		*/
		RECORE_API Module(uint32 nModuleID);

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API ~Module();

		/**
		*  @brief
		*    Set module information
		*
		*  @param[in] sName
		*    Module name
		*  @param[in] sVendor
		*    Module vendor
		*  @param[in] sLicense
		*    Module license
		*  @param[in] sDescription
		*    Module description
		*/
		RECORE_API void setModuleInfo(const String &sName, const String &sVendor, const String &sLicense, const String &sDescription);

		/**
		*  @brief
		*    Add class
		*
		*  @param[in] pClass
		*    Pointer to class (must be valid)
		*/
		inline void addClass(const Class *pClass);

		/**
		*  @brief
		*    Remove class
		*
		*  @param[in] pClass
		*    Pointer to class (must be valid)
		*/
		inline void removeClass(const Class *pClass);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Module information
		uint32			   m_nModuleID;		/**< Module ID */
		bool			   m_bPlugin;		/**< Is module a plugin? */
		DynLib			  *m_pDynLib;		/**< Plugin library (can be a null pointer, has the ownership over the dynamic library instance) */
		String			   m_sFilename;		/**< Absolute absolute filename of dynamic library or executable that contains the plugin (native path style, can be empty in case it was impossible to determine the filename) */
		String			   m_sName;			/**< Name of module */
		String			   m_sVendor;		/**< Vendor of module */
		String			   m_sLicense;		/**< License of module */
		String			   m_sDescription;	/**< Description of module */
		// Classes
		std::list<const Class*> m_lstClasses;	/**< List of classes */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Reflect/Module.inl"
