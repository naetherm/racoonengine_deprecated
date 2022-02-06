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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for platform specific 'DynLib' implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'DynLib' abstraction
*/
class IDynLib {


  //[-------------------------------------------------------]
  //[ Friends                                               ]
  //[-------------------------------------------------------]
  friend class DynLib;


  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Constructor
  */
  IDynLib() = default;

  /**
  *  @brief
  *    Destructor
  */
  virtual ~IDynLib() = default;


  //[-------------------------------------------------------]
  //[ Protected virtual IDynLib functions                ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Returns if the dynamic library has been loaded
  *
  *  @return
  *    'true' if loaded, else 'false'
  */
  [[nodiscard]] virtual bool isLoaded() const = 0;

  /**
  *  @brief
  *    Loads a dynamic library
  *
  *  @param[in] cUrl
  *    Path to the dynamic library
  *
  *  @return
  *    'true' if the library could be loaded, else false
  */
  [[nodiscard]] virtual bool load(const String &cUrl) = 0;

  /**
  *  @brief
  *    Get the absolute path to the dynamic library
  *
  *  @return
  *    The absolute path to the dynamic library (native path style)
  */
  [[nodiscard]] virtual String getAbsPath() const = 0;

  /**
  *  @brief
  *    Unloads the dynamic library
  *
  *  @return
  *    'true' if the library could be unloaded, else false
  */
  virtual bool unload() = 0;

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
  [[nodiscard]] virtual void *getSymbol(const String &sSymbol) const = 0;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
