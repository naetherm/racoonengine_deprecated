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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * Attribute
 *
 * @brief
 * An attribute is a wrapper around a specific value. This will be used for the gui system where we
 * can directly observe changes on data types.
 *
 * @tparam TType
 * The type of the attribute.
 */
template<typename TType>
class Value {
public:

  /**
   * @brief
   * Default constructor.
   */
  Value();

  /**
   * @brief
   * Constructor.
   *
   * @param[in] rhs
   * Value.
   */
  Value(const TType& rhs);

  /**
   * @brief
   * Constructor.
   *
   * @param[in] rhs
   * Value.
   */
  Value(TType&& rhs);

  /**
   * @brief
   * Constructor.
   *
   * @param[in] rhs
   * Value.
   */
  Value(const Value<TType>& rhs);

  /**
   * @brief
   * Destructor.
   */
  ~Value();


  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * Value.
   *
   * @return
   * Reference to this instance.
   */
  Value<TType>& operator=(const TType& rhs);

  /**
   * @brief
   * Copy operator.
   *
   * @param[in] rhs
   * Value.
   *
   * @return
   * Reference to this instance.
   */
  Value<TType>& operator=(const Value<TType>& rhs);

  /**
   * @brief
   * Compare operator.
   *
   * @param[in] rhs
   * Value to compare to.
   *
   * @return
   * True if equal, false otherwise.
   */
  bool operator==(const Value<TType>& rhs) const;

  /**
   * @brief
   * Compare operator.
   *
   * @param[in] rhs
   * Value to compare to.
   *
   * @return
   * True if not equal, false otherwise.
   */
  bool operator!=(const Value<TType>& rhs) const;


  /**
   * @brief
   * Returns whether the value was set.
   *
   * @return
   * True if set, false otherwise.
   */
  bool isSet() const;

  /**
   * @brief
   * Returns const value.
   *
   * @return
   * Const value.
   */
  const TType get() const;

  /**
   * @brief
   * Returns value.
   *
   * @return
   * Value.
   */
  TType get();

  /**
   * @brief
   * Sets value.
   *
   * @param[in] value
   * Value.
   */
  void set(TType value);

private:
  /** Value */
  TType mValue;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Core/Value.inl"
