/*********************************************************\
 * Copyright (c) 2012-2022 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERenderer/RERenderer.h"
#include <RECore/String/StringId.h>
#include <RECore/Utility/GetInvalid.h>
#include "RERenderer/Resource/MaterialBlueprint/Listener/IMaterialBlueprintResourceListener.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	#include <glm/glm.hpp>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer
{
	class HosekWilkieSky;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32			 TextureResourceId;	///< POD texture resource identifier
	typedef RECore::StringId			 AssetId;			///< Asset identifier, internally just a POD "RECore::uint32", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef std::vector<AssetId> AssetIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material blueprint resource listener
	*
	*  @remarks
	*    Several things have to be taken into consideration when feeding shaders with parameters:
	*    - Reversed-Z ( https://developer.nvidia.com/content/depth-precision-visualized ) for improved depth buffer precision to reduce z-fighting
	*    - Camera relative rendering for rendering large scale scenes without jittering/wobbling
	*    - Single pass stereo rendering via instancing
	*/
	class MaterialBlueprintResourceListener : public IMaterialBlueprintResourceListener
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the asset IDs of automatically generated dynamic default texture assets
		*
		*  @param[out] assetIds
		*    Receives the asset IDs of automatically generated dynamic default texture assets, the list is not cleared before new entries are added
		*
		*  @remarks
		*    The material blueprint resource listener automatically generates some dynamic default texture assets one can reference e.g. inside material blueprint resources:
		*    - "RacoonEngine/Texture/DynamicByCode/IdentityColorCorrectionLookupTable3D"
		*    - "RacoonEngine/Texture/DynamicByCode/SsaoSampleKernel"
		*    - "RacoonEngine/Texture/DynamicByCode/SsaoNoise4x4"
		*/
		static void getDefaultTextureAssetIds(AssetIds& assetIds);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline MaterialBlueprintResourceListener() :
			mIdentityColorCorrectionLookupTable3D(RECore::getInvalid<TextureResourceId>()),
			mSsaoSampleKernelTextureResourceId(RECore::getInvalid<TextureResourceId>()),
			mSsaoNoiseTexture4x4ResourceId(RECore::getInvalid<TextureResourceId>()),
			mRenderer(nullptr),
			mPassData(nullptr),
			mCompositorContextData(nullptr),
			mWorldSpaceCameraPosition(0.0f, 0.0f, 0.0f),
			mCameraRelativeWorldSpaceCameraPosition{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
			mRenderTargetWidth(1),
			mRenderTargetHeight(1),
			mNearZ(0.0f),
			mFarZ(0.0f),
			mPreviousJitter(0.0f, 0.0f),
			mPreviousNumberOfRenderedFrames(RECore::getInvalid<RECore::uint64>()),
			mHosekWilkieSky(nullptr),
			#ifdef DEBUG
				mIsComputePipeline(false),
			#endif
			mObjectSpaceToWorldSpaceTransform(nullptr),
			mMaterialTechnique(nullptr)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MaterialBlueprintResourceListener() override
		{
			clear();
		}

		/**
		*  @brief
		*    Clear the material blueprint resource listener by freeing e.g. allocated memory
		*/
		void clear();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		explicit MaterialBlueprintResourceListener(const MaterialBlueprintResourceListener&) = delete;
		MaterialBlueprintResourceListener& operator=(const MaterialBlueprintResourceListener&) = delete;

		[[nodiscard]] inline const PassBufferManager::PassData& getPassData() const	// Memory address received via "Renderer::MaterialBlueprintResourceListener::beginFillPass()", can be invalid outside the correct scope, don't destroy the memory
		{
			RHI_ASSERT(mRenderer->getContext(), nullptr != mPassData, "Invalid pass data")
			return *mPassData;
		}


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::IMaterialBlueprintResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onStartup(const IRenderer& renderer) override;
		virtual void onShutdown(const IRenderer& renderer) override;

		inline virtual void beginFillUnknown() override
		{
			// Nothing here
		}

		[[nodiscard]] inline virtual bool fillUnknownValue([[maybe_unused]] RECore::uint32 referenceValue, [[maybe_unused]] RECore::uint8* buffer, [[maybe_unused]] RECore::uint32 numberOfBytes) override
		{
			// Nothing here

			// Value not filled
			return false;
		}

		virtual void beginFillPass(IRenderer& renderer, const RERHI::RHIRenderTarget* renderTarget, const CompositorContextData& compositorContextData, PassBufferManager::PassData& passData) override;
		[[nodiscard]] virtual bool fillPassValue(RECore::uint32 referenceValue, RECore::uint8* buffer, RECore::uint32 numberOfBytes) override;

		inline virtual void beginFillMaterial() override
		{
			// Nothing here
		}

		[[nodiscard]] inline virtual bool fillMaterialValue([[maybe_unused]] RECore::uint32 referenceValue, [[maybe_unused]] RECore::uint8* buffer, [[maybe_unused]] RECore::uint32 numberOfBytes) override
		{
			// Nothing here

			// Value not filled
			return false;
		}

		inline virtual void beginFillInstance(const PassBufferManager::PassData& passData, const RECore::Transform& objectSpaceToWorldSpaceTransform, MaterialTechnique& materialTechnique) override
		{
			// Remember the pass data memory address of the current scope
			// TODO(naetherm) Rethink the evil const-cast on here
			mPassData = &const_cast<PassBufferManager::PassData&>(passData);

			// Remember the instance data of the current scope
			mObjectSpaceToWorldSpaceTransform = &objectSpaceToWorldSpaceTransform;
			mMaterialTechnique				  = &materialTechnique;
		}

		[[nodiscard]] virtual bool fillInstanceValue(RECore::uint32 referenceValue, RECore::uint8* buffer, RECore::uint32 numberOfBytes, RECore::uint32 instanceTextureBufferStartIndex) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] glm::vec3 getWorldSpaceSunlightDirection() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Resource
		TextureResourceId mIdentityColorCorrectionLookupTable3D;
		TextureResourceId mSsaoSampleKernelTextureResourceId;
		TextureResourceId mSsaoNoiseTexture4x4ResourceId;

		// Pass
		IRenderer*					 mRenderer;					///< Memory address received via "Renderer::MaterialBlueprintResourceListener::beginFillPass()", can be a null pointer outside the correct scope, don't destroy the memory
		PassBufferManager::PassData* mPassData;					///< Memory address received via "Renderer::MaterialBlueprintResourceListener::beginFillPass()", can be a null pointer outside the correct scope, don't destroy the memory
		const CompositorContextData* mCompositorContextData;
		glm::dvec3					 mWorldSpaceCameraPosition;	///< Cached 64 bit world space position of the camera since often accessed due to camera relative rendering
		glm::vec3					 mCameraRelativeWorldSpaceCameraPosition[2];
		RECore::uint32					 mRenderTargetWidth;
		RECore::uint32					 mRenderTargetHeight;
		float						 mNearZ;
		float						 mFarZ;
		glm::vec2					 mPreviousJitter;
		RECore::uint64					 mPreviousNumberOfRenderedFrames;
		HosekWilkieSky*				 mHosekWilkieSky;
		#ifdef DEBUG
			bool					 mIsComputePipeline;
		#endif

		// Instance
		const RECore::Transform*   mObjectSpaceToWorldSpaceTransform;
		MaterialTechnique* mMaterialTechnique;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
