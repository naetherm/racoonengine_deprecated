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
*    Axis control
*/
class Axis final : public Control
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
  inline Axis(Controller& controller, const std::string& name, const std::string& description) :
    Control(controller, ControlType::AXIS, name, description),
    mValue(0.0f),
    mRelativeValue(false)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] axis
  *    Other axis
  */
  inline explicit Axis(const Axis& axis) :
    Control(axis.mController, ControlType::AXIS, axis.mName, axis.mDescription),
    mValue(axis.mValue),
    mRelativeValue(axis.mRelativeValue)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  inline ~Axis()
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Comparison operator
  *
  *  @param[in] axis
  *    Axis to compare with
  *
  *  @return
  *    "true" if both axes are equal, else "false"
  */
  [[nodiscard]] inline bool operator ==(const Axis& axis) const
  {
    return (mValue == axis.mValue && mRelativeValue == axis.mRelativeValue);
  }

  /**
  *  @brief
  *    Copy operator
  *
  *  @param[in] axis
  *    Other axis
  *
  *  @return
  *    Reference to this axis
  */
  inline Axis& operator =(const Axis& axis)
  {
    // Copy value
    mValue		   = axis.mValue;
    mRelativeValue = axis.mRelativeValue;

    // Control has changed
    informUpdate();

    // Return reference to this axis
    return *this;
  }

  /**
  *  @brief
  *    Get axis value
  *
  *  @return
  *    Current value
  *
  *  @remarks
  *    Please note that a value can be absolute (for instance the x-axis of a joystick) or
  *    relative (for instance the x-axis of a mouse). While an absolute axis is usually timing
  *    independent, a relative axis just tells you about a state change since the last update.
  *    Therefore, we strongly recommend to always use "isRelativeValue()" to check for the value
  *    type in order to, for instance, multiply a absolute value with the current time difference
  *    since the last frame/update to get correctly timed movement.
  */
  [[nodiscard]] inline float getValue() const
  {
    return mValue;
  }

  /**
  *  @brief
  *    Set axis value
  *
  *  @param[in] value
  *    Current value
  *  @param[in] relativeValue
  *    "true" if the current value is relative, else "false" if it's a absolute value
  */
  inline void setValue(float value, bool relativeValue)
  {
    // Set value
    mValue		   = value;
    mRelativeValue = relativeValue;

    // Control has changed
    informUpdate();
  }

  /**
  *  @brief
  *    Return whether the current value is relative or absolute
  *
  *  @return
  *    "true" if the current value is relative, else "false" if it's a absolute value
  */
  [[nodiscard]] inline bool isRelativeValue() const
  {
    return mRelativeValue;
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  float mValue;			///< Value of the axis
  bool  mRelativeValue;	///< Is the current value a relative one?


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
