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
#include "RERHI/RERHI.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI
{


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Shader bytecode (aka shader microcode, binary large object (BLOB))
*/
class ShaderBytecode final
{

  // Public methods
public:
  inline ShaderBytecode() :
    mNumberOfBytes(0),
    mBytecode(nullptr)
  {}

  inline ~ShaderBytecode()
  {
    delete [] mBytecode;
  }

  [[nodiscard]] inline RECore::uint32 getNumberOfBytes() const
  {
    return mNumberOfBytes;
  }

  [[nodiscard]] inline const RECore::uint8* getBytecode() const
  {
    return mBytecode;
  }

  inline void setBytecodeCopy(RECore::uint32 numberOfBytes, RECore::uint8* bytecode)
  {
    delete [] mBytecode;
    mNumberOfBytes = numberOfBytes;
    mBytecode = new RECore::uint8[mNumberOfBytes];
    memcpy(mBytecode, bytecode, mNumberOfBytes);
  }

  // Private data
private:
  RECore::uint32 mNumberOfBytes;	///< Number of bytes in the bytecode
  RECore::uint8* mBytecode;			///< Shader bytecode, can be a null pointer

};

/**
*  @brief
*    Shader source code
*/
struct ShaderSourceCode final
{
  // Public data
  const char* sourceCode	= nullptr;	///< Shader ASCII source code, if null pointer or empty string a null pointer will be returned
  const char* profile		= nullptr;	///< Shader ASCII profile to use, if null pointer or empty string, a default profile will be used which usually tries to use the best available profile that runs on most hardware (Examples: "glslf", "arbfp1", "ps_3_0")
  const char* arguments	= nullptr;	///< Optional shader compiler ASCII arguments, can be a null pointer or empty string
  const char* entry		= nullptr;	///< ASCII entry point, if null pointer or empty string, "main" is used as default

  // Public methods
  inline ShaderSourceCode(const char* _sourceCode) :
    sourceCode(_sourceCode)
  {}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
