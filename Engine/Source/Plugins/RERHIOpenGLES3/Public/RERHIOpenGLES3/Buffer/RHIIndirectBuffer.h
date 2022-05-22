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
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


/**
*  @brief
*    OpenGL ES 3 indirect buffer object emulation class
*/
class IndirectBuffer final : public RERHI::RHIIndirectBuffer
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Default constructor
  *
  *  @param[in] openGLES3Rhi
  *    Owner OpenGL ES 3 RHI instance
  *  @param[in] numberOfBytes
  *    Number of bytes within the indirect buffer, must be valid
  *  @param[in] data
  *    Indirect buffer data, can be a null pointer (empty buffer)
  *  @param[in] indirectBufferFlags
  *    Indirect buffer flags, see "RERHI::IndirectBufferFlag"
  */
  IndirectBuffer(RHIDynamicRHI& openGLES3Rhi, RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RECore::uint32 indirectBufferFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~IndirectBuffer() override;

  /**
  *  @brief
  *    Return writable indirect buffer emulation data pointer
  *
  *  @return
  *    Writable indirect buffer emulation data pointer, can be a null pointer, don't destroy the returned instance
  */
  [[nodiscard]] inline RECore::uint8* getWritableEmulationData() const
  {
    return mData;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIIndirectBuffer methods           ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual const RECore::uint8* getEmulationData() const override
  {
    return mData;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), IndirectBuffer, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit IndirectBuffer(const IndirectBuffer& source) = delete;
  IndirectBuffer& operator =(const IndirectBuffer& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RECore::uint32 mNumberOfBytes;
  RECore::uint8* mData;				///< Indirect buffer data, can be a null pointer


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
