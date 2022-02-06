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
#include "REInput/Input/Control/Effect.h"
#include "REInput/Input/Control/LED.h"
#include <RECore/Reflect/Event/EventHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


class ConnectionDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    WiiMote input device
*
*  @remarks
*    This class supports the following device backend types:
*    - ConnectionDevice
*/
class WiiMote final : public Device
{


  //[-------------------------------------------------------]
  //[ Controller definition                                 ]
  //[-------------------------------------------------------]
public:
  // LEDs and effects
  LED		LEDs;				///< LEDs
  Effect	Rumble1;			///< Rumble effect (motor #1)

  // Main buttons
  //Button  Button1;			///< Button '1'
  //Button  Button2;			///< Button '2'
  Button  ButtonA;			///< Button 'A'
  Button  ButtonB;			///< Button 'B'
  Button  ButtonMinus;		///< Button 'Minus'
  Button  ButtonPlus;			///< Button 'Plus'
  Button  ButtonHome;			///< Button 'Home'
  Button  ButtonLeft;			///< Button 'Left'
  Button  ButtonRight;		///< Button 'Right'
  Button  ButtonUp;			///< Button 'Up'
  Button  ButtonDown;			///< Button 'Down'

  // Main values
  Axis	AccX;				///< Acceleration (X)
  Axis	AccY;				///< Acceleration (Y)
  Axis	AccZ;				///< Acceleration (Z)
  Axis	OrientX;			///< Orientation (X)
  Axis	OrientY;			///< Orientation (Y)
  Axis	OrientZ;			///< Orientation (Z)
  Axis	Roll;				///< Rotation (roll)
  Axis	Pitch;				///< Rotation (pitch)
  Axis	PointerX;			///< Pointer (X)
  Axis	PointerY;			///< Pointer (Y)

  // Nunchuk
  Button  NunchukButtonC;		///< Nunchuk Button 'C'
  Button  NunchukButtonZ;		///< Nunchuk Button 'Z'
  Axis	NunchukAccX;		///< Nunchuk Acceleration (X)
  Axis	NunchukAccY;		///< Nunchuk Acceleration (Y)
  Axis	NunchukAccZ;		///< Nunchuk Acceleration (Z)
  Axis	NunchukOrientX;		///< Nunchuk Orientation (X)
  Axis	NunchukOrientY;		///< Nunchuk Orientation (Y)
  Axis	NunchukOrientZ;		///< Nunchuk Orientation (Z)
  Axis	NunchukRoll;		///< Nunchuk Rotation (roll)
  Axis	NunchukPitch;		///< Nunchuk Rotation (pitch)
  Axis	NunchukX;			///< Nunchuk joystick (X)
  Axis	NunchukY;			///< Nunchuk joystick (Y)


  //[-------------------------------------------------------]
  //[ Public data types                                     ]
  //[-------------------------------------------------------]
public:
  // Definitions
  enum EProductID {
    // HID definitions
    VendorID  = 0x057e,		///< 'Nintendo'
    ProductID = 0x0306,		///< 'Wiimote'

    // Bluetooth definitions
    DeviceClass0 = 0x04,
    DeviceClass1 = 0x25,
    DeviceClass2 = 0x00
  };

  /**
  *  @brief
  *    Report type
  */
  enum EReport {
    ReportStatus			= 0x20,	///< Status report
    ReportReadMemory		= 0x21,	///< Data from memory
    ReportButtons			= 0x30,	///< Buttons
    ReportButtonsAccel		= 0x31,	///< Buttons and acceleration
    ReportButtonsAccelIR	= 0x33,	///< Buttons, acceleration and IR (extended)
    ReportButtonsAccelExt	= 0x35,	///< Buttons, acceleration and extension
    ReportButtonsAccelIRExt	= 0x37	///< Buttons, acceleration, IR (basic) and extension
  };

  /**
  *  @brief
  *    Infrared sensor mode
  */
  enum EIRMode {
    IROff		= 0x00,	///< IR off
    IRBasic		= 0x01,	///< Basic IR mode
    IRExtended	= 0x03,	///< Extended IR mode
    IRFull		= 0x05	///< Full IR mode
  };

  /**
  *  @brief
  *    WiiMote extension
  */
  enum EExtension {
    ExtNone					= 0x0000,	///< No extension
    ExtNunchuk				= 0xfefe,	///< Nunchuk
    ExtClassic				= 0xfdfd,	///< Classic controller
    ExtPartiallyInserted	= 0xffff	///< Extension not inserted correctly
  };


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
  *    System specific device implementation, must be valid!
  */
  REINPUT_API WiiMote(InputManager& inputManager, const std::string &sName, DeviceImpl *pImpl);

