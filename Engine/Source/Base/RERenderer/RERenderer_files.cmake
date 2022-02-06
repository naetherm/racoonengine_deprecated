
set(FILES
  Private/Context.cpp
  Private/RendererImpl.cpp

  # Application
  Private/Application/RendererApplication.cpp

  # Core
  Private/Core/Platform/PlatformManager.cpp
  Private/Core/Renderer/RenderTargetTextureSignature.cpp
  Private/Core/Renderer/FramebufferSignature.cpp
  Private/Core/Renderer/FramebufferManager.cpp
  Private/Core/Renderer/RenderPassManager.cpp
  Private/Core/Renderer/RenderTargetTextureManager.cpp

  # DebugGui
  Private/DebugGui/DebugGuiManager.cpp
  Private/DebugGui/DebugGuiHelper.cpp

  # RenderQueue
  Private/RenderQueue/Renderable.cpp
  Private/RenderQueue/RenderableManager.cpp
  Private/RenderQueue/RenderQueue.cpp

  # Resource
  Private/Resource/RendererResourceManager.cpp
  Private/Resource/CompositorNode/CompositorNodeInstance.cpp
  Private/Resource/CompositorNode/CompositorNodeResource.cpp
  Private/Resource/CompositorNode/CompositorNodeResourceManager.cpp
  Private/Resource/CompositorNode/CompositorTarget.cpp
  Private/Resource/CompositorNode/Loader/CompositorNodeResourceLoader.cpp
  Private/Resource/CompositorNode/Pass/CompositorPassFactory.cpp
  Private/Resource/CompositorNode/Pass/DebugGui/CompositorInstancePassDebugGui.cpp
  Private/Resource/CompositorNode/Pass/Scene/CompositorInstancePassScene.cpp
  Private/Resource/CompositorNode/Pass/Scene/CompositorResourcePassScene.cpp
  Private/Resource/CompositorNode/Pass/VrHiddenAreaMesh/CompositorResourcePassVrHiddenAreaMesh.cpp
  Private/Resource/CompositorNode/Pass/VrHiddenAreaMesh/CompositorInstancePassVrHiddenAreaMesh.cpp
  Private/Resource/CompositorNode/Pass/Clear/CompositorInstancePassClear.cpp
  Private/Resource/CompositorNode/Pass/Clear/CompositorResourcePassClear.cpp
  Private/Resource/CompositorNode/Pass/Compute/CompositorInstancePassCompute.cpp
  Private/Resource/CompositorNode/Pass/Compute/CompositorResourcePassCompute.cpp
  Private/Resource/CompositorNode/Pass/Copy/CompositorInstancePassCopy.cpp
  Private/Resource/CompositorNode/Pass/Copy/CompositorResourcePassCopy.cpp
  Private/Resource/CompositorNode/Pass/GenerateMipmaps/CompositorInstancePassGenerateMipmaps.cpp
  Private/Resource/CompositorNode/Pass/GenerateMipmaps/CompositorResourcePassGenerateMipmaps.cpp
  Private/Resource/CompositorNode/Pass/ResolveMultisample/CompositorInstancePassResolveMultisample.cpp
  Private/Resource/CompositorNode/Pass/ResolveMultisample/CompositorResourcePassResolveMultisample.cpp
  Private/Resource/CompositorNode/Pass/ShadowMap/CompositorInstancePassShadowMap.cpp
  Private/Resource/CompositorNode/Pass/ShadowMap/CompositorResourcePassShadowMap.cpp
  Private/Resource/CompositorWorkspace/CompositorContextData.cpp
  Private/Resource/CompositorWorkspace/CompositorWorkspaceInstance.cpp
  Private/Resource/CompositorWorkspace/CompositorWorkspaceResourceManager.cpp
  Private/Resource/CompositorWorkspace/Loader/CompositorWorkspaceResourceLoader.cpp
  Private/Resource/Material/MaterialProperties.cpp
  Private/Resource/Material/MaterialTechnique.cpp
  Private/Resource/Material/MaterialResourceManager.cpp
  Private/Resource/Material/MaterialResource.cpp
  Private/Resource/Material/MaterialPropertyValue.cpp
  Private/Resource/Material/Loader/MaterialResourceLoader.cpp
  Private/Resource/MaterialBlueprint/MaterialBlueprintResource.cpp
  Private/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.cpp
  Private/Resource/MaterialBlueprint/Loader/MaterialBlueprintResourceLoader.cpp
  Private/Resource/MaterialBlueprint/Listener/MaterialBlueprintResourceListener.cpp
  Private/Resource/MaterialBlueprint/Cache/GraphicsProgramCacheManager.cpp
  Private/Resource/MaterialBlueprint/Cache/GraphicsPipelineStateSignature.cpp
  Private/Resource/MaterialBlueprint/Cache/GraphicsPipelineStateCompiler.cpp
  Private/Resource/MaterialBlueprint/Cache/GraphicsPipelineStateCacheManager.cpp
  Private/Resource/MaterialBlueprint/Cache/ComputePipelineStateSignature.cpp
  Private/Resource/MaterialBlueprint/Cache/ComputePipelineStateCompiler.cpp
  Private/Resource/MaterialBlueprint/Cache/ComputePipelineStateCacheManager.cpp
  Private/Resource/MaterialBlueprint/BufferManager/UniformInstanceBufferManager.cpp
  Private/Resource/MaterialBlueprint/BufferManager/PassBufferManager.cpp
  Private/Resource/MaterialBlueprint/BufferManager/TextureInstanceBufferManager.cpp
  Private/Resource/MaterialBlueprint/BufferManager/MaterialBufferSlot.cpp
  Private/Resource/MaterialBlueprint/BufferManager/MaterialBufferManager.cpp
  Private/Resource/MaterialBlueprint/BufferManager/LightBufferManager.cpp
  Private/Resource/MaterialBlueprint/BufferManager/IndirectBufferManager.cpp
  Private/Resource/Mesh/MeshResource.cpp
  Private/Resource/Mesh/MeshResourceManager.cpp
  Private/Resource/Mesh/Loader/IMeshResourceLoader.cpp
  Private/Resource/Mesh/Loader/MeshResourceLoader.cpp
  Private/Resource/Scene/SceneNode.cpp
  Private/Resource/Scene/SceneResource.cpp
  Private/Resource/Scene/SceneResourceManager.cpp
  Private/Resource/Scene/Loader/SceneResourceLoader.cpp
  Private/Resource/Scene/Factory/SceneFactory.cpp
  Private/Resource/Scene/Culling/SceneCullingManager.cpp
  Private/Resource/Scene/Item/ISceneItem.cpp
  Private/Resource/Scene/Item/MaterialSceneItem.cpp
  Private/Resource/Scene/Item/Mesh/SkeletonMeshSceneItem.cpp
  Private/Resource/Scene/Item/Mesh/MeshSceneItem.cpp
  Private/Resource/Scene/Item/Camera/CameraSceneItem.cpp
  Private/Resource/Scene/Item/Debug/DebugDrawSceneItem.cpp
  Private/Resource/Scene/Item/Grass/GrassSceneItem.cpp
  Private/Resource/Scene/Item/Light/SunlightSceneItem.cpp
  Private/Resource/Scene/Item/Light/LightSceneItem.cpp
  Private/Resource/Scene/Item/Particles/ParticlesSceneItem.cpp
  Private/Resource/Scene/Item/Sky/HosekWilkieSky.cpp
  Private/Resource/Scene/Item/Sky/SkySceneItem.cpp
  Private/Resource/Scene/Item/Terrain/TerrainSceneItem.cpp
  Private/Resource/Scene/Item/Volume/VolumeSceneItem.cpp
  Private/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.cpp
  Private/Resource/ShaderBlueprint/Cache/ShaderProperties.cpp
  Private/Resource/ShaderBlueprint/Cache/ShaderBuilder.cpp
  Private/Resource/ShaderBlueprint/Cache/Preprocessor.cpp
  Private/Resource/ShaderBlueprint/Cache/ShaderCacheManager.cpp
  Private/Resource/ShaderBlueprint/Loader/ShaderBlueprintResourceLoader.cpp
  Private/Resource/ShaderPiece/ShaderPieceResourceManager.cpp
  Private/Resource/ShaderPiece/Loader/ShaderPieceResourceLoader.cpp
  Private/Resource/Skeleton/SkeletonResourceManager.cpp
  Private/Resource/Skeleton/SkeletonResource.cpp
  Private/Resource/Skeleton/Loader/SkeletonResourceLoader.cpp
  Private/Resource/SkeletonAnimation/SkeletonAnimationResourceManager.cpp
  Private/Resource/SkeletonAnimation/SkeletonAnimationEvaluator.cpp
  Private/Resource/SkeletonAnimation/SkeletonAnimationController.cpp
  Private/Resource/SkeletonAnimation/Loader/SkeletonAnimationResourceLoader.cpp
  Private/Resource/Texture/TextureResourceManager.cpp
  Private/Resource/Texture/Loader/Lz4DdsTextureResourceLoader.cpp
  Private/Resource/Texture/Loader/KtxTextureResourceLoader.cpp
  Private/Resource/Texture/Loader/ITextureResourceLoader.cpp
  Private/Resource/Texture/Loader/CrnTextureResourceLoader.cpp
  Private/Resource/Texture/Loader/CrnArrayTextureResourceLoader.cpp
  Private/Resource/VertexAttributes/VertexAttributesResourceManager.cpp
  Private/Resource/VertexAttributes/Loader/VertexAttributesResourceLoader.cpp

  # Vr
  Private/Vr/OpenVR/OpenVRRuntimeLinking.cpp
  Private/Vr/OpenVR/VrManagerOpenVR.cpp
  Private/Vr/OpenVR/Loader/OpenVRMeshResourceLoader.cpp
  Private/Vr/OpenVR/Loader/OpenVRTextureResourceLoader.cpp
  )