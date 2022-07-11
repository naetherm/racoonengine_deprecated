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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIVulkan/RHIResourceGroup.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/RHIRootSignature.h"
#include "RERHIVulkan/Buffer/RHIIndexBuffer.h"
#include "RERHIVulkan/Buffer/RHIIndirectBuffer.h"
#include "RERHIVulkan/Buffer/RHIVertexBuffer.h"
#include "RERHIVulkan/Buffer/RHIVertexArray.h"
#include "RERHIVulkan/Buffer/RHITextureBuffer.h"
#include "RERHIVulkan/Buffer/RHIStructuredBuffer.h"
#include "RERHIVulkan/Buffer/RHIUniformBuffer.h"
#include "RERHIVulkan/Texture/RHITexture1D.h"
#include "RERHIVulkan/Texture/RHITexture1DArray.h"
#include "RERHIVulkan/Texture/RHITexture2D.h"
#include "RERHIVulkan/Texture/RHITexture2DArray.h"
#include "RERHIVulkan/Texture/RHITexture3D.h"
#include "RERHIVulkan/Texture/RHITextureCubeArray.h"
#include "RERHIVulkan/Texture/RHITextureCube.h"
#include "RERHIVulkan/State/RHISamplerState.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

ResourceGroup::ResourceGroup(RootSignature &rootSignature, RECore::uint32 rootParameterIndex, VkDescriptorSet vkDescriptorSet,
                             RECore::uint32 numberOfResources, RERHI::RHIResource **resources,
                             RERHI::RHISamplerState **samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHIResourceGroup(rootSignature.getRhi() RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mRootSignature(rootSignature),
  mVkDescriptorSet(vkDescriptorSet),
  mNumberOfResources(numberOfResources),
  mResources(RHI_MALLOC_TYPED(rootSignature.getRhi().getContext(), RERHI::RHIResource*, mNumberOfResources)),
  mSamplerStates(nullptr) {
  mRootSignature.AddReference();

  // Process all resources and add our reference to the RHI resource
  const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
  if (nullptr != samplerStates) {
    mSamplerStates = RHI_MALLOC_TYPED(vulkanRhi.getContext(), RERHI::RHISamplerState*, mNumberOfResources);
    for (RECore::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
      RERHI::RHISamplerState *samplerState = mSamplerStates[resourceIndex] = samplerStates[resourceIndex];
      if (nullptr != samplerState) {
        samplerState->AddReference();
      }
    }
  }
  for (RECore::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex, ++resources) {
    RERHI::RHIResource *resource = *resources;
    RHI_ASSERT(nullptr != resource, "Invalid Vulkan resource")
    mResources[resourceIndex] = resource;
    resource->AddReference();

    // Check the type of resource to set
    // TODO(naetherm) Some additional resource type root signature security checks in debug build?
    const RERHI::ResourceType resourceType = resource->getResourceType();
    switch (resourceType) {
      case RERHI::ResourceType::VERTEX_BUFFER: {
        const VkDescriptorBufferInfo vkDescriptorBufferInfo =
          {
            static_cast<VertexBuffer *>(resource)->getVkBuffer(),  // buffer (VkBuffer)
            0,                            // offset (VkDeviceSize)
            VK_WHOLE_SIZE                      // range (VkDeviceSize)
          };
        const VkWriteDescriptorSet vkWriteDescriptorSet =
          {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,    // sType (VkStructureType)
            nullptr,                  // pNext (const void*)
            mVkDescriptorSet,              // dstSet (VkDescriptorSet)
            resourceIndex,                // dstBinding (RECore::uint32)
            0,                      // dstArrayElement (RECore::uint32)
            1,                      // descriptorCount (RECore::uint32)
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,      // descriptorType (VkDescriptorType)
            nullptr,                  // pImageInfo (const VkDescriptorImageInfo*)
            &vkDescriptorBufferInfo,          // pBufferInfo (const VkDescriptorBufferInfo*)
            nullptr                    // pTexelBufferView (const VkBufferView*)
          };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case RERHI::ResourceType::INDEX_BUFFER: {
        const VkDescriptorBufferInfo vkDescriptorBufferInfo =
          {
            static_cast<IndexBuffer *>(resource)->getVkBuffer(),  // buffer (VkBuffer)
            0,                          // offset (VkDeviceSize)
            VK_WHOLE_SIZE                    // range (VkDeviceSize)
          };
        const VkWriteDescriptorSet vkWriteDescriptorSet =
          {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,    // sType (VkStructureType)
            nullptr,                  // pNext (const void*)
            mVkDescriptorSet,              // dstSet (VkDescriptorSet)
            resourceIndex,                // dstBinding (RECore::uint32)
            0,                      // dstArrayElement (RECore::uint32)
            1,                      // descriptorCount (RECore::uint32)
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,      // descriptorType (VkDescriptorType)
            nullptr,                  // pImageInfo (const VkDescriptorImageInfo*)
            &vkDescriptorBufferInfo,          // pBufferInfo (const VkDescriptorBufferInfo*)
            nullptr                    // pTexelBufferView (const VkBufferView*)
          };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case RERHI::ResourceType::TEXTURE_BUFFER: {
        const RERHI::DescriptorRange &descriptorRange = reinterpret_cast<const RERHI::DescriptorRange *>(rootSignature.getRootSignature().parameters[rootParameterIndex].descriptorTable.descriptorRanges)[resourceIndex];
        RHI_ASSERT(RERHI::DescriptorRangeType::SRV == descriptorRange.rangeType ||
                   RERHI::DescriptorRangeType::UAV == descriptorRange.rangeType,
                   "Vulkan texture buffer must bound at SRV or UAV descriptor range type")
        const VkBufferView vkBufferView = static_cast<TextureBuffer *>(resource)->getVkBufferView();
        const VkWriteDescriptorSet vkWriteDescriptorSet =
          {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                                                        // sType (VkStructureType)
            nullptr,                                                                      // pNext (const void*)
            mVkDescriptorSet,                                                                  // dstSet (VkDescriptorSet)
            resourceIndex,                                                                    // dstBinding (RECore::uint32)
            0,                                                                          // dstArrayElement (RECore::uint32)
            1,                                                                          // descriptorCount (RECore::uint32)
            (RERHI::DescriptorRangeType::SRV == descriptorRange.rangeType) ? VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER
                                                                           : VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,  // descriptorType (VkDescriptorType)
            nullptr,                                                                      // pImageInfo (const VkDescriptorImageInfo*)
            nullptr,                                                                      // pBufferInfo (const VkDescriptorBufferInfo*)
            &vkBufferView                                                                    // pTexelBufferView (const VkBufferView*)
          };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case RERHI::ResourceType::STRUCTURED_BUFFER: {
        [[maybe_unused]] const RERHI::DescriptorRange &descriptorRange = reinterpret_cast<const RERHI::DescriptorRange *>(rootSignature.getRootSignature().parameters[rootParameterIndex].descriptorTable.descriptorRanges)[resourceIndex];
        RHI_ASSERT(RERHI::DescriptorRangeType::SRV == descriptorRange.rangeType ||
                   RERHI::DescriptorRangeType::UAV == descriptorRange.rangeType,
                   "Vulkan structured buffer must bound at SRV or UAV descriptor range type")
        const VkDescriptorBufferInfo vkDescriptorBufferInfo =
          {
            static_cast<StructuredBuffer *>(resource)->getVkBuffer(),  // buffer (VkBuffer)
            0,                              // offset (VkDeviceSize)
            VK_WHOLE_SIZE                        // range (VkDeviceSize)
          };
        const VkWriteDescriptorSet vkWriteDescriptorSet =
          {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,  // sType (VkStructureType)
            nullptr,                // pNext (const void*)
            mVkDescriptorSet,            // dstSet (VkDescriptorSet)
            resourceIndex,              // dstBinding (RECore::uint32)
            0,                    // dstArrayElement (RECore::uint32)
            1,                    // descriptorCount (RECore::uint32)
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,    // descriptorType (VkDescriptorType)
            nullptr,                // pImageInfo (const VkDescriptorImageInfo*)
            &vkDescriptorBufferInfo,        // pBufferInfo (const VkDescriptorBufferInfo*)
            nullptr                  // pTexelBufferView (const VkBufferView*)
          };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case RERHI::ResourceType::INDIRECT_BUFFER: {
        const VkDescriptorBufferInfo vkDescriptorBufferInfo =
          {
            static_cast<IndirectBuffer *>(resource)->getVkBuffer(),  // buffer (VkBuffer)
            0,                            // offset (VkDeviceSize)
            VK_WHOLE_SIZE                      // range (VkDeviceSize)
          };
        const VkWriteDescriptorSet vkWriteDescriptorSet =
          {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,    // sType (VkStructureType)
            nullptr,                  // pNext (const void*)
            mVkDescriptorSet,              // dstSet (VkDescriptorSet)
            resourceIndex,                // dstBinding (RECore::uint32)
            0,                      // dstArrayElement (RECore::uint32)
            1,                      // descriptorCount (RECore::uint32)
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,      // descriptorType (VkDescriptorType)
            nullptr,                  // pImageInfo (const VkDescriptorImageInfo*)
            &vkDescriptorBufferInfo,          // pBufferInfo (const VkDescriptorBufferInfo*)
            nullptr                    // pTexelBufferView (const VkBufferView*)
          };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case RERHI::ResourceType::UNIFORM_BUFFER: {
        const VkDescriptorBufferInfo vkDescriptorBufferInfo =
          {
            static_cast<UniformBuffer *>(resource)->getVkBuffer(),  // buffer (VkBuffer)
            0,                            // offset (VkDeviceSize)
            VK_WHOLE_SIZE                      // range (VkDeviceSize)
          };
        const VkWriteDescriptorSet vkWriteDescriptorSet =
          {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,  // sType (VkStructureType)
            nullptr,                // pNext (const void*)
            mVkDescriptorSet,            // dstSet (VkDescriptorSet)
            resourceIndex,              // dstBinding (RECore::uint32)
            0,                    // dstArrayElement (RECore::uint32)
            1,                    // descriptorCount (RECore::uint32)
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,    // descriptorType (VkDescriptorType)
            nullptr,                // pImageInfo (const VkDescriptorImageInfo*)
            &vkDescriptorBufferInfo,        // pBufferInfo (const VkDescriptorBufferInfo*)
            nullptr                  // pTexelBufferView (const VkBufferView*)
          };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case RERHI::ResourceType::TEXTURE_1D:
      case RERHI::ResourceType::TEXTURE_1D_ARRAY:
      case RERHI::ResourceType::TEXTURE_2D:
      case RERHI::ResourceType::TEXTURE_2D_ARRAY:
      case RERHI::ResourceType::TEXTURE_3D:
      case RERHI::ResourceType::TEXTURE_CUBE:
      case RERHI::ResourceType::TEXTURE_CUBE_ARRAY: {
        // Evaluate the texture type and get the Vulkan image view
        VkImageView vkImageView = VK_NULL_HANDLE;
        VkImageLayout vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        switch (resourceType) {
          case RERHI::ResourceType::TEXTURE_1D: {
            const Texture1D *texture1D = static_cast<Texture1D *>(resource);
            vkImageView = texture1D->getVkImageView();
            vkImageLayout = texture1D->getVkImageLayout();
            break;
          }

          case RERHI::ResourceType::TEXTURE_1D_ARRAY: {
            const Texture1DArray *texture1DArray = static_cast<Texture1DArray *>(resource);
            vkImageView = texture1DArray->getVkImageView();
            vkImageLayout = texture1DArray->getVkImageLayout();
            break;
          }

          case RERHI::ResourceType::TEXTURE_2D: {
            const Texture2D *texture2D = static_cast<Texture2D *>(resource);
            vkImageView = texture2D->getVkImageView();
            vkImageLayout = texture2D->getVkImageLayout();
            break;
          }

          case RERHI::ResourceType::TEXTURE_2D_ARRAY: {
            const Texture2DArray *texture2DArray = static_cast<Texture2DArray *>(resource);
            vkImageView = texture2DArray->getVkImageView();
            vkImageLayout = texture2DArray->getVkImageLayout();
            break;
          }

          case RERHI::ResourceType::TEXTURE_3D: {
            const Texture3D *texture3D = static_cast<Texture3D *>(resource);
            vkImageView = texture3D->getVkImageView();
            vkImageLayout = texture3D->getVkImageLayout();
            break;
          }

          case RERHI::ResourceType::TEXTURE_CUBE: {
            const TextureCube *textureCube = static_cast<TextureCube *>(resource);
            vkImageView = textureCube->getVkImageView();
            vkImageLayout = textureCube->getVkImageLayout();
            break;
          }

          case RERHI::ResourceType::TEXTURE_CUBE_ARRAY: {
            const TextureCubeArray *textureCubeArray = static_cast<TextureCubeArray *>(resource);
            vkImageView = textureCubeArray->getVkImageView();
            vkImageLayout = textureCubeArray->getVkImageLayout();
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
          case RERHI::ResourceType::INDIRECT_BUFFER:
          case RERHI::ResourceType::TEXTURE_BUFFER:
          case RERHI::ResourceType::STRUCTURED_BUFFER:
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
            RHI_ASSERT(false, "Invalid Vulkan RHI implementation resource type")
            break;
        }

        // Get the sampler state
        const SamplerState *samplerState = (nullptr != mSamplerStates)
                                           ? static_cast<const SamplerState *>(mSamplerStates[resourceIndex]) : nullptr;

        // Update Vulkan descriptor sets
        const VkDescriptorImageInfo vkDescriptorImageInfo =
          {
            (nullptr != samplerState) ? samplerState->getVkSampler() : VK_NULL_HANDLE,  // sampler (VkSampler)
            vkImageView,                                // imageView (VkImageView)
            vkImageLayout                                // imageLayout (VkImageLayout)
          };
        const VkWriteDescriptorSet vkWriteDescriptorSet =
          {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,                                    // sType (VkStructureType)
            nullptr,                                                  // pNext (const void*)
            mVkDescriptorSet,                                              // dstSet (VkDescriptorSet)
            resourceIndex,                                                // dstBinding (RECore::uint32)
            0,                                                      // dstArrayElement (RECore::uint32)
            1,                                                      // descriptorCount (RECore::uint32)
            (nullptr != samplerState) ? VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
                                      : VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,  // descriptorType (VkDescriptorType)
            &vkDescriptorImageInfo,                                            // pImageInfo (const VkDescriptorImageInfo*)
            nullptr,                                                  // pBufferInfo (const VkDescriptorBufferInfo*)
            nullptr                                                    // pTexelBufferView (const VkBufferView*)
          };
        vkUpdateDescriptorSets(vkDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
        break;
      }

      case RERHI::ResourceType::SAMPLER_STATE:
        // Nothing to do in here, Vulkan is using combined image samplers
        break;

      case RERHI::ResourceType::ROOT_SIGNATURE:
      case RERHI::ResourceType::RESOURCE_GROUP:
      case RERHI::ResourceType::GRAPHICS_PROGRAM:
      case RERHI::ResourceType::VERTEX_ARRAY:
      case RERHI::ResourceType::RENDER_PASS:
      case RERHI::ResourceType::QUERY_POOL:
      case RERHI::ResourceType::SWAP_CHAIN:
      case RERHI::ResourceType::FRAMEBUFFER:
      case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
      case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
      case RERHI::ResourceType::VERTEX_SHADER:
      case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
      case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
      case RERHI::ResourceType::GEOMETRY_SHADER:
      case RERHI::ResourceType::FRAGMENT_SHADER:
      case RERHI::ResourceType::TASK_SHADER:
      case RERHI::ResourceType::MESH_SHADER:
      case RERHI::ResourceType::COMPUTE_SHADER:
        RHI_ASSERT(false, "Invalid Vulkan RHI implementation resource type")
        break;
    }
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT)
        {
          RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Resource group", 17)	// 17 = "Resource group: " including terminating zero
          Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT, (RECore::uint64)mVkDescriptorSet, detailedDebugName);
        }
#endif
}

/**
*  @brief
*    Destructor
*/
ResourceGroup::~ResourceGroup() {
// Remove our reference from the RHI resources
  const RERHI::RHIContext &context = getRhi().getContext();
  if (nullptr != mSamplerStates) {
    for (RECore::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
      RERHI::RHISamplerState *samplerState = mSamplerStates[resourceIndex];
      if (nullptr != samplerState) {
        samplerState->Release();
      }
    }
    RHI_FREE(context, mSamplerStates);
  }
  for (RECore::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
    mResources[resourceIndex]->Release();
  }
  RHI_FREE(context, mResources);

// Free Vulkan descriptor set
  if (VK_NULL_HANDLE != mVkDescriptorSet) {
    vkFreeDescriptorSets(static_cast<RHIDynamicRHI &>(mRootSignature.getRhi()).getVulkanContext().getVkDevice(),
                         mRootSignature.getVkDescriptorPool(), 1, &mVkDescriptorSet);
  }
  mRootSignature.Release();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
