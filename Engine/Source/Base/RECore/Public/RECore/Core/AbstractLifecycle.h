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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * AbstractLifecycle
 *
 * @brief
 * Abstract life cycle class
 *
 * @remarks
 * @verbatim
 *    The "Life cycle" is:
 *      "onCreate()"         - Called directly after the object has been created
 *        ("onRestart()")    - Called directly before a stopped object is going to start again (always followed by "onStart()")
 *        "onStart()"        - Called when the object becoming visible to the user
 *          "onResume()"     - Called when the object has the focus (keep the implementation lightweight)
 *          "onPause()"      - Called when the object has no longer the focus (keep the implementation lightweight)
 *        "onStop()"         - Called when the object is no longer visible to the user
 *      "onDestroy()"        - Called before the object is going to be finally destroyed
 *    This life cycle has the same interface as the "Android Activity Life cycle"
 *    (http://developer.android.com/reference/android/app/Activity.html#ActivityLifecycle)
 *    because this interface looks just the right way to go for this purpose... also there
 *    are nice diagrams and documentations explaining the interface so it shouldn't be to
 *    hard to understand it.
 * @endverbatim
 */
class AbstractLifecycle {


  //[-------------------------------------------------------]
  //[ Public virtual AbstractLifecycle functions            ]
  //[-------------------------------------------------------]
public:
  /**
   * @brief
   * Called directly after the object has been created
   */
  virtual void onCreate() = 0;

  /**
   * @brief
   * Called directly before a stopped object is going to start again (always followed by "onStart()")
   */
  virtual void onRestart() = 0;

  /**
   * @brief
   * Called when the object becoming visible to the user
   *
   * @return
   * 'true' if all went fine, else 'false' (on failure, no "onResume()", "onPause()" or "onStop()" will be called)
   */
  [[nodiscard]] virtual bool onStart() = 0;

  /**
   * @brief
   * Called when the object has the focus (keep the implementation lightweight)
   */
  virtual void onResume() = 0;

  /**
   * @brief
   * Called when the object has no longer the focus (keep the implementation lightweight)
   */
  virtual void onPause() = 0;

  /**
   * @brief
   * Called when the object is no longer visible to the user
   */
  virtual void onStop() = 0;

  /**
   * @brief
   * Called before the object is going to be finally destroyed
   */
  virtual void onDestroy() = 0;


  //[-------------------------------------------------------]
  //[ Protected functions                                   ]
  //[-------------------------------------------------------]
protected:
  /**
   * @brief
   * Default constructor
   */
  AbstractLifecycle() = default;

  /**
   * @brief
   * Destructor
   */
  virtual ~AbstractLifecycle() = default;


  //[-------------------------------------------------------]
  //[ Private functions                                     ]
  //[-------------------------------------------------------]
private:
  /**
   * @brief
   * Copy constructor
   *
   * @param[in] cSource
   * Source to copy from
   */
  AbstractLifecycle(const AbstractLifecycle &cSource) = default;

  /**
   * @brief
   * Copy operator
   *
   * @param[in] cSource
   * Source to copy from
   *
   * @return
   * Reference to this instance
   */
  AbstractLifecycle &operator=(const AbstractLifecycle &cSource) = default;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
