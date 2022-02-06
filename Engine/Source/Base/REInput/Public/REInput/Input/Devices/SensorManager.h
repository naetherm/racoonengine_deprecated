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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sensor manager input device
*
*  @remarks
*    The sensor manager is a collection of sensors usually available on a mobile device:
*    - Accelerometer
*    - Magnetic field
*    - Gyroscope
*    - Light
*    - Proximity
*
*    This class supports the following device backend types:
*    - UpdateDevice
*/
class SensorManager final : public Device
{


  //[-------------------------------------------------------]
  //[ Controller definition                                 ]
  //[-------------------------------------------------------]
public:
  // Accelerometer
  Axis	AccelerationX;	///< X acceleration axis (Accelerometer)
  Axis	AccelerationY;	///< Y acceleration axis (Accelerometer)
  Axis	AccelerationZ;	///< Z acceleration axis (Accelerometer)
  // Magnetic field
  Axis	MagneticX;		///< X magnetic axis (Magnetic field)
  Axis	MagneticY;		///< Y magnetic axis (Magnetic field)
  Axis	MagneticZ;		///< Z magnetic axis (Magnetic field)
  // Gyroscope
  Axis	RotationX;		///< X rotation axis (Gyroscope)
  Axis	RotationY;		///< Y rotation axis (Gyroscope)
  Axis	RotationZ;		///< Z rotation axis (Gyroscope)
  // Light
  Axis	Light;			///< Light
  // Proximity
  Axis	Proximity;		///< Proximity


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
  REINPUT_API SensorManager(InputManager& inputManager, const std::string &sName, DeviceImpl *pImpl);

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~SensorManager() override
  {
    // Nothing here
  }

  SensorManager &operator =(const SensorManager &cSource) = delete;


  //[-------------------------------------------------------]
  //[ Public virtual Controller functions                   ]
  //[-------------------------------------------------------]
public:
  REINPUT_API virtual void update() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
