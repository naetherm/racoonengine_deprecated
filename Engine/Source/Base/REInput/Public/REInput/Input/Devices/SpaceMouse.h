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
#include "REInput/Input/Devices/Device.h"
#include "REInput/Input/Control/Axis.h"
#include "REInput/Input/Control/Button.h"
#include <RECore/Reflect/Event/EventHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


class HIDDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    SpaceMouse input device
*
*  @remarks
*    This class supports the following device backend types:
*    - HIDDevice
*/
class SpaceMouse final : public Device
{


  //[-------------------------------------------------------]
  //[ Public definitions                                    ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    SpaceMouse IDs
  *
  *  @remarks
  *    It is not recommended to use the ProductID, because it's different for each individual product (NOT future safe)
  */
  enum EProductID {
    VendorID				 = 1133,	///< '3DConnexion'
    SpaceMousePlus_ProductID = 0xc603,	///< 'SpaceMousePlus'
    SpaceBall_ProductID		 = 0xc621,	///< 'SpaceBall'
    SpaceTraveler_ProductID	 = 0xc623,	///< 'SpaceTraveler'
    SpacePilot_ProductID	 = 0xc625,	///< 'SpacePilot'
    SpaceNavigator_ProductID = 0xc626,	///< 'SpaceNavigator'
    SpaceExplorer_ProductID	 = 0xc627	///< 'SpaceExplorer'
  };


  //[-------------------------------------------------------]
  //[ Controller definition                                 ]
  //[-------------------------------------------------------]
public:
  Axis	TransX;		///< Absolute x translation axis (the values are usually roughly between [-400 .. 400])
  Axis	TransY;		///< Absolute y translation axis (the values are usually roughly between [-400 .. 400])
  Axis	TransZ;		///< Absolute z translation axis (the values are usually roughly between [-400 .. 400])
  Axis	RotX;		///< Absolute x rotation axis (the values are usually roughly between [-400 .. 400])
  Axis	RotY;		///< Absolute y rotation axis (the values are usually roughly between [-400 .. 400])
  Axis	RotZ;		///< Absolute z rotation axis (the values are usually roughly between [-400 .. 400])
  //Button	Button0;	///< Button #0
  //Button	Button1;	///< Button #1
  //Button	Button2;	///< Button #2
  //Button	Button3;	///< Button #3
  //Button	Button4;	///< Button #4
  //Button	Button5;	///< Button #5
  Button	Button6;	///< Button #6
  Button	Button7;	///< Button #7


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
  *    Device name
  *  @param[in] pImpl
  *    System specific device implementation, can, but shouldn't be a null pointer
  */
  REINPUT_API SpaceMouse(InputManager& inputManager, const std::string &sName, DeviceImpl *pImpl);

  /**
  *  @brief
  *    Destructor
  */
  REINPUT_API virtual ~SpaceMouse() override;


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Copy operator
  *
  *  @param[in] cSource
  *    Source to copy from
  *
  *  @return
  *    This instance
  */
  SpaceMouse &operator =(const SpaceMouse &cSource) = delete;

  /**
  *  @brief
  *    Called when the HID device has read some data
  */
  void onDeviceRead();


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // Event handlers
  RECore::EventHandler<> EventHandlerOnDeviceRead;

  HIDDevice *m_pHIDDevice;	///< HID device


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
