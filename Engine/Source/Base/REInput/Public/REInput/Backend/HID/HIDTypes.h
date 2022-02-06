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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
static constexpr int UsagePageGeneric	= 0x01;
static constexpr int UsageJoystick				= 0x04;
static constexpr int UsageGamepad				= 0x05;
static constexpr int UsageMultiAxisController	= 0x08;
static constexpr int UsageX						= 0x30;
static constexpr int UsageY						= 0x31;
static constexpr int UsageZ						= 0x32;
static constexpr int UsageRX					= 0x33;
static constexpr int UsageRY					= 0x34;
static constexpr int UsageRZ					= 0x35;
static constexpr int UsageSlider				= 0x36;
static constexpr int UsageDial					= 0x37;
static constexpr int UsageWheel					= 0x38;
static constexpr int UsageHat					= 0x39;


// Usage Page: Buttons
static constexpr int UsagePageButtons	= 0x09;
static constexpr int UsageNoButton				= 0x00;
static constexpr int UsageButton1				= 0x01;
static constexpr int UsageButton2				= 0x02;
static constexpr int UsageButton3				= 0x03;
static constexpr int UsageButton4				= 0x04;
static constexpr int UsageButton5				= 0x05;
static constexpr int UsageButton6				= 0x06;
static constexpr int UsageButton7				= 0x07;
static constexpr int UsageButton8				= 0x08;


// Usage Page: LED (used for rumble on some gamepads)
static constexpr int UsagePageLED		= 0x08;
static constexpr int UsageSlowBlinkOnTime		= 0x43;
static constexpr int UsageSlowBlinkOffTime		= 0x44;
static constexpr int UsageFastBlinkOnTime		= 0x45;
static constexpr int UsageFastBlinkOffTime		= 0x46;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