  /**
  *  @brief
  *    Destructor
  */
  REINPUT_API virtual ~WiiMote() override;

  /**
  *  @brief
  *    Get report mode
  *
  *  @return
  *    Report mode
  */
  [[nodiscard]] inline EReport getReportMode() const
  {
    return m_nReportMode;
  }

  /**
  *  @brief
  *    Set report mode
  *
  *  @param[in] nReportMode
  *    Report mode
  *  @param[in] bContinuous
  *    Continuous updates?
  */
  REINPUT_API void setReportMode(EReport nReportMode, bool bContinuous = false);

  /**
  *  @brief
  *    Get infrared mode
  *
  *  @return
  *    Infrared mode
  */
  [[nodiscard]] inline EIRMode getIRMode() const
  {
    return m_nIRMode;
  }

  /**
  *  @brief
  *    Set infrared mode
  *
  *  @param[in] nIRMode
  *    Infrared mode
  */
  REINPUT_API void setIRMode(EIRMode nIRMode);

  /**
  *  @brief
  *    Get extension
  *
  *  @return
  *    Extension type
  */
  [[nodiscard]] inline EExtension getExtension() const
  {
    return m_nExtension;
  }

  /**
  *  @brief
  *    Get battery state
  *
  *  @return
  *    Battery state
  */
  [[nodiscard]] inline uint8_t getBattery() const
  {
    return m_nBattery;
  }

  /**
  *  @brief
  *    calibrate device
  */
  inline void calibrate()
  {
    sendCalibrationRequest();
  }


  //[-------------------------------------------------------]
  //[ Public virtual Controller functions                   ]
  //[-------------------------------------------------------]
public:
  REINPUT_API virtual void updateOutputControl(Control *pControl) override;


  //[-------------------------------------------------------]
  //[ Private data types                                    ]
  //[-------------------------------------------------------]
private:
  /**
  *  @brief
  *    Acceleration data
  */
  struct SAcceleration {
    // Status
    int nUpdateNearG;	///< Update counter when acceleration is near 1G

    // Status
    float fAccX;		///< Acceleration: X
    float fAccY;		///< Acceleration: Y
    float fAccZ;		///< Acceleration: Z
    float fOrientX;		///< Orientation:  X
    float fOrientY;		///< Orientation:  Y
    float fOrientZ;		///< Orientation:  Z
    float fRoll;		///< Angle:        Roll
    float fPitch;		///< Angle:        Pitch

    // Calibration
    uint8_t nX0;	///< Calibration: X0
    uint8_t nY0;	///< Calibration: Y0
    uint8_t nZ0;	///< Calibration: Z0
    uint8_t nXG;	///< Calibration: XG
    uint8_t nYG;	///< Calibration: YG
    uint8_t nZG;	///< Calibration: ZG

    // Calculate orientation from acceleration data
    void calculateOrientation();
  };

  /**
  *  @brief
  *    Nunchuk joystick data
  */
  struct SJoystick {
    // Status
    float fX;	///< X position
    float fY;	///< Y position

    // Calibration
    uint8_t nMinX;	///< Calibration: Minimum X
    uint8_t nMidX;	///< Calibration: Middle  X
    uint8_t nMaxX;	///< Calibration: Maximum X
    uint8_t nMinY;	///< Calibration: Minimum Y
    uint8_t nMidY;	///< Calibration: Middle  Y
    uint8_t nMaxY;	///< Calibration: Maximum Y
  };

  /**
  *  @brief
  *    IR sensor dot
  */
  struct SDot {
    bool  bFound;	///< The dot has been found
    int   nRawX;	///< Raw X position
    int   nRawY;	///< Raw Y position
    float fX;		///< X position (0..1)
    float fY;		///< Y position (0..1)
    int   nSize;	///< Dot size
  };


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
  WiiMote &operator =(const WiiMote &cSource) = delete;

  /**
  *  @brief
  *    Called when the HID device has been connected
  */
  void onDeviceConnect();

  /**
  *  @brief
  *    Called when the HID device has been disconnected
  */
  void onDeviceDisconnect();

  /**
  *  @brief
  *    Called when the HID device has read some data
  */
  inline void onDeviceRead()
  {
    // Read data
    onReadData();

    // Propagate changes
    m_bChanged = true;
  }

