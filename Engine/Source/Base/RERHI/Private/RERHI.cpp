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

#include "RERHI/Rhi.h"
#include "RERHI/RHIDynamicRHI.h"
#include "RERHI/RHIResourceGroup.h"
#include "RERHI/RHIRootSignature.h"
#include "RERHI/RHICapabilities.h"
#include "RERHI/RHIContext.h"
#include "RERHI/RHIResource.h"
#include "RERHI/RHIStatistics.h"
#include "RERHI/Buffer/RHICommandBuffer.h"
#include "RERHI/Buffer/RHIBuffer.h"
#include "RERHI/Buffer/RHIStructuredBuffer.h"
#include "RERHI/Buffer/RHIBufferManager.h"
#include "RERHI/Buffer/RHIIndexBuffer.h"
#include "RERHI/Buffer/RHIUniformBuffer.h"
#include "RERHI/Buffer/RHITextureBuffer.h"
#include "RERHI/Buffer/RHIIndirectBuffer.h"
#include "RERHI/Buffer/RHIVertexArray.h"
#include "RERHI/Buffer/RHIVertexBuffer.h"
#include "RERHI/RenderTarget/RHIRenderTarget.h"
#include "RERHI/RenderTarget/RHISwapChain.h"
#include "RERHI/RenderTarget/RHIFramebuffer.h"
#include "RERHI/RenderTarget/RHIRenderPass.h"
#include "RERHI/Query/RHIQueryPool.h"
#include "RERHI/Shader/RHIComputeShader.h"
#include "RERHI/Shader/RHIMeshShader.h"
#include "RERHI/Shader/RHITaskShader.h"
#include "RERHI/Shader/RHIFragmentShader.h"
#include "RERHI/Shader/RHITessellationControlShader.h"
#include "RERHI/Shader/RHITessellationEvaluationShader.h"
#include "RERHI/Shader/RHIVertexShader.h"
#include "RERHI/Shader/RHIGeometryShader.h"
#include "RERHI/Shader/RHIShaderLanguage.h"
#include "RERHI/Shader/RHIGraphicsProgram.h"
#include "RERHI/Shader/RHIShader.h"
#include "RERHI/State/RHIGraphicsPipelineState.h"
#include "RERHI/State/RHIComputePipelineState.h"
#include "RERHI/State/RHISamplerState.h"
#include "RERHI/State/RHIPipelineState.h"
#include "RERHI/State/RHIState.h"
#include "RERHI/Texture/RHITextureCubeArray.h"
#include "RERHI/Texture/RHITextureCube.h"
#include "RERHI/Texture/RHITexture2DArray.h"
#include "RERHI/Texture/RHITexture2D.h"
#include "RERHI/Texture/RHITexture1DArray.h"
#include "RERHI/Texture/RHITexture1D.h"
#include "RERHI/Texture/RHITexture3D.h"
#include "RERHI/Texture/RHITextureManager.h"
#include "RERHI/Texture/RHITexture.h"
