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
#include "REInput/Input/Control/Control.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

/**
*  @brief
*    LED control
*
*  @note
*    - An LED control can manage up to 32 LEDs
*/
class LED final : public Control
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] controller
  *    Owning controller
  *  @param[in] name
  *    UTF-8 control name
  *  @param[in] description
  *    UTF-8 control description
  */
  inline LED(Controller& controller, const std::string& name, const std::string& description) :
    Control(controller, ControlType::LED, name, description),
    mLedStates(0)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] led
  *    Other LED
  */
  inline explicit LED(const LED& led) :
    Control(led.mController, ControlType::LED, led.mName, led.mDescription),
    mLedStates(led.mLedStates)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  inline ~LED()
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Comparison operator
  *
  *  @param[in] led
  *    LED to compare with
  *
  *  @return
  *    "true" if both LEDs are equal, else "false"
  */
  [[nodiscard]] inline bool operator ==(const LED& led) const
  {
    return (mLedStates == led.mLedStates);
  }

  /**
  *  @brief
  *    Copy operator
  *
  *  @param[in] led
  *    Other LED
  *
  *  @return
  *    Reference to this LED
  */
  inline LED& operator =(const LED& led)
  {
    // Copy value
    mLedStates = led.mLedStates;

    // Control has changed
    informUpdate();

    // Return reference to this LED
    return *this;
  }

  /**
  *  @brief
  *    Get state of all LEDs as a bit field
  *
  *  @return
  *    LED states as a bit field
  */
  [[nodiscard]] inline uint32_t getLedStates() const
  {
    return mLedStates;
  }

  /**
  *  @brief
  *    Set state of all LEDs as a bit field
  *
  *  @param[in] ledStates
  *    LED states
  */
  inline void setLedStates(uint32_t ledStates)
  {
    // Set state of LEDs
    mLedStates = ledStates;

    // Control has changed
    informUpdate();
  }

  /**
  *  @brief
  *    Get LED status
  *
  *  @param[in] ledIndex
  *    Index of LED (0..31)
  *
  *  @return
  *    "true" if the LED is currently on, else "false"
  */
  [[nodiscard]] inline bool isOn(int ledIndex) const
  {
    return (ledIndex >= 0 && ledIndex < 32) ? (((mLedStates >> ledIndex) & 1) != 0) : false;
  }

  /**
  *  @brief
  *    Set LED status
  *
  *  @param[in] ledIndex
  *    Index of LED (0..31)
  *  @param[in] on
  *    "true" if the LED is on, else "false"
  */
  inline void setOn(int ledIndex, bool on)
  {
    // Check index
    if (ledIndex >= 0 && ledIndex < 32)
    {
      // Set LED state
      const uint32_t mask = (static_cast<uint32_t>(1) << ledIndex);
      if (on)
      {
        mLedStates |= mask;
      }
      else
      {
        mLedStates &= mask;
      }

      // Control has changed
      informUpdate();
    }
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  uint32_t mLedStates;	///< State of all LEDs


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