  /**
  *  @brief
  *    Data from the WiiMote has been received
  */
  void onReadData();

  /**
  *  @brief
  *    Data from memory received
  */
  void onReadMemory();

  /**
  *  @brief
  *    Calibration data received
  */
  void onReadCalibration();

  /**
  *  @brief
  *    Extension information received
  */
  void onReadExtensionType();

  /**
  *  @brief
  *    Nunchuk calibration data received
  */
  void onReadNunchukCalibration();

  /**
  *  @brief
  *    Classic calibration data received
  */
  void onReadClassicCalibration();

  /**
  *  @brief
  *    Status information received
  */
  void onReadStatus();

  /**
  *  @brief
  *    Button status received
  */
  void onReadButtons();

  /**
  *  @brief
  *    Acceleration status received
  */
  void onReadAccel();

  /**
  *  @brief
  *    IR status received
  */
  void onReadIR();

  /**
  *  @brief
  *    Extension status received
  */
  void onReadExtension(uint32_t nOffset);

  /**
  *  @brief
  *    Nunchuk status received
  */
  void onReadNunchuk(uint32_t nOffset);

  /**
  *  @brief
  *    Classic-controller status received
  */
  void onReadClassic(uint32_t nOffset);

  /**
  *  @brief
  *    Read from WiiMote memory
  *
  *  @param[in] nAddress
  *    Address to read from
  *  @param[in] nSize
  *    Size to read
  */
  void readMemory(int nAddress, uint8_t nSize);

  /**
  *  @brief
  *    Write to WiiMote memory
  *
  *  @param[in] nAddress
  *    Address to write to
  *  @param[in] pBuffer
  *    Buffer containing the data
  *  @param[in] nSize
  *    Size to write
  */
  void writeMemory(int nAddress, const uint8_t* pBuffer, uint8_t nSize);

  /**
  *  @brief
  *    Write a single byte to WiiMote memory
  *
  *  @param[in] nAddress
  *    Address to write to
  *  @param[in] nData
  *    Byte to write
  */
  inline void writeMemory(int nAddress, uint8_t nData)
  {
    // Write one byte
    writeMemory(nAddress, &nData, 1);
  }

  /**
  *  @brief
  *    Clear output report
  */
  inline void clearReport()
  {
    memset(m_pOutputBuffer, 0, 22);
  }

  /**
  *  @brief
  *    Send data to WiiMote device
  *
  *  @param[in] pBuffer
  *    Pointer to buffer (must be valid!)
  *  @param[in] nSize
  *    Size of buffer
  */
  void send(uint8_t *pBuffer, uint32_t nSize);

  /**
  *  @brief
  *    Decrypt data
  *
  *  @param[in] nOffset
  *    Start address inside m_nWriteBuffer
  *  @param[in] nSize
  *    Size inside m_nWriteBuffer
  */
  void decryptBuffer(uint32_t nOffset, uint32_t nSize);

  /**
  *  @brief
  *    Get WiiMote status
  */
  void sendStatusRequest();

  /**
  *  @brief
  *    Get calibration information
  */
  void sendCalibrationRequest();


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // Event handlers
  RECore::EventHandler<> EventHandlerOnDeviceConnect;
  RECore::EventHandler<> EventHandlerOnDeviceDisconnect;
  RECore::EventHandler<> EventHandlerOnDeviceRead;

  // HID connection
  ConnectionDevice *m_pConnectionDevice;	///< Connection device, always valid
  uint8_t			 *m_pInputBuffer;		///< Input buffer
  uint8_t			 *m_pOutputBuffer;		///< Output buffer

  // WiiMote options
  EReport	   m_nReportMode;	///< Report mode
  EIRMode	   m_nIRMode;		///< IR mode
  EExtension m_nExtension;	///< Extension type

  // WiiMote status
  uint8_t			m_nBattery;		///< Battery (percent)
  uint8_t			m_nLEDs;		///< LEDs status
  uint8_t			m_nRumble;		///< Rumble state (1=on, 0=off)
  uint16_t		m_nButtons;		///< WiiMote Buttons
  SAcceleration	m_sAcc;			///< Acceleration sensor
  SDot			m_sDots[2];		///< IR dots
  float			m_vIRPos[2];	///< IR position (X, Y between 0..1)

  // Nunchuk status
  uint16_t	  m_nNunchukButtons;	///< Nunchuk buttons
  SAcceleration m_sNunchukAcc;		///< Nunchuk acceleration sensor
  SJoystick	  m_sNunchukJoy;		///< Nunchuk joystick


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
