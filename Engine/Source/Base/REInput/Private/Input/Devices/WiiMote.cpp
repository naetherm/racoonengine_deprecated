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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "REInput/Input/Devices/WiiMote.h"
#include "REInput/Backend/ConnectionDevice.h"
#include <math.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {

//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    WiiMote ports
*/
enum EPort {
  ControlPort		= 17,	///< Control port (output)
  InterruptPort	= 19	///< Interrupt port (input)
};

/**
*  @brief
*    WiiMote commands
*/
enum ECommand {
  CmdNone				= 0x00,	///< No command
  CmdLEDs				= 0x11,	///< Read LEDs
  CmdType				= 0x12,	///< Set report mode
  CmdIR				= 0x13,	///< Enable IR
  CmdSpeakerEnable	= 0x14,	///< Enable speaker
  CmdStatus			= 0x15,	///< Get status
  CmdWriteMemory		= 0x16,	///< Write to memory
  CmdReadMemory		= 0x17,	///< Read from memory
  CmdSpeakerData		= 0x18,	///< Send speaker data
  CmdSpeakerMute		= 0x19,	///< Mute speaker
  CmdIR2				= 0x1a	///< Enable IR
};

/**
*  @brief
*    WiiMote registers
*/
enum ERegister {
  RegCalibration			= 0x00000016,	///< Calibration
  RegIR					= 0x04b00030,	///< IR
  RegIRSensitivity1		= 0x04b00000,	///< IR sensitivity (1)
  RegIRSensitivity2		= 0x04b0001a,	///< IR sensitivity (2)
  RegIRMode				= 0x04b00033,	///< IR mode
  RegExtensionInit		= 0x04a40040,	///< Extension initialization
  RegExtensionType		= 0x04a400fe,	///< Extension type
  RegExtensionCalibration	= 0x04a40020	///< Extension calibration
};

/**
*  @brief
*    WiiMote buttons
*/
enum EButton {
  BtnLeft		= 0x0001,	///< Button "Left"
  BtnRight	= 0x0002,	///< Button "Right"
  BtnDown		= 0x0004,	///< Button "Down"
  BtnUp		= 0x0008,	///< Button "Up"
  BtnPlus		= 0x0010,	///< Button "+"
  Btn2		= 0x0100,	///< Button "2"
  Btn1		= 0x0200,	///< Button "1"
  BtnB		= 0x0400,	///< Button "B"
  BtnA		= 0x0800,	///< Button "A"
  BtnMinus	= 0x1000,	///< Button "-"
  BtnHome		= 0x8000	///< Button "Home"
};

/**
*  @brief
*    Nunchuk buttons
*/
enum ENunchukButton {
  BtnNunchukZ	= 0x0001,	///< Button "Z"
  BtnNunchukC	= 0x0002	///< Button "C"
};

// IR camera
static constexpr int IR_MaxX	= 1016;	///< Effective X Resolution of IR sensor
static constexpr int IR_MaxY	=  760;	///< Effective Y Resolution of IR sensor


