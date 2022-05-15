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
#include "RECore/Math/Plane.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Vec2i
 *
 * @brief
 * Basic 2d vector implementation
 */
class Vec2i {
public:

  /**
   * Default constructor
   */
  Vec2i();

  /**
   * @brief
   * Copy constructor.
   *
   * @param[in] rhs
   * The object to copy.
   */
  Vec2i(const Vec2i& rhs);

  /**
   * @brief
   * Constructor
   *
   * @param[in] x
   * x coordinate
   * @param[in] y
   * y coordinate
   */
  Vec2i(int x, int y);

  /**
   * @brief
   * Destructor.
   */
  ~Vec2i() = default;


  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * The other object.
   *
   * @return
   * Reference to this instance.
   */
  inline Vec2i& operator=(const Vec2i& rhs);

  /**
   * @brief
   * Compare operator
   */
  inline bool operator==(const Vec2i& rhs) const;

  /**
   * @brief
   * Compare operator
   */
  inline bool operator!=(const Vec2i& rhs) const;


  /**
   * @brief
   * Returns x component.
   *
   * @return
   * X component.
   */
  [[nodiscard]] inline int getX() const;

  /**
   * @brief
   * Returns y component.
   *
   * @return
   * Y component.
   */
  [[nodiscard]] inline int getY() const;

  /**
   * @brief
   * Sets the x component
   *
   * @param[in] x
   * The x component.
   */
  inline void setX(int x);

  /**
   * @brief
   * Sets the y component
   *
   * @param[in] x
   * The y component.
   */
  inline void setY(int y);

private:

  int x;
  int y;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Math/Vec2i.inl"