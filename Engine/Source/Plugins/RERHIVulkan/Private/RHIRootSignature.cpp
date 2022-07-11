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
#include "RERHIVulkan/RHIRootSignature.h"
#include "RERHIVulkan/RHIResourceGroup.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

RootSignature::RootSignature(RHIDynamicRHI &vulkanRhi, const RERHI::RootSignature &rootSignature
                             RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHIRootSignature(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mRootSignature(rootSignature),
  mVkPipelineLayout(VK_NULL_HANDLE),
  mVkDescriptorPool(VK_NULL_HANDLE) {
  static constexpr RECore::uint32 maxSets = 4242;  // TODO(naetherm) We probably need to get this provided from the outside

  // Copy the parameter data
  const RERHI::RHIContext &context = vulkanRhi.getContext();
  const RECore::uint32 numberOfRootParameters = mRootSignature.numberOfParameters;
  if (numberOfRootParameters > 0) {
    mRootSignature.parameters = RHI_MALLOC_TYPED(context, RERHI::RootParameter, numberOfRootParameters);
    RERHI::RootParameter *destinationRootParameters = const_cast<RERHI::RootParameter *>(mRootSignature.parameters);
    memcpy(destinationRootParameters, rootSignature.parameters, sizeof(RERHI::RootParameter) * numberOfRootParameters);

    // Copy the descriptor table data
    for (RECore::uint32 rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex) {
      RERHI::RootParameter &destinationRootParameter = destinationRootParameters[rootParameterIndex];
      const RERHI::RootParameter &sourceRootParameter = rootSignature.parameters[rootParameterIndex];
      if (RERHI::RootParameterType::DESCRIPTOR_TABLE == destinationRootParameter.parameterType) {
        const RECore::uint32 numberOfDescriptorRanges = destinationRootParameter.descriptorTable.numberOfDescriptorRanges;
        destinationRootParameter.descriptorTable.descriptorRanges = reinterpret_cast<uintptr_t>(RHI_MALLOC_TYPED(
          context, RERHI::DescriptorRange, numberOfDescriptorRanges));
        memcpy(reinterpret_cast<RERHI::DescriptorRange *>(destinationRootParameter.descriptorTable.descriptorRanges),
               reinterpret_cast<const RERHI::DescriptorRange *>(sourceRootParameter.descriptorTable.descriptorRanges),
               sizeof(RERHI::DescriptorRange) * numberOfDescriptorRanges);
      }
    }
  }

  { // Copy the static sampler data
    const RECore::uint32 numberOfStaticSamplers = mRootSignature.numberOfStaticSamplers;
    if (numberOfStaticSamplers > 0) {
      mRootSignature.staticSamplers = RHI_MALLOC_TYPED(context, RERHI::StaticSampler, numberOfStaticSamplers);
      memcpy(const_cast<RERHI::StaticSampler *>(mRootSignature.staticSamplers), rootSignature.staticSamplers,
             sizeof(RERHI::StaticSampler) * numberOfStaticSamplers);
    }
  }

  // Create the Vulkan descriptor set layout
  const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
  VkDescriptorSetLayouts vkDescriptorSetLayouts;
  RECore::uint32 numberOfUniformTexelBuffers = 0;  // "VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER"
  RECore::uint32 numberOfStorageTexelBuffers = 0;  // "VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER"
  RECore::uint32 numberOfStorageImage = 0;      // "VK_DESCRIPTOR_TYPE_STORAGE_IMAGE"
  RECore::uint32 numberOfStorageBuffers = 0;    // "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER"
  RECore::uint32 numberOfUniformBuffers = 0;    // "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER"
  RECore::uint32 numberOfCombinedImageSamplers = 0;  // "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"
  if (numberOfRootParameters > 0) {
    // Fill the Vulkan descriptor set layout bindings
    vkDescriptorSetLayouts.reserve(numberOfRootParameters);
    mVkDescriptorSetLayouts.resize(numberOfRootParameters);
    std::fill(mVkDescriptorSetLayouts.begin(), mVkDescriptorSetLayouts.end(),
              static_cast<VkDescriptorSetLayout>(VK_NULL_HANDLE));  // TODO(naetherm) Get rid of this
    typedef std::vector<VkDescriptorSetLayoutBinding> VkDescriptorSetLayoutBindings;
    VkDescriptorSetLayoutBindings vkDescriptorSetLayoutBindings;
    vkDescriptorSetLayoutBindings.reserve(numberOfRootParameters);
    for (RECore::uint32 rootParameterIndex = 0; rootParameterIndex < numberOfRootParameters; ++rootParameterIndex) {
      vkDescriptorSetLayoutBindings.clear();

      // TODO(naetherm) For now we only support descriptor tables
      const RERHI::RootParameter &rootParameter = rootSignature.parameters[rootParameterIndex];
      if (RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
        // Process descriptor ranges
        const RERHI::DescriptorRange *descriptorRange = reinterpret_cast<const RERHI::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges);
        for (RECore::uint32 descriptorRangeIndex = 0; descriptorRangeIndex <
                                                rootParameter.descriptorTable.numberOfDescriptorRanges; ++descriptorRangeIndex, ++descriptorRange) {
          // Evaluate parameter type
          VkDescriptorType vkDescriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
          switch (descriptorRange->resourceType) {
            case RERHI::ResourceType::TEXTURE_BUFFER:
              RHI_ASSERT(RERHI::DescriptorRangeType::SRV == descriptorRange->rangeType ||
                         RERHI::DescriptorRangeType::UAV == descriptorRange->rangeType,
                         "Vulkan RHI implementation: Invalid descriptor range type")
              if (RERHI::DescriptorRangeType::SRV == descriptorRange->rangeType) {
                vkDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
                ++numberOfUniformTexelBuffers;
              } else {
                vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
                ++numberOfStorageTexelBuffers;
              }
              break;

            case RERHI::ResourceType::VERTEX_BUFFER:
            case RERHI::ResourceType::INDEX_BUFFER:
            case RERHI::ResourceType::STRUCTURED_BUFFER:
            case RERHI::ResourceType::INDIRECT_BUFFER:
              RHI_ASSERT(RERHI::DescriptorRangeType::SRV == descriptorRange->rangeType ||
                         RERHI::DescriptorRangeType::UAV == descriptorRange->rangeType,
                         "Vulkan RHI implementation: Invalid descriptor range type")
              vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
              ++numberOfStorageBuffers;
              break;

            case RERHI::ResourceType::UNIFORM_BUFFER:
              RHI_ASSERT(RERHI::DescriptorRangeType::UBV == descriptorRange->rangeType ||
                         RERHI::DescriptorRangeType::UAV == descriptorRange->rangeType,
                         "Vulkan RHI implementation: Invalid descriptor range type")
              vkDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
              ++numberOfUniformBuffers;
              break;

            case RERHI::ResourceType::TEXTURE_1D:
            case RERHI::ResourceType::TEXTURE_1D_ARRAY:
            case RERHI::ResourceType::TEXTURE_2D:
            case RERHI::ResourceType::TEXTURE_2D_ARRAY:
            case RERHI::ResourceType::TEXTURE_3D:
            case RERHI::ResourceType::TEXTURE_CUBE:
            case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
              RHI_ASSERT(RERHI::DescriptorRangeType::SRV == descriptorRange->rangeType ||
                         RERHI::DescriptorRangeType::UAV == descriptorRange->rangeType,
                         "Vulkan RHI implementation: Invalid descriptor range type")
              if (RERHI::DescriptorRangeType::SRV == descriptorRange->rangeType) {
                vkDescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                ++numberOfCombinedImageSamplers;
              } else {
                vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                ++numberOfStorageImage;
              }
              break;

            case RERHI::ResourceType::SAMPLER_STATE:
              // Nothing here due to usage of "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"
              RHI_ASSERT(RERHI::DescriptorRangeType::SAMPLER == descriptorRange->rangeType,
                         "Vulkan RHI implementation: Invalid descriptor range type")
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
              RHI_ASSERT(false, "Vulkan RHI implementation: Invalid resource type")
              break;
          }

          // Evaluate shader visibility
          VkShaderStageFlags vkShaderStageFlags = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
          switch (descriptorRange->shaderVisibility) {
            case RERHI::ShaderVisibility::ALL:
              vkShaderStageFlags = VK_SHADER_STAGE_ALL;
              break;

            case RERHI::ShaderVisibility::VERTEX:
              vkShaderStageFlags = VK_SHADER_STAGE_VERTEX_BIT;
              break;

            case RERHI::ShaderVisibility::TESSELLATION_CONTROL:
              vkShaderStageFlags = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
              break;

            case RERHI::ShaderVisibility::TESSELLATION_EVALUATION:
              vkShaderStageFlags = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
              break;

            case RERHI::ShaderVisibility::GEOMETRY:
              vkShaderStageFlags = VK_SHADER_STAGE_GEOMETRY_BIT;
              break;

            case RERHI::ShaderVisibility::FRAGMENT:
              vkShaderStageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
              break;

            case RERHI::ShaderVisibility::TASK:
              vkShaderStageFlags = VK_SHADER_STAGE_TASK_BIT_NV;
              break;

            case RERHI::ShaderVisibility::MESH:
              vkShaderStageFlags = VK_SHADER_STAGE_MESH_BIT_NV;
              break;

            case RERHI::ShaderVisibility::COMPUTE:
              vkShaderStageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
              break;

            case RERHI::ShaderVisibility::ALL_GRAPHICS:
              vkShaderStageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
              break;
          }

          // Add the Vulkan descriptor set layout binding
          if (VK_DESCRIPTOR_TYPE_MAX_ENUM != vkDescriptorType) {
            const VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding =
              {
                descriptorRangeIndex,  // binding (RECore::uint32)
                vkDescriptorType,    // descriptorType (VkDescriptorType)
                1,            // descriptorCount (RECore::uint32)
                vkShaderStageFlags,    // stageFlags (VkShaderStageFlags)
                nullptr          // pImmutableSamplers (const VkSampler*)
              };
            vkDescriptorSetLayoutBindings.push_back(vkDescriptorSetLayoutBinding);
          }
        }
      }

      // Create the Vulkan descriptor set layout
      if (!vkDescriptorSetLayoutBindings.empty()) {
        const VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutCreateInfo =
          {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,      // sType (VkStructureType)
            nullptr,                            // pNext (const void*)
            0,                                // flags (VkDescriptorSetLayoutCreateFlags)
            static_cast<RECore::uint32>(vkDescriptorSetLayoutBindings.size()),  // bindingCount (RECore::uint32)
            vkDescriptorSetLayoutBindings.data()              // pBindings (const VkDescriptorSetLayoutBinding*)
          };
        if (
          vkCreateDescriptorSetLayout(vkDevice, &vkDescriptorSetLayoutCreateInfo, vulkanRhi.getVkAllocationCallbacks(),
                                      &mVkDescriptorSetLayouts[rootParameterIndex]) != VK_SUCCESS) {
          RE_LOG(Critical, "Failed to create the Vulkan descriptor set layout")
        }
        vkDescriptorSetLayouts.push_back(mVkDescriptorSetLayouts[rootParameterIndex]);
      }
    }
  }

  { // Create the Vulkan pipeline layout
    const VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo =
      {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,                // sType (VkStructureType)
        nullptr,                                  // pNext (const void*)
        0,                                      // flags (VkPipelineLayoutCreateFlags)
        static_cast<RECore::uint32>(vkDescriptorSetLayouts.size()),            // setLayoutCount (RECore::uint32)
        vkDescriptorSetLayouts.empty() ? nullptr
                                       : vkDescriptorSetLayouts.data(),  // pSetLayouts (const VkDescriptorSetLayout*)
        0,                                      // pushConstantRangeCount (RECore::uint32)
        nullptr                                    // pPushConstantRanges (const VkPushConstantRange*)
      };
    if (vkCreatePipelineLayout(vkDevice, &vkPipelineLayoutCreateInfo, vulkanRhi.getVkAllocationCallbacks(),
                               &mVkPipelineLayout) != VK_SUCCESS) {
      RE_LOG(Critical, "Failed to create the Vulkan pipeline layout")
    }
  }

  { // Create the Vulkan descriptor pool
    typedef std::array<VkDescriptorPoolSize, 3> VkDescriptorPoolSizes;
    VkDescriptorPoolSizes vkDescriptorPoolSizes;
    RECore::uint32 numberOfVkDescriptorPoolSizes = 0;

    // "VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER"
    if (numberOfCombinedImageSamplers > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfCombinedImageSamplers;    // descriptorCount (RECore::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // "VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER"
    if (numberOfUniformTexelBuffers > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfUniformTexelBuffers;  // descriptorCount (RECore::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // "VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER"
    if (numberOfStorageTexelBuffers > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfStorageTexelBuffers;  // descriptorCount (RECore::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // "VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER"
    if (numberOfUniformBuffers > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfUniformBuffers;  // descriptorCount (RECore::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // "VK_DESCRIPTOR_TYPE_STORAGE_IMAGE"
    if (numberOfStorageImage > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfStorageImage;    // descriptorCount (RECore::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // "VK_DESCRIPTOR_TYPE_STORAGE_BUFFER"
    if (numberOfStorageBuffers > 0) {
      VkDescriptorPoolSize &vkDescriptorPoolSize = vkDescriptorPoolSizes[numberOfVkDescriptorPoolSizes];
      vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;  // type (VkDescriptorType)
      vkDescriptorPoolSize.descriptorCount = maxSets * numberOfStorageBuffers;  // descriptorCount (RECore::uint32)
      ++numberOfVkDescriptorPoolSizes;
    }

    // Create the Vulkan descriptor pool
    if (numberOfVkDescriptorPoolSizes > 0) {
      const VkDescriptorPoolCreateInfo VkDescriptorPoolCreateInfo =
        {
          VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,    // sType (VkStructureType)
          nullptr,                      // pNext (const void*)
          VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,  // flags (VkDescriptorPoolCreateFlags)
          maxSets,                      // maxSets (RECore::uint32)
          numberOfVkDescriptorPoolSizes,            // poolSizeCount (RECore::uint32)
          vkDescriptorPoolSizes.data()            // pPoolSizes (const VkDescriptorPoolSize*)
        };
      if (vkCreateDescriptorPool(vkDevice, &VkDescriptorPoolCreateInfo, vulkanRhi.getVkAllocationCallbacks(),
                                 &mVkDescriptorPool) != VK_SUCCESS) {
        RE_LOG(Critical, "Failed to create the Vulkan descriptor pool")
      }
    }
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT)
        {
          RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Root signature", 17)	// 17 = "Root signature: " including terminating zero
          for (VkDescriptorSetLayout vkDescriptorSetLayout : mVkDescriptorSetLayouts)
          {
            Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT, (RECore::uint64)vkDescriptorSetLayout, detailedDebugName);
          }
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT, (RECore::uint64)mVkPipelineLayout, detailedDebugName);
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT, (RECore::uint64)mVkDescriptorPool, detailedDebugName);
        }
#endif
}

/**
*  @brief
*    Destructor
*/
RootSignature::~RootSignature() {
  const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();

// Destroy the Vulkan descriptor pool
  if (VK_NULL_HANDLE != mVkDescriptorPool) {
    vkDestroyDescriptorPool(vkDevice, mVkDescriptorPool, vulkanRhi.getVkAllocationCallbacks());
  }

// Destroy the Vulkan pipeline layout
  if (VK_NULL_HANDLE != mVkPipelineLayout) {
    vkDestroyPipelineLayout(vkDevice, mVkPipelineLayout, vulkanRhi.getVkAllocationCallbacks());
  }

// Destroy the Vulkan descriptor set layout
  for (VkDescriptorSetLayout vkDescriptorSetLayout: mVkDescriptorSetLayouts) {
    if (VK_NULL_HANDLE != vkDescriptorSetLayout) {
      vkDestroyDescriptorSetLayout(vkDevice, vkDescriptorSetLayout, vulkanRhi.getVkAllocationCallbacks());
    }
  }

// Destroy the root signature data
  const RERHI::RHIContext &context = vulkanRhi.getContext();
  if (nullptr != mRootSignature.parameters) {
    for (RECore::uint32 rootParameterIndex = 0;
         rootParameterIndex < mRootSignature.numberOfParameters; ++rootParameterIndex) {
      const RERHI::RootParameter &rootParameter = mRootSignature.parameters[rootParameterIndex];
      if (RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
        RHI_FREE(context, reinterpret_cast<RERHI::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges));
      }
    }
    RHI_FREE(context, const_cast<RERHI::RootParameter *>(mRootSignature.parameters));
  }
  RHI_FREE(context, const_cast<RERHI::StaticSampler *>(mRootSignature.staticSamplers));
}


// TODO(naetherm) Try to somehow simplify the internal dependencies to be able to put this method directly into the class
RERHI::RHIResourceGroup *RootSignature::createResourceGroup(RECore::uint32 rootParameterIndex, RECore::uint32 numberOfResources,
                                                            RERHI::RHIResource **resources,
                                                            RERHI::RHISamplerState **samplerStates
                                                            RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  const RERHI::RHIContext &context = vulkanRhi.getContext();

  // Sanity checks
  RHI_ASSERT(VK_NULL_HANDLE != mVkDescriptorPool, "The Vulkan descriptor pool instance must be valid")
  RHI_ASSERT(rootParameterIndex < mVkDescriptorSetLayouts.size(), "The Vulkan root parameter index is out-of-bounds")
  RHI_ASSERT(numberOfResources > 0, "The number of Vulkan resources must not be zero")
  RHI_ASSERT(nullptr != resources, "The Vulkan resource pointers must be valid")

  // Allocate Vulkan descriptor set
  VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;
  if ((*resources)->getResourceType() != RERHI::ResourceType::SAMPLER_STATE) {
    const VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo =
      {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,  // sType (VkStructureType)
        nullptr,                    // pNext (const void*)
        mVkDescriptorPool,                // descriptorPool (VkDescriptorPool)
        1,                        // descriptorSetCount (RECore::uint32)
        &mVkDescriptorSetLayouts[rootParameterIndex]  // pSetLayouts (const VkDescriptorSetLayout*)
      };
    if (vkAllocateDescriptorSets(vulkanRhi.getVulkanContext().getVkDevice(), &vkDescriptorSetAllocateInfo,
                                 &vkDescriptorSet) != VK_SUCCESS) {
      RE_LOG(Critical, "Failed to allocate the Vulkan descriptor set")
    }
  }

  // Create resource group
  return RHI_NEW(context, ResourceGroup)(*this, rootParameterIndex, vkDescriptorSet, numberOfResources, resources,
                                         samplerStates RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
