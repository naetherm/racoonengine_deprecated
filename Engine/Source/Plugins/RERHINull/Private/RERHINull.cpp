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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHINull/RERHINull.h"
#include "RERHINull/RHIResourceGroup.h"
#include "RERHINull/RHIRootSignature.h"
#include "RERHINull/RHIDynamicRHI.h"
#include "RERHINull/Buffer/RHIVertexBuffer.h"
#include "RERHINull/Buffer/RHIVertexArray.h"
#include "RERHINull/Buffer/RHIIndirectBuffer.h"
#include "RERHINull/Buffer/RHITextureBuffer.h"
#include "RERHINull/Buffer/RHIUniformBuffer.h"
#include "RERHINull/Buffer/RHIIndexBuffer.h"
#include "RERHINull/Buffer/RHIBufferManager.h"
#include "RERHINull/Buffer/RHIStructuredBuffer.h"
#include "RERHINull/Query/RHIQueryPool.h"
#include "RERHINull/RenderTarget/RHIRenderPass.h"
#include "RERHINull/RenderTarget/RHIFramebuffer.h"
#include "RERHINull/RenderTarget/RHISwapChain.h"
#include "RERHINull/Shader/RHIGraphicsProgram.h"
#include "RERHINull/Shader/RHIShaderLanguage.h"
#include "RERHINull/Shader/RHIGeometryShader.h"
#include "RERHINull/Shader/RHIVertexShader.h"
#include "RERHINull/Shader/RHITessellationEvaluationShader.h"
#include "RERHINull/Shader/RHITessellationControlShader.h"
#include "RERHINull/Shader/RHIFragmentShader.h"
#include "RERHINull/Shader/RHITaskShader.h"
#include "RERHINull/Shader/RHIMeshShader.h"
#include "RERHINull/Shader/RHIComputeShader.h"
#include "RERHINull/State/RHIComputePipelineState.h"
#include "RERHINull/State/RHIGraphicsPipelineState.h"
#include "RERHINull/State/RHISamplerState.h"
#include "RERHINull/Texture/RHITextureManager.h"
#include "RERHINull/Texture/RHITexture3D.h"
#include "RERHINull/Texture/RHITexture1D.h"
#include "RERHINull/Texture/RHITexture1DArray.h"
#include "RERHINull/Texture/RHITexture2D.h"
#include "RERHINull/Texture/RHITexture2DArray.h"
#include "RERHINull/Texture/RHITextureCube.h"
#include "RERHINull/Texture/RHITextureCubeArray.h"