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
#include "RECore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class IDynLib;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Dynamic library (aka "shared library") class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class RECORE_API DynLib {


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  */
  DynLib();

  /**
  *  @brief
  *    Destructor
  *
  *  @note
  *    - The dynamic library is unloaded automatically
  */
  ~DynLib();

  /**
  *  @brief
  *    Loads a dynamic library
  *
  *  @param[in] sPath
  *    Path to the dynamic library
  *
  *  @return
  *    'true' if the library could be loaded, else 'false'
  *
  *  @note
  *    - Dependent dynamic libraries are first searched within the same path as the given one
  *    - If the library has already been loaded, this function will fail
  */
  [[nodiscard]] inline bool load(const String &sPath);

  /**
  *  @brief
  *    Returns if the dynamic library has been loaded
  *
  *  @return
  *    'true' if loaded, else 'false'
  */
  [[nodiscard]] inline bool isLoaded() const;

  /**
  *  @brief
  *    Get the path (set within 'Load()') to the dynamic library
  *
  *  @return
  *    The path to the dynamic library (e.g. "file://C:/"RacoonEngine/Runtime/x86/PLCore.dll" on Windows)
  */
  [[nodiscard]] inline String getPath() const;

  /**
  *  @brief
  *    Get the absolute path to the dynamic library
  *
  *  @return
  *    The absolute path to the dynamic library (native path style, e.g. "C:\RacoonEngine\Runtime\x86\PLCore.dll" on Windows)
  */
  [[nodiscard]] inline String getAbsPath() const;

  /**
  *  @brief
  *    Returns a pointer to a symbol in the library
  *
  *  @param[in] sSymbol
  *    Name of the symbol to retrieve
  *
  *  @return
  *    Pointer to the symbol, or a null pointer on error
  *
  *  @note
  *    - The pointer to the symbol only stays valid as long as this dynamic library instance is not unloaded
  */
  [[nodiscard]] inline void *getSymbol(const String &sSymbol) const;

  /**
  *  @brief
  *    Unloads the dynamic library
  *
  *  @return
  *    'true' if the library could be unloaded, else false
  */
  [[nodiscard]] inline bool unload();


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] cSource
  *    Source to copy from
  */
  DynLib(const DynLib &cSource);

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
  DynLib &operator=(const DynLib &cSource);


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  IDynLib *m_pImpl;  /**< Pointer to the system specific implementation (assumed to be never a null pointer!) */
  String msUrl;      /**< String to the dynamic library */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/System/DynLib.inl"