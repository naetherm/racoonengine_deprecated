

set(FILES
  Private/RHIRootSignature.cpp
  Private/RHIResourceGroup.cpp
  Private/Extensions.cpp
  Private/Helper.cpp
  Private/Mapping.cpp
  Private/OpenGLRuntimeLinking.cpp
  Private/RHIDynamicRHI.cpp
  Private/RHIOpenGLContext.cpp
  # Buffer
  Private/Buffer/RHIVertexBuffer.cpp
  Private/Buffer/RHIVertexArray.cpp
  Private/Buffer/RHIUniformBuffer.cpp
  Private/Buffer/RHITextureBuffer.cpp
  Private/Buffer/RHIStructuredBuffer.cpp
  Private/Buffer/RHIIndirectBuffer.cpp
  Private/Buffer/RHIIndexBuffer.cpp
  Private/Buffer/RHIBufferManager.cpp
  Private/Buffer/RHIIndexBufferBind.cpp
  Private/Buffer/RHIIndexBufferDsa.cpp
  Private/Buffer/RHIIndirectBufferBind.cpp
  Private/Buffer/RHIIndirectBufferDsa.cpp
  Private/Buffer/RHIStructuredBufferBind.cpp
  Private/Buffer/RHIStructuredBufferDsa.cpp
  Private/Buffer/RHITextureBufferBind.cpp
  Private/Buffer/RHITextureBufferDsa.cpp
  Private/Buffer/RHIUniformBufferBind.cpp
  Private/Buffer/RHIUniformBufferDsa.cpp
  Private/Buffer/RHIVertexArrayNoVao.cpp
  Private/Buffer/RHIVertexArrayVao.cpp
  Private/Buffer/RHIVertexArrayVaoBind.cpp
  Private/Buffer/RHIVertexArrayVaoDsa.cpp
  Private/Buffer/RHIVertexBufferBind.cpp
  Private/Buffer/RHIVertexBufferDsa.cpp
  # Query
  Private/Query/RHIQueryPool.cpp
  Private/Query/PipelineStatisticsQueryPool.cpp
  Private/Query/RHIOcclusionTimestampQueryPool.cpp
  # RenderTarget
  Private/RenderTarget/RHISwapChain.cpp
  Private/RenderTarget/RHIRenderPass.cpp
  Private/RenderTarget/RHIFramebuffer.cpp
  Private/RenderTarget/RHIFramebufferBind.cpp
  Private/RenderTarget/RHIFramebufferDsa.cpp
  # Shader
  # Shader/Monolithic
  Private/Shader/Monolithic/RHIComputeShaderMonolithic.cpp
  Private/Shader/Monolithic/RHIFragmentShaderMonolithic.cpp
  Private/Shader/Monolithic/RHIGeometryShaderMonolithic.cpp
  Private/Shader/Monolithic/RHIGraphicsProgramMonolithic.cpp
  Private/Shader/Monolithic/RHIGraphicsProgramMonolithicDsa.cpp
  Private/Shader/Monolithic/RHIMeshShaderMonolithic.cpp
  Private/Shader/Monolithic/RHIShaderLanguageMonolithic.cpp
  Private/Shader/Monolithic/RHITaskShaderMonolithic.cpp
  Private/Shader/Monolithic/RHITessellationControlShaderMonolithic.cpp
  Private/Shader/Monolithic/RHITessellationEvaluationShaderMonolithic.cpp
  Private/Shader/Monolithic/RHIVertexShaderMonolithic.cpp
  # Shader/Separate
  Private/Shader/Separate/RHIComputeShaderSeparate.cpp
  Private/Shader/Separate/RHIFragmentShaderSeparate.cpp
  Private/Shader/Separate/RHIGeometryShaderSeparate.cpp
  Private/Shader/Separate/RHIGraphicsProgramSeparate.cpp
  Private/Shader/Separate/RHIGraphicsProgramSeparateDsa.cpp
  Private/Shader/Separate/RHIMeshShaderSeparate.cpp
  Private/Shader/Separate/RHIShaderLanguageSeparate.cpp
  Private/Shader/Separate/RHITaskShaderSeparate.cpp
  Private/Shader/Separate/RHITessellationControlShaderSeparate.cpp
  Private/Shader/Separate/RHITessellationEvaluationShaderSeparate.cpp
  Private/Shader/Separate/RHIVertexShaderSeparate.cpp
  # State
  Private/State/RHISamplerState.cpp
  Private/State/RHIGraphicsPipelineState.cpp
  Private/State/RHIComputePipelineState.cpp
  Private/State/RHIBlendState.cpp
  Private/State/RHIComputePipelineStateMonolithic.cpp
  Private/State/RHIComputePipelineStateSeparate.cpp
  Private/State/RHIDepthStencilState.cpp
  Private/State/RHIOpenGLSamplerState.cpp
  Private/State/RHIRasterizerState.cpp
  Private/State/RHISamplerStateBind.cpp
  Private/State/RHISamplerStateDsa.cpp
  Private/State/RHISamplerStateSo.cpp
  # Texture
  Private/Texture/RHITextureCubeArray.cpp
  Private/Texture/RHITextureManager.cpp
  Private/Texture/RHITextureCube.cpp
  Private/Texture/RHITextureCubeArray.cpp
  Private/Texture/RHITexture3D.cpp
  Private/Texture/RHITexture2DArray.cpp
  Private/Texture/RHITexture2D.cpp
  Private/Texture/RHITexture1DArray.cpp
  Private/Texture/RHITexture1D.cpp
  Private/Texture/RHITexture1DArrayBind.cpp
  Private/Texture/RHITexture1DArrayDsa.cpp
  Private/Texture/RHITexture1DBind.cpp
  Private/Texture/RHITexture1DDsa.cpp
  Private/Texture/RHITexture2DArrayBind.cpp
  Private/Texture/RHITexture2DArrayDsa.cpp
  Private/Texture/RHITexture2DBind.cpp
  Private/Texture/RHITexture2DDsa.cpp
  Private/Texture/RHITexture3DBind.cpp
  Private/Texture/RHITexture3DDsa.cpp
  Private/Texture/RHITextureCubeArrayBind.cpp
  Private/Texture/RHITextureCubeArrayDsa.cpp
  Private/Texture/RHITextureCubeBind.cpp
  Private/Texture/RHITextureCubeDsa.cpp
)