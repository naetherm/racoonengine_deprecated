/*********************************************************\
 * Copyright (c) 2012-2022 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/RECore.h"

#include <ctime>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Stopwatch
 *
 * @brief
 * Stopwatch
 *
 * @verbatim
 * Usage example:
 *
 *   // Start the stopwatch
 *   Stopwatch stopwatch(true);
 *
 *   // Do some stuff
 *
 *   // Measure elapsed time
 *   float elapsedSeconds = stopwatch.getSeconds();
 * @endverbatim
 */
class Stopwatch final {


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Default constructor
   */
  Stopwatch();

  /**
   * @brief
   * Constructor
   *
   * @param[in] startAtOnce
   * If this parameter is "true", the stopwatch is started automatically at once
   */
  explicit Stopwatch(bool startAtOnce);

  /**
   * @brief
   * Destructor
   */
  inline ~Stopwatch() = default;

  /**
   * @brief
   * Start the stopwatch
   *
   * @note
   * - If the stopwatch is already running it's restarted
   */
  void start();

  /**
   * @brief
   * Stop the stopwatch
   *
   * @return
   * The elapsed time in microseconds since "start()"
   *
   * @note
   * - Often it's adequate to just request the past time using
   *   e.g. "getMilliseconds()" and not explicitly stopping the stopwatch
   */
  std::time_t stop();

  /**
   * @brief
   * Return the number of weeks since the stopwatch was started
   *
   * @return
   * Number of weeks elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float getWeeks() const {
    return getDays() / 7.0f;
  }

  /**
   * @brief
   * Return the number of days since the stopwatch was started
   *
   * @return
   * Number of days elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float getDays() const {
    return getHours() / 24.0f;
  }

  /**
   * @brief
   * Return the number of hours since the stopwatch was started
   *
   * @return
   * Number of hours elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float getHours() const {
    return getMinutes() / 60.0f;
  }

  /**
   * @brief
   * Return the number of minutes since the stopwatch was started
   *
   * @return
   * Number of minutes elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float getMinutes() const {
    return getSeconds() / 60.0f;
  }

  /**
   * @brief
   * Return the number of seconds since the stopwatch was started
   *
   * @return
   * Number of seconds elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float getSeconds() const {
    return getMilliseconds() / 1000.0f;
  }

  /**
   * @brief
   * Return the number of milliseconds since the stopwatch was started
   *
   * @return
   * Number of milliseconds elapsed since the stopwatch was started
   */
  [[nodiscard]] inline float getMilliseconds() const {
    return static_cast<float>(getMicroseconds()) / 1000.0f;
  }

  /**
   * @brief
   * Retrieve the number of microseconds since the stopwatch was started
   *
   * @return
   * Number of microseconds elapsed since the stopwatch was started
   */
  [[nodiscard]] inline std::time_t getMicroseconds() const {
    return mRunning ? (getSystemMicroseconds() - mStart) : (mStop - mStart);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:

  /**
   * @brief
   * Retrieve the number of microseconds since the system was started
   *
   * @return
   * Number of microseconds elapsed since the system was started
   */
  [[nodiscard]] std::time_t getSystemMicroseconds() const;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  bool mRunning;  ///< Is the stopwatch currently running?
  std::time_t mStart;    ///< Stopwatch start time (microseconds)
  std::time_t mStop;    ///< Stopwatch stop time (microseconds)
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
