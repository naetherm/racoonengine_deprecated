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
#include "RECore/System/IDynLib.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class RECORE_API WindowsDynLib : public IDynLib {
public:

  /**
   * @brief
   * Constructor.
   */
  WindowsDynLib();

  /**
   * @brief
   * Destructor.
   */
  ~WindowsDynLib();


  //[-------------------------------------------------------]
  //[ Protected IDynLib functions                ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Returns if the dynamic library has been loaded
  *
  *  @return
  *    'true' if loaded, else 'false'
  */
  [[nodiscard]] bool isLoaded() const override;

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
  [[nodiscard]] bool load(const String &cUrl) override;

  /**
  *  @brief
  *    Get the absolute path to the dynamic library
  *
  *  @return
  *    The absolute path to the dynamic library (native path style)
  */
  [[nodiscard]] String getAbsPath() const override;

  /**
  *  @brief
  *    Unloads the dynamic library
  *
  *  @return
  *    'true' if the library could be unloaded, else false
  */
  bool unload() override;

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
  [[nodiscard]] void *getSymbol(const String &sSymbol) const override;

private:
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
