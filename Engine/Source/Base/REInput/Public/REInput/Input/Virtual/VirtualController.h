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
#include "REInput/REInput.h"
#include "REInput/Input/Controller.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual input controller
*
*  @remarks
*    A virtual input controller is a controller that is used to map real input devices to virtual controls.
*    Usually, you have one virtual input controller for your application, or in rare occasions more than one,
*    e.g. one for every window or player. The virtual controller connects itself to the physical input devices,
*    a virtual function can be used to alter this behavior in derived classes. The virtual controller should
*    then be connected to the controllers of input-enabled objects, such as scene nodes or modifiers.
*/
class VirtualController : public Controller
{


  //[-------------------------------------------------------]
  //[ Public functions                                      ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] inputManager
  *    Owner input manager
  *  @param[in] sName
  *    Controller name
  *  @param[in] sDescription
  *    Controller description
  */
  inline VirtualController(InputManager& inputManager, const std::string &sName, const std::string &sDescription) :
    Controller(inputManager, ControllerType::VIRTUAL, sName, sDescription)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~VirtualController() override
  {
    // Nothing here
  }

  VirtualController &operator =(const VirtualController &cSource) = delete;


  //[-------------------------------------------------------]
  //[ Public virtual VirtualController functions            ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Connect virtual controller to physical devices
  *
  *  @remarks
  *    This function shall enumerate the available devices and connect
  *    itself to the proper controls of those input devices. Override
  *    this function in derived classes to alter it's behavior or to
  *    support your own type of virtual controllers
  */
  inline virtual void connectToDevices()
  {
    // Nothing here
  }


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
