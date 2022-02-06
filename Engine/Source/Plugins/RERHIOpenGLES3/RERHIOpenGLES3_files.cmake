
set(FILES
  #Private/OpenGLES3Rhi.cpp
  Private/Extensions.cpp
  Private/ExtensionsRuntimeLinking.cpp
  Private/Mapping.cpp
  Private/OpenGLES3Context.cpp
  Private/OpenGLES3ContextRuntimeLinking.cpp
  Private/OpenGLES3RuntimeLinking.cpp
  Private/RHIDynamicRHI.cpp
  Private/RHIResourceGroup.cpp
  Private/RHIRootSignature.cpp

  # Buffer
  Private/Buffer/RHIVertexBuffer.cpp
  Private/Buffer/RHIVertexArray.cpp
  Private/Buffer/RHIUniformBuffer.cpp
  Private/Buffer/RHITextureBuffer.cpp
  Private/Buffer/RHIStructuredBuffer.cpp
  Private/Buffer/RHIIndexBuffer.cpp
  Private/Buffer/RERHIIndirectBuffer.cpp
  Private/Buffer/RHIVertexArray.cpp
  Private/Buffer/RHIVertexBuffer.cpp
  Private/Buffer/RHIBufferManager.cpp
  Private/Buffer/RHITextureBufferBind.cpp
  Private/Buffer/RHITextureBufferBindEmulation.cpp

  # RenderTarget
  Private/RenderTarget/RHISwapChain.cpp
  Private/RenderTarget/RHIRenderPass.cpp
  Private/RenderTarget/RHIFramebuffer.cpp

  # Shader
  Private/Shader/RHIVertexShaderGLSL.cpp
  Private/Shader/RHIShaderLanguageGLSL.cpp
  Private/Shader/RHIGraphicsProgramGLSL.cpp
  Private/Shader/RHIFragmentShaderGLSL.cpp

  # State
  Private/State/RHISamplerState.cpp
  Private/State/RHIGraphicsPipelineState.cpp
  Private/State/RHIRasterizerState.cpp
  Private/State/RHIDepthStencilState.cpp
  Private/State/RHIBlendState.cpp

  # Texture
  Private/Texture/RHITextureManager.cpp
  Private/Texture/RHITextureCube.cpp
  Private/Texture/RHITexture3D.cpp
  Private/Texture/RHITexture2DArray.cpp
  Private/Texture/RHITexture2D.cpp
  Private/Texture/RHITexture1DArray.cpp
  Private/Texture/RHITexture1D.cpp
  Private/Texture/RHITextureCubeArray.cpp
)