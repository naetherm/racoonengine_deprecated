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
//#include "RECore/File/Url.h"
#include "RECore/File/IFile.h"
#include "RECore/File/IFileManager.h"
#include "RECore/Reflect/Class.h"
#include "RECore/Reflect/ClassManager.h"
//#include "RECore/Tools/LoadableManager.h"
#include "RECore/Scripting/Script.h"
#include "RECore/Scripting/ScriptBinding.h"
#include "RECore/Scripting/ScriptManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Public static RECore::Singleton functions             ]
//[-------------------------------------------------------]
ScriptManager &ScriptManager::instance()
{
	static ScriptManager SSingleton;
  return SSingleton;
}



//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the filename extension of a given script language
*/
String ScriptManager::getScriptLanguageExtension(const String &sScriptLanguage)
{
	registerClasses();

	// Get the RTTI class of the given script language
  std::map<String, const Class *>::iterator pClass = m_mapScriptLanguages.find(sScriptLanguage);
	if (pClass != m_mapScriptLanguages.end()) {
		// Parse formats
    auto iter = pClass->second->getProperties().find("Formats");
    if (iter != pClass->second->getProperties().end()) {
      const String sFormats = iter->second;
      if (sFormats.length()) {
        // Setup the tokenizer
        CTokenizer cTokenizer;
        cTokenizer.Start(sFormats);
        cTokenizer.SetDelimiters(" ,\t\r\n");
        cTokenizer.SetSingleChars("");

        // Return the first found filename extension
        return cTokenizer.GetNextToken();
      }
    }
	}

	// Error!
	return "";
}

/**
*  @brief
*    Creates a script instance
*/
Script *ScriptManager::create(const String &sScriptLanguage, bool bAddBindings)
{
	// Get the proper script class
	registerClasses();
  std::map<String, const Class *>::iterator pClass = m_mapScriptLanguages.find(sScriptLanguage);
	if (pClass != m_mapScriptLanguages.end()) {
		// Create an instance of the script class
		Script *pScript = reinterpret_cast<Script*>(pClass->second->create());

		// Bind all available script bindings at once?
		if (bAddBindings)
			pScript->addBindings();

		// Return the created script instance
		return pScript;
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Creates a script instance by using a given filename
*/
Script *ScriptManager::createFromFile(const IFileManager& fileManager, const String &sFilename, bool bAddBindings)
{
	// Create the script instance by using the extension of the given filename to detect the script language
	//Script *pScript = create(getScriptLanguageByExtension(Url(sFilename).GetExtension()), bAddBindings);
  Script *pScript = create(sFilename.substr(sFilename.findLastOf(".")), bAddBindings);
	if (pScript) {
		// Get the script source code
		//const String sSourceCode = LoadableManager::GetInstance()->LoadStringFromFile(sFilename);
    IFile* file = fileManager.openFile(IFileManager::FileMode::READ, sFilename);

    sizeT nNumOfBytes = file->getNumberOfBytes();

    const char* rawSourceCode = new char[nNumOfBytes];
    file->read(&rawSourceCode, nNumOfBytes);
    const String sSourceCode(rawSourceCode, nNumOfBytes);
    delete[] rawSourceCode;

    fileManager.closeFile(*file);

		// Set the script source code
		if (!sSourceCode.length() || !pScript->setSourceCode(sSourceCode)) {
			// Error! Destroy the created script instance...
			delete pScript;
			pScript = nullptr;
		}
	}

	// Done
	return pScript;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptManager::ScriptManager() :
	SlotClassLoaded(&ScriptManager::onClassLoaded, this),
	SlotClassUnloaded(&ScriptManager::onClassUnloaded, this)
{
	// The script manager MUST be informed if new classes are registered in order to register new script languages!
	ClassManager::instance().EventClassLoaded.connect(SlotClassLoaded);
	ClassManager::instance().EventClassUnloaded.connect(SlotClassUnloaded);

	{ // Register all script languages
		std::list<const Class*> lstClasses;
		ClassManager::instance().getClasses(lstClasses, "RECore::Script", Recursive, NoBase, NoAbstract);
		std::list<const Class*>::iterator cIterator = lstClasses.begin();
		while (cIterator != lstClasses.end()) {
      m_lstNewClasses.push_back(*cIterator);
      cIterator++;
    }
	}

	{ // Register all script bindings
		std::list<const Class*> lstClasses;
		ClassManager::instance().getClasses(lstClasses, "RECore::ScriptBinding", Recursive, NoBase, NoAbstract);
		std::list<const Class*>::iterator cIterator = lstClasses.begin();
		while (cIterator != lstClasses.end()) {
      m_lstNewClasses.push_back(*cIterator);
      cIterator++;
    }
	}
}

/**
*  @brief
*    Destructor
*/
ScriptManager::~ScriptManager()
{
	// Destroy all script binding instances, usually when we're in here there should no such instances be left
	for (uint32 i=0; i<m_lstScriptBindings.size(); i++)
		delete m_lstScriptBindings[i];
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Unregister a class
*/
void ScriptManager::onClassUnloaded(const Class *pClass)
{
  /*
	// Destroy script binding instance if required
	for (uint32 i=0; i<m_lstScriptBindings.size(); i++) {
		// Get the current script binding instance
		ScriptBinding *pScriptBinding = m_lstScriptBindings[i];

		// Is this an instance of the unloaded class?
		if (pScriptBinding->GetClass() == pClass) {
			// Destroy instance
			m_lstScriptBindings.RemoveAtIndex(i);
			delete pScriptBinding;

			// Get us out of the loop (there's just one instance per script binding class)
			i = m_lstScriptBindings.size();
		}
	}
   */
  for (auto iter=m_lstScriptBindings.begin(); iter != m_lstScriptBindings.end(); iter++) {
    // Get the current script binding instance
    ScriptBinding *pScriptBinding = *iter;

    // Is this an instance of the unloaded class?
    if (pScriptBinding->getClass() == pClass) {
      // Destroy instance
      m_lstScriptBindings.erase(iter);
      delete pScriptBinding;
    }
  }
}

/**
*  @brief
*    Registers queued classes
*/
void ScriptManager::registerClasses()
{
	// Is there anything on the register queue?
	if (m_lstNewClasses.size()) {
		std::vector<const Class*>::iterator cIterator = m_lstNewClasses.begin();
		while (cIterator != m_lstNewClasses.end()) {
			// Get the class
			const Class *pClass = *cIterator;

			// Check parameter and base class
			static const String sScriptClassString = "RECore::Script";
			static const String sScriptBindingClassString = "RECore::ScriptBinding";
			if (pClass->isDerivedFrom(sScriptClassString)) {
				// Register script language
				auto languageIter = pClass->getProperties().find("Language");
        if (languageIter != pClass->getProperties().end()) {
          const String sLanguage = languageIter->second;
          if (sLanguage.length() && m_mapScriptLanguages.find(sLanguage) == m_mapScriptLanguages.end()) {
            m_lstScriptLanguages.push_back(sLanguage);
            m_mapScriptLanguages.emplace(sLanguage, pClass);

            // Parse formats
            auto formatIter = pClass->getProperties().find("Formats");
            if (formatIter != pClass->getProperties().end()) {
              const String sFormats = formatIter->second;
              if (sFormats.length()) {
                // Setup the tokenizer
                CTokenizer cTokenizer;
                cTokenizer.Start(sFormats);
                cTokenizer.SetDelimiters(" ,\t\r\n");
                cTokenizer.SetSingleChars("");

                // Register the filename extensions of this script language
                String sToken = cTokenizer.GetNextToken();
                while (sToken.length()) {
                  m_mapScriptLanguagesByExtension.emplace(sToken, sLanguage);
                  sToken = cTokenizer.GetNextToken();
                }
              }
            }
          }
        }
			} else if (pClass->isDerivedFrom(sScriptBindingClassString)) {
				// Create an script binding instance
				ScriptBinding *pScriptBinding = static_cast<ScriptBinding*>(pClass->create());
				if (pScriptBinding)
					m_lstScriptBindings.push_back(pScriptBinding);
			}

      // Increment
      ++cIterator;
		}
		m_lstNewClasses.clear();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
