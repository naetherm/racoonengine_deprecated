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
#include <limits>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * MakeId
 *
 * @tparam TType
 *
 * @brief
 * A generic ID maker implementation.
 */
template<typename TType>
class MakeId {
//[-------------------------------------------------------]
//[ Public type definitions                               ]
//[-------------------------------------------------------]
public:

  typedef TType ValueType;

//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:

  /**
   * @brief
   * Constructor.
   *
   * @param [in]nMaxID
   * The maximum ID that can be reached through the ID maker.
   */
  MakeId(const ValueType nMaxID = std::numeric_limits<ValueType>::max());

  /**
   * @brief
   * Destructor.
   */
  ~MakeId();


  bool createID(ValueType& nId);

  bool createRangeID(ValueType& nId, const ValueType nCount);

  bool destroyID(const ValueType nId);

  bool destroyRangeID(const ValueType nId, const ValueType nCount);

  bool isID(const ValueType nId) const;

  ValueType getAvailableIDs() const;

  ValueType getLargestContinuousRange() const;

//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:

  void insertRange(const ValueType nIndex);

  void destroyRange(const ValueType nIndex);

//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:

  struct Range {
    ValueType mFirst;
    ValueType mLast;
  };

  Range* mpRanges;
  ValueType mnNumRanges;
  ValueType mnCapacity;
};

using MakeIdInt8 = MakeId<int8>;
using MakeIdInt16 = MakeId<int16>;
using MakeIdInt32 = MakeId<int32>;
using MakeIdInt64 = MakeId<int64>;
using MakeIdUInt8 = MakeId<uint8>;
using MakeIdUInt16 = MakeId<uint16>;
using MakeIdUInt32 = MakeId<uint32>;
using MakeIdUInt64 = MakeId<uint64>;

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // namespace RECore

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "RECore/Tools/MakeId.inl"