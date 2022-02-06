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


class Control;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]

/**
*  @brief
*    Connection between two controllers/controls
*/
class Connection final
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] inputControl
  *    Input control
  *  @param[in] outputControl
  *    Output control
  *  @param[in] scale
  *    Scale factor
  */
  REINPUT_API Connection(Control& inputControl, Control& outputControl, float scale = 1.0f);

  /**
  *  @brief
  *    Destructor
  */
  inline ~Connection()
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Get input control
  *
  *  @return
  *    Reference to control that is on the input side of the connection
  */
  [[nodiscard]] inline Control& getInputControl() const
  {
    return mInputControl;
  }

  /**
  *  @brief
  *    Get output control
  *
  *  @return
  *    Reference to control that is on the output side of the connection
  */
  [[nodiscard]] inline Control& getOutputControl() const
  {
    return mOutputControl;
  }

  /**
  *  @brief
  *    Check if connection is value
  *
  *  @return
  *    "true" if connection is valid, else "false"
  *
  *  @remarks
  *    A connection is invalid e.g. when you try to connect different types of controls
  *    without using the proper connection type (see derived classes for connection classes
  *    that can convert values into other types).
  *    It is also not valid to use a control of a device as an output, because devices can only be
  *    used as input, not as output of controls (a device is controlled by the physical device only).
  */
  [[nodiscard]] inline bool isValid() const
  {
    return mValid;
  }

  /**
  *  @brief
  *    Pass value from input to output
  */
  REINPUT_API void passValue();

  /**
  *  @brief
  *    Pass value backwards from output to input
  */
  REINPUT_API void passValueBackwards();

  Connection& operator =(const Connection& control) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  Control& mInputControl;		///< Input control
  Control& mOutputControl;	///< Output control
  bool	 mValid;			///< "true" if connection is valid, else "false"
  float	 mScale;			///< Scale factor


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
