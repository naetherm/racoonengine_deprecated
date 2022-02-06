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
*    Effect control
*
*  @note
*    - Effects are output controls, such as rumble, force-feedback effects etc.
*/
class Effect final : public Control
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
  inline Effect(Controller& controller, const std::string& name, const std::string& description) :
    Control(controller, ControlType::EFFECT, name, description),
    mValue(0.0f)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Copy constructor
  *
  *  @param[in] effect
  *    Other effect
  */
  inline explicit Effect(const Effect& effect) :
    Control(effect.mController, ControlType::EFFECT, effect.mName, effect.mDescription),
    mValue(effect.mValue)
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Destructor
  */
  inline ~Effect()
  {
    // Nothing here
  }

  /**
  *  @brief
  *    Comparison operator
  *
  *  @param[in] effect
  *    Effect to compare with
  *
  *  @return
  *    "true" if both effects are equal, else "false"
  */
  [[nodiscard]] inline bool operator ==(const Effect& effect) const
  {
    return (mValue == effect.mValue);
  }

  /**
  *  @brief
  *    Copy operator
  *
  *  @param[in] effect
  *    Other effect
  *
  *  @return
  *    Reference to this effect
  */
  inline Effect& operator =(const Effect& effect)
  {
    // Copy value
    mValue = effect.mValue;

    // Control has changed
    informUpdate();

    // Return reference to this effect
    return *this;
  }

  /**
  *  @brief
  *    Get effect value
  *
  *  @return
  *    Current value; usually, an effect value should be in the range of 0..1 (but it's up to the actual device definition)
  */
  [[nodiscard]] inline float getValue() const
  {
    return mValue;
  }

  /**
  *  @brief
  *    Set effect value
  *
  *  @param[in] value
  *    Current value; usually, an effect value should be in the range of 0..1 (but it's up to the actual device definition)
  */
  inline void setValue(float value)
  {
    // Set value
    mValue = value;

    // Control has changed
    informUpdate();
  }


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  float mValue;	///< Value of the effect


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // REInput
