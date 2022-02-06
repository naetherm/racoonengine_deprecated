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
#include "RECore/RECore.h"
#include "RECore/Core/Manager.h"
#include "RECore/Reflect/Event/EventHandler.h"
#include <map>
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Class;
class Script;
class ScriptBinding;
class IFileManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Script manager
*/
class ScriptManager : public Manager {


  //[-------------------------------------------------------]
  //[ Public static RECore::Singleton functions             ]
  //[-------------------------------------------------------]
  // This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
  // -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
  // -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
  // -> See RECore::Singleton for more details about singletons
public:
  static RECORE_API ScriptManager &instance();


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Returns a list of the names of the supported script languages
  *
  *  @return
  *    List of the names of the supported script languages
  */
  inline const std::vector<String> &getScriptLanguages();

  /**
  *  @brief
  *    Returns the name of a script language by it's filename extension
  *
  *  @param[in] sExtension
  *    Script language filename extension to check (e.g. "js")
  *
  *  @return
  *    Script language, empty string on error (possibly not supported filename extension)
  */
  inline String getScriptLanguageByExtension(const String &sExtension);

  /**
  *  @brief
  *    Checks whether or not a given script language is supported
  *
  *  @param[in] sScriptLanguage
  *    Script language to check
  *
  *  @return
  *    'true' if the given script language is supported, else 'false'
  */
  inline bool isSupported(const String &sScriptLanguage);

  /**
  *  @brief
  *    Returns the filename extension of a given script language
  *
  *  @param[in] sScriptLanguage
  *    Script language to return the filename extension from
  *
  *  @return
  *    The filename extension of the given script language, empty string on error (possibly not supported script language)
  *
  *  @note
  *    - If the script language has more than one filename extension, the first filename extension will be returned
  */
  RECORE_API String getScriptLanguageExtension(const String &sScriptLanguage);

  /**
  *  @brief
  *    Returns a list of all script binding instances
  *
  *  @return
  *    List of all script binding instances
  */
  inline const std::vector<ScriptBinding *> &getScriptBindings();

  /**
  *  @brief
  *    Creates a script instance
  *
  *  @param[in] sScriptLanguage
  *    Script language to use
  *  @param[in] bAddBindings
  *    If 'true', add all available script bindings automatically (see "Script::addBindings()")
  *
  *  @return
  *    The created script instance, null pointer on error
  */
  RECORE_API Script *create(const String &sScriptLanguage, bool bAddBindings = true);

  /**
  *  @brief
  *    Creates a script instance by using a given filename
  *
  *  @param[in] sFilename
  *    Script filename
  *  @param[in] bAddBindings
  *    If 'true', add all available script bindings automatically (see "Script::addBindings()")
  *  @param[in] nStringFormat
  *    String encoding format to use when dealing with string functions (not supported by all file implementations)
  *
  *  @return
  *    The created script instance, null pointer on error (Unknown filename extension? File not found? Error within the script?)
  *
  *  @remarks
  *    While the "create()"-method only creates an empty script instance, the "createFromFile()"-method will
  *    - Create an empty script instance by using the filename extension as indication of the script language to use
  *    - Load in the script source code by using the given script filename
  *    - Assign the loaded script source code to the created, previously empty, script
  *    The result is a ready to be used script by just using this one method. The drawback is, that you can't use
  *    custom global functions when using this method because the custom global functions have to be defined before
  *    the script source code is assigned to the script (resulting in script compilation).
  *
  *  @note
  *    - Convenience method
  *    - It's not recommended to use Unicode by because internally wchar_t is used and this data type has not
  *      the same size on every platform (use ASCII or UTF8 instead)
  */
  RECORE_API Script *createFromFile(const IFileManager& fileManager, const String &sFilename, bool bAddBindings = true);


  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Constructor
  */
  RECORE_API ScriptManager();

  /**
  *  @brief
  *    Destructor
  */
  RECORE_API virtual ~ScriptManager();


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Register a class
  *
  *  @param[in] pClass
  *    Class to register, must be valid!
  *
  *  @note
  *    - If the class is not derived from 'RECore::Script' it is ignored
  *    - The given class must be a new one, so this function did not check whether the
  *      class is already registered
  */
  inline void onClassLoaded(const Class *pClass);

  /**
  *  @brief
  *    Unregister a class
  *
  *  @param[in] pClass
  *    Class to unregister, must be valid!
  */
  void onClassUnloaded(const Class *pClass);

  /**
  *  @brief
  *    Registers queued classes
  */
  RECORE_API void registerClasses();


  //[-------------------------------------------------------]
  //[ Private slots                                         ]
  //[-------------------------------------------------------]
private:
  EventHandler<const Class *> SlotClassLoaded;
  EventHandler<const Class *> SlotClassUnloaded;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  std::vector<const Class *> m_lstNewClasses;          /**< New classes to register as soon as required */
  // Script languages
  std::vector<String> m_lstScriptLanguages;        /**< List of script languages */
  std::map<String, const Class *> m_mapScriptLanguages;        /**< Map of script languages (key = class name) */
  std::map<String, String> m_mapScriptLanguagesByExtension;  /**< Map of script languages by extension (key = extension) */
  // Script bindings
  std::vector<ScriptBinding *> m_lstScriptBindings;        /**< List of script bindings */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Scripting/ScriptManager.inl"