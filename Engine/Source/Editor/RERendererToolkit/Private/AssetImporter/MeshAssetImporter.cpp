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
#include "RERendererToolkit/AssetImporter/MeshAssetImporter.h"
#include "RERendererToolkit/Helper/AssimpLogStream.h"
#include "RERendererToolkit/Helper/AssimpIOSystem.h"
#include "RERendererToolkit/Helper/AssimpHelper.h"
#include "RERendererToolkit/Helper/StringHelper.h"
#include "RERendererToolkit/Helper/JsonHelper.h"
#include "RERendererToolkit/Context.h"

#include <RECore/File/IFile.h>
#include <RECore/File/IFileManager.h>
#include <RECore/File/FileSystemHelper.h>
#include <RECore/Log/Log.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
PRAGMA_WARNING_DISABLE_MSVC(4061)	// warning C4061: enumerator 'FORCE_32BIT' in switch of enum 'aiMetadataType' is not explicitly handled by a case label
PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: '=': conversion from 'uint32_t' to 'int', signed/unsigned mismatch
PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
PRAGMA_WARNING_DISABLE_MSVC(4571)	// warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
PRAGMA_WARNING_DISABLE_MSVC(4619)	// warning C4619: #pragma warning: there is no warning number '4351'
PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: 'std::_Tree<std::_Tmap_traits<_Kty,_Ty,_Pr,_Alloc,false>>': copy constructor was implicitly defined as deleted
PRAGMA_WARNING_DISABLE_MSVC(5219)	// warning C5219: implicit conversion from 'int' to 'float', possible loss of data
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
PRAGMA_WARNING_POP

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
PRAGMA_WARNING_DISABLE_MSVC(4061)	// warning C4061: enumerator 'rapidjson::kNumberType' in switch of enum 'rapidjson::Type' is not explicitly handled by a case label
PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: '=': conversion from 'int' to 'rapidjson::internal::BigInteger::Type', signed/unsigned mismatch
PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: 'rapidjson::GenericMember<Encoding,Allocator>': copy constructor was implicitly defined as deleted
PRAGMA_WARNING_DISABLE_MSVC(4626)	// warning C4626: 'std::codecvt_base': assignment operator was implicitly defined as deleted
PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '__GNUC__' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#include <rapidjson/document.h>
PRAGMA_WARNING_POP