//[-------------------------------------------------------]
//[ Global helper functions                               ]
//[-------------------------------------------------------]
bool ValueChanged(float fValueOld, float fValueNew)
{
  // Apply epsilon boundary
  return (std::fabs(fValueNew - fValueOld) >= 0.01f);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
WiiMote::WiiMote(InputManager& inputManager, const std::string &sName, DeviceImpl *pImpl) :
  Device(inputManager, sName, "WiiMote controller", pImpl),
  LEDs			(*this,	"LEDs",				"LEDs"),
  Rumble1			(*this,	"Rumble1",			"Rumble motor #1"),
  //Button1			(*this,	"Button1",			"Button '1'"),
  //Button2			(*this,	"Button2",			"Button '2'"),
  ButtonA			(*this,	"ButtonA",			"Button 'A'"),
  ButtonB			(*this,	"ButtonB",			"Button 'B'"),
  ButtonMinus		(*this,	"ButtonMinus",		"Button 'Minus'"),
  ButtonPlus		(*this,	"ButtonPlus",		"Button 'Plus'"),
  ButtonHome		(*this,	"ButtonHome",		"Button 'Home'"),
  ButtonLeft		(*this,	"ButtonLeft",		"Button 'Left'"),
  ButtonRight		(*this,	"ButtonRight",		"Button 'Right'"),
  ButtonUp		(*this,	"ButtonUp",			"Button 'Up'"),
  ButtonDown		(*this,	"ButtonDown",		"Button 'Down'"),
  AccX			(*this,	"AccX",				"Acceleration axis (X)"),
  AccY			(*this,	"AccY",				"Acceleration axis (Y)"),
  AccZ			(*this,	"AccZ",				"Acceleration axis (Z)"),
  OrientX			(*this,	"OrientX",			"Orientation axis (X)"),
  OrientY			(*this,	"OrientY",			"Orientation axis (Y)"),
  OrientZ			(*this,	"OrientZ",			"Orientation axis (Z)"),
  Roll			(*this,	"Roll",				"Rotation (roll)"),
  Pitch			(*this,	"Pitch",			"Rotation (pitch)"),
  PointerX		(*this,	"PointerX",			"Pointer(X)"),
  PointerY		(*this,	"PointerY",			"Pointer(Y)"),
  NunchukButtonC	(*this,	"NunchukButtonC",	"Nunchuk button 'C'"),
  NunchukButtonZ	(*this,	"NunchukButtonZ",	"Nunchuk button 'Z'"),
  NunchukAccX		(*this,	"NunchukAccX",		"Nunchuk acceleration axis (X)"),
  NunchukAccY		(*this,	"NunchukAccY",		"Nunchuk acceleration axis (Y)"),
  NunchukAccZ		(*this,	"NunchukAccZ",		"Nunchuk acceleration axis (Z)"),
  NunchukOrientX	(*this,	"NunchukOrientX",	"Nunchuk orientation axis (X)"),
  NunchukOrientY	(*this,	"NunchukOrientY",	"Nunchuk orientation axis (Y)"),
  NunchukOrientZ	(*this,	"NunchukOrientZ",	"Nunchuk orientation axis (Z)"),
  NunchukRoll		(*this,	"NunchukRoll",		"Nunchuk rotation (roll)"),
  NunchukPitch	(*this,	"NunchukPitch",		"Nunchuk rotation (pitch)"),
  NunchukX		(*this,	"NunchukX",			"Nunchuk joystick (X)"),
  NunchukY		(*this,	"NunchukY",			"Nunchuk joystick (Y)"),
  EventHandlerOnDeviceConnect	  (&WiiMote::onDeviceConnect,	 this),
  EventHandlerOnDeviceDisconnect(&WiiMote::onDeviceDisconnect, this),
  EventHandlerOnDeviceRead	  (&WiiMote::onDeviceRead,		 this),
  m_pConnectionDevice(static_cast<ConnectionDevice*>(pImpl)),
  m_pInputBuffer(nullptr),
  m_pOutputBuffer(nullptr),
  m_nReportMode(ReportButtons),
  m_nIRMode(IROff),
  m_nExtension(ExtNone),
  m_nBattery(0),
  m_nLEDs(0),
  m_nRumble(0),
  m_nButtons(0),
  m_vIRPos{0.0f, 0.0f},
  m_nNunchukButtons(0)
{
  // Set input and output report size
  if (m_pConnectionDevice->getDeviceType() == ConnectionDevice::DeviceTypeBluetooth) {
    m_pConnectionDevice->setInputReportSize(22);
    m_pConnectionDevice->setOutputReportSize(22);
  }

  // Connect to HIDDevice events
  m_pConnectionDevice->OnConnectSignal.connect(EventHandlerOnDeviceConnect);
  m_pConnectionDevice->OnDisconnectSignal.connect(EventHandlerOnDeviceDisconnect);
  m_pConnectionDevice->OnReadSignal.connect(EventHandlerOnDeviceRead);

  // Connect to device
  m_pConnectionDevice->open(ControlPort, InterruptPort);

  // Initialize data
  m_sAcc.nUpdateNearG			 = 0;
  m_sNunchukAcc.nUpdateNearG	 = 0;
}

WiiMote::~WiiMote()
{
  // We use m_pImpl here to check, because if the device backend has been deleted before, m_pImpl has
  // been reset to a null pointer, but not m_pConnectionDevice as this is unknown in the base class
  if (m_pImpl) {
    // Disconnect
    m_pConnectionDevice->close();
  }
}

void WiiMote::setReportMode(EReport nReportMode, bool bContinuous)
{
  // Set report mode
  m_nReportMode = nReportMode;

  // Enable/Disable infrared sensor
  switch (m_nReportMode) {
    case ReportButtonsAccelIR:
      setIRMode(IRExtended);
      break;

    case ReportButtonsAccelIRExt:
      setIRMode(IRBasic);
      break;

    case ReportStatus:
    case ReportReadMemory:
    case ReportButtons:
    case ReportButtonsAccel:
    case ReportButtonsAccelExt:
    default:
      setIRMode(IROff);
      break;
  }

  // Set new mode
  clearReport();
  m_pOutputBuffer[0] = CmdType;
  m_pOutputBuffer[1] = (bContinuous ? 0x04u : 0x00u) | m_nRumble;
  m_pOutputBuffer[2] = static_cast<uint8_t>(nReportMode);
  send(m_pOutputBuffer, 3);
}

void WiiMote::setIRMode(EIRMode nIRMode)
{
  // Set IR mode
  m_nIRMode = nIRMode;

  // Set new mode
  if (m_nIRMode != IROff) {
    clearReport();
    m_pOutputBuffer[0] = CmdIR;
    m_pOutputBuffer[1] = 0x04u | m_nRumble;
    send(m_pOutputBuffer, 2);
    m_pOutputBuffer[0] = CmdIR2;
    m_pOutputBuffer[1] = 0x04u | m_nRumble;
    send(m_pOutputBuffer, 2);

    static constexpr uint8_t ir_sens1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0xc0};
    static constexpr uint8_t ir_sens2[] = {0x40, 0x00};

    writeMemory(RegIR, 0x08);
    writeMemory(RegIRSensitivity1, ir_sens1, sizeof(ir_sens1));
    writeMemory(RegIRSensitivity2, ir_sens2, sizeof(ir_sens2));
    writeMemory(RegIRMode, static_cast<uint8_t>(nIRMode));
  } else {
    clearReport();
    m_pOutputBuffer[0] = CmdIR;
    m_pOutputBuffer[1] = m_nRumble;
    send(m_pOutputBuffer, 2);
    m_pOutputBuffer[0] = CmdIR2;
    m_pOutputBuffer[1] = m_nRumble;
    send(m_pOutputBuffer, 2);
  }
}


//[-------------------------------------------------------]
//[ Public virtual Controller functions                   ]
//[-------------------------------------------------------]
void WiiMote::updateOutputControl(Control *pControl)
{
  // Update LEDs
  if (pControl == &LEDs) {
    // Set LEDs
    m_nLEDs = static_cast<uint8_t>(LEDs.getLedStates()) & 0x0fu;

    // Send command
    clearReport();
    m_pOutputBuffer[0] = CmdLEDs;
    m_pOutputBuffer[1] = static_cast<uint8_t>((m_nLEDs << 4) | m_nRumble);
    send(m_pOutputBuffer, 2);

    // Update rumble
  } else if (pControl == &Rumble1) {
    // Switch rumble on or off?
    if (Rumble1.getValue() > 0) {
      // Enable rumble
      m_nRumble = 1;
      sendStatusRequest();
    } else {
      // Disable rumble
      m_nRumble = 0;
      sendStatusRequest();
    }
  }
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
void WiiMote::onDeviceConnect()
{
  m_pInputBuffer  = m_pConnectionDevice->getInputBuffer();
  m_pOutputBuffer = m_pConnectionDevice->getOutputBuffer();

  sendStatusRequest();
  sendCalibrationRequest();
  setReportMode(ReportButtonsAccelIR, false);
}

void WiiMote::onDeviceDisconnect()
{
}

void WiiMote::onReadData()
{
  // What kind of data has been received?
  switch (m_pInputBuffer[0]) {
    // Get buttons
    case ReportButtons:
      onReadButtons();
      break;

      // Get buttons and acceleration
    case ReportButtonsAccel:
      onReadButtons();
      onReadAccel();
      break;

      // Get buttons, acceleration and extension
    case ReportButtonsAccelExt:
      onReadButtons();
      onReadAccel();
      decryptBuffer(0, m_pConnectionDevice->getInputReportSize());
      onReadExtension(6);
      break;

      // Get buttons, acceleration and IR (extended mode)
    case ReportButtonsAccelIR:
      onReadButtons();
      onReadAccel();
      onReadIR();
      break;

      // Get buttons, acceleration, IR (basic mode) and extension
    case ReportButtonsAccelIRExt:
      onReadButtons();
      onReadAccel();
      onReadIR();
      decryptBuffer(0, m_pConnectionDevice->getInputReportSize());
      onReadExtension(16);
      break;

      // Get data from memory
    case ReportReadMemory:
      onReadButtons();
      onReadMemory();
      break;

      // Get status report
    case ReportStatus:
      onReadStatus();
      break;
  }
}

void WiiMote::onReadMemory()
{
  // Check memory address
  if ((m_pInputBuffer[3] & 0x08) != 0) {
    // Error: Invalid read-address
    return;
  } else if((m_pInputBuffer[3] & 0x07) != 0) {
    // Error: Attempt to read from write-only registers
    return;
  }

  // Get size
  int nSize = m_pInputBuffer[3] >> 4;

  // Get address
  int nAddress = m_pInputBuffer[4]<<8 | m_pInputBuffer[5];

  // *NOTE*: this is a major (but convenient) hack!  The returned data only
  //          contains the lower two bytes of the address that was queried.
  //			as these don't collide between any of the addresses/registers
  //			we currently read, it's OK to match just those two bytes
  switch (nAddress) {
    case (RegCalibration & 0xffff):
      if (nSize != 6)
        break;	// Error! Wrong size ...
      onReadCalibration();
      break;

    case (RegExtensionType & 0xffff):
      if (nSize != 1)
        break;	// Error! Wrong size ...
      onReadExtensionType();
      break;

    case (RegExtensionCalibration & 0xffff):
      if (nSize != 15)
        break;	// Error! Wrong size ...
      decryptBuffer(6, 16);
      if (m_nExtension == ExtNunchuk)
        onReadNunchukCalibration();
      else if (m_nExtension == ExtClassic)
        onReadClassicCalibration();
      break;
  }
}

void WiiMote::onReadCalibration()
{
  // Get calibration info
  m_sAcc.nX0 = m_pInputBuffer[6+0];
  m_sAcc.nY0 = m_pInputBuffer[6+1];
  m_sAcc.nZ0 = m_pInputBuffer[6+2];
  m_sAcc.nXG = m_pInputBuffer[6+4];
  m_sAcc.nYG = m_pInputBuffer[6+5];
  m_sAcc.nZG = m_pInputBuffer[6+6];
}

void WiiMote::onReadExtensionType()
{
  // Get extension type
  uint16_t nExtension = *reinterpret_cast<uint16_t*>(&m_pInputBuffer[6]);

  // If new extension, get calibration info
  if (nExtension == ExtNunchuk && m_nExtension != ExtNunchuk) {
    // Nunchuk registered
    readMemory(RegExtensionCalibration, 16);
    m_nExtension = ExtNunchuk;
  } else if (nExtension == ExtClassic && m_nExtension != ExtClassic) {
    // Classic-controller registered
    readMemory(RegExtensionCalibration, 16);
    m_nExtension = ExtClassic;
  } else if (nExtension == ExtPartiallyInserted && m_nExtension != ExtPartiallyInserted) {
    // Extension not registered correctly
    m_nExtension = ExtPartiallyInserted;
    sendStatusRequest();
  } else {
    // Unknown extension
  }
}

void WiiMote::onReadNunchukCalibration()
{
  // Get nunchuk calibration
  m_sNunchukAcc.nX0	 = m_pInputBuffer[6+0];
  m_sNunchukAcc.nY0	 = m_pInputBuffer[6+1];
  m_sNunchukAcc.nZ0	 = m_pInputBuffer[6+2];
  m_sNunchukAcc.nXG	 = m_pInputBuffer[6+4];
  m_sNunchukAcc.nYG	 = m_pInputBuffer[6+5];
  m_sNunchukAcc.nZG	 = m_pInputBuffer[6+6];
  m_sNunchukJoy.nMaxX  = m_pInputBuffer[6+8];
  m_sNunchukJoy.nMinX  = m_pInputBuffer[6+9];
  m_sNunchukJoy.nMidX  = m_pInputBuffer[6+10];
  m_sNunchukJoy.nMaxY  = m_pInputBuffer[6+11];
  m_sNunchukJoy.nMinY  = m_pInputBuffer[6+12];
  m_sNunchukJoy.nMidY  = m_pInputBuffer[6+13];

  // Reset report mode
  setReportMode(m_nReportMode, false);
}

void WiiMote::onReadClassicCalibration()
{
  // TODO(naetherm)
  /*
  Internal.ClassicController.CalibrationInfo.MaxXL = m_pInputBuffer[6+ 0] >> 2;
  Internal.ClassicController.CalibrationInfo.MinXL = m_pInputBuffer[6+ 1] >> 2;
  Internal.ClassicController.CalibrationInfo.MidXL = m_pInputBuffer[6+ 2] >> 2;
  Internal.ClassicController.CalibrationInfo.MaxYL = m_pInputBuffer[6+ 3] >> 2;
  Internal.ClassicController.CalibrationInfo.MinYL = m_pInputBuffer[6+ 4] >> 2;
  Internal.ClassicController.CalibrationInfo.MidYL = m_pInputBuffer[6+ 5] >> 2;
  Internal.ClassicController.CalibrationInfo.MaxXR = m_pInputBuffer[6+ 6] >> 3;
  Internal.ClassicController.CalibrationInfo.MinXR = m_pInputBuffer[6+ 7] >> 3;
  Internal.ClassicController.CalibrationInfo.MidXR = m_pInputBuffer[6+ 8] >> 3;
  Internal.ClassicController.CalibrationInfo.MaxYR = m_pInputBuffer[6+ 9] >> 3;
  Internal.ClassicController.CalibrationInfo.MinYR = m_pInputBuffer[6+10] >> 3;
  Internal.ClassicController.CalibrationInfo.MidYR = m_pInputBuffer[6+11] >> 3;
  // this doesn't seem right...
  //	Internal.ClassicController.CalibrationInfo.MinTriggerL = m_pInputBuffer[6+12] >> 3;
  //	Internal.ClassicController.CalibrationInfo.MaxTriggerL = m_pInputBuffer[6+14] >> 3;
  //	Internal.ClassicController.CalibrationInfo.MinTriggerR = m_pInputBuffer[6+13] >> 3;
  //	Internal.ClassicController.CalibrationInfo.MaxTriggerR = m_pInputBuffer[6+15] >> 3;
  Internal.ClassicController.CalibrationInfo.MinTriggerL = 0;
  Internal.ClassicController.CalibrationInfo.MaxTriggerL = 31;
  Internal.ClassicController.CalibrationInfo.MinTriggerR = 0;
  Internal.ClassicController.CalibrationInfo.MaxTriggerR = 31;
  SetReportType(ReportType);
  */
}

void WiiMote::onReadStatus()
{
  // Read button state
  onReadButtons();

  // Get battery
  m_nBattery = m_pInputBuffer[6] / 2u;

  // Get LEDs
  m_nLEDs = static_cast<uint8_t>(m_pInputBuffer[3] >> 4);

  // Get extension
  bool bExtension = ((m_pInputBuffer[3] & 0x02) != 0);
  if ((m_nExtension == ExtNone || m_nExtension == ExtPartiallyInserted) && bExtension) {
    // Initialize extension
    writeMemory(RegExtensionInit, 0x00);
    readMemory (RegExtensionType, 2);
  }
}

void WiiMote::onReadButtons()
{
  // Get state of buttons
  m_nButtons = *reinterpret_cast<uint16_t*>(&m_pInputBuffer[1]);

  // Button '1'
  bool bPressed = ((m_nButtons & Btn1) != 0);
  //if (Button1.isPressed() != bPressed)
  //{
  //	Button1.setPressed(bPressed);
  //}

  // Button '2'
  //bPressed = ((m_nButtons & Btn2) != 0);
  //if (Button2.isPressed() != bPressed)
  //{
  //	Button2.setPressed(bPressed);
  //}

  // Button 'A'
  bPressed = ((m_nButtons & BtnA) != 0);
  if (ButtonA.isPressed() != bPressed)
  {
    ButtonA.setPressed(bPressed);
  }

  // Button 'B'
  bPressed = ((m_nButtons & BtnB) != 0);
  if (ButtonB.isPressed() != bPressed)
  {
    ButtonB.setPressed(bPressed);
  }

  // Button 'Minus'
  bPressed = ((m_nButtons & BtnMinus) != 0);
  if (ButtonMinus.isPressed() != bPressed)
  {
    ButtonMinus.setPressed(bPressed);
  }

  // Button 'Plus'
  bPressed = ((m_nButtons & BtnPlus) != 0);
  if (ButtonPlus.isPressed() != bPressed)
  {
    ButtonPlus.setPressed(bPressed);
  }

  // Button 'Home'
  bPressed = ((m_nButtons & BtnHome) != 0);
  if (ButtonHome.isPressed() != bPressed)
  {
    ButtonHome.setPressed(bPressed);
  }

  // Button 'Left'
  bPressed = ((m_nButtons & BtnLeft) != 0);
  if (ButtonLeft.isPressed() != bPressed)
  {
    ButtonLeft.setPressed(bPressed);
  }

  // Button 'Right'
  bPressed = ((m_nButtons & BtnRight) != 0);
  if (ButtonRight.isPressed() != bPressed)
  {
    ButtonRight.setPressed(bPressed);
  }

  // Button 'Up'
  bPressed = ((m_nButtons & BtnUp) != 0);
  if (ButtonUp.isPressed() != bPressed)
  {
    ButtonUp.setPressed(bPressed);
  }

  // Button 'Down'
  bPressed = ((m_nButtons & BtnDown) != 0);
  if (ButtonDown.isPressed() != bPressed)
  {
    ButtonDown.setPressed(bPressed);
  }
}

void WiiMote::onReadAccel()
{
  // Get raw acceleration data
  uint8_t nRawX = m_pInputBuffer[3];
  uint8_t nRawY = m_pInputBuffer[4];
  uint8_t nRawZ = m_pInputBuffer[5];

  // Compute acceleration
  m_sAcc.fAccX = static_cast<float>(nRawX - m_sAcc.nX0) / static_cast<float>(m_sAcc.nXG - m_sAcc.nX0);
  m_sAcc.fAccY = static_cast<float>(nRawY - m_sAcc.nY0) / static_cast<float>(m_sAcc.nYG - m_sAcc.nY0);
  m_sAcc.fAccZ = static_cast<float>(nRawZ - m_sAcc.nZ0) / static_cast<float>(m_sAcc.nZG - m_sAcc.nZ0);

  // AccX
  if (ValueChanged(AccX.getValue(), m_sAcc.fAccX))
    AccX.setValue(m_sAcc.fAccX, false);

  // AccY
  if (ValueChanged(AccY.getValue(), m_sAcc.fAccY))
    AccY.setValue(m_sAcc.fAccY, false);

  // AccZ
  if (ValueChanged(AccZ.getValue(), m_sAcc.fAccZ))
    AccZ.setValue(m_sAcc.fAccZ, false);

  // Compute orientation
  m_sAcc.calculateOrientation();

  // OrientX
  if (ValueChanged(OrientX.getValue(), m_sAcc.fOrientX))
    OrientX.setValue(m_sAcc.fOrientX, false);

  // OrientY
  if (ValueChanged(OrientY.getValue(), m_sAcc.fOrientY))
    OrientY.setValue(m_sAcc.fOrientY, false);

  // OrientZ
  if (ValueChanged(OrientZ.getValue(), m_sAcc.fOrientZ))
    OrientZ.setValue(m_sAcc.fOrientZ, false);

  // Roll
  if (ValueChanged(Roll.getValue(), m_sAcc.fRoll))
    Roll.setValue(m_sAcc.fRoll, false);

  // Pitch
  if (ValueChanged(Pitch.getValue(), m_sAcc.fPitch))
    Pitch.setValue(m_sAcc.fPitch, false);
}

void WiiMote::onReadIR()
{
  // Get IR mode
  switch (m_nIRMode) {
    // Basic mode
    case IRBasic:
      m_sDots[0].bFound = !(m_pInputBuffer[6] == 0xff && m_pInputBuffer[ 7] == 0xff);
      m_sDots[1].bFound = !(m_pInputBuffer[9] == 0xff && m_pInputBuffer[10] == 0xff);
      if (m_sDots[1].bFound) {
        m_sDots[1].nRawX = m_pInputBuffer[ 9] | ((m_pInputBuffer[8] >> 0) & 0x03) << 8;
        m_sDots[1].nRawY = m_pInputBuffer[10] | ((m_pInputBuffer[8] >> 2) & 0x03) << 8;
      }
      m_sDots[0].nSize = 0;
      m_sDots[1].nSize = 0;
      break;

      // Extended mode
    case IRExtended:
      m_sDots[0].bFound = !(m_pInputBuffer[6]==0xff && m_pInputBuffer[ 7]==0xff && m_pInputBuffer[ 8]==0xff);
      m_sDots[1].bFound = !(m_pInputBuffer[9]==0xff && m_pInputBuffer[10]==0xff && m_pInputBuffer[11]==0xff);
      if (m_sDots[0].bFound)
        m_sDots[0].nSize = m_pInputBuffer[8] & 0x0f;
      if (m_sDots[1].bFound) {
        m_sDots[1].nRawX = m_pInputBuffer[ 9] | ((m_pInputBuffer[11] >> 4) & 0x03) << 8;
        m_sDots[1].nRawY = m_pInputBuffer[10] | ((m_pInputBuffer[11] >> 6) & 0x03) << 8;
        m_sDots[1].nSize = m_pInputBuffer[11] & 0x0f;
      }
      break;

      // Full mode
    case IRFull:
      // Unimplemented
      return;

      // Off/unknown
    case IROff:
    default:
      return;
  }

  // Check first dot
  if (m_sDots[0].bFound) {
    m_sDots[0].nRawX = m_pInputBuffer[6] | ((m_pInputBuffer[8] >> 4) & 0x03) << 8;
    m_sDots[0].nRawY = m_pInputBuffer[7] | ((m_pInputBuffer[8] >> 6) & 0x03) << 8;
    m_sDots[0].fX    = 1.f - (static_cast<float>(m_sDots[0].nRawX) / static_cast<float>(IR_MaxX));
    m_sDots[0].fY    =	     (static_cast<float>(m_sDots[0].nRawY) / static_cast<float>(IR_MaxY));
  }

  // Check second dot
  if (m_sDots[1].bFound) {
    m_sDots[1].fX = 1.f - (static_cast<float>(m_sDots[1].nRawX) / static_cast<float>(IR_MaxX));
    m_sDots[1].fY =	      (static_cast<float>(m_sDots[1].nRawY) / static_cast<float>(IR_MaxY));
  }

  // Compute IR center
  m_vIRPos[0] = (m_sDots[0].fX + m_sDots[1].fX) / 2;
  m_vIRPos[1] = (m_sDots[0].fY + m_sDots[1].fY) / 2;

  // PointerX
  if (ValueChanged(PointerX.getValue(), m_vIRPos[0]))
    PointerX.setValue(m_vIRPos[0], false);

  // PointerY
  if (ValueChanged(PointerY.getValue(), m_vIRPos[1]))
    PointerY.setValue(m_vIRPos[1], false);
}

void WiiMote::onReadExtension(uint32_t nOffset)
{
  // Check extension type
  switch (m_nExtension) {
    // Nunchuk
    case ExtNunchuk:
      onReadNunchuk(nOffset);
      break;

      // Classic controller
    case ExtClassic:
      onReadClassic(nOffset);
      break;

    case ExtNone:
    case ExtPartiallyInserted:
    default:
      // TODO(naetherm) Review this: Do we need to handle this case?
      break;
  }
}

void WiiMote::onReadNunchuk(uint32_t nOffset)
{
  // Get buttons
  m_nNunchukButtons = 0;
  if ((m_pInputBuffer[nOffset+5] & 0x02) == 0)
    m_nNunchukButtons |= BtnNunchukC;
  if ((m_pInputBuffer[nOffset+5] & 0x01) == 0)
    m_nNunchukButtons |= BtnNunchukZ;

  // Button 'C'
  bool bPressed = ((m_nButtons & BtnNunchukC) != 0);
  if (NunchukButtonC.isPressed() != bPressed)
  {
    NunchukButtonC.setPressed(bPressed);
  }

  // Button 'Z'
  bPressed = ((m_nButtons & BtnNunchukZ) != 0);
  if (NunchukButtonZ.isPressed() != bPressed)
  {
    NunchukButtonZ.setPressed(bPressed);
  }

  // Get raw acceleration data
  uint8_t nRawX = m_pInputBuffer[nOffset+2];
  uint8_t nRawY = m_pInputBuffer[nOffset+3];
  uint8_t nRawZ = m_pInputBuffer[nOffset+4];

  // Compute acceleration
  m_sNunchukAcc.fAccX = (static_cast<float>(nRawX) - static_cast<float>(m_sNunchukAcc.nX0)) / (static_cast<float>(m_sNunchukAcc.nXG) - static_cast<float>(m_sNunchukAcc.nX0));
  m_sNunchukAcc.fAccY = (static_cast<float>(nRawY) - static_cast<float>(m_sNunchukAcc.nY0)) / (static_cast<float>(m_sNunchukAcc.nYG) - static_cast<float>(m_sNunchukAcc.nY0));
  m_sNunchukAcc.fAccZ = (static_cast<float>(nRawZ) - static_cast<float>(m_sNunchukAcc.nZ0)) / (static_cast<float>(m_sNunchukAcc.nZG) - static_cast<float>(m_sNunchukAcc.nZ0));

  // AccX
  if (ValueChanged(NunchukAccX.getValue(), m_sNunchukAcc.fAccX))
    NunchukAccX.setValue(m_sNunchukAcc.fAccX, false);

  // AccY
  if (ValueChanged(NunchukAccY.getValue(), m_sNunchukAcc.fAccY))
    NunchukAccY.setValue(m_sNunchukAcc.fAccY, false);

  // AccZ
  if (ValueChanged(NunchukAccZ.getValue(), m_sNunchukAcc.fAccZ))
    NunchukAccZ.setValue(m_sNunchukAcc.fAccZ, false);

  // Compute orientation
  m_sNunchukAcc.calculateOrientation();

  // OrientX
  if (ValueChanged(NunchukOrientX.getValue(), m_sNunchukAcc.fOrientX))
    NunchukOrientX.setValue(m_sNunchukAcc.fOrientX, false);

  // OrientY
  if (ValueChanged(NunchukOrientY.getValue(), m_sNunchukAcc.fOrientY))
    NunchukOrientY.setValue(m_sNunchukAcc.fOrientY, false);

  // OrientZ
  if (ValueChanged(NunchukOrientZ.getValue(), m_sNunchukAcc.fOrientZ))
    NunchukOrientZ.setValue(m_sNunchukAcc.fOrientZ, false);

  // Roll
  if (ValueChanged(NunchukRoll.getValue(), m_sNunchukAcc.fRoll))
    NunchukRoll.setValue(m_sNunchukAcc.fRoll, false);

  // Pitch
  if (ValueChanged(NunchukPitch.getValue(), m_sNunchukAcc.fPitch))
    NunchukPitch.setValue(m_sNunchukAcc.fPitch, false);

  // Get raw joystick position
  uint8_t nJoyRawX = m_pInputBuffer[nOffset+0];
  uint8_t nJoyRawY = m_pInputBuffer[nOffset+1];

  // Compute joystick position
  if (m_sNunchukJoy.nMaxX != 0x00) {
    m_sNunchukJoy.fX  = (static_cast<float>(nJoyRawX) - static_cast<float>(m_sNunchukJoy.nMidX)) / (static_cast<float>(m_sNunchukJoy.nMaxX) - static_cast<float>(m_sNunchukJoy.nMinX));
    m_sNunchukJoy.fX *= 2.0f;
  }

  if (m_sNunchukJoy.nMaxY != 0x00) {
    m_sNunchukJoy.fY = (static_cast<float>(nJoyRawY) - static_cast<float>(m_sNunchukJoy.nMidY)) / (static_cast<float>(m_sNunchukJoy.nMaxY) - static_cast<float>(m_sNunchukJoy.nMinY));
    m_sNunchukJoy.fY *= 2.0f;
  }

  // NunchukX
  if (ValueChanged(NunchukX.getValue(), m_sNunchukJoy.fX))
    NunchukX.setValue(m_sNunchukJoy.fX, false);

  // NunchukY
  if (ValueChanged(NunchukY.getValue(), m_sNunchukJoy.fY))
    NunchukY.setValue(m_sNunchukJoy.fY, false);
}

void WiiMote::onReadClassic(uint32_t)
{
  // TODO(naetherm)
  /*
  // buttons:
  uint16_t bits = *(uint16_t*)(m_pInputBuffer+4);

  // joysticks:
  wiimote_state::joystick &joyL = Internal.ClassicController.JoystickL;
  wiimote_state::joystick &joyR = Internal.ClassicController.JoystickR;

  // copy the current joystick state to detect changes
  wiimote_state::joystick curr_joy_l = joyL;
  wiimote_state::joystick curr_joy_r = joyR;

  joyL.RawX = (float) (m_pInputBuffer[nOffset+0] & 0x3f);
  joyL.RawY = (float) (m_pInputBuffer[nOffset+1] & 0x3f);
  joyR.RawX = (float)((m_pInputBuffer[nOffset+2]			>> 7) |
             ((m_pInputBuffer[nOffset+1] & 0xc0) >> 5) |
             ((m_pInputBuffer[nOffset+0] & 0xc0) >> 3));
  joyR.RawY = (float) (m_pInputBuffer[nOffset+2] & 0x1f);

  float xr, yr, xl, yl;
  if (Internal.ClassicController.CalibrationInfo.MaxXL != 0x00)
    xl = ((float)joyL.RawX - Internal.ClassicController.CalibrationInfo.MidXL) /
       ((float)Internal.ClassicController.CalibrationInfo.MaxXL -
           Internal.ClassicController.CalibrationInfo.MinXL);

  if (Internal.ClassicController.CalibrationInfo.MaxYL != 0x00)
    yl = ((float)joyL.RawY - Internal.ClassicController.CalibrationInfo.MidYL) /
       ((float)Internal.ClassicController.CalibrationInfo.MaxYL -
           Internal.ClassicController.CalibrationInfo.MinYL);

  if (Internal.ClassicController.CalibrationInfo.MaxXR != 0x00)
    xr = ((float)joyR.RawX - Internal.ClassicController.CalibrationInfo.MidXR) /
       ((float)Internal.ClassicController.CalibrationInfo.MaxXR -
           Internal.ClassicController.CalibrationInfo.MinXR);

  if (Internal.ClassicController.CalibrationInfo.MaxYR != 0x00)
    yr = ((float)joyR.RawY - Internal.ClassicController.CalibrationInfo.MidYR) /
       ((float)Internal.ClassicController.CalibrationInfo.MaxYR -
           Internal.ClassicController.CalibrationInfo.MinYR);

  // i prefer the joystick outputs to range -1 - +1 (note this also affects
  //  the deadzone calculations)
  xl *= 2; yl *= 2; xr *= 2; yr *= 2;

  // apply the dead zones (if any) and write the final values to the state
  ApplyJoystickDeadZonesAndWrite(joyL, xl, yl);
  ApplyJoystickDeadZonesAndWrite(joyR, xr, yr);

  // have the joystick states changed?
  if (memcmp(&curr_joy_l, &joyL, sizeof(curr_joy_l)) != 0)
    changed |= CLASSIC_JOYSTICK_L_CHANGED;
  if (memcmp(&curr_joy_r, &joyR, sizeof(curr_joy_r)) != 0)
    changed |= CLASSIC_JOYSTICK_R_CHANGED;

  // triggers
  uint8_t raw_trigger_l = ((m_pInputBuffer[nOffset+2] & 0x60) >> 2) |
              (m_pInputBuffer[nOffset+3]		  >> 5);
  uint8_t raw_trigger_r =   m_pInputBuffer[nOffset+3] & 0x1f;

  if ((raw_trigger_l != Internal.ClassicController.RawTriggerL) ||
    (raw_trigger_r != Internal.ClassicController.RawTriggerR))
       changed |= CLASSIC_TRIGGERS_CHANGED;

  Internal.ClassicController.RawTriggerL  = raw_trigger_l;
  Internal.ClassicController.RawTriggerR  = raw_trigger_r;

  if (Internal.ClassicController.CalibrationInfo.MaxTriggerL != 0x00)
    Internal.ClassicController.TriggerL =
         (float)Internal.ClassicController.RawTriggerL /
        ((float)Internal.ClassicController.CalibrationInfo.MaxTriggerL -
            Internal.ClassicController.CalibrationInfo.MinTriggerL);

  if (Internal.ClassicController.CalibrationInfo.MaxTriggerR != 0x00)
    Internal.ClassicController.TriggerR =
         (float)Internal.ClassicController.RawTriggerR /
        ((float)Internal.ClassicController.CalibrationInfo.MaxTriggerR -
            Internal.ClassicController.CalibrationInfo.MinTriggerR);
  */
}

void WiiMote::readMemory(int nAddress, uint8_t nSize)
{
  // Send command
  clearReport();
  m_pOutputBuffer[0] = CmdReadMemory;
  m_pOutputBuffer[1] = static_cast<uint8_t>(((nAddress & 0xff000000) >> 24) | m_nRumble);
  m_pOutputBuffer[2] = static_cast<uint8_t>((nAddress	 & 0x00ff0000) >> 16);
  m_pOutputBuffer[3] = static_cast<uint8_t>((nAddress	 & 0x0000ff00) >>  8);
  m_pOutputBuffer[4] = static_cast<uint8_t>((nAddress	 & 0x000000ff));
  m_pOutputBuffer[5] = static_cast<uint8_t>((nSize	 & 0xff00	 ) >>  8);
  m_pOutputBuffer[6] = static_cast<uint8_t>((nSize	 & 0xff));
  send(m_pOutputBuffer, 7);
}

void WiiMote::writeMemory(int nAddress, const uint8_t* pBuffer, uint8_t nSize)
{
  // Check buffer
  if (pBuffer && nSize <= 16) {
    // Clear write buffer
    memset(m_pOutputBuffer, 0, m_pConnectionDevice->getOutputReportSize());

    // Set command
    clearReport();
    m_pOutputBuffer[0] = CmdWriteMemory;
    m_pOutputBuffer[1] = static_cast<uint8_t>(((nAddress & 0xff000000) >> 24) | m_nRumble);
    m_pOutputBuffer[2] = static_cast<uint8_t>( (nAddress & 0x00ff0000) >> 16);
    m_pOutputBuffer[3] = static_cast<uint8_t>( (nAddress & 0x0000ff00) >>  8);
    m_pOutputBuffer[4] = static_cast<uint8_t>( (nAddress & 0x000000ff));
    m_pOutputBuffer[5] = nSize;
    memcpy(m_pOutputBuffer+6, pBuffer, nSize);

    // Send command
    m_pConnectionDevice->write(m_pOutputBuffer, m_pConnectionDevice->getOutputReportSize());
  }
}

void WiiMote::decryptBuffer(uint32_t nOffset, uint32_t nSize)
{
  // Decrypt buffer
  for (unsigned i=0; i<nSize; i++)
    m_pInputBuffer[nOffset+i] = (((m_pInputBuffer[nOffset+i] ^ 0x17u) + 0x17u) & 0xffu);
}

void WiiMote::send(uint8_t *pBuffer, uint32_t nSize)
{
  // This is kinda strange. On Windows, using the HID API, a whole output report of size 22 has to be sent to the device,
  // otherwise strange things happen. On Linux, using the BlueZ API, only as many data as needed has to be sent, otherwise
  // we get a ResultErrInvalidParameter error.

  // Send data to device
  if (m_pConnectionDevice->getDeviceType() == ConnectionDevice::DeviceTypeBluetooth)
    m_pConnectionDevice->write(pBuffer, nSize);
  else
    m_pConnectionDevice->write(pBuffer, 22);
}

void WiiMote::sendStatusRequest()
{
  clearReport();
  m_pOutputBuffer[0] = CmdStatus;
  m_pOutputBuffer[1] = m_nRumble;
  send(m_pOutputBuffer, 2);
}

void WiiMote::sendCalibrationRequest()
{
  readMemory(RegCalibration, 7);
}

void WiiMote::SAcceleration::calculateOrientation()
{
  // Calculate orientation from acceleration data
  static constexpr float fEpsilon = 0.2f;
  float fSquareLen =  fAccX*fAccX + fAccY*fAccY + fAccZ*fAccZ;
  if ((fSquareLen >= (1.0f - fEpsilon)) && (fSquareLen <= (1.0f + fEpsilon))) {
    // Is the acceleration near 1G for at least 2 update cycles?
    if (++nUpdateNearG >= 2) {
      // Normalize vector
      float inv_len = 1.f / sqrtf(fSquareLen);
      fOrientX = fAccX * inv_len;
      fOrientY = fAccY * inv_len;
      fOrientZ = fAccZ * inv_len;

      // Get pitch and roll angles
      fPitch = -asinf(fOrientY) * 57.2957795f;
      fRoll  =  asinf(fOrientX) * 57.2957795f;
      if (fOrientZ < 0) {
        fPitch = (fOrientY < 0 ?  180 - fPitch : -180 - fPitch);
        fRoll  = (fOrientX < 0 ? -180 - fRoll  :  180 - fRoll);
      }
    }
  } else {
    // Reset update counter
    nUpdateNearG = 0;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
