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
#include "RECore/Application/ApplicationContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Frontend context
*
*  @remarks
*    The frontend context stores data and information for an frontend,
*    such as startup information (what was the current directory when the
*    frontend started) or paths to needed resources.
*/
class FrontendContext : public ApplicationContext {


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  */
  RECORE_API FrontendContext();

  /**
  *  @brief
  *    Destructor
  */
  RECORE_API ~FrontendContext() override = default;


  //[-------------------------------------------------------]
  //[ Options and data                                      ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    Get frontend executable name
  *
  *  @return
  *    Name of the frontend executable (default: "")
  *
  *  @remarks
  *    The name of the frontend executable (one can also call it "host", the running executable) is
  *    used as default whenever a name is required (log, configuration, window title and so on).
  */
  inline String getName() const;

  /**
  *  @brief
  *    Set frontend executable name
  *
  *  @param[in] sName
  *    Name of the frontend executable (default: "")
  *
  *  @see
  *    - GetName()
  */
  inline void setName(const String &sName);

  /**
  *  @brief
  *    Returns the name of the frontend RTTI class to use
  *
  *  @return
  *    The name of the frontend RTTI class to use (default: "RECore::FrontendRacoonEngine")
  */
  inline String getFrontend() const;

  /**
  *  @brief
  *    Sets the name of the frontend RTTI class to use
  *
  *  @param[in] sFrontend
  *    The name of the frontend RTTI class to use (default: "RECore::FrontendRacoonEngine")
  */
  inline void setFrontend(const String &sFrontend);

  /**
  *  @brief
  *    Returns the name of the frontend RTTI class constructor to use
  *
  *  @return
  *    The name of the frontend RTTI class constructor to use (default: "")
  */
  inline String getFrontendConstructor() const;

  /**
  *  @brief
  *    Sets the name of the frontend RTTI class constructor to use
  *
  *  @param[in]
  *    The name of the frontend RTTI class constructor to use (default: "")
  */
  inline void setFrontendConstructor(const String &sFrontendConstructor);

  /**
  *  @brief
  *    Returns the parameters for the frontend RTTI class constructor
  *
  *  @return
  *    Parameters for the frontend RTTI class constructor (default: "")
  */
  inline String getFrontendConstructorParameters() const;

  /**
  *  @brief
  *    Sets the parameters for the frontend RTTI class constructor
  *
  *  @param[in] sFrontendConstructorParameters
  *    Parameters for the frontend RTTI class constructor (default: "")
  */
  inline void setFrontendConstructorParameters(const String &sFrontendConstructorParameters);

  /**
  *  @brief
  *    Returns the parameters for the instanced frontend RTTI class
  *
  *  @return
  *    The parameters for the instanced frontend RTTI class (default: "")
  */
  inline String getFrontendParameters() const;

  /**
  *  @brief
  *    Sets the parameters for the instanced frontend RTTI class
  *
  *  @param[in] sFrontendParameters
  *    The parameters for the instanced frontend RTTI class (default: "")
  */
  inline void setFrontendParameters(const String &sFrontendParameters);

  /**
  *  @brief
  *    Returns the name of the frontend implementation RTTI class to use
  *
  *  @return
  *    The name of the frontend implementation RTTI class to use (default: "PLFrontendOS::Frontend")
  */
  inline String getFrontendImplementation() const;

  /**
  *  @brief
  *    Sets the name of the frontend implementation RTTI class to use
  *
  *  @param[in] sFrontendImplementation
  *    The name of the frontend implementation RTTI class to use (default: "PLFrontendOS::Frontend")
  */
  inline void setFrontendImplementation(const String &sFrontendImplementation);

  /**
  *  @brief
  *    Returns the name of the frontend implementation RTTI class constructor to use
  *
  *  @return
  *    The name of the frontend implementation RTTI class constructor to use (default: "")
  */
  inline String getFrontendImplementationConstructor() const;

  /**
  *  @brief
  *    Sets the name of the frontend implementation RTTI class constructor to use
  *
  *  @param[in] sFrontendImplementationConstructor
  *    The name of the frontend implementation RTTI class constructor to use (default: "")
  */
  inline void setFrontendImplementationConstructor(const String &sFrontendImplementationConstructor);

  /**
  *  @brief
  *    Returns the parameters for the frontend implementation RTTI class constructor
  *
  *  @return
  *    The parameters for the frontend implementation RTTI class constructor (default: "")
  */
  inline String getFrontendImplementationConstructorParameters() const;

  /**
  *  @brief
  *    Sets the parameters for the frontend implementation RTTI class constructor
  *
  *  @param[in] sFrontendImplementationConstructorParameters
  *    The parameters for the frontend implementation RTTI class constructor (default: "")
  */
  inline void
  setFrontendImplementationConstructorParameters(const String &sFrontendImplementationConstructorParameters);

  /**
  *  @brief
  *    Returns the parameters for the instanced frontend implementation RTTI class
  *
  *  @return
  *    The parameters for the instanced frontend implementation RTTI class (default: "")
  */
  inline String getFrontendImplementationParameters() const;

  /**
  *  @brief
  *    Sets the parameters for the instanced frontend implementation RTTI class
  *
  *  @param[in] sFrontendImplementationParameters
  *    The parameters for the instanced frontend implementation RTTI class (default: "")
  */
  inline void setFrontendImplementationParameters(const String &sFrontendImplementationParameters);


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  String m_sName;                      /**< Name of the frontend executable                                   (default: "") */
  String m_sFrontend;                    /**< Name of the frontend RTTI class to use                            (default: "RECore::FrontendRacoonEngine") */
  String m_sFrontendConstructor;              /**< Name of the frontend RTTI class constructor to use                (default: "") */
  String m_sFrontendConstructorParameters;        /**< Parameters for the frontend RTTI class constructor                (default: "") */
  String m_sFrontendParameters;              /**< Parameters for the instanced frontend RTTI class                  (default: "") */
  String m_sFrontendImplementation;            /**< Name of the frontend implementation RTTI class to use             (default: "PLFrontendOS::Frontend") */
  String m_sFrontendImplementationConstructor;      /**< Name of the frontend implementation RTTI class constructor to use (default: "") */
  String m_sFrontendImplementationConstructorParameters;  /**< Parameters for the frontend implementation RTTI class constructor (default: "") */
  String m_sFrontendImplementationParameters;        /**< Parameters for the instanced frontend implementation RTTI class   (default: "") */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Frontend/FrontendContext.inl"
