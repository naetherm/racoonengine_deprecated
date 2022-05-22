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
#include "RECore/RECore.h"
#include "RECore/Core/Manager.h"
#include "RECore/Time/Stopwatch.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Time manager
*/
class TimeManager final : public RECore::Manager {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  TimeManager();

  explicit TimeManager(const TimeManager &) = delete;

  inline ~TimeManager() = default;

  TimeManager &operator=(const TimeManager &) = delete;


  [[nodiscard]] inline const Stopwatch &getSinceStartStopwatch() const {
    return mSinceStartStopwatch;
  }

  [[nodiscard]] inline float getPastSecondsSinceLastFrame() const {
    return mPastSecondsSinceLastFrame;
  }

  [[nodiscard]] inline float getGlobalTimeInSeconds() const {
    return mGlobalTimeInSeconds;
  }

  [[nodiscard]] inline RECore::uint64 getNumberOfRenderedFrames() const {
    return mNumberOfRenderedFrames;
  }

  [[nodiscard]] inline float getFramesPerSecond() const {
    return mFramesPerSecond;
  }

  /**
  *  @brief
  *    Time manager update
  *
  *  @note
  *    - Call this once per frame
  */
  void update();


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  Stopwatch mSinceStartStopwatch;
  Stopwatch mPerUpdateStopwatch;
  float mPastSecondsSinceLastFrame;
  float mGlobalTimeInSeconds;
  RECore::uint64 mNumberOfRenderedFrames;
  float mFramesPerSecond;
  float mFramerateSecondsPerFrame[120];  // Calculate estimate of framerate over the last two seconds
  int mFramerateSecondsPerFrameIndex;
  float mFramerateSecondsPerFrameAccumulated;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
