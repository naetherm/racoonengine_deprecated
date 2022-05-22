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
#include "RERenderer/RenderQueue/RenderQueue.h"
#include "RERenderer/RenderQueue/RenderableManager.h"
#include "RERenderer/Resource/CompositorWorkspace/CompositorContextData.h"
#include "RERenderer/Resource/Texture/TextureResource.h"
#include "RERenderer/Resource/Texture/TextureResourceManager.h"
#include "RERenderer/Resource/Mesh/MeshResourceManager.h"
#include "RERenderer/Resource/Material/MaterialResourceManager.h"
#include "RERenderer/Resource/Material/MaterialTechnique.h"
#include "RERenderer/Resource/Material/MaterialResource.h"
#include "RERenderer/Resource/MaterialBlueprint/Cache/ComputePipelineStateCache.h"
#include "RERenderer/Resource/MaterialBlueprint/Cache/GraphicsPipelineStateCache.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/PassBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/LightBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/IndirectBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/UniformInstanceBufferManager.h"
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/TextureInstanceBufferManager.h"
#include "RERenderer/Core/IProfiler.h"
#include <RECore/Math/Transform.h>

#include <array>
#include <algorithm>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{
 

		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		// Flip the float to deal with negative & positive numbers
		// - See "Rough sorting by depth" - http://aras-p.info/blog/2014/01/16/rough-sorting-by-depth/
		[[nodiscard]] inline RECore::uint32 floatFlip(RECore::uint32 f)
		{
			const RECore::uint32 mask = static_cast<RECore::uint32>(-int(f >> 31) | 0x80000000);
			return (f ^ mask);
		}

		// Taking highest 10 bits for rough sort of floats.
		// - 0.01 maps to 752; 0.1 to 759; 1.0 to 766; 10.0 to 772;
		// - 100.0 to 779 etc. Negative numbers go similarly in 0..511 range.
		// - See "Rough sorting by depth" - http://aras-p.info/blog/2014/01/16/rough-sorting-by-depth/
		[[nodiscard]] inline RECore::uint32 depthToBits(float depth, RECore::uint32 depthBits)
		{
			union { float f; RECore::uint32 i; } f2i;
			f2i.f = depth;
			f2i.i = floatFlip(f2i.i);			// Flip bits to be sortable
			return (f2i.i >> (32 - depthBits));	// Take highest n-bits
		}

		inline void setShaderPropertiesPropertyValue(RERenderer::MaterialPropertyId materialPropertyId, const RERenderer::MaterialPropertyValue& materialPropertyValue, RERenderer::ShaderProperties& shaderProperties)
		{
			switch (materialPropertyValue.getValueType())
			{
				case RERenderer::MaterialPropertyValue::ValueType::BOOLEAN:
					shaderProperties.setPropertyValue(materialPropertyId, materialPropertyValue.getBooleanValue());
					break;

				case RERenderer::MaterialPropertyValue::ValueType::INTEGER:
					shaderProperties.setPropertyValue(materialPropertyId, materialPropertyValue.getIntegerValue());
					break;

				case RERenderer::MaterialPropertyValue::ValueType::UNKNOWN:
				case RERenderer::MaterialPropertyValue::ValueType::INTEGER_2:
				case RERenderer::MaterialPropertyValue::ValueType::INTEGER_3:
				case RERenderer::MaterialPropertyValue::ValueType::INTEGER_4:
				case RERenderer::MaterialPropertyValue::ValueType::FLOAT:
				case RERenderer::MaterialPropertyValue::ValueType::FLOAT_2:
				case RERenderer::MaterialPropertyValue::ValueType::FLOAT_3:
				case RERenderer::MaterialPropertyValue::ValueType::FLOAT_4:
				case RERenderer::MaterialPropertyValue::ValueType::FLOAT_3_3:
				case RERenderer::MaterialPropertyValue::ValueType::FLOAT_4_4:
				case RERenderer::MaterialPropertyValue::ValueType::FILL_MODE:
				case RERenderer::MaterialPropertyValue::ValueType::CULL_MODE:
				case RERenderer::MaterialPropertyValue::ValueType::CONSERVATIVE_RASTERIZATION_MODE:
				case RERenderer::MaterialPropertyValue::ValueType::DEPTH_WRITE_MASK:
				case RERenderer::MaterialPropertyValue::ValueType::STENCIL_OP:
				case RERenderer::MaterialPropertyValue::ValueType::COMPARISON_FUNC:
				case RERenderer::MaterialPropertyValue::ValueType::BLEND:
				case RERenderer::MaterialPropertyValue::ValueType::BLEND_OP:
				case RERenderer::MaterialPropertyValue::ValueType::FILTER_MODE:
				case RERenderer::MaterialPropertyValue::ValueType::TEXTURE_ADDRESS_MODE:
				case RERenderer::MaterialPropertyValue::ValueType::TEXTURE_ASSET_ID:
				case RERenderer::MaterialPropertyValue::ValueType::GLOBAL_MATERIAL_PROPERTY_ID:
				default:
					ASSERT(false, "Invalid material property value type")	// TODO(naetherm) Error handling
					break;
			}
		}

		FORCEINLINE void gatherShaderProperties(const RERenderer::MaterialResource& materialResource, const RERenderer::MaterialBlueprintResource& materialBlueprintResource, const RERenderer::MaterialProperties& globalMaterialProperties, const RERenderer::Renderable& renderable, bool singlePassStereoInstancing, RERenderer::ShaderProperties& shaderProperties, RERenderer::ShaderProperties& scratchOptimizedShaderProperties)
		{
			shaderProperties.clear();

			{ // Gather shader properties from static material properties generating shader combinations
				const RERenderer::MaterialProperties::SortedPropertyVector& sortedMaterialPropertyVector = materialResource.getSortedPropertyVector();
				const size_t numberOfMaterialProperties = sortedMaterialPropertyVector.size();
				for (size_t i = 0; i < numberOfMaterialProperties; ++i)
				{
					const RERenderer::MaterialProperty& materialProperty = sortedMaterialPropertyVector[i];
					if (materialProperty.getUsage() == RERenderer::MaterialProperty::Usage::SHADER_COMBINATION)
					{
						switch (materialProperty.getValueType())
						{
							case RERenderer::MaterialPropertyValue::ValueType::BOOLEAN:
								shaderProperties.setPropertyValue(materialProperty.getMaterialPropertyId(), materialProperty.getBooleanValue());
								break;

							case RERenderer::MaterialPropertyValue::ValueType::INTEGER:
								shaderProperties.setPropertyValue(materialProperty.getMaterialPropertyId(), materialProperty.getIntegerValue());
								break;

							case RERenderer::MaterialPropertyValue::ValueType::GLOBAL_MATERIAL_PROPERTY_ID:
							{
								const RERenderer::MaterialProperty* globalMaterialProperty = globalMaterialProperties.getPropertyById(materialProperty.getGlobalMaterialPropertyId());
								if (nullptr != globalMaterialProperty)
								{
									setShaderPropertiesPropertyValue(materialProperty.getMaterialPropertyId(), *globalMaterialProperty, shaderProperties);
								}
								else
								{
									// Try global material property reference fallback
									globalMaterialProperty = materialBlueprintResource.getMaterialProperties().getPropertyById(materialProperty.getGlobalMaterialPropertyId());
									if (nullptr != globalMaterialProperty)
									{
										setShaderPropertiesPropertyValue(materialProperty.getMaterialPropertyId(), *globalMaterialProperty, shaderProperties);
									}
									else
									{
										// Error, can't resolve reference
										ASSERT(false, "Unknown global material property")	// TODO(naetherm) Error handling
									}
								}
								break;
							}

							case RERenderer::MaterialPropertyValue::ValueType::UNKNOWN:
							case RERenderer::MaterialPropertyValue::ValueType::INTEGER_2:
							case RERenderer::MaterialPropertyValue::ValueType::INTEGER_3:
							case RERenderer::MaterialPropertyValue::ValueType::INTEGER_4:
							case RERenderer::MaterialPropertyValue::ValueType::FLOAT:
							case RERenderer::MaterialPropertyValue::ValueType::FLOAT_2:
							case RERenderer::MaterialPropertyValue::ValueType::FLOAT_3:
							case RERenderer::MaterialPropertyValue::ValueType::FLOAT_4:
							case RERenderer::MaterialPropertyValue::ValueType::FLOAT_3_3:
							case RERenderer::MaterialPropertyValue::ValueType::FLOAT_4_4:
							case RERenderer::MaterialPropertyValue::ValueType::FILL_MODE:
							case RERenderer::MaterialPropertyValue::ValueType::CULL_MODE:
							case RERenderer::MaterialPropertyValue::ValueType::CONSERVATIVE_RASTERIZATION_MODE:
							case RERenderer::MaterialPropertyValue::ValueType::DEPTH_WRITE_MASK:
							case RERenderer::MaterialPropertyValue::ValueType::STENCIL_OP:
							case RERenderer::MaterialPropertyValue::ValueType::COMPARISON_FUNC:
							case RERenderer::MaterialPropertyValue::ValueType::BLEND:
							case RERenderer::MaterialPropertyValue::ValueType::BLEND_OP:
							case RERenderer::MaterialPropertyValue::ValueType::FILTER_MODE:
							case RERenderer::MaterialPropertyValue::ValueType::TEXTURE_ADDRESS_MODE:
							case RERenderer::MaterialPropertyValue::ValueType::TEXTURE_ASSET_ID:
							default:
								ASSERT(false, "Unknown material property value type")	// TODO(naetherm) Error handling
								break;
						}
					}
				}
			}

			// Automatic "UseGpuSkinning"-property setting
			if (RECore::isValid(renderable.getSkeletonResourceId()))
			{
				static constexpr RECore::uint32 USE_GPU_SKINNING = STRING_ID("UseGpuSkinning");
				if (nullptr != materialBlueprintResource.getMaterialProperties().getPropertyById(USE_GPU_SKINNING))
				{
					shaderProperties.setPropertyValue(USE_GPU_SKINNING, 1);
				}
			}

			materialBlueprintResource.optimizeShaderProperties(shaderProperties, scratchOptimizedShaderProperties);

			// Automatic build-in "SinglePassStereoInstancing"-property setting
			if (singlePassStereoInstancing)
			{
				static constexpr RECore::uint32 SINGLE_PASS_STEREO_INSTANCING = STRING_ID("SinglePassStereoInstancing");
				scratchOptimizedShaderProperties.setPropertyValue(SINGLE_PASS_STEREO_INSTANCING, 1);
			}
		}


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
	RenderQueue::RenderQueue(IndirectBufferManager& indirectBufferManager, RECore::uint8 minimumRenderQueueIndex, RECore::uint8 maximumRenderQueueIndex, bool positionOnlyPass, bool transparentPass, bool doSort) :
		mRenderer(indirectBufferManager.getRenderer()),
		mIndirectBufferManager(indirectBufferManager),
		mNumberOfNullDrawCalls(0),
		mNumberOfDrawIndexedCalls(0),
		mNumberOfDrawCalls(0),
		mMinimumRenderQueueIndex(minimumRenderQueueIndex),
		mMaximumRenderQueueIndex(maximumRenderQueueIndex),
		mPositionOnlyPass(positionOnlyPass),
		mTransparentPass(transparentPass),
		mDoSort(doSort)
	{
		RHI_ASSERT(mMaximumRenderQueueIndex >= mMinimumRenderQueueIndex, "Invalid minimum/maximum render queue index")
		mQueues.resize(static_cast<size_t>(mMaximumRenderQueueIndex - mMinimumRenderQueueIndex + 1));
	}

	void RenderQueue::clear()
	{
		if (getNumberOfDrawCalls() > 0)
		{
			for (Queue& queue : mQueues)
			{
				queue.queuedRenderables.clear();
				queue.sorted = false;
			}
			mNumberOfNullDrawCalls = mNumberOfDrawIndexedCalls = mNumberOfDrawCalls = 0;
		}
	}

	void RenderQueue::addRenderablesFromRenderableManager(const RenderableManager& renderableManager, MaterialTechniqueId materialTechniqueId, const CompositorContextData& compositorContextData, bool castShadows)
	{
		// Sanity check
		RHI_ASSERT(renderableManager.isVisible(), "Invalid renderable manager visibility")

		// Sorting key bits
		static constexpr RECore::uint32 PIPELINE_STATE_NUMBER_OF_BITS	= 16;
		static constexpr RECore::uint32 VERTEX_ARRAY_NUMBER_OF_BITS	= 16;
		static constexpr RECore::uint32 RESOURCE_GROUP_NUMBER_OF_BITS	= 11;	// TODO(naetherm) Add resource group sorting
		static constexpr RECore::uint32 DEPTH_NUMBER_OF_BITS			= 21;

		// Sorting key bit shift: Opaque renderables are first sorted by pipeline state, then by vertex array, then by depth front to back
		static constexpr RECore::uint32 PIPELINE_STATE_SHIFT_OPAQUE	= 64							- PIPELINE_STATE_NUMBER_OF_BITS;	// = 48
		static constexpr RECore::uint32 VERTEX_ARRAY_SHIFT_OPAQUE		= PIPELINE_STATE_SHIFT_OPAQUE	- VERTEX_ARRAY_NUMBER_OF_BITS;		// = 32
		static constexpr RECore::uint32 RESOURCE_GROUP_SHIFT_OPAQUE	= VERTEX_ARRAY_SHIFT_OPAQUE		- RESOURCE_GROUP_NUMBER_OF_BITS;	// = 21
		static constexpr RECore::uint32 DEPTH_SHIFT_OPAQUE			= RESOURCE_GROUP_SHIFT_OPAQUE	- DEPTH_NUMBER_OF_BITS;				// = 0

		// Sorting key transparent bit shift: Transparent renderables are sorted by depth back to front, then by pipeline state, then by vertex array
		static constexpr RECore::uint32 DEPTH_SHIFT_TRANSPARENT			= 64								- DEPTH_NUMBER_OF_BITS;				// = 43
		static constexpr RECore::uint32 PIPELINE_STATE_SHIFT_TRANSPARENT	= DEPTH_SHIFT_TRANSPARENT			- PIPELINE_STATE_NUMBER_OF_BITS;	// = 27
		static constexpr RECore::uint32 VERTEX_ARRAY_SHIFT_TRANSPARENT	= PIPELINE_STATE_SHIFT_TRANSPARENT	- VERTEX_ARRAY_NUMBER_OF_BITS;		// = 11
		static constexpr RECore::uint32 RESOURCE_GROUP_SHIFT_TRANSPARENT	= VERTEX_ARRAY_SHIFT_TRANSPARENT	- RESOURCE_GROUP_NUMBER_OF_BITS;	// = 0

		// Quantize the cached distance to camera
		// -> Solid: Sort from front to back to benefit from early z rejection
		// -> Transparent: Sort from back to front to have correct alpha blending
		const RECore::uint32 quantizedDepth = ::detail::depthToBits(mTransparentPass ? -renderableManager.getCachedDistanceToCamera() : renderableManager.getCachedDistanceToCamera(), DEPTH_NUMBER_OF_BITS);

		// Optionally adjust and check the LOD index
		RECore::uint8 lodIndex = mRenderer.getMeshResourceManager().getNumberOfTopMeshLodsToRemove();
		RHI_ASSERT(0 != renderableManager.getNumberOfLods(), "Invalid renderable manager which has no LODs: There must always be at least one LOD, namely the original none reduced version")
		const RECore::uint8 numberOfLods = renderableManager.getNumberOfLods();
		if (lodIndex >= numberOfLods)
		{
			// Silently clamp to maximum LOD
			lodIndex = static_cast<RECore::uint8>(static_cast<int>(numberOfLods) - 1);
		}

		// Register the renderables inside our renderables queue
		const MaterialResourceManager& materialResourceManager = mRenderer.getMaterialResourceManager();
		const MaterialBlueprintResourceManager& materialBlueprintResourceManager = mRenderer.getMaterialBlueprintResourceManager();
		const MaterialProperties& globalMaterialProperties = materialBlueprintResourceManager.getGlobalMaterialProperties();
		const bool singlePassStereoInstancing = compositorContextData.getSinglePassStereoInstancing();
		const RenderableManager::Renderables& renderables = renderableManager.getRenderables();
		const RECore::uint32 numberOfRenderablesPerLod = static_cast<RECore::uint32>(renderables.size()) / numberOfLods;	// Each LOD has the same number of renderables
		RECore::uint32 renderableIndex = numberOfRenderablesPerLod * lodIndex;
		RECore::uint32 renderableEndIndex = renderableIndex + numberOfRenderablesPerLod;
		for (; renderableIndex < renderableEndIndex; ++renderableIndex)
		{
			const Renderable& renderable = renderables[renderableIndex];
			if (!castShadows || renderable.getCastShadows())
			{
				// It's valid if one or more renderables inside a renderable manager don't fall into the range processed by this render queue
				// -> At least one renderable should fall into the range processed by this render queue or the render queue is used wrong
				const RECore::uint8 renderQueueIndex = renderable.getRenderQueueIndex();
				if (renderQueueIndex >= mMinimumRenderQueueIndex && renderQueueIndex <= mMaximumRenderQueueIndex)
				{
					// Material resource
					const MaterialResource* materialResource = materialResourceManager.tryGetById(renderable.getMaterialResourceId());
					if (nullptr != materialResource)
					{
						MaterialTechnique* materialTechnique = materialResource->getMaterialTechniqueById(materialTechniqueId);
						if (nullptr != materialTechnique)
						{
							MaterialBlueprintResource* materialBlueprintResource = materialBlueprintResourceManager.tryGetById(materialTechnique->getMaterialBlueprintResourceId());
							if (nullptr != materialBlueprintResource && RECore::IResource::LoadingState::LOADED == materialBlueprintResource->getLoadingState())
							{
								// Get the pipeline state object (PSO) to use, preferably by using cached information
								RERHI::RHIPipelineState* foundPipelineState = nullptr;
								if (RECore::isValid(materialBlueprintResource->getComputeShaderBlueprintResourceId()))
								{
									// Compute material blueprint resource

									// Get a simple conservative combined generation counter to detect whether or not the renderable pipeline state cache is still considered to be valid
									const RECore::uint32 generationCounter = materialResource->getMaterialProperties().getShaderCombinationGenerationCounter() + globalMaterialProperties.getShaderCombinationGenerationCounter() + materialBlueprintResource->getMaterialProperties().getShaderCombinationGenerationCounter();

									// Get the pipeline state object (PSO) to use, preferably by using cached information
									Renderable::PipelineStateCaches& pipelineStateCaches = const_cast<Renderable::PipelineStateCaches&>(renderable.mPipelineStateCaches);
									for (Renderable::PipelineStateCache& pipelineStateCache : pipelineStateCaches)
									{
										if (materialTechniqueId == pipelineStateCache.materialTechniqueId)
										{
											if (generationCounter != pipelineStateCache.generationCounter)
											{
												::detail::gatherShaderProperties(*materialResource, *materialBlueprintResource, globalMaterialProperties, renderable, singlePassStereoInstancing, mScratchShaderProperties, mScratchOptimizedShaderProperties);
												const ComputePipelineStateCache* computePipelineStateCache = materialBlueprintResource->getComputePipelineStateCacheManager().getComputePipelineStateCacheByCombination(mScratchOptimizedShaderProperties, false);

												// As long as we received a fallback compute pipeline state cache, we can't update the renderable pipeline state cache
												if (nullptr != computePipelineStateCache && nullptr != computePipelineStateCache->getComputePipelineStateObjectPtr() && !computePipelineStateCache->isUsingFallback())
												{
													pipelineStateCache.generationCounter = generationCounter;
													pipelineStateCache.pipelineStatePtr = computePipelineStateCache->getComputePipelineStateObjectPtr();
												}
											}
											foundPipelineState = static_cast<RERHI::RHIComputePipelineState*>(pipelineStateCache.pipelineStatePtr->GetPointer());
											RHI_ASSERT(nullptr != foundPipelineState, "Invalid found compute pipeline state")
											break;
										}
									}
									if (nullptr == foundPipelineState)
									{
										::detail::gatherShaderProperties(*materialResource, *materialBlueprintResource, globalMaterialProperties, renderable, singlePassStereoInstancing, mScratchShaderProperties, mScratchOptimizedShaderProperties);
										const ComputePipelineStateCache* computePipelineStateCache = materialBlueprintResource->getComputePipelineStateCacheManager().getComputePipelineStateCacheByCombination(mScratchOptimizedShaderProperties, false);
										if (nullptr != computePipelineStateCache && nullptr != computePipelineStateCache->getComputePipelineStateObjectPtr())
										{
											// As long as we received a fallback compute pipeline state cache, we can't put it into the renderable pipeline state cache
											if (computePipelineStateCache->isUsingFallback())
											{
												foundPipelineState = static_cast<RERHI::RHIComputePipelineState*>(computePipelineStateCache->getComputePipelineStateObjectPtr());
											}
											else
											{
												foundPipelineState = static_cast<RERHI::RHIComputePipelineState*>(pipelineStateCaches.emplace_back(materialTechniqueId, generationCounter, computePipelineStateCache->getComputePipelineStateObjectPtr()).pipelineStatePtr.GetPointer());
											}
											RHI_ASSERT(nullptr != foundPipelineState, "Invalid found compute pipeline state")
										}
									}
								}
								else
								{
									// Graphics material blueprint resource

									// Get a simple conservative combined generation counter to detect whether or not the renderable pipeline state cache is still considered to be valid
									const RECore::uint32 generationCounter = materialResource->getMaterialProperties().getShaderCombinationGenerationCounter() + globalMaterialProperties.getShaderCombinationGenerationCounter() + materialBlueprintResource->getMaterialProperties().getShaderCombinationGenerationCounter() + materialTechnique->getSerializedGraphicsPipelineStateHash();

									// Get the pipeline state object (PSO) to use, preferably by using cached information
									Renderable::PipelineStateCaches& pipelineStateCaches = const_cast<Renderable::PipelineStateCaches&>(renderable.mPipelineStateCaches);
									for (Renderable::PipelineStateCache& pipelineStateCache : pipelineStateCaches)
									{
										if (materialTechniqueId == pipelineStateCache.materialTechniqueId)
										{
											if (generationCounter != pipelineStateCache.generationCounter)
											{
												::detail::gatherShaderProperties(*materialResource, *materialBlueprintResource, globalMaterialProperties, renderable, singlePassStereoInstancing, mScratchShaderProperties, mScratchOptimizedShaderProperties);
												const GraphicsPipelineStateCache* graphicsPipelineStateCache = materialBlueprintResource->getGraphicsPipelineStateCacheManager().getGraphicsPipelineStateCacheByCombination(materialTechnique->getSerializedGraphicsPipelineStateHash(), mScratchOptimizedShaderProperties, false);

												// As long as we received a fallback graphics pipeline state cache, we can't update the renderable pipeline state cache
												if (nullptr != graphicsPipelineStateCache && nullptr != graphicsPipelineStateCache->getGraphicsPipelineStateObjectPtr() && !graphicsPipelineStateCache->isUsingFallback())
												{
													pipelineStateCache.generationCounter = generationCounter;
													pipelineStateCache.pipelineStatePtr = graphicsPipelineStateCache->getGraphicsPipelineStateObjectPtr();
												}
											}
											foundPipelineState = static_cast<RERHI::RHIGraphicsPipelineState*>(pipelineStateCache.pipelineStatePtr->GetPointer());
											RHI_ASSERT(nullptr != foundPipelineState, "Invalid found graphics pipeline state")
											break;
										}
									}
									if (nullptr == foundPipelineState)
									{
										::detail::gatherShaderProperties(*materialResource, *materialBlueprintResource, globalMaterialProperties, renderable, singlePassStereoInstancing, mScratchShaderProperties, mScratchOptimizedShaderProperties);
										const GraphicsPipelineStateCache* graphicsPipelineStateCache = materialBlueprintResource->getGraphicsPipelineStateCacheManager().getGraphicsPipelineStateCacheByCombination(materialTechnique->getSerializedGraphicsPipelineStateHash(), mScratchOptimizedShaderProperties, false);
										if (nullptr != graphicsPipelineStateCache && nullptr != graphicsPipelineStateCache->getGraphicsPipelineStateObjectPtr())
										{
											// As long as we received a fallback graphics pipeline state cache, we can't put it into the renderable pipeline state cache
											if (graphicsPipelineStateCache->isUsingFallback())
											{
												foundPipelineState = static_cast<RERHI::RHIGraphicsPipelineState*>(graphicsPipelineStateCache->getGraphicsPipelineStateObjectPtr());
											}
											else
											{
												foundPipelineState = static_cast<RERHI::RHIGraphicsPipelineState*>(pipelineStateCaches.emplace_back(materialTechniqueId, generationCounter, graphicsPipelineStateCache->getGraphicsPipelineStateObjectPtr()).pipelineStatePtr.GetPointer());
											}
											RHI_ASSERT(nullptr != foundPipelineState, "Invalid found graphics pipeline state")
										}
									}
								}
								if (nullptr != foundPipelineState)
								{
									const RECore::uint16 pipelineStateId = foundPipelineState->getId();
									const RECore::uint16 resourceGroupId = 0;	// TODO(naetherm) Add resource group sorting
									const RECore::uint32 vertexArrayId = mPositionOnlyPass ? ((nullptr != renderable.getPositionOnlyVertexArrayPtrWithFallback()) ? renderable.getPositionOnlyVertexArrayPtrWithFallback()->getId() : 0u) : ((nullptr != renderable.getVertexArrayPtr()) ? renderable.getVertexArrayPtr()->getId() : 0u);

									// Define helper macros
									#define RENDER_QUEUE_MAKE_MASK(x) ((1u << (x)) - 1u)
									#define RENDER_QUEUE_HASH(x, bits, shift) (RECore::uint64((x) & RENDER_QUEUE_MAKE_MASK((bits))) << (shift))

									// Generate the sorting key
									RECore::uint64 sortingKey;	// Guaranteed to be initialized below
									if (mTransparentPass)
									{
										// Transparent renderables are sorted by depth back to front, then by pipeline state, then by vertex array
										sortingKey =
										RENDER_QUEUE_HASH(quantizedDepth,	DEPTH_NUMBER_OF_BITS,			DEPTH_SHIFT_TRANSPARENT)			|
										RENDER_QUEUE_HASH(pipelineStateId,	PIPELINE_STATE_NUMBER_OF_BITS,	PIPELINE_STATE_SHIFT_TRANSPARENT)	|
										RENDER_QUEUE_HASH(vertexArrayId,	VERTEX_ARRAY_NUMBER_OF_BITS,	VERTEX_ARRAY_SHIFT_TRANSPARENT)		|
										RENDER_QUEUE_HASH(resourceGroupId,	RESOURCE_GROUP_NUMBER_OF_BITS,	RESOURCE_GROUP_SHIFT_TRANSPARENT);
									}
									else
									{
										// Opaque renderables are first sorted by pipeline state, then by vertex array, then by depth front to back
										sortingKey =
										RENDER_QUEUE_HASH(pipelineStateId,	PIPELINE_STATE_NUMBER_OF_BITS,	PIPELINE_STATE_SHIFT_OPAQUE)	|
										RENDER_QUEUE_HASH(vertexArrayId,	VERTEX_ARRAY_NUMBER_OF_BITS,	VERTEX_ARRAY_SHIFT_OPAQUE)		|
										RENDER_QUEUE_HASH(resourceGroupId,	RESOURCE_GROUP_NUMBER_OF_BITS,	RESOURCE_GROUP_SHIFT_OPAQUE)	|
										RENDER_QUEUE_HASH(quantizedDepth,	DEPTH_NUMBER_OF_BITS,			DEPTH_SHIFT_OPAQUE);
									}

									// Undefine helper macros
									#undef RENDER_QUEUE_HASH
									#undef RENDER_QUEUE_MAKE_MASK

									// Register the renderable inside our renderables queue
									Queue& queue = mQueues[static_cast<size_t>(renderQueueIndex - mMinimumRenderQueueIndex)];
									RHI_ASSERT(!queue.sorted, "Ensure render queue is still in filling state and not already in rendering state")
									queue.queuedRenderables.emplace_back(renderable, *materialResource, *materialTechnique, *materialBlueprintResource, *foundPipelineState, sortingKey);
									if (0 != renderable.getNumberOfIndices())
									{
										if (renderable.getDrawIndexed())
										{
											++mNumberOfDrawIndexedCalls;
										}
										else
										{
											++mNumberOfDrawCalls;
										}
									}
									else
									{
										++mNumberOfNullDrawCalls;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void RenderQueue::fillGraphicsCommandBuffer(const RERHI::RHIRenderTarget& renderTarget, const CompositorContextData& compositorContextData, RERHI::RHICommandBuffer& commandBuffer)
	{
		// Sanity check
		RHI_ASSERT(getNumberOfDrawCalls() > 0, "Don't call the fill command buffer method if there's no work to be done")
		RHI_ASSERT(mScratchCommandBuffer.isEmpty(), "Scratch command buffer should be empty at this point in time")

		// No combined scoped profiler CPU and GPU sample as well as renderer debug event command by intent, this is something the caller has to take care of
		// RENDERER_SCOPED_PROFILER_EVENT(mRenderer.getContext(), commandBuffer, "Graphics render queue")

		// TODO(naetherm) This is just a dummy implementation. For example automatic instancing has to be incorporated as well as more efficient buffer management.
		const MaterialBlueprintResourceManager& materialBlueprintResourceManager = mRenderer.getMaterialBlueprintResourceManager();
		UniformInstanceBufferManager& uniformInstanceBufferManager = materialBlueprintResourceManager.getUniformInstanceBufferManager();
		TextureInstanceBufferManager& textureInstanceBufferManager = materialBlueprintResourceManager.getTextureInstanceBufferManager();
		LightBufferManager& lightBufferManager = materialBlueprintResourceManager.getLightBufferManager();
		const RECore::uint32 instanceCount = (compositorContextData.getSinglePassStereoInstancing() ? 2u : 1u);

		// Process all render queues
		// -> When adding renderables from renderable manager we could build up a minimum/maximum used render queue index to sometimes reduce
		//    the number of iterations. On the other hand, there are usually much more renderables added as iterations in here so this possible
		//    optimization might be a fact a performance degeneration while at the same time increasing the code complexity. So, not implemented by intent.
		if (mQueues.size() == 1 && mQueues[0].queuedRenderables.size() == 1)
		{
			// Get queued renderable data
			const QueuedRenderable&			   queuedRenderable			   = mQueues[0].queuedRenderables[0];
			const Renderable&				   renderable				   = *queuedRenderable.renderable;
			const MaterialResource&			   materialResource			   = *queuedRenderable.materialResource;
				  MaterialTechnique&		   materialTechnique		   = *queuedRenderable.materialTechnique;
				  MaterialBlueprintResource&   materialBlueprintResource   = *queuedRenderable.materialBlueprintResource;
				  RERHI::RHIGraphicsPipelineState& foundGraphicsPipelineState  = *static_cast<RERHI::RHIGraphicsPipelineState*>(queuedRenderable.foundPipelineState);
			compositorContextData.mCurrentlyBoundMaterialBlueprintResource = &materialBlueprintResource;

			// Set the used graphics pipeline state object (PSO)
			RERHI::Command::SetGraphicsPipelineState::create(commandBuffer, &foundGraphicsPipelineState);

			// Setup input assembly (IA): Set the used vertex array
			RERHI::Command::SetGraphicsVertexArray::create(commandBuffer, mPositionOnlyPass ? renderable.getPositionOnlyVertexArrayPtrWithFallback() : renderable.getVertexArrayPtr());

			{ // Fill the pass buffer manager
				PassBufferManager* passBufferManager = materialBlueprintResource.getPassBufferManager();
				if (nullptr != passBufferManager)
				{
					passBufferManager->fillBuffer(&renderTarget, compositorContextData, materialResource);
				}
			}

			// Bind the graphics material blueprint resource and instance and light buffer manager to the used RHI
			materialBlueprintResource.fillGraphicsCommandBuffer(commandBuffer);
			const MaterialBlueprintResource::UniformBuffer* instanceUniformBuffer = materialBlueprintResource.getInstanceUniformBuffer();
			const MaterialBlueprintResource::TextureBuffer* instanceTextureBuffer = materialBlueprintResource.getInstanceTextureBuffer();
			if (nullptr != instanceTextureBuffer)
			{
				RHI_ASSERT(nullptr != instanceUniformBuffer, "Invalid instance uniform buffer")
				textureInstanceBufferManager.startupBufferFilling(materialBlueprintResource, commandBuffer);
			}
			else if (nullptr != instanceUniformBuffer)
			{
				uniformInstanceBufferManager.startupBufferFilling(materialBlueprintResource, commandBuffer);
			}
			lightBufferManager.fillGraphicsCommandBuffer(materialBlueprintResource, commandBuffer);

			{ // Cheap state change: Bind the material technique to the used RHI
				RECore::uint32 resourceGroupRootParameterIndex = RECore::getInvalid<RECore::uint32>();
				RERHI::RHIResourceGroup* resourceGroup = nullptr;
				materialTechnique.fillGraphicsCommandBuffer(mRenderer, commandBuffer, resourceGroupRootParameterIndex, &resourceGroup);
				if (RECore::isValid(resourceGroupRootParameterIndex) && nullptr != resourceGroup)
				{
					RERHI::Command::SetGraphicsResourceGroup::create(commandBuffer, resourceGroupRootParameterIndex, resourceGroup);
				}
			}

			// Fill the instance buffer manager
			RECore::uint32 startInstanceLocation = 0;
			if (nullptr != instanceTextureBuffer)
			{
				RHI_ASSERT(nullptr != instanceUniformBuffer, "Invalid instance uniform buffer")
				startInstanceLocation = textureInstanceBufferManager.fillBuffer(compositorContextData.getWorldSpaceCameraPosition(), materialBlueprintResource, materialBlueprintResource.getPassBufferManager(), *instanceUniformBuffer, renderable, materialTechnique, commandBuffer);
			}
			else if (nullptr != instanceUniformBuffer)
			{
				startInstanceLocation = uniformInstanceBufferManager.fillBuffer(materialBlueprintResource, materialBlueprintResource.getPassBufferManager(), *instanceUniformBuffer, renderable, materialTechnique, commandBuffer);
			}

			// Render the specified geometric primitive, based on indexing into an array of vertices
			// -> Please note that it's valid that there are no indices, for example "RERenderer::CompositorInstancePassDebugGui" is using the render queue only to set the material resource blueprint
			if (0 != renderable.getNumberOfIndices())
			{
				// Fill indirect buffer
				if (renderable.getDrawIndexed())
				{
					RERHI::Command::DrawIndexedGraphics::create(commandBuffer, renderable.getNumberOfIndices(), instanceCount * renderable.getInstanceCount(), renderable.getStartIndexLocation(), 0, startInstanceLocation);
				}
				else
				{
					RERHI::Command::DrawGraphics::create(commandBuffer, renderable.getNumberOfIndices(), instanceCount * renderable.getInstanceCount(), renderable.getStartIndexLocation(), startInstanceLocation);
				}
			}
		}
		else
		{
			// Track currently bound RHI resources and states to void generating redundant commands
			bool vertexArraySet = false;
			RERHI::RHIVertexArray* currentVertexArray = nullptr;
			RERHI::RHIGraphicsPipelineState* currentGraphicsPipelineState = nullptr;

			// We try to minimize state changes across multiple render queue fill command buffer calls, but while doing so we still need to take into account
			// that pass data like world space to clip space transform might have been changed and needs to be updated inside the pass uniform buffer
			bool enforcePassBufferManagerFillBuffer = true;

			// Get indirect buffer
			RERHI::RHIIndirectBuffer* indirectBuffer = nullptr;
			RECore::uint32 indirectBufferOffset = 0;
			RECore::uint8* indirectBufferData = nullptr;
			if (mNumberOfDrawIndexedCalls > 0 || mNumberOfDrawCalls > 0 )
			{
				IndirectBufferManager::IndirectBuffer* managedIndirectBuffer = mIndirectBufferManager.getIndirectBuffer(sizeof(RERHI::DrawIndexedArguments) * mNumberOfDrawIndexedCalls + sizeof(RERHI::DrawArguments) * mNumberOfDrawCalls);
				RHI_ASSERT(nullptr != managedIndirectBuffer, "Invalid managed indirect buffer")
				indirectBuffer		 = managedIndirectBuffer->indirectBuffer;
				indirectBufferOffset = managedIndirectBuffer->indirectBufferOffset;
				indirectBufferData   = managedIndirectBuffer->mappedData;
			}

			// For gathering multi-draw-indirect data
			std::array<RERHI::RHIResourceGroup*, 16> currentSetGraphicsResourceGroup;	// TODO(naetherm) Use maximum number of graphics resource groups here, 16 is considered a save number of root parameters
			RECore::uint32 currentDrawIndirectBufferOffset = indirectBufferOffset;
			RECore::uint32 currentNumberOfDraws = 0;
			bool currentDrawIndexed = false;

			// Process queues
			for (Queue& queue : mQueues)
			{
				QueuedRenderables& queuedRenderables = queue.queuedRenderables;
				if (!queuedRenderables.empty())
				{
					// Sort queued renderables
					if (!queue.sorted && mDoSort)
					{
						// TODO(naetherm) Exploit temporal coherence across frames then use insertion sorts as explained by L. Spiro in
						// http://www.gamedev.net/topic/661114-temporal-coherence-and-render-queue-sorting/?view=findpost&p=5181408
						// Keep a list of sorted indices from the previous frame (one per camera).
						// If we have the sorted list "5, 1, 4, 3, 2, 0":
						// * If it grew from last frame, append: 5, 1, 4, 3, 2, 0, 6, 7 and use insertion sort.
						// * If it's the same, leave it as is, and use insertion sort just in case.
						// * If it's shorter, reset the indices 0, 1, 2, 3, 4; probably use quicksort or other generic sort
						// TODO(naetherm) Use radix sort? ( https://www.quora.com/What-is-the-most-efficient-way-to-sort-a-million-32-bit-integers )
						std::sort(queuedRenderables.begin(), queuedRenderables.end());
						queue.sorted = true;
					}

					// Inject queued renderables into the RHI
					for (const QueuedRenderable& queuedRenderable : queuedRenderables)
					{
						RHI_ASSERT(nullptr != queuedRenderable.renderable, "Invalid renderable")

						// Get queued renderable data
						const Renderable&				   renderable				  = *queuedRenderable.renderable;
						const MaterialResource&			   materialResource			  = *queuedRenderable.materialResource;
							  MaterialTechnique&		   materialTechnique		  = *queuedRenderable.materialTechnique;
							  MaterialBlueprintResource&   materialBlueprintResource  = *queuedRenderable.materialBlueprintResource;
							  RERHI::RHIGraphicsPipelineState& foundGraphicsPipelineState = *static_cast<RERHI::RHIGraphicsPipelineState*>(queuedRenderable.foundPipelineState);

						// Set the used graphics pipeline state object (PSO)
						if (currentGraphicsPipelineState != &foundGraphicsPipelineState)
						{
							currentGraphicsPipelineState = &foundGraphicsPipelineState;
							RERHI::Command::SetGraphicsPipelineState::create(mScratchCommandBuffer, currentGraphicsPipelineState);
						}

						{ // Setup input assembly (IA): Set the used vertex array
							const RERHI::RHIVertexArrayPtr& vertexArrayPtr = mPositionOnlyPass ? renderable.getPositionOnlyVertexArrayPtrWithFallback() : renderable.getVertexArrayPtr();
							if (!vertexArraySet || currentVertexArray != vertexArrayPtr)
							{
								vertexArraySet = true;
								currentVertexArray = vertexArrayPtr;
								RERHI::Command::SetGraphicsVertexArray::create(mScratchCommandBuffer, currentVertexArray);
							}
						}

						// Expensive state change: Handle material blueprint resource switches
						// -> Render queue should be sorted by material blueprint resource first to reduce those expensive state changes
						bool bindMaterialBlueprint = false;
						PassBufferManager* passBufferManager = nullptr;
						const MaterialBlueprintResource::UniformBuffer* instanceUniformBuffer = materialBlueprintResource.getInstanceUniformBuffer();
						const MaterialBlueprintResource::TextureBuffer* instanceTextureBuffer = materialBlueprintResource.getInstanceTextureBuffer();
						if (compositorContextData.mCurrentlyBoundMaterialBlueprintResource != &materialBlueprintResource)
						{
							compositorContextData.mCurrentlyBoundMaterialBlueprintResource = &materialBlueprintResource;
							std::fill(currentSetGraphicsResourceGroup.begin(), currentSetGraphicsResourceGroup.end(), nullptr);
							bindMaterialBlueprint = true;
						}
						if (bindMaterialBlueprint || enforcePassBufferManagerFillBuffer)
						{
							// Fill the pass buffer manager
							passBufferManager = materialBlueprintResource.getPassBufferManager();
							if (nullptr != passBufferManager)
							{
								passBufferManager->fillBuffer(&renderTarget, compositorContextData, materialResource);
								enforcePassBufferManagerFillBuffer = false;
							}
						}
						if (bindMaterialBlueprint)
						{
							// Bind the graphics material blueprint resource and instance and light buffer manager to the used RHI
							materialBlueprintResource.fillGraphicsCommandBuffer(mScratchCommandBuffer);
							if (nullptr != instanceTextureBuffer)
							{
								RHI_ASSERT(nullptr != instanceUniformBuffer, "Invalid instance uniform buffer")
								textureInstanceBufferManager.startupBufferFilling(materialBlueprintResource, mScratchCommandBuffer);
							}
							else if (nullptr != instanceUniformBuffer)
							{
								uniformInstanceBufferManager.startupBufferFilling(materialBlueprintResource, mScratchCommandBuffer);
							}
							lightBufferManager.fillGraphicsCommandBuffer(materialBlueprintResource, mScratchCommandBuffer);
						}
						else if (nullptr != passBufferManager)
						{
							// Bind pass buffer manager since we filled the buffer
							passBufferManager->fillGraphicsCommandBuffer(mScratchCommandBuffer);
						}

						{ // Cheap state change: Bind the material technique to the used RHI
							RECore::uint32 resourceGroupRootParameterIndex = RECore::getInvalid<RECore::uint32>();
							RERHI::RHIResourceGroup* resourceGroup = nullptr;
							materialTechnique.fillGraphicsCommandBuffer(mRenderer, mScratchCommandBuffer, resourceGroupRootParameterIndex, &resourceGroup);
							if (RECore::isValid(resourceGroupRootParameterIndex) && nullptr != resourceGroup && currentSetGraphicsResourceGroup[resourceGroupRootParameterIndex] != resourceGroup)
							{
								currentSetGraphicsResourceGroup[resourceGroupRootParameterIndex] = resourceGroup;
								RERHI::Command::SetGraphicsResourceGroup::create(mScratchCommandBuffer, resourceGroupRootParameterIndex, resourceGroup);
							}
						}

						// Fill the instance buffer manager
						RECore::uint32 startInstanceLocation = 0;
						if (nullptr != instanceTextureBuffer)
						{
							RHI_ASSERT(nullptr != instanceUniformBuffer, "Invalid instance uniform buffer")
							startInstanceLocation = textureInstanceBufferManager.fillBuffer(compositorContextData.getWorldSpaceCameraPosition(), materialBlueprintResource, materialBlueprintResource.getPassBufferManager(), *instanceUniformBuffer, renderable, materialTechnique, mScratchCommandBuffer);
						}
						else if (nullptr != instanceUniformBuffer)
						{
							startInstanceLocation = uniformInstanceBufferManager.fillBuffer(materialBlueprintResource, materialBlueprintResource.getPassBufferManager(), *instanceUniformBuffer, renderable, materialTechnique, mScratchCommandBuffer);
						}

						// Emit draw command, if necessary
						const RERHI::RHIIndirectBufferPtr& renderableIndirectBufferPtr = renderable.getIndirectBufferPtr();
						if (renderable.getDrawIndexed() != currentDrawIndexed || !mScratchCommandBuffer.isEmpty() || nullptr != renderableIndirectBufferPtr)
						{
							if (currentDrawIndexed)
							{
								if (currentNumberOfDraws)
								{
									RERHI::Command::DrawIndexedGraphics::create(commandBuffer, *indirectBuffer, currentDrawIndirectBufferOffset, currentNumberOfDraws);
									currentNumberOfDraws = 0;
								}
							}
							else if (currentNumberOfDraws)
							{
								RERHI::Command::DrawGraphics::create(commandBuffer, *indirectBuffer, currentDrawIndirectBufferOffset, currentNumberOfDraws);
								currentNumberOfDraws = 0;
							}
							currentDrawIndirectBufferOffset = indirectBufferOffset;
						}

						// Append scratch command buffer into the main command buffer
						if (!mScratchCommandBuffer.isEmpty())
						{
							mScratchCommandBuffer.appendToCommandBufferAndClear(commandBuffer);
						}

						// Render the specified geometric primitive, based on indexing into an array of vertices
						if (nullptr != renderableIndirectBufferPtr)
						{
							// Use a given indirect buffer which content is e.g. filled by a compute shader
							if (renderable.getDrawIndexed())
							{
								RERHI::Command::DrawIndexedGraphics::create(commandBuffer, *renderableIndirectBufferPtr, renderable.getIndirectBufferOffset(), renderable.getNumberOfDraws());
							}
							else
							{
								RERHI::Command::DrawGraphics::create(commandBuffer, *renderableIndirectBufferPtr, renderable.getIndirectBufferOffset(), renderable.getNumberOfDraws());
							}
						}
						// Please note that it's valid that there are no indices, for example "RERenderer::CompositorInstancePassDebugGui" is using the render queue only to set the material resource blueprint
						else if (0 != renderable.getNumberOfIndices())
						{
							// Sanity checks
							RHI_ASSERT(nullptr != indirectBuffer, "Invalid indirect buffer")
							RHI_ASSERT(nullptr != indirectBuffer, "Invalid indirect buffer data")

							// Fill indirect buffer
							if (renderable.getDrawIndexed())
							{
								// Fill indirect buffer
								RERHI::DrawIndexedArguments* drawIndexedArguments = reinterpret_cast<RERHI::DrawIndexedArguments*>(indirectBufferData + indirectBufferOffset);
								drawIndexedArguments->indexCountPerInstance	= renderable.getNumberOfIndices();
								drawIndexedArguments->instanceCount			= instanceCount * renderable.getInstanceCount();
								drawIndexedArguments->startIndexLocation	= renderable.getStartIndexLocation();
								drawIndexedArguments->baseVertexLocation	= 0;
								drawIndexedArguments->startInstanceLocation	= startInstanceLocation;

								// Advance indirect buffer offset
								indirectBufferOffset += sizeof(RERHI::DrawIndexedArguments);
								currentDrawIndexed = true;
							}
							else
							{
								// Fill indirect buffer
								RERHI::DrawArguments* drawArguments = reinterpret_cast<RERHI::DrawArguments*>(indirectBufferData + indirectBufferOffset);
								drawArguments->vertexCountPerInstance = renderable.getNumberOfIndices();
								drawArguments->instanceCount		  = instanceCount * renderable.getInstanceCount();
								drawArguments->startVertexLocation	  = renderable.getStartIndexLocation();
								drawArguments->startInstanceLocation  = startInstanceLocation;

								// Advance indirect buffer offset
								indirectBufferOffset += sizeof(RERHI::DrawArguments);
								currentDrawIndexed = false;
							}
							++currentNumberOfDraws;
						}
					}
				}
			}

			// Emit last open draw command, if necessary
			if (currentNumberOfDraws)
			{
				if (currentDrawIndexed)
				{
					RERHI::Command::DrawIndexedGraphics::create(commandBuffer, *indirectBuffer, currentDrawIndirectBufferOffset, currentNumberOfDraws);
				}
				else
				{
					RERHI::Command::DrawGraphics::create(commandBuffer, *indirectBuffer, currentDrawIndirectBufferOffset, currentNumberOfDraws);
				}
			}
		}
	}

	void RenderQueue::fillComputeCommandBuffer(const CompositorContextData& compositorContextData, RERHI::RHICommandBuffer& commandBuffer)
	{
		// Sanity check
		RHI_ASSERT(getNumberOfDrawCalls() > 0, "Don't call the fill command buffer method if there's no work to be done")
		RHI_ASSERT(mScratchCommandBuffer.isEmpty(), "Scratch command buffer should be empty at this point in time")

		// No combined scoped profiler CPU and GPU sample as well as renderer debug event command by intent, this is something the caller has to take care of
		// RENDERER_SCOPED_PROFILER_EVENT(mRenderer.getContext(), commandBuffer, "Compute render queue")

		// TODO(naetherm) This is just a dummy implementation. For example automatic instancing has to be incorporated as well as more efficient buffer management.
		const TextureResourceManager& textureResourceManager = mRenderer.getTextureResourceManager();
		const MaterialBlueprintResourceManager& materialBlueprintResourceManager = mRenderer.getMaterialBlueprintResourceManager();
		// TextureInstanceBufferManager& textureInstanceBufferManager = materialBlueprintResourceManager.getTextureInstanceBufferManager();	// TODO(naetherm) Think about compute instance buffer support
		LightBufferManager& lightBufferManager = materialBlueprintResourceManager.getLightBufferManager();

		// Process all render queues
		// -> When adding renderables from renderable manager we could build up a minimum/maximum used render queue index to sometimes reduce
		//    the number of iterations. On the other hand, there are usually much more renderables added as iterations in here so this possible
		//    optimization might be a fact a performance degeneration while at the same time increasing the code complexity. So, not implemented by intent.
		if (mQueues.size() == 1 && mQueues[0].queuedRenderables.size() == 1)
		{
			// Get queued renderable data
			const QueuedRenderable&			  queuedRenderable			= mQueues[0].queuedRenderables[0];
			const MaterialResource&			  materialResource			= *queuedRenderable.materialResource;
				  MaterialBlueprintResource&  materialBlueprintResource = *queuedRenderable.materialBlueprintResource;
				  RERHI::RHIComputePipelineState& foundComputePipelineState = *static_cast<RERHI::RHIComputePipelineState*>(queuedRenderable.foundPipelineState);
			compositorContextData.mCurrentlyBoundMaterialBlueprintResource = &materialBlueprintResource;

			// Determine group count for dispatch compute
			RECore::uint32 groupCountX = 0;
			RECore::uint32 groupCountY = 0;
			RECore::uint32 groupCountZ = 0;
			{
				// Use mandatory fixed build in material property "LocalComputeSize" for the compute shader local size (also known as number of threads)
				const MaterialProperty* materialProperty = materialResource.getPropertyById(MaterialResource::LOCAL_COMPUTE_SIZE_PROPERTY_ID);
				RHI_ASSERT(nullptr != materialProperty, "Invalid material property")
				RHI_ASSERT(materialProperty->getUsage() == MaterialProperty::Usage::STATIC, "Invalid material property usage")
				const int* localComputeSizeInteger3Value = materialProperty->getInteger3Value();

				// Use mandatory fixed build in material property "GlobalComputeSize" for the compute shader global size
				materialProperty = materialResource.getPropertyById(MaterialResource::GLOBAL_COMPUTE_SIZE_PROPERTY_ID);
				RHI_ASSERT(nullptr != materialProperty, "Invalid material property")
				RHI_ASSERT(materialProperty->getUsage() == MaterialProperty::Usage::STATIC || materialProperty->getUsage() == MaterialProperty::Usage::MATERIAL_REFERENCE, "Invalid material property usage")
				compositorContextData.mGlobalComputeSize[0] = 1;
				compositorContextData.mGlobalComputeSize[1] = 1;
				compositorContextData.mGlobalComputeSize[2] = 1;
				if (materialProperty->getUsage() == MaterialProperty::Usage::STATIC)
				{
					// Static value
					const int* globalComputeSizeInteger3Value = materialProperty->getInteger3Value();
					compositorContextData.mGlobalComputeSize[0] = static_cast<RECore::uint32>(globalComputeSizeInteger3Value[0]);
					compositorContextData.mGlobalComputeSize[1] = static_cast<RECore::uint32>(globalComputeSizeInteger3Value[1]);
					compositorContextData.mGlobalComputeSize[2] = static_cast<RECore::uint32>(globalComputeSizeInteger3Value[2]);
				}
				else
				{
					// Material property reference
					const MaterialPropertyId materialPropertyId = materialProperty->getReferenceValue();
					materialProperty = materialResource.getPropertyById(materialPropertyId);
					RHI_ASSERT(materialProperty->getValueType() == MaterialPropertyValue::ValueType::TEXTURE_ASSET_ID, "Invalid material property value type")
					RHI_ASSERT(materialProperty->getUsage() == MaterialProperty::Usage::TEXTURE_REFERENCE, "Invalid material property usage")
					const TextureResource* textureResource = textureResourceManager.getTextureResourceByAssetId(materialProperty->getTextureAssetIdValue());
					RHI_ASSERT(nullptr != textureResource, "Invalid texture resource")
					const RERHI::RHITexture* texture = textureResource->getTexturePtr();
					RHI_ASSERT(nullptr != texture, "Invalid texture")
					switch (texture->getResourceType())
					{
						case RERHI::ResourceType::TEXTURE_1D:
							compositorContextData.mGlobalComputeSize[0] = static_cast<const RERHI::RHITexture1D*>(texture)->getWidth();
							break;

						case RERHI::ResourceType::TEXTURE_1D_ARRAY:
							compositorContextData.mGlobalComputeSize[0] = static_cast<const RERHI::RHITexture1DArray*>(texture)->getWidth();
							break;

						case RERHI::ResourceType::TEXTURE_2D:
						{
							const RERHI::RHITexture2D* texture2D = static_cast<const RERHI::RHITexture2D*>(texture);
							compositorContextData.mGlobalComputeSize[0] = texture2D->getWidth();
							compositorContextData.mGlobalComputeSize[1] = texture2D->getHeight();
							break;
						}

						case RERHI::ResourceType::TEXTURE_2D_ARRAY:
						{
							const RERHI::RHITexture2DArray* texture2DArray = static_cast<const RERHI::RHITexture2DArray*>(texture);
							compositorContextData.mGlobalComputeSize[0] = texture2DArray->getWidth();
							compositorContextData.mGlobalComputeSize[1] = texture2DArray->getHeight();
							break;
						}

						case RERHI::ResourceType::TEXTURE_3D:
						{
							const RERHI::RHITexture3D* texture3D = static_cast<const RERHI::RHITexture3D*>(texture);
							compositorContextData.mGlobalComputeSize[0] = texture3D->getWidth();
							compositorContextData.mGlobalComputeSize[1] = texture3D->getHeight();
							compositorContextData.mGlobalComputeSize[2] = texture3D->getDepth();
							break;
						}

						case RERHI::ResourceType::TEXTURE_CUBE:
						{
							const RERHI::RHITextureCube* textureCube = static_cast<const RERHI::RHITextureCube*>(texture);
							compositorContextData.mGlobalComputeSize[0] = textureCube->getWidth();
							compositorContextData.mGlobalComputeSize[1] = textureCube->getWidth();
							compositorContextData.mGlobalComputeSize[2] = 6;	// TODO(naetherm) Or better 1?
							break;
						}

						case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
						{
							const RERHI::RHITextureCubeArray* textureCubeArray = static_cast<const RERHI::RHITextureCubeArray*>(texture);
							compositorContextData.mGlobalComputeSize[0] = textureCubeArray->getWidth();
							compositorContextData.mGlobalComputeSize[1] = textureCubeArray->getWidth();
							compositorContextData.mGlobalComputeSize[2] = 6;	// TODO(naetherm) Or better 1?
							break;
						}

						case RERHI::ResourceType::ROOT_SIGNATURE:
						case RERHI::ResourceType::RESOURCE_GROUP:
						case RERHI::ResourceType::GRAPHICS_PROGRAM:
						case RERHI::ResourceType::VERTEX_ARRAY:
						case RERHI::ResourceType::RENDER_PASS:
						case RERHI::ResourceType::QUERY_POOL:
						case RERHI::ResourceType::SWAP_CHAIN:
						case RERHI::ResourceType::FRAMEBUFFER:
						case RERHI::ResourceType::VERTEX_BUFFER:
						case RERHI::ResourceType::INDEX_BUFFER:
						case RERHI::ResourceType::TEXTURE_BUFFER:
						case RERHI::ResourceType::STRUCTURED_BUFFER:
						case RERHI::ResourceType::INDIRECT_BUFFER:
						case RERHI::ResourceType::UNIFORM_BUFFER:
						case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
						case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
						case RERHI::ResourceType::SAMPLER_STATE:
						case RERHI::ResourceType::VERTEX_SHADER:
						case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
						case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
						case RERHI::ResourceType::GEOMETRY_SHADER:
						case RERHI::ResourceType::FRAGMENT_SHADER:
						case RERHI::ResourceType::TASK_SHADER:
						case RERHI::ResourceType::MESH_SHADER:
						case RERHI::ResourceType::COMPUTE_SHADER:
						default:
							// Error!
							RHI_ASSERT(false, "We should never end up in here")
							break;
					}
				}

				// Determine group count
				groupCountX = static_cast<RECore::uint32>(std::ceil(static_cast<float>(compositorContextData.mGlobalComputeSize[0]) / static_cast<float>(localComputeSizeInteger3Value[0])));
				groupCountY = static_cast<RECore::uint32>(std::ceil(static_cast<float>(compositorContextData.mGlobalComputeSize[1]) / static_cast<float>(localComputeSizeInteger3Value[1])));
				groupCountZ = static_cast<RECore::uint32>(std::ceil(static_cast<float>(compositorContextData.mGlobalComputeSize[2]) / static_cast<float>(localComputeSizeInteger3Value[2])));
			}

			// Set the used compute pipeline state object (PSO)
			RERHI::Command::SetComputePipelineState::create(commandBuffer, &foundComputePipelineState);

			{ // Fill the pass buffer manager
				PassBufferManager* passBufferManager = materialBlueprintResource.getPassBufferManager();
				if (nullptr != passBufferManager)
				{
					passBufferManager->fillBuffer(nullptr, compositorContextData, materialResource);
				}
			}

			// Bind the compute material blueprint resource and instance and light buffer manager to the used RHI
			materialBlueprintResource.fillComputeCommandBuffer(commandBuffer);
			const MaterialBlueprintResource::UniformBuffer* instanceUniformBuffer = materialBlueprintResource.getInstanceUniformBuffer();
			if (nullptr != instanceUniformBuffer)
			{
				// TODO(naetherm) Think about compute instance buffer support
				RHI_ASSERT(false, "We should never end up in here")
				// textureInstanceBufferManager.startupBufferFilling(*materialBlueprintResource, commandBuffer);
			}
			lightBufferManager.fillComputeCommandBuffer(materialBlueprintResource, commandBuffer);

			{ // Cheap state change: Bind the material technique to the used RHI
				RECore::uint32 resourceGroupRootParameterIndex = RECore::getInvalid<RECore::uint32>();
				RERHI::RHIResourceGroup* resourceGroup = nullptr;
				queuedRenderable.materialTechnique->fillComputeCommandBuffer(mRenderer, commandBuffer, resourceGroupRootParameterIndex, &resourceGroup);
				if (RECore::isValid(resourceGroupRootParameterIndex) && nullptr != resourceGroup)
				{
					RERHI::Command::SetComputeResourceGroup::create(commandBuffer, resourceGroupRootParameterIndex, resourceGroup);
				}
			}

			// Fill the instance buffer manager
			// TODO(naetherm) Think about compute instance buffer support
			// [[maybe_unused]] const RECore::uint32 startInstanceLocation = (nullptr != instanceUniformBuffer) ? textureInstanceBufferManager.fillBuffer(materialBlueprintResource, materialBlueprintResource.getPassBufferManager(), *instanceUniformBuffer, renderable, *materialTechnique, commandBuffer) : 0;

			// Dispatch compute
			RERHI::Command::DispatchCompute::create(commandBuffer, groupCountX, groupCountY, groupCountZ);
		}
		else
		{
			RHI_ASSERT(false, "We should never end up in here")
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
