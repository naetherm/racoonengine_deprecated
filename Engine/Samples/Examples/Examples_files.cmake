
set(FILES
  Private/Main.cpp
  Private/ExampleRunner.cpp
  #Private/Framework/ApplicationImplSdl2.cpp
  Private/Framework/ExampleBase.cpp
  Private/Framework/IApplication.cpp
  Private/Framework/IApplicationRenderer.cpp
  Private/Framework/IApplicationRhi.cpp

  Private/Basics/Texture/Texture.cpp
  Private/Basics/GeometryShader/GeometryShader.cpp
  Private/Basics/ComputeShader/ComputeShader.cpp
  Private/Basics/CubeTexture/CubeTexture.cpp
  Private/Basics/IndirectBuffer/IndirectBuffer.cpp
  Private/Basics/Instancing/Instancing.cpp
  Private/Basics/MeshShader/MeshShader.cpp
  Private/Basics/MultipleRenderTargets/MultipleRenderTargets.cpp
  Private/Basics/MultipleSwapChains/MultipleSwapChains.cpp
  Private/Basics/Queries/Queries.cpp
  Private/Basics/RenderToTexture/RenderToTexture.cpp
  Private/Basics/EnhancedRenderToTexture/RenderToTexture.cpp
  Private/Basics/TessellationShader/TessellationShader.cpp
  Private/Basics/Triangle/Triangle.cpp
  Private/Basics/VertexBuffer/VertexBuffer.cpp

  Private/Advanced/Gpgpu/Gpgpu.cpp
  Private/Advanced/IcosahedronTessellation/IcosahedronTessellation.cpp
  Private/Advanced/InstancedCubes/InstancedCubes.cpp
  Private/Advanced/InstancedCubes/CubeRendererDrawInstanced/BatchDrawInstanced.cpp
  Private/Advanced/InstancedCubes/CubeRendererDrawInstanced/CubeRendererDrawInstanced.cpp
  Private/Advanced/InstancedCubes/CubeRendererInstancedArrays/BatchInstancedArrays.cpp
  Private/Advanced/InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays.cpp

  Private/Renderer/Mesh/Mesh.cpp
  Private/Renderer/Scene/FreeCameraController.cpp
  Private/Renderer/Scene/Scene.cpp
  Private/Renderer/Scene/VrController.cpp
  Private/Renderer/Compositor/CompositorPassFactory.cpp
  Private/Renderer/Compositor/Compositor.cpp
  Private/Renderer/Compositor/CompositorInstancePass.cpp
  Private/Renderer/ImGuiExampleSelector/ImGuiExampleSelector.cpp
  Private/Renderer/Gui/Gui.cpp
  Private/Renderer/Gui/SceneView.cpp
  Private/Renderer/Gui/ImGuizmo.cpp
)