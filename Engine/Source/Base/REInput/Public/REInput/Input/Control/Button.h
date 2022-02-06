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
*    Button control
*/
class Button final : public Control
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
  *  @param[in] character
  *    Character associated with the button, "\0" if none
  */
  inline Button(Controller& controller, const std::string& name, const std::string& description, char character = '\0') :
    Control(controller, ControlType::BUTTON, name, description),
    mCharacter(character),
    mPressed(false),
    mHit(false)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] button
  *    Other button
  */
  inline explicit Button(const Button& button) :
    Control(button.mController, ControlType::BUTTON, button.mName, button.mDescription),
    mCharacter(button.mCharacter),
    mPressed(button.mPressed),
    mHit(button.mHit)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  inline ~Button()
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Comparison operator
  *
  *  @param[in] button
  *    Button to compare with
  *
  *  @return
  *    "true", if both buttons are equal, else "false"
  */
  [[nodiscard]] inline bool operator ==(const Button& button) const
  {
    return (mCharacter == button.mCharacter && mPressed == button.mPressed && mHit == button.mHit);
  }

  /**
  *  @brief
  *    Copy operator
  *
  *  @param[in] button
  *    Other button
  *
  *  @return
  *    Reference to this button
  */
  inline Button& operator =(const Button& button)
  {
    // Copy value
    mCharacter = button.mCharacter;
    mPressed   = button.mPressed;
    mHit	   = button.mHit;

    // Control has changed
    informUpdate();

    // Return reference to this button
    return *this;
  }

  /**
  *  @brief
  *    Get character
  *
  *  @return
  *    Character associated with the button, "\0" if none
  */
  [[nodiscard]] inline char getCharacter() const
  {
    return mCharacter;
  }

  /**
  *  @brief
  *    Get button status
  *
  *  @return
  *    "true" if the button is currently pressed, else "false"
  */
  [[nodiscard]] inline bool isPressed() const
  {
    return mPressed;
  }

  /**
  *  @brief
  *    Set button status
  *
  *  @param[in] pressed
  *    "true" if the button is pressed, else "false"
  */
  inline void setPressed(bool pressed)
  {
    // If the button was previously pressed but now isn't, we received a hit
    mHit = (mPressed && !pressed);

    // Set state
    mPressed = pressed;

    // Control has changed
    informUpdate();
  }

  /**
  *  @brief
  *    Check if the button has been hit without modifying the internal state
  *
  *  @return
  *    "true" if the button has been hit since the last call of this method, else 'false'
  *
  *  @note
  *    - This method will not reset the hit-state after being called (see "checkHitAndReset()")
  */
  [[nodiscard]] inline bool isHit() const
  {
    return mHit;
  }

  /**
  *  @brief
  *    Check if the button has been hit including modifying the internal state
  *
  *  @return
  *    "true" if the button has been hit since the last call of this method, else "false"
  *
  *  @remarks
  *    This will return the hit-state of the button and also reset it immediately (so the next call
  *    to "checkHitAndReset()" will return false). If you only want to check, but not reset the hit-state of
  *    a button, you should call "isHit()".
  */
  [[nodiscard]] inline bool checkHitAndReset()
  {
    const bool bHit = mHit;
    mHit = false;
    return bHit;
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // Button information
  char mCharacter;	///< Character associated with the button, "\0" if none

  // Button status
  bool mPressed;	///< Is the button currently pressed?
  bool mHit;		///< Has the button been hit in the meantime?


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
