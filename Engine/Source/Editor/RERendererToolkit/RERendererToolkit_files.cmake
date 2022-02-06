
set(FILES
  Private/RendererToolkitImpl.cpp
  # AssetCompiler
  Private/AssetCompiler/SkeletonAnimationAssetCompiler.cpp
  Private/AssetCompiler/SceneAssetCompiler.cpp
  Private/AssetCompiler/MeshAssetCompiler.cpp
  Private/AssetCompiler/CompositorWorkspaceAssetCompiler.cpp
  Private/AssetCompiler/CompositorNodeAssetCompiler.cpp
  Private/AssetCompiler/SkeletonAnimationAssetCompiler.cpp
  Private/AssetCompiler/SkeletonAssetCompiler.cpp
  Private/AssetCompiler/MaterialAssetCompiler.cpp
  Private/AssetCompiler/MaterialBlueprintAssetCompiler.cpp
  Private/AssetCompiler/ShaderBlueprintAssetCompiler.cpp
  Private/AssetCompiler/ShaderPieceAssetCompiler.cpp
  Private/AssetCompiler/TextureAssetCompiler.cpp
  Private/AssetCompiler/VertexAttributesAssetCompiler.cpp
  # Helper
  Private/Helper/AssimpLogStream.cpp
  Private/Helper/JsonMaterialHelper.cpp
  Private/Helper/StringHelper.cpp
  Private/Helper/AssimpIOSystem.cpp
  Private/Helper/AssimpHelper.cpp
  Private/Helper/CacheManager.cpp
  Private/Helper/JsonHelper.cpp
  Private/Helper/JsonMaterialBlueprintHelper.cpp
  # Project
  Private/Project/ProjectAssetMonitor.cpp
  Private/Project/ProjectImpl.cpp
  # Importer
  Private/AssetImporter/MeshAssetImporter.cpp
  Private/AssetImporter/SketchfabAssetImporter.cpp
)