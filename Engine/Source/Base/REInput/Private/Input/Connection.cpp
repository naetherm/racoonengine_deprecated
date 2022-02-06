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
#include "REInput/Input/Connection.h"
#include "REInput/Input/Controller.h"
#include "REInput/Input/Control/Axis.h"
#include "REInput/Input/Control/Button.h"
#include "REInput/Input/Control/Effect.h"
#include "REInput/Input/Control/LED.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace REInput {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
Connection::Connection(Control& inputControl, Control& outputControl, float scale) :
  mInputControl(inputControl),
  mOutputControl(outputControl),
  mValid(false),
  mScale(scale)
{
  // Perform sanity checks
  // -> Check if both controls are valid and of the same control type
  // -> Check that the controls are either both input or both output controls
  // -> Check that the output control does not belong to a device
  if (&mInputControl != &mOutputControl && mInputControl.getControlType() == mOutputControl.getControlType() && mInputControl.isInputControl() == mOutputControl.isInputControl() && mOutputControl.getController().getControllerType() != ControllerType::DEVICE)
  {
    // Connection is valid (can be changed by derived classes that have additional tests)
    mValid = true;
  }
  else
  {
    assert(false && "Invalid input connection");
  }
}

void Connection::passValue()
{
  // Check if connection is valid
  if (mValid)
  {
    // Pass value
    switch (mInputControl.getControlType())
    {
      case ControlType::BUTTON:
        static_cast<Button&>(mOutputControl).setPressed(static_cast<Button&>(mInputControl).isPressed());
        break;

      case ControlType::AXIS:
      {
        const Axis& inputAxisControl = static_cast<Axis&>(mInputControl);
        static_cast<Axis&>(mOutputControl).setValue(inputAxisControl.getValue() * mScale, inputAxisControl.isRelativeValue());
        break;
      }

      case ControlType::UNKNOWN:
      case ControlType::LED:
      case ControlType::EFFECT:
      default:
        // There's nothing to pass on
        break;
    }
  }
}

void Connection::passValueBackwards()
{
  // Check if connection is valid
  if (mValid)
  {
    // Pass value
    switch (mOutputControl.getControlType())
    {
      case ControlType::LED:
        static_cast<LED&>(mInputControl).setLedStates(static_cast<LED&>(mOutputControl).getLedStates());
        break;

      case ControlType::EFFECT:
        static_cast<Effect&>(mInputControl).setValue(static_cast<Effect&>(mOutputControl).getValue());
        break;

      case ControlType::UNKNOWN:
      case ControlType::BUTTON:
      case ControlType::AXIS:
      default:
        // There's nothing to pass backwards
        break;
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
