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
#include "RERHINull/RERHINull.h"
#include <RERHI/Texture/RHITexture1D.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null 1D texture class
*/
class Texture1D final : public RERHI::RHITexture1D
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] nullRhi
  *    Owner null RHI instance
  *  @param[in] width
  *    The width of the texture
  */
  Texture1D(RHIDynamicRHI& nullRhi, RECore::uint32 width RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~Texture1D() override;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  virtual void selfDestruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit Texture1D(const Texture1D& source) = delete;
  Texture1D& operator =(const Texture1D& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
