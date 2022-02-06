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


class Controller;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Input control base class
*
*  @remarks
*    A control is part of an input controller, e.g. a button or an axis.
*/
class Control
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
  *  @param[in] controlType
  *    Control type
  *  @param[in] name
  *    UTF-8 control name
  *  @param[in] description
  *    UTF-8 control description
  */
  REINPUT_API Control(Controller& controller, ControlType controlType, const std::string& name, const std::string& description);

  /**
  *  @brief
  *    Destructor
  */
  REINPUT_API ~Control();

  /**
  *  @brief
  *    Get controller
  *
  *  @return
  *    Reference to controller that owns the control
  */
  [[nodiscard]] inline Controller& getController() const
  {
    return mController;
  }

  /**
  *  @brief
  *    Get control type
  *
  *  @return
  *    Type of control
  */
  [[nodiscard]] inline ControlType getControlType() const
  {
    return mControlType;
  }

  /**
  *  @brief
  *    Check if control is input or output control
  *
  *  @return
  *    "true" if control is input control, "false" if output
  */
  [[nodiscard]] inline bool isInputControl() const
  {
    // Input controls are:  Axis and button
    // Output controls are: Effect and LED
    return (ControlType::AXIS == mControlType || ControlType::BUTTON == mControlType);
  }

  /**
  *  @brief
  *    Get control name
  *
  *  @return
  *    UTF-8 control name
  */
  [[nodiscard]] inline const std::string& getName() const
  {
    return mName;
  }

  /**
  *  @brief
  *    Get control description
  *
  *  @return
  *    UTF-8 control description
  */
  [[nodiscard]] inline const std::string& getDescription() const
  {
    return mDescription;
  }


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Inform input manager that control has been changed
  */
  REINPUT_API void informUpdate();

  Control& operator =(const Control& control) = delete;


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  Controller& mController;	///< Owning controller
  ControlType mControlType;	///< Control type
  std::string mName;			///< Control name
  std::string mDescription;	///< Control description


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
