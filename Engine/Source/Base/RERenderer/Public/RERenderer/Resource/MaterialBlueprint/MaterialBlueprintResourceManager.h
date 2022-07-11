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
#include <RECore/Resource/ResourceManager.h>
#include "RERenderer/Resource/Material/MaterialProperties.h"
#include "RERenderer/IRenderer.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4365)	// warning C4365: 'argument': conversion from 'std::_Atomic_integral_t' to 'long', signed/unsigned mismatch
	PRAGMA_WARNING_DISABLE_MSVC(4623)	// warning C4623: 'std::_UInt_is_zero': default constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(4625)	// warning C4625: 'std::_Generic_error_category': copy constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(4626)	// warning C4626: 'std::_Generic_error_category': assignment operator was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(5026)	// warning C5026: 'std::_Generic_error_category': move constructor was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(5027)	// warning C5027: 'std::_Generic_error_category': move assignment operator was implicitly defined as deleted
	PRAGMA_WARNING_DISABLE_MSVC(5039)	// warning C5039: '_Thrd_start': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception.
	#include <mutex>
	#include <unordered_map>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RECore {
class IFile;
class MemoryFile;
}
namespace RERenderer {
class IRenderer;

class LightBufferManager;

class IndirectBufferManager;

class MaterialBlueprintResource;

class UniformInstanceBufferManager;

class TextureInstanceBufferManager;

class MaterialBlueprintResourceLoader;

class IMaterialBlueprintResourceListener;
}namespace RECore {
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, RECore::uint32 MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32 MaterialBlueprintResourceId;	///< POD material blueprint resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class MaterialBlueprintResourceManager final : public RECore::ResourceManager<MaterialBlueprintResource>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;
		friend class IResource;						// Needed so that inside this classes an static_cast<CompositorNodeResourceManager*>(IResourceManager*) works
		friend class MaterialTechnique;				// Needs to be able to call "Renderer::MaterialBlueprintResourceManager::addSerializedGraphicsPipelineState()"
		friend class GraphicsPipelineStateCompiler;	// Needs to be able to call "Renderer::MaterialBlueprintResourceManager::applySerializedGraphicsPipelineState()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::unordered_map<RECore::uint32, RERHI::SerializedGraphicsPipelineState> SerializedGraphicsPipelineStates;	///< Key = FNV1a hash of "RERHI::SerializedGraphicsPipelineState"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline IRenderer& getRenderer() const
		{
			return mRenderer;
		}

		[[nodiscard]] inline bool getCreateInitialPipelineStateCaches() const
		{
			return mCreateInitialPipelineStateCaches;
		}

		inline void setCreateInitialPipelineStateCaches(bool createInitialPipelineStateCaches)
		{
			mCreateInitialPipelineStateCaches = createInitialPipelineStateCaches;
		}

		void loadMaterialBlueprintResourceByAssetId(AssetId assetId, MaterialBlueprintResourceId& materialBlueprintResourceId, RECore::IResourceListener* resourceListener = nullptr, bool reload = false, RECore::ResourceLoaderTypeId resourceLoaderTypeId = RECore::getInvalid<RECore::ResourceLoaderTypeId>(), bool createInitialPipelineStateCaches = true);	// Asynchronous
		void setInvalidResourceId(MaterialBlueprintResourceId& materialBlueprintResourceId, RECore::IResourceListener& resourceListener) const;

		[[nodiscard]] inline IMaterialBlueprintResourceListener& getMaterialBlueprintResourceListener() const
		{
			// We know this pointer must always be valid
			RHI_ASSERT(nullptr != mMaterialBlueprintResourceListener, "Invalid material blueprint resource listener")
			return *mMaterialBlueprintResourceListener;
		}

		void setMaterialBlueprintResourceListener(IMaterialBlueprintResourceListener* materialBlueprintResourceListener);	// Does not take over the control of the memory

		/**
		*  @brief
		*    Return the global material properties
		*
		*  @return
		*    The global material properties
		*
		*  @remarks
		*    The material blueprint resource manager itself is setting the following global material properties:
		*    - Floating point property "GlobalPastSecondsSinceLastFrame"
		*    - Floating point property "GlobalTimeInSeconds"
		*    - Floating point property "PreviousGlobalTimeInSeconds"
		*    - Integer property "GlobalNumberOfMultisamples" (see "Renderer::CompositorWorkspaceInstance::setNumberOfMultisamples()")
		*/
		[[nodiscard]] inline MaterialProperties& getGlobalMaterialProperties()
		{
			return mGlobalMaterialProperties;
		}

		[[nodiscard]] inline const MaterialProperties& getGlobalMaterialProperties() const
		{
			return mGlobalMaterialProperties;
		}

		/**
		*  @brief
		*    Called pre command buffer dispatch
		*/
		void onPreCommandBufferDispatch();

		//[-------------------------------------------------------]
		//[ Default texture filtering                             ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline RERHI::FilterMode getDefaultTextureFilterMode() const
		{
			return mDefaultTextureFilterMode;
		}

		[[nodiscard]] inline RECore::uint8 getDefaultMaximumTextureAnisotropy() const
		{
			return mDefaultMaximumTextureAnisotropy;
		}

		void setDefaultTextureFiltering(RERHI::FilterMode filterMode, RECore::uint8 maximumAnisotropy);

		//[-------------------------------------------------------]
		//[ RECore::Manager                                               ]
		//[-------------------------------------------------------]
		[[nodiscard]] inline UniformInstanceBufferManager& getUniformInstanceBufferManager() const
		{
			// We know this pointer must always be valid
			RHI_ASSERT(nullptr != mUniformInstanceBufferManager, "Invalid uniform instance buffer manager")
			return *mUniformInstanceBufferManager;
		}

		[[nodiscard]] inline TextureInstanceBufferManager& getTextureInstanceBufferManager() const
		{
			// We know this pointer must always be valid
			RHI_ASSERT(nullptr != mTextureInstanceBufferManager, "Invalid texture instance buffer manager")
			return *mTextureInstanceBufferManager;
		}

		[[nodiscard]] inline IndirectBufferManager& getIndirectBufferManager() const
		{
			// We know this pointer must always be valid
			RHI_ASSERT(nullptr != mIndirectBufferManager, "Invalid indirect buffer manager")
			return *mIndirectBufferManager;
		}

		[[nodiscard]] inline LightBufferManager& getLightBufferManager() const
		{
			// We know this pointer must always be valid
			RHI_ASSERT(nullptr != mLightBufferManager, "Invalid light buffer manager")
			return *mLightBufferManager;
		}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual RECore::uint32 getNumberOfResources() const override;
		[[nodiscard]] virtual RECore::IResource& getResourceByIndex(RECore::uint32 index) const override;
		[[nodiscard]] virtual RECore::IResource& getResourceByResourceId(RECore::ResourceId resourceId) const override;
		[[nodiscard]] virtual RECore::IResource* tryGetResourceByResourceId(RECore::ResourceId resourceId) const override;
		virtual void reloadResourceByAssetId(AssetId assetId) override;
		virtual void update() override;


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
	private:
		[[nodiscard]] virtual RECore::IResourceLoader* createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit MaterialBlueprintResourceManager(IRenderer& renderer);
		virtual ~MaterialBlueprintResourceManager() override;
		explicit MaterialBlueprintResourceManager(const MaterialBlueprintResourceManager&) = delete;
		MaterialBlueprintResourceManager& operator=(const MaterialBlueprintResourceManager&) = delete;

		//[-------------------------------------------------------]
		//[ Pipeline state object cache                           ]
		//[-------------------------------------------------------]
		void addSerializedGraphicsPipelineState(RECore::uint32 serializedGraphicsPipelineStateHash, const RERHI::SerializedGraphicsPipelineState& serializedGraphicsPipelineState);
		void applySerializedGraphicsPipelineState(RECore::uint32 serializedGraphicsPipelineStateHash, RERHI::GraphicsPipelineState& graphicsPipelineState);
		void clearPipelineStateObjectCache();
		void loadPipelineStateObjectCache(RECore::IFile& file);
		[[nodiscard]] bool doesPipelineStateObjectCacheNeedSaving() const;
		void savePipelineStateObjectCache(RECore::MemoryFile& memoryFile);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IRenderer&							mRenderer;									///< Renderer instance, do not destroy the instance
		bool								mCreateInitialPipelineStateCaches;			///< Create initial graphics and compute pipeline state caches after a material blueprint has been loaded?
		IMaterialBlueprintResourceListener*	mMaterialBlueprintResourceListener;			///< Material blueprint resource listener, always valid, do not destroy the instance
		MaterialProperties					mGlobalMaterialProperties;					///< Global material properties
		RERHI::FilterMode						mDefaultTextureFilterMode;					///< Default texture filter mode
		RECore::uint8								mDefaultMaximumTextureAnisotropy;			///< Default maximum texture anisotropy
		std::mutex							mSerializedGraphicsPipelineStatesMutex;		///< "Renderer::GraphicsPipelineStateCompiler" is running asynchronous, hence we need to synchronize the serialized graphics pipeline states access
		SerializedGraphicsPipelineStates	mSerializedGraphicsPipelineStates;			///< Serialized pipeline states
		UniformInstanceBufferManager*		mUniformInstanceBufferManager;				///< Uniform instance buffer manager, always valid in a sane none-legacy environment
		TextureInstanceBufferManager*		mTextureInstanceBufferManager;				///< Texture instance buffer manager, always valid in a sane none-legacy environment
		IndirectBufferManager*				mIndirectBufferManager;						///< Indirect buffer manager, always valid in a sane none-legacy environment
		LightBufferManager*					mLightBufferManager;						///< Light buffer manager, always valid in a sane none-legacy environment

		// Internal resource manager implementation
    RECore::ResourceManagerTemplate<MaterialBlueprintResource, MaterialBlueprintResourceLoader, MaterialBlueprintResourceId, 64>* mInternalResourceManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
