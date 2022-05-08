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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <RECore/String/StringId.h>
#include <RECore/Math/Transform.h>
#include "RERenderer/Resource/Scene/Item/Light/LightSceneItem.h"
#include "RERenderer/Resource/Scene/Item/Debug/DebugDrawSceneItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::StringId AssetId;				///< Asset identifier, internally just a POD "uint32_t", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef RECore::StringId SceneItemTypeId;		///< Scene item type identifier, internally just a POD "uint32_t"
	typedef uint32_t MaterialTechniqueId;	///< Material technique identifier, result of hashing the material technique name via "RERenderer::StringId"


	// Scene file format content:
	// - File format header
	// - Scene header
	namespace v1Scene
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr uint32_t FORMAT_TYPE	 = STRING_ID("Scene");
		static constexpr uint32_t FORMAT_VERSION = 5;

		#pragma pack(push)
		#pragma pack(1)
			struct SceneHeader final
			{
				uint32_t unused;	// TODO(naetherm) Currently the scene header is unused
			};

			struct Nodes final
			{
				uint32_t numberOfNodes;
			};

			struct Node final
			{
				RECore::Transform transform;
				uint32_t  numberOfItems;
			};

			struct ItemHeader final
			{
				SceneItemTypeId typeId;
				uint32_t		numberOfBytes;
			};

			struct CameraItem final
			{
			};

			struct LightItem final
			{
				LightSceneItem::LightType lightType			   = LightSceneItem::LightType::POINT;
				float					  color[3]			   = { 1.0f, 1.0f, 1.0f };
				float					  radius			   = 1.0f;
				float					  innerAngle		   = glm::radians(40.0f);
				float					  outerAngle		   = glm::radians(50.0f);
				float					  nearClipDistance	   = 0.0f;
				int32_t					  iesLightProfileIndex = -1;	///< Illuminating Engineering Society (IES) light profile index (<0 = no IES)
			};

			struct SunlightItem final
			{
				// Usually fixed
				float sunriseTime	   = 07.50f;	// O'clock
				float sunsetTime	   = 20.50f;	// O'clock
				float eastDirection	   = 0.0f;
				float angleOfIncidence = glm::radians(20.0f);
				float timeOfDay		   = 10.00f;	// O'clock
			};

			struct MeshItem final
			{
				AssetId  meshAssetId;
				uint32_t numberOfSubMeshMaterialAssetIds = 0;
			};

			struct SkeletonMeshItem final	// : public MeshItem -> Not derived by intent to be able to reuse the mesh item serialization 1:1
			{
				AssetId skeletonAnimationAssetId;
			};

			struct MaterialData final
			{
				AssetId				materialAssetId;												///< If material blueprint asset ID is set, material asset ID must be invalid
				MaterialTechniqueId	materialTechniqueId		   = RECore::getInvalid<MaterialTechniqueId>();	///< Must always be valid
				AssetId				materialBlueprintAssetId;										///< If material asset ID is set, material blueprint asset ID must be invalid
				uint32_t			numberOfMaterialProperties = 0;
			};

			struct DebugDrawItem final
			{
				MaterialData materialData[RERenderer::DebugDrawSceneItem::RenderableIndex::NUMBER_OF_INDICES];
			};

			struct SkyItem final
			{
				MaterialData materialData;
			};

			struct VolumeItem final
			{
				MaterialData materialData;
			};

			struct GrassItem final
			{
				MaterialData materialData;
			};

			struct TerrainItem final
			{
				MaterialData materialData;
			};

			struct ParticlesItem final
			{
				MaterialData materialData;
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1Scene
} // RECore
