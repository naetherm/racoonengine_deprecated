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
*    GLSL fragment shader ("pixel shader" in Direct3D terminology) class
*/
class FragmentShaderGlsl final : public RERHI::RHIFragmentShader
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor for creating a fragment shader from shader source code
  *
  *  @param[in] openGLES3Rhi
  *    Owner OpenGL ES 3 RHI instance
  *  @param[in] sourceCode
  *    Shader ASCII source code, must be valid
  */
  FragmentShaderGlsl(RHIDynamicRHI& openGLES3Rhi, const char* sourceCode RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~FragmentShaderGlsl() override;

  /**
  *  @brief
  *    Return the OpenGL ES 3 shader
  *
  *  @return
  *    The OpenGL ES 3 shader, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline RECore::uint32 getOpenGLES3Shader() const
  {
    return mOpenGLES3Shader;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIShader methods                   ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual const char* getShaderLanguageName() const override;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), FragmentShaderGlsl, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit FragmentShaderGlsl(const FragmentShaderGlsl& source) = delete;
  FragmentShaderGlsl& operator =(const FragmentShaderGlsl& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RECore::uint32 mOpenGLES3Shader;	///< OpenGL ES 3 shader, can be zero if no resource is allocated


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
