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
#include "RECore/Reflect/Object.h"
#include "RECore/Core/AbstractContext.h"
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * ApplicationContext
 *
 * @brief
 * Application context
 *
 * @remarks
 * The application context stores data and information for an application,
 * such as startup information (what was the current directory when the
 * application started) or paths to needed resources.
 */
class RECORE_API ApplicationContext : public Object, public AbstractContext {


  //[-------------------------------------------------------]
  //[ RTTI interface                                        ]
  //[-------------------------------------------------------]
  re_class_def(RECORE_API)
  re_class_def_end


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Constructor
   */
  RECORE_API ApplicationContext() = default;

  /**
   * @brief
   * Destructor
   */
  RECORE_API ~ApplicationContext() override = default;


  /**
  *  @brief
  *    Get absolute path of application executable
  *
  *  @return
  *    Path to executable (native path style, e.g. on Windows: 'C:\MyApplication\x86\Test.exe')
  */
  [[nodiscard]] inline String getExecutableFilename() const;

  /**
  *  @brief
  *    Set absolute path of application executable
  *
  *  @param[in] sExecutableFilename
  *    Path to executable (e.g. on Windows: 'C:\MyApplication\x86\Test.exe', automatically converted internally to native path style)
  */
  inline void setExecutableFilename(const String &executableFilename);

  /**
   * @brief
   * Get command line arguments.
   *
   * @return
   * Lust of command line arguments that were passed to the program.
   */
  [[nodiscard]] inline const std::vector<String>& getArguments() const;

  /**
   * @brief
   * Set command line arguments.
   *
   * @param[in] arguments
   * List of command line arguments that were passed to the program.
   */
  inline void setArguments(const std::vector<String>& arguments);

protected:
  String			msExecutableFilename;
  std::vector<String> mlstArguments;

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Application/ApplicationContext.inl"