#include <array>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
namespace detail
{


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
typedef std::vector<uint8_t> FileData;
typedef std::vector<std::string> TextureFilenames;
typedef std::unordered_map<std::string, TextureFilenames> MaterialTextureFilenames;	// Key = material name
typedef std::unordered_map<std::string, std::string> MaterialNameToAssetId;	// Key = source material name (e.g. "/Head"), value = imported material filename (e.g. "./Spino_Head.asset")

struct ImporterContext final
{
  std::string			  meshFilename;
  bool				  hasSkeleton			   = false;
  bool				  removeRedundantMaterials = true;
  MaterialNameToAssetId materialNameToAssetId;
};

typedef std::array<const char*, 52> SketchfabMeshFormats;
SketchfabMeshFormats g_SketchfabMeshFormats = {
  ".3dc", ".asc",															// 3DC point cloud
  ".3ds",																	// 3DS
  ".ac",																	// ac3d
  ".abc",																	// Alembic
  ".obj",																	// Alias Wavefront
  ".bvh",																	// Biovision Hierarchy
  ".blend",																// Blender
  ".geo",																	// Carbon Graphics Inc
  ".dae", ".zae",															// Collada
  ".dwf",																	// Design Web Format
  ".dw",																	// Designer Workbench
  ".x",																	// DirectX
  ".dxf",																	// Drawing eXchange Format
  ".fbx",																	// Autodesk Filmbox, FBX
  ".ogr",																	// GDAL vector format
  ".gta",																	// Generic Tagged Arrays
  ".gltf", ".glb",														// GL Transmission Format
  ".igs", ".iges",														// Initial Graphics Exchange Specification, IGES
  ".mu", ".craft",														// Kerbal Space Program
  ".kmz",																	// Google Earth, Keyhole Markup Language
  ".las",																	// LIDAR point clouds
  ".lwo", ".lws",															// Lightwave
  ".q3d",																	// Mimesys Q3D
  ".mc2obj", ".dat",														// Minecraft
  ".flt",																	// Open Flight
  ".iv",																	// Open Inventor
  ".osg", ".osgt", ".osgb", ".osgterrain", ".osgtgz", ".osgx", ".ive",	// OpenSceneGraph
  ".ply",																	// Polygon File Format
  ".bsp",																	// Quake
  ".md2", ".mdl", 														// Quake / Valve source engine
  ".shp",																	// Shape
  ".stl", ".sta",															// Stereolithography, Standard Tessellation Language
  ".txp",																	// Terrapage format database
  ".vpk",																	// Valve source engine
  ".wrl", ".vrml", ".wrz"													// Virtual Reality Modeling Language, VRML
};

typedef std::array<const char*, 4> SupportedMeshFormats;
SupportedMeshFormats g_SupportedMeshFormats = {
  ".obj",			// Alias Wavefront
  ".fbx",			// Autodesk Filmbox, FBX
  ".gltf", ".glb"	// GL Transmission Format
};

/*
		Sketchfab texture naming conventions: https://help.sketchfab.com/hc/en-us/articles/202600873-Materials-and-Textures#textures-auto-pbr
		"
		Automatic PBR Mapping

		Use our texture naming conventions to help us automatically choose settings and apply textures to the right shader slots. The format is "MaterialName_suffix.extension". For example, if you have a material named "Material1", you could name your textures like "Material1_diffuse.png", "Material1_metallic.png", etc.

		Avoid names with special characters, especially periods '.', underscores '_', and hyphens '-' because it can break the match.

		These are the strings we look for in the suffix:

		- Diffuse / Albedo / Base Color: 'diffuse', 'albedo', 'basecolor'
		- Metalness: 'metalness', 'metallic', 'metal', 'm'
		- Specular: 'specular', 'spec', 's'
		- Specular F0: 'specularf0', 'f0'
		- Roughness: 'roughness', 'rough', 'r'
		- Glossiness: 'glossiness', 'glossness', 'gloss', 'g', 'glossy'
		- AO: 'ambient occlusion', 'ao', 'occlusion', 'lightmap', 'diffuseintensity'
		- Cavity: 'cavity'
		- Normal Map: '''normal', 'nrm', 'normalmap'
		- Bump Map: 'bump', 'bumpmap', 'heightmap'
		- Emission: 'emission', 'emit', 'emissive'
		- Transparency: 'transparency', 'transparent', 'opacity', 'mask', 'alpha'
		"
		- Found also undocumented semantics in downloaded Sketchfab files:
			- "", "d", "diff", "dif" = Diffuse map
			- "n", "norm" = Normal map
			- "glow" = Emissive map
			- "light", "Ambient_Occlusion", "AmbientOccl" = Ambient occlusion map
			- Case variations, of course
		- PBR on Sketchfab: https://help.sketchfab.com/hc/en-us/articles/204429595-Materials-PBR-
		*/
enum SemanticType {
  ALBEDO_MAP,
  NORMAL_MAP,
  HEIGHT_MAP,
  ROUGHNESS_MAP,
  GLOSS_MAP,
  METALLIC_MAP,
  EMISSIVE_MAP,
  NUMBER_OF_SEMANTICS
};
typedef std::vector<const char*> SemanticStrings;
typedef std::array<SemanticStrings, SemanticType::NUMBER_OF_SEMANTICS> Semantics;
const Semantics g_Semantics = {{
   // ALBEDO_MAP
   { "diffuse", "albedo", "basecolor", "", "d", "diff", "dif" },
   // NORMAL_MAP
   { "normal", "nrm", "normalmap", "n", "norm" },
   // HEIGHT_MAP
   { "bump", "bumpmap", "heightmap" },
   // ROUGHNESS_MAP
   { "roughness", "rough", "r" },
   // GLOSS_MAP
   { "glossiness", "glossness", "gloss", "g", "glossy" },
   // METALLIC_MAP
   { "metalness", "metallic", "metal", "m",
     "specular", "spec", "s"		// Specular = roughness	TODO(naetherm) Need a support strategy for specular map
   },
   // EMISSIVE_MAP
   { "emission", "emit", "emissive", "glow" }
 }};


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERendererToolkit
{

void MeshAssetImporter::import(const IAssetImporter::Input &input) {

}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERendererToolkit
