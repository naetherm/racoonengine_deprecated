
set(FILES
  Private/Helper.cpp
  Private/Mapping.cpp
  Private/RERHIVulkan.cpp
  Private/RHIDynamicRHI.cpp
  Private/RHIResourceGroup.cpp
  Private/RHIRootSignature.cpp
  Private/VulkanContext.cpp
  Private/VulkanRuntimeLinking.cpp

  # Buffer
  Private/Buffer/RHIBufferManager.cpp
  Private/Buffer/RHIIndexBuffer.cpp
  Private/Buffer/RHIIndirectBuffer.cpp
  Private/Buffer/RHIStructuredBuffer.cpp
  Private/Buffer/RHITextureBuffer.cpp
  Private/Buffer/RHIUniformBuffer.cpp
  Private/Buffer/RHIVertexArray.cpp
  Private/Buffer/RHIVertexBuffer.cpp

  # Query
  Private/Query/RHIQueryPool.cpp

  # RenderTarget
  Private/RenderTarget/RHIFramebuffer.cpp
  Private/RenderTarget/RHIRenderPass.cpp
  Private/RenderTarget/RHISwapChain.cpp

  # Shader
  Private/Shader/RHIComputeShaderGLSL.cpp
  Private/Shader/RHIFragmentShaderGLSL.cpp
  Private/Shader/RHIGeometryShaderGLSL.cpp
  Private/Shader/RHIGraphicsProgramGLSL.cpp
  Private/Shader/RHIMeshShaderGLSL.cpp
  Private/Shader/RHIShaderLanguageGLSL.cpp
  Private/Shader/RHITaskShaderGLSL.cpp
  Private/Shader/RHITessellationControlShaderGLSL.cpp
  Private/Shader/RHITessellationEvaluationShaderGLSL.cpp
  Private/Shader/RHIVertexShaderGLSL.cpp

  # State
  Private/State/RHIComputePipelineState.cpp
  Private/State/RHIGraphicsPipelineState.cpp
  Private/State/RHISamplerState.cpp

  # Texture
  Private/Texture/RHITextureCubeArray.cpp
  Private/Texture/RHITexture1D.cpp
  Private/Texture/RHITexture1DArray.cpp
  Private/Texture/RHITexture2D.cpp
  Private/Texture/RHITexture2DArray.cpp
  Private/Texture/RHITexture3D.cpp
  Private/Texture/RHITextureCube.cpp
  Private/Texture/RHITextureManager.cpp
)