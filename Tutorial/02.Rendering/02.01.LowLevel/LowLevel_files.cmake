
set(FILES
  Private/Main.cpp
  Private/Application.cpp
  Private/ExampleBase.cpp

  Private/ComputeShader/ComputeShader.cpp
  Private/CubeTexture/CubeTexture.cpp
  Private/GeometryShader/GeometryShader.cpp
  Private/IndirectBuffer/IndirectBuffer.cpp
  Private/Instancing/Instancing.cpp
  Private/MeshShader/MeshShader.cpp
  Private/Queries/Queries.cpp
  Private/RenderToTexture/RenderToTexture.cpp
  Private/TessellationShader/TessellationShader.cpp
  Private/Texture/Texture.cpp
  Private/Triangle/Triangle.cpp
  Private/VertexBuffer/VertexBuffer.cpp

  Private/Gpgpu/Gpgpu.cpp
  Private/IcosahedronTessellation/IcosahedronTessellation.cpp
  Private/InstancedCubes/InstancedCubes.cpp
  Private/InstancedCubes/CubeRendererDrawInstanced/BatchDrawInstanced.cpp
  Private/InstancedCubes/CubeRendererDrawInstanced/CubeRendererDrawInstanced.cpp
  Private/InstancedCubes/CubeRendererInstancedArrays/BatchInstancedArrays.cpp
  Private/InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays.cpp
)