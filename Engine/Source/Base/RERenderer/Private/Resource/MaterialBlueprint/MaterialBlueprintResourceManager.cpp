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
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "RERenderer/Resource/MaterialBlueprint/Loader/MaterialBlueprintResourceLoader.h"
#include "RERenderer/Resource/MaterialBlueprint/Listener/MaterialBlueprintResourceListener.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/UniformInstanceBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/TextureInstanceBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/IndirectBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/LightBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/MaterialBufferManager.h"
#include "RERenderer/Resource/Material/MaterialResourceManager.h"
#include "RERenderer/Resource/Material/MaterialTechnique.h"
#include "RERenderer/Resource/Material/MaterialResource.h"
#include <RECore/Resource/ResourceManagerTemplate.h>
#include "RERenderer/IRenderer.h"
#include <RECore/Time/TimeManager.h>
#include "RERenderer/Context.h"
#include <RECore/Log/Log.h>


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
		struct MaterialBlueprintCacheEntry final
		{
			RECore::AssetId materialBlueprintAssetId;
			RECore::uint32		  numberOfBytes;
		};


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		static RERenderer::MaterialBlueprintResourceListener defaultMaterialBlueprintResourceListener;


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	// TODO(naetherm) Work-in-progress
	void MaterialBlueprintResourceManager::loadMaterialBlueprintResourceByAssetId(AssetId assetId, MaterialBlueprintResourceId& materialBlueprintResourceId, RECore::IResourceListener* resourceListener, bool reload, RECore::ResourceLoaderTypeId resourceLoaderTypeId, bool createInitialPipelineStateCaches)
	{
		// Choose default resource loader type ID, if necessary
		if (RECore::isInvalid(resourceLoaderTypeId))
		{
			resourceLoaderTypeId = MaterialBlueprintResourceLoader::TYPE_ID;
		}

		// Get or create the instance
		MaterialBlueprintResource* materialBlueprintResource = mInternalResourceManager->getResourceByAssetId(assetId);

		// Create the resource instance
		const RECore::Asset* asset = mRenderer.getAssetManager().tryGetAssetByAssetId(assetId);
		RHI_ASSERT(nullptr != asset, "Unknown asset ID")
		bool load = (reload && nullptr != asset);
		if (nullptr == materialBlueprintResource && nullptr != asset)
		{
			materialBlueprintResource = &mInternalResourceManager->getResources().addElement();
			materialBlueprintResource->setResourceManager(this);
			materialBlueprintResource->setAssetId(assetId);
			materialBlueprintResource->setResourceLoaderTypeId(resourceLoaderTypeId);
			load = true;
		}

		// Before connecting a resource listener, ensure we set the output resource ID at once so it can already directly be used inside the resource listener
		if (nullptr != materialBlueprintResource)
		{
			materialBlueprintResourceId = materialBlueprintResource->getId();
			if (nullptr != resourceListener)
			{
				materialBlueprintResource->connectResourceListener(*resourceListener);
			}
		}
		else
		{
			materialBlueprintResourceId = RECore::getInvalid<MaterialBlueprintResourceId>();
		}

		// Load the resource, if required
		if (load)
		{
			// Commit resource streamer asset load request
			mRenderer.getResourceStreamer().commitLoadRequest(RECore::ResourceStreamer::LoadRequest(*asset, resourceLoaderTypeId, reload, *this, materialBlueprintResourceId));

			// TODO(naetherm) Currently material blueprint resource loading is a blocking process.
			//          Later on, we can probably just write "mInternalResourceManager->loadResourceByAssetId(assetId, meshResourceId, resourceListener, reload, resourceLoaderTypeId);" and be done in this method.
			materialBlueprintResource->enforceFullyLoaded();

			// Create default pipeline state caches
			// -> Material blueprints should be loaded by a cache manager upfront so that the following expensive call doesn't cause runtime hiccups
			// -> Runtime hiccups would also be there without fallback pipeline state caches, so there's no real way around
			// -> We must enforce fully loaded material blueprint resource state for this
			if (mCreateInitialPipelineStateCaches && createInitialPipelineStateCaches)
			{
				materialBlueprintResource->createPipelineStateCaches(true);
			}
		}
	}

	void MaterialBlueprintResourceManager::setInvalidResourceId(MaterialBlueprintResourceId& materialBlueprintResourceId, RECore::IResourceListener& resourceListener) const
	{
		MaterialBlueprintResource* materialBlueprintResource = tryGetById(materialBlueprintResourceId);
		if (nullptr != materialBlueprintResource)
		{
			materialBlueprintResource->disconnectResourceListener(resourceListener);
		}
		RECore::setInvalid(materialBlueprintResourceId);
	}

	void MaterialBlueprintResourceManager::setMaterialBlueprintResourceListener(IMaterialBlueprintResourceListener* materialBlueprintResourceListener)
	{
		// There must always be a valid material blueprint resource listener instance
		if (mMaterialBlueprintResourceListener != materialBlueprintResourceListener)
		{
			// We know there must be a currently set material blueprint resource listener
			RHI_ASSERT(nullptr != mMaterialBlueprintResourceListener, "Invalid material blueprint resource listener")
			mMaterialBlueprintResourceListener->onShutdown(mRenderer);
			mMaterialBlueprintResourceListener = (nullptr != materialBlueprintResourceListener) ? materialBlueprintResourceListener : &::detail::defaultMaterialBlueprintResourceListener;
			mMaterialBlueprintResourceListener->onStartup(mRenderer);
		}
	}

	void MaterialBlueprintResourceManager::onPreCommandBufferDispatch()
	{
		if (nullptr != mUniformInstanceBufferManager)
		{
			mUniformInstanceBufferManager->onPreCommandBufferDispatch();
		}
		if (nullptr != mTextureInstanceBufferManager)
		{
			mTextureInstanceBufferManager->onPreCommandBufferDispatch();
		}
		if (nullptr != mIndirectBufferManager)
		{
			mIndirectBufferManager->onPreCommandBufferDispatch();
		}
	}

	void MaterialBlueprintResourceManager::setDefaultTextureFiltering(RERHI::FilterMode filterMode, RECore::uint8 maximumAnisotropy)
	{
		// State change?
		if (mDefaultTextureFilterMode != filterMode || mDefaultMaximumTextureAnisotropy != maximumAnisotropy)
		{
			// Backup the new state
			mDefaultTextureFilterMode = filterMode;
			mDefaultMaximumTextureAnisotropy = maximumAnisotropy;

			{ // Recreate sampler state instances of all material blueprint resources
				const RECore::uint32 numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
				for (RECore::uint32 i = 0; i < numberOfElements; ++i)
				{
					mInternalResourceManager->getResources().getElementByIndex(i).onDefaultTextureFilteringChanged(mDefaultTextureFilterMode, mDefaultMaximumTextureAnisotropy);
				}
			}

			{ // Make the resource groups of all material techniques dirty to instantly see default texture filtering changes
				const MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
				const RECore::uint32 numberOfElements = materialResourceManager.getNumberOfResources();
				for (RECore::uint32 i = 0; i < numberOfElements; ++i)
				{
					for (MaterialTechnique* materialTechnique : materialResourceManager.getByIndex(i).getSortedMaterialTechniqueVector())
					{
						materialTechnique->makeResourceGroupDirty();
					}
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceManager methods     ]
	//[-------------------------------------------------------]
	RECore::uint32 MaterialBlueprintResourceManager::getNumberOfResources() const
	{
		return mInternalResourceManager->getResources().getNumberOfElements();
	}

RECore::IResource& MaterialBlueprintResourceManager::getResourceByIndex(RECore::uint32 index) const
	{
		return mInternalResourceManager->getResources().getElementByIndex(index);
	}

RECore::IResource& MaterialBlueprintResourceManager::getResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().getElementById(resourceId);
	}

RECore::IResource* MaterialBlueprintResourceManager::tryGetResourceByResourceId(RECore::ResourceId resourceId) const
	{
		return mInternalResourceManager->getResources().tryGetElementById(resourceId);
	}

	void MaterialBlueprintResourceManager::reloadResourceByAssetId(AssetId assetId)
	{
		// TODO(naetherm) Experimental implementation (take care of resource cleanup etc.)
		const RECore::uint32 numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
		for (RECore::uint32 i = 0; i < numberOfElements; ++i)
		{
			MaterialBlueprintResource& materialBlueprintResource = mInternalResourceManager->getResources().getElementByIndex(i);
			if (materialBlueprintResource.getAssetId() == assetId)
			{
				{ // Properly release material buffer slots
					MaterialBufferManager* materialBufferManager = materialBlueprintResource.mMaterialBufferManager;
					const MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
					const RECore::uint32 numberOfMaterials = materialResourceManager.getNumberOfResources();
					for (RECore::uint32 materialIndex = 0; materialIndex < numberOfMaterials; ++materialIndex)
					{
						for (MaterialTechnique* materialTechnique : materialResourceManager.getByIndex(materialIndex).getSortedMaterialTechniqueVector())
						{
							if (materialTechnique->getMaterialBlueprintResourceId() == materialBlueprintResource.getId() && RECore::isValid(materialTechnique->getAssignedMaterialSlot()))
							{
								materialBufferManager->releaseSlot(*materialTechnique);
							}
						}
					}
				}

				// Reload material blueprint resource
				MaterialBlueprintResourceId materialBlueprintResourceId = RECore::getInvalid<MaterialBlueprintResourceId>();
				loadMaterialBlueprintResourceByAssetId(assetId, materialBlueprintResourceId, nullptr, true, materialBlueprintResource.getResourceLoaderTypeId());

				// Clear pipeline state cache manager
				materialBlueprintResource.clearPipelineStateObjectCache();

				{ // Make the texture resource groups of all material techniques
					MaterialBufferManager* materialBufferManager = materialBlueprintResource.mMaterialBufferManager;
					const MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
					const RECore::uint32 numberOfMaterials = materialResourceManager.getNumberOfResources();

					// Loop through all materials
					for (RECore::uint32 materialIndex = 0; materialIndex < numberOfMaterials; ++materialIndex)
					{
						// Loop through all material techniques of the current material
						MaterialResource& materialResource = materialResourceManager.getByIndex(materialIndex);
						for (MaterialTechnique* materialTechnique : materialResource.getSortedMaterialTechniqueVector())
						{
							if (materialTechnique->getMaterialBlueprintResourceId() == materialBlueprintResource.getId())
							{
								materialTechnique->makeResourceGroupDirty();
								for (const MaterialProperty& materialProperty : materialResource.getSortedPropertyVector())
								{
									// Update material property values as long as a material property was not explicitly overwritten inside a material
									if (!materialProperty.isOverwritten())
									{
										const MaterialPropertyId materialPropertyId = materialProperty.getMaterialPropertyId();
										const MaterialProperty* blueprintMaterialProperty = materialBlueprintResource.getMaterialProperties().getPropertyById(materialPropertyId);
										if (nullptr != blueprintMaterialProperty)
										{
											materialResource.setPropertyByIdInternal(materialPropertyId, *blueprintMaterialProperty, materialProperty.getUsage(), false);
										}
									}
								}
								if (nullptr != materialBufferManager)
								{
									materialBufferManager->requestSlot(*materialTechnique);
								}
								materialTechnique->calculateSerializedGraphicsPipelineStateHash();
							}
						}
					}
				}
				break;
			}
		}
	}

	void MaterialBlueprintResourceManager::update()
	{
		const RECore::TimeManager& timeManager = mRenderer.getTimeManager();
		mGlobalMaterialProperties.setPropertyById(STRING_ID("GlobalPastSecondsSinceLastFrame"), MaterialPropertyValue::fromFloat(timeManager.getPastSecondsSinceLastFrame()), MaterialProperty::Usage::SHADER_UNIFORM);
		{ // Set previous global time in seconds
			const MaterialPropertyValue* materialProperty = mGlobalMaterialProperties.getPropertyById(STRING_ID("GlobalTimeInSeconds"));
			mGlobalMaterialProperties.setPropertyById(STRING_ID("PreviousGlobalTimeInSeconds"), (nullptr != materialProperty) ? *materialProperty : MaterialPropertyValue::fromFloat(timeManager.getGlobalTimeInSeconds()), MaterialProperty::Usage::SHADER_UNIFORM);
		}
		mGlobalMaterialProperties.setPropertyById(STRING_ID("GlobalTimeInSeconds"), MaterialPropertyValue::fromFloat(timeManager.getGlobalTimeInSeconds()), MaterialProperty::Usage::SHADER_UNIFORM);
		mGlobalMaterialProperties.setPropertyById(STRING_ID("GlobalFramesPerSecond"), MaterialPropertyValue::fromFloat(timeManager.getFramesPerSecond()), MaterialProperty::Usage::SHADER_UNIFORM);
	}


	//[-------------------------------------------------------]
	//[ Private virtual RECore::IResourceManager methods    ]
	//[-------------------------------------------------------]
  RECore::IResourceLoader* MaterialBlueprintResourceManager::createResourceLoaderInstance(RECore::ResourceLoaderTypeId resourceLoaderTypeId)
	{
		return new MaterialBlueprintResourceLoader(*this, mRenderer);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	MaterialBlueprintResourceManager::MaterialBlueprintResourceManager(IRenderer& renderer) :
		mRenderer(renderer),
		mCreateInitialPipelineStateCaches(renderer.getRhi().getNameId() != RERHI::NameId::OPENGLES3),	// TODO(naetherm) Not all example material blueprints are OpenGL ES 3 ready, yet
		mMaterialBlueprintResourceListener(&::detail::defaultMaterialBlueprintResourceListener),
		mDefaultTextureFilterMode(RERHI::FilterMode::MIN_MAG_MIP_LINEAR),
		mDefaultMaximumTextureAnisotropy(1),
		mUniformInstanceBufferManager(nullptr),
		mTextureInstanceBufferManager(nullptr),
		mIndirectBufferManager(nullptr),
		mLightBufferManager(nullptr)
	{
		// Create internal resource manager
		mInternalResourceManager = new RECore::ResourceManagerTemplate<MaterialBlueprintResource, MaterialBlueprintResourceLoader, MaterialBlueprintResourceId, 64>(renderer.getAssetManager(), renderer.getResourceStreamer(), *this);

		// Startup material blueprint resource listener
		mMaterialBlueprintResourceListener->onStartup(mRenderer);

		// Create buffer managers
		const RERHI::Capabilities& capabilities = renderer.getRhi().getCapabilities();
		if (capabilities.maximumUniformBufferSize > 0 && capabilities.maximumTextureBufferSize > 0)
		{
			mUniformInstanceBufferManager = new UniformInstanceBufferManager(renderer);
			mTextureInstanceBufferManager = new TextureInstanceBufferManager(renderer);
			mIndirectBufferManager = new IndirectBufferManager(renderer);
			mLightBufferManager = new LightBufferManager(renderer);
		}

		// Update at once to have all managed global material properties known from the start
		update();
		mGlobalMaterialProperties.setPropertyById(STRING_ID("GlobalNumberOfMultisamples"), MaterialPropertyValue::fromInteger(0), MaterialProperty::Usage::SHADER_COMBINATION);
	}

	MaterialBlueprintResourceManager::~MaterialBlueprintResourceManager()
	{
		// Destroy buffer managers
		delete mUniformInstanceBufferManager;
		delete mTextureInstanceBufferManager;
		delete mIndirectBufferManager;
		delete mLightBufferManager;

		// Shutdown material blueprint resource listener (we know there must be such an instance)
		RHI_ASSERT(nullptr != mMaterialBlueprintResourceListener, "Invalid material blueprint resource listener")
		mMaterialBlueprintResourceListener->onShutdown(mRenderer);

		// Destroy internal resource manager
		delete mInternalResourceManager;

		// Explicitly clear the default material blueprint resource listener in order to avoid false-positive "_CrtMemDumpAllObjectsSince()" memory leak detection
		::detail::defaultMaterialBlueprintResourceListener.clear();
	}

	void MaterialBlueprintResourceManager::addSerializedGraphicsPipelineState(RECore::uint32 serializedGraphicsPipelineStateHash, const RERHI::SerializedGraphicsPipelineState& serializedGraphicsPipelineState)
	{
		std::lock_guard<std::mutex> serializedGraphicsPipelineStatesMutexLock(mSerializedGraphicsPipelineStatesMutex);
		SerializedGraphicsPipelineStates::iterator iterator = mSerializedGraphicsPipelineStates.find(serializedGraphicsPipelineStateHash);
		if (iterator == mSerializedGraphicsPipelineStates.cend())
		{
			mSerializedGraphicsPipelineStates.emplace(serializedGraphicsPipelineStateHash, serializedGraphicsPipelineState);
		}
	}

	void MaterialBlueprintResourceManager::applySerializedGraphicsPipelineState(RECore::uint32 serializedGraphicsPipelineStateHash, RERHI::GraphicsPipelineState& graphicsPipelineState)
	{
		std::lock_guard<std::mutex> serializedGraphicsPipelineStatesMutexLock(mSerializedGraphicsPipelineStatesMutex);
		MaterialBlueprintResourceManager::SerializedGraphicsPipelineStates::const_iterator iterator = mSerializedGraphicsPipelineStates.find(serializedGraphicsPipelineStateHash);
		if (iterator != mSerializedGraphicsPipelineStates.cend())
		{
			static_cast<RERHI::SerializedGraphicsPipelineState&>(graphicsPipelineState) = iterator->second;
		}
		else
		{
			// We can and will end up in here when e.g. heating the shader cache
		}
	}

	void MaterialBlueprintResourceManager::clearPipelineStateObjectCache()
	{
		// Loop through all material blueprint resources and clear the cache entries
		const RECore::uint32 numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
		for (RECore::uint32 i = 0; i < numberOfElements; ++i)
		{
			mInternalResourceManager->getResources().getElementByIndex(i).clearPipelineStateObjectCache();
		}
	}

	void MaterialBlueprintResourceManager::loadPipelineStateObjectCache(RECore::IFile& file)
	{
		{ // Read the serialized graphics pipeline states
			RECore::uint32 numberOfElements = RECore::getInvalid<RECore::uint32>();
			file.read(&numberOfElements, sizeof(RECore::uint32));
			mSerializedGraphicsPipelineStates.reserve(numberOfElements);
			for (RECore::uint32 i = 0; i < numberOfElements; ++i)
			{
				RECore::uint32 key = RECore::getInvalid<RECore::uint32>();
				file.read(&key, sizeof(RECore::uint32));
				RERHI::SerializedGraphicsPipelineState serializedGraphicsPipelineState = {};
				file.read(&serializedGraphicsPipelineState, sizeof(RERHI::SerializedGraphicsPipelineState));
				mSerializedGraphicsPipelineStates.emplace(key, serializedGraphicsPipelineState);
			}
		}

		{ // Read the pipeline state object cache header which consists of information about the contained material blueprint resources
			RECore::uint32 numberOfElements = 0;
			file.read(&numberOfElements, sizeof(RECore::uint32));
			if (numberOfElements > 0)
			{
				std::vector< ::detail::MaterialBlueprintCacheEntry> materialBlueprintCacheEntries;
				materialBlueprintCacheEntries.resize(numberOfElements);
				file.read(materialBlueprintCacheEntries.data(), sizeof(::detail::MaterialBlueprintCacheEntry) * numberOfElements);

				// Loop through all material blueprint resources and read the cache entries
				for (RECore::uint32 i = 0; i < numberOfElements; ++i)
				{
					// TODO(naetherm) Currently material blueprint resource loading is a blocking process
					const ::detail::MaterialBlueprintCacheEntry& materialBlueprintCacheEntry = materialBlueprintCacheEntries[i];
					MaterialBlueprintResourceId materialBlueprintResourceId = RECore::getInvalid<MaterialBlueprintResourceId>();
					loadMaterialBlueprintResourceByAssetId(materialBlueprintCacheEntry.materialBlueprintAssetId, materialBlueprintResourceId, nullptr, false, RECore::getInvalid<RECore::ResourceLoaderTypeId>(), false);
					if (RECore::isValid(materialBlueprintResourceId))
					{
						mInternalResourceManager->getResources().getElementById(materialBlueprintResourceId).loadPipelineStateObjectCache(file);
					}
					else
					{
						RE_LOG(Warning, "The pipeline state object cache contains an unknown material blueprint asset. Might have happened due to renaming or removal which can be considered normal during development, but not in shipped builds.")

						// TODO(naetherm) Enable file skip after "RERenderer::MaterialBlueprintResource::savePipelineStateObjectCache()" has been implemented
						// file.skip(materialBlueprintCacheEntry.numberOfBytes);
					}
				}
			}
			else
			{
				RE_LOG(Warning, "The pipeline state object cache contains no elements which is a bit unusual")
			}
		}
	}

	bool MaterialBlueprintResourceManager::doesPipelineStateObjectCacheNeedSaving() const
	{
		const RECore::uint32 numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
		for (RECore::uint32 i = 0; i < numberOfElements; ++i)
		{
			if (mInternalResourceManager->getResources().getElementByIndex(i).doesPipelineStateObjectCacheNeedSaving())
			{
				// Cache saving needed
				return true;
			}
		}

		// No cache saving needed
		return false;
	}

	void MaterialBlueprintResourceManager::savePipelineStateObjectCache(RECore::MemoryFile& memoryFile)
	{
		{ // Write the serialized graphics pipeline states
			const RECore::uint32 numberOfElements = static_cast<RECore::uint32>(mSerializedGraphicsPipelineStates.size());
			memoryFile.write(&numberOfElements, sizeof(RECore::uint32));
			for (const auto& elementPair : mSerializedGraphicsPipelineStates)
			{
				memoryFile.write(&elementPair.first, sizeof(RECore::uint32));
				memoryFile.write(&elementPair.second, sizeof(RERHI::SerializedGraphicsPipelineState));
			}
		}

		{ // Write the pipeline state object cache header which consists of information about the contained material blueprint resources
			const RECore::uint32 numberOfElements = mInternalResourceManager->getResources().getNumberOfElements();
			memoryFile.write(&numberOfElements, sizeof(RECore::uint32));
			RECore::uint32 firstMaterialBlueprintCacheEntryIndex = 0;
			for (RECore::uint32 i = 0; i < numberOfElements; ++i)
			{
				::detail::MaterialBlueprintCacheEntry materialBlueprintCacheEntry;
				materialBlueprintCacheEntry.materialBlueprintAssetId = mInternalResourceManager->getResources().getElementByIndex(i).getAssetId();
				materialBlueprintCacheEntry.numberOfBytes			 = 0;	// At this point in time we don't know the number of bytes the material blueprint cache entry consumes
				memoryFile.write(&materialBlueprintCacheEntry, sizeof(::detail::MaterialBlueprintCacheEntry));
				if (0 == firstMaterialBlueprintCacheEntryIndex)
				{
					firstMaterialBlueprintCacheEntryIndex = static_cast<RECore::uint32>(memoryFile.getNumberOfBytes() - sizeof(::detail::MaterialBlueprintCacheEntry));
				}
			}
			::detail::MaterialBlueprintCacheEntry* firstMaterialBlueprintCacheEntry = reinterpret_cast< ::detail::MaterialBlueprintCacheEntry*>(&memoryFile.getByteVector()[firstMaterialBlueprintCacheEntryIndex]);

			// Loop through all material blueprint resources and write the cache entries
			for (RECore::uint32 i = 0; i < numberOfElements; ++i)
			{
				const RECore::uint32 fileStart = static_cast<RECore::uint32>(memoryFile.getNumberOfBytes());
				mInternalResourceManager->getResources().getElementByIndex(i).savePipelineStateObjectCache(memoryFile);
				firstMaterialBlueprintCacheEntry[i].numberOfBytes = static_cast<RECore::uint32>(memoryFile.getNumberOfBytes() - fileStart);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
