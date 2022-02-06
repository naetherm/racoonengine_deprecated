

set(FILES
  Private/RERHINull.cpp
  Private/RHIDynamicRHI.cpp
  Private/RHIResourceGroup.cpp
  Private/RHIRootSignature.cpp
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
  Private/Shader/RHIComputeShader.cpp
  Private/Shader/RHIFragmentShader.cpp
  Private/Shader/RHIGeometryShader.cpp
  Private/Shader/RHIGraphicsProgram.cpp
  Private/Shader/RHIMeshShader.cpp
  Private/Shader/RHIShaderLanguage.cpp
  Private/Shader/RHITaskShader.cpp
  Private/Shader/RHITessellationControlShader.cpp
  Private/Shader/RHITessellationEvaluationShader.cpp
  Private/Shader/RHIVertexShader.cpp
  # State
  Private/State/RHIComputePipelineState.cpp
  Private/State/RHIGraphicsPipelineState.cpp
  Private/State/RHISamplerState.cpp
  # Texture
  Private/Texture/RHITexture1D.cpp
  Private/Texture/RHITexture1DArray.cpp
  Private/Texture/RHITexture2D.cpp
  Private/Texture/RHITexture2DArray.cpp
  Private/Texture/RHITexture3D.cpp
  Private/Texture/RHITextureCube.cpp
  Private/Texture/RHITextureCubeArray.cpp
  Private/Texture/RHITextureManager.cpp
)