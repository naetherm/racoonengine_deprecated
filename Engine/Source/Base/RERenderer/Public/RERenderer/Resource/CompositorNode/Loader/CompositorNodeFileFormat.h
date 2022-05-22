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
#include "RERenderer/Core/Renderer/FramebufferSignature.h"
#include "RERenderer/Core/Renderer/RenderTargetTextureSignature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::StringId AssetId;					///< Asset identifier, internally just a POD "RECore::uint32", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef RECore::StringId CompositorChannelId;		///< Compositor channel identifier, internally just a POD "RECore::uint32"
	typedef RECore::StringId CompositorFramebufferId;	///< Compositor framebuffer identifier, internally just a POD "RECore::uint32"
	typedef RECore::StringId CompositorPassTypeId;		///< Compositor pass type identifier, internally just a POD "RECore::uint32"
	typedef RECore::uint32 MaterialTechniqueId;		///< Material technique identifier, result of hashing the material technique name via "RERenderer::StringId"
	typedef RECore::StringId MaterialPropertyId;		///< Material property identifier, internally just a POD "RECore::uint32", result of hashing the property name


	// Compositor node file format content:
	// - File format header
	// - Compositor node header
	namespace v1CompositorNode
	{


		//[-------------------------------------------------------]
		//[ Definitions                                           ]
		//[-------------------------------------------------------]
		static constexpr RECore::uint32 FORMAT_TYPE	 = STRING_ID("CompositorNode");
		static constexpr RECore::uint32 FORMAT_VERSION = 9;

		#pragma pack(push)
		#pragma pack(1)
			struct CompositorNodeHeader final
			{
				RECore::uint32 numberOfInputChannels;
				RECore::uint32 numberOfRenderTargetTextures;
				RECore::uint32 numberOfFramebuffers;
				RECore::uint32 numberOfTargets;
				RECore::uint32 numberOfOutputChannels;
			};

			struct Channel final
			{
				CompositorChannelId id;
			};

			struct RenderTargetTexture final
			{
				AssetId						 assetId;
				RenderTargetTextureSignature renderTargetTextureSignature;
			};

			struct Framebuffer final
			{
				CompositorFramebufferId compositorFramebufferId;
				FramebufferSignature	framebufferSignature;
			};

			struct Target final
			{
				CompositorChannelId		compositorChannelId;
				CompositorFramebufferId compositorFramebufferId;
				RECore::uint32				numberOfPasses;
			};

			struct PassHeader final
			{
				CompositorPassTypeId compositorPassTypeId;
				RECore::uint32			 numberOfBytes;
			};

			// Keep this in sync with "Renderer::ICompositorResourcePass::deserialize() -> PassData"
			struct Pass
			{
				static constexpr RECore::uint32 MAXIMUM_PASS_NAME_LENGTH = 63 + 1;	// +1 for the terminating zero

				char	 name[MAXIMUM_PASS_NAME_LENGTH] = { "Compositor pass" };	///< Human readable ASCII pass name for debugging and profiling, contains terminating zero
				float	 minimumDepth					= 0.0f;
				float	 maximumDepth					= 1.0f;
				RECore::uint32 numberOfExecutions				= RECore::getInvalid<RECore::uint32>();
				bool	 skipFirstExecution				= false;
			};

			struct PassClear final : public Pass
			{
				RECore::uint32 flags	  = 0;		///< Combination of "RERHI::ClearFlag"
				float	 color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				float	 z		  = 0.0f;	///< 0 instead of 1 due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
				RECore::uint32 stencil  = 0;
			};

			struct PassVrHiddenAreaMesh final : public Pass
			{
				RECore::uint32 flags	 = 0;	///< Combination of "RERHI::ClearFlag", except for color-flag
				RECore::uint32 stencil = 0;
			};

			struct PassScene : public Pass
			{
				RECore::uint8				minimumRenderQueueIndex = 0;	///< Inclusive
				RECore::uint8				maximumRenderQueueIndex = 255;	///< Inclusive
				bool				transparentPass			= false;
				MaterialTechniqueId	materialTechniqueId		= RECore::getInvalid<MaterialTechniqueId>();
			};

			struct PassShadowMap final : public PassScene
			{
				AssetId textureAssetId;
				AssetId depthToExponentialVarianceMaterialBlueprintAssetId;
				AssetId blurMaterialBlueprintAssetId;
			};

			struct PassResolveMultisample final : public Pass
			{
				CompositorFramebufferId sourceMultisampleCompositorFramebufferId;
			};

			struct PassCopy final : public Pass
			{
				AssetId destinationTextureAssetId;
				AssetId sourceTextureAssetId;
			};

			struct PassGenerateMipmaps final : public Pass
			{
				AssetId			   textureAssetId;
				AssetId			   materialBlueprintAssetId;
				MaterialPropertyId textureMaterialBlueprintProperty;
			};

			struct PassCompute : public Pass
			{
				AssetId				materialAssetId;												///< If material blueprint asset ID is set, material asset ID must be invalid
				MaterialTechniqueId	materialTechniqueId		   = RECore::getInvalid<MaterialTechniqueId>();	///< Must always be valid
				AssetId				materialBlueprintAssetId;										///< If material asset ID is set, material blueprint asset ID must be invalid
				RECore::uint32			numberOfMaterialProperties = 0;
			};

			// The material definition is not mandatory for the debug GUI, if nothing is defined the fixed build in RHI configuration resources will be used instead
			struct PassDebugGui final : public PassCompute
			{
			};
		#pragma pack(pop)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // v1CompositorNode
} // RECore
