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
#include "REInput/Input/Control/Effect.h"
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
*    Joystick input device
*
*  @remarks
*    This class supports the following device backend types:
*    - UpdateDevice
*    - HIDDevice
*/
class Joystick final : public Device
{


  //[-------------------------------------------------------]
  //[ Controller definition                                 ]
  //[-------------------------------------------------------]
public:
  // Input
  Axis	X;			///< X axis
  Axis	Y;			///< Y axis
  Axis	Z;			///< Z axis
  Axis	RX;			///< Rotation X axis
  Axis	RY;			///< Rotation Y axis
  Axis	RZ;			///< Rotation Z axis
  Axis	Hat;		///< Hat axis
  //Button	Button0;	///< Button #0
  //Button	Button1;	///< Button #1
  //Button	Button2;	///< Button #2
  //Button	Button3;	///< Button #3
  //Button	Button4;	///< Button #4
  //Button	Button5;	///< Button #5
  //Button	Button6;	///< Button #6
  //Button	Button7;	///< Button #7
  //Button	Button8;	///< Button #8
  //Button	Button9;	///< Button #9
  //Button	Button10;	///< Button #10
  //Button	Button11;	///< Button #11
  //Button	Button12;	///< Button #12
  //Button	Button13;	///< Button #13
  //Button	Button14;	///< Button #14
  //Button	Button15;	///< Button #15
  //Button	Button16;	///< Button #16
  //Button	Button17;	///< Button #17
  //Button	Button18;	///< Button #18
  //Button	Button19;	///< Button #19
  //Button	Button20;	///< Button #20
  //Button	Button21;	///< Button #21
  //Button	Button22;	///< Button #22
  //Button	Button23;	///< Button #23
  //Button	Button24;	///< Button #24
  //Button	Button25;	///< Button #25
  //Button	Button26;	///< Button #26
  //Button	Button27;	///< Button #27
  //Button	Button28;	///< Button #28
  //Button	Button29;	///< Button #29
  //Button	Button30;	///< Button #30
  //Button	Button31;	///< Button #31

  // Effects
  Effect	Rumble1;	///< Rumble effect (motor #1)
  Effect	Rumble2;	///< Rumble effect (motor #2)
  Effect	Rumble3;	///< Rumble effect (motor #3)
  Effect	Rumble4;	///< Rumble effect (motor #4)


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
  REINPUT_API Joystick(InputManager& inputManager, const std::string &sName, DeviceImpl *pImpl);

  /**
  *  @brief
  *    Destructor
  */
  REINPUT_API virtual ~Joystick() override;

  /**
  *  @brief
  *    Get threshold
  *
  *  @return
  *    Threshold
  */
  [[nodiscard]] inline int getThreshold() const
  {
    return m_nThreshold;
  }

  /**
  *  @brief
  *    Set threshold
  *
  *  @param[in] nThreshold
  *    Threshold
  */
  inline void setThreshold(int nThreshold = 12000)
  {
    m_nThreshold = nThreshold;
  }


  //[-------------------------------------------------------]
  //[ Public virtual Controller functions                   ]
  //[-------------------------------------------------------]
public:
  REINPUT_API virtual void update() override;
  REINPUT_API virtual void updateOutputControl(Control *pControl) override;


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
  Joystick &operator =(const Joystick &cSource) = delete;

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
  // HID connection
  HIDDevice *m_pHIDDevice;	///< HID device, can be a null pointer

  // Configuration
  int	m_nThreshold;			///< Movement threshold


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
