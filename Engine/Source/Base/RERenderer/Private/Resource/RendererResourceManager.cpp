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
#include "RERenderer/Resource/RendererResourceManager.h"
#include <RECore/Math/Math.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RERHI::RHIResourceGroup* RendererResourceManager::createResourceGroup(RERHI::RHIRootSignature& rootSignature, RECore::uint32 rootParameterIndex, RECore::uint32 numberOfResources, RERHI::RHIResource** resources, RERHI::RHISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT)
	{
		// Create hash
		RECore::uint32 hash = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&rootSignature), sizeof(RERHI::RHIRootSignature&));
		hash = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&rootParameterIndex), sizeof(RECore::uint32), hash);
		hash = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&numberOfResources), sizeof(RECore::uint32), hash);
		for (RECore::uint32 i = 0; i < numberOfResources; ++i)
		{
			hash = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&resources[i]), sizeof(RERHI::RHIResource*), hash);
			if (nullptr != samplerStates && nullptr != samplerStates[i])
			{
				hash = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&samplerStates[i]), sizeof(RERHI::RHISamplerState*), hash);
			}
			else
			{
				static const RECore::uint32 NOTHING = 42;	// Not "static constexpr" by intent
				hash = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&NOTHING), sizeof(RECore::uint32), hash);
			}
		}
		ResourceGroups::const_iterator iterator = mResourceGroups.find(hash);
		if (mResourceGroups.cend() != iterator)
		{
			return iterator->second;
		}
		else
		{
			// Create RHI resource and add the managers reference
			RERHI::RHIResourceGroup* resourceGroup = rootSignature.createResourceGroup(rootParameterIndex, numberOfResources, resources, samplerStates RHI_RESOURCE_DEBUG_PASS_PARAMETER);
			resourceGroup->AddReference();
			mResourceGroups.emplace(hash, resourceGroup);
			return resourceGroup;
		}
	}

	void RendererResourceManager::garbageCollection()
	{
		// TODO(naetherm) "RERenderer::RendererResourceManager": From time to time, look for orphaned RHI resources and free them. Currently a trivial approach is used which might cause hiccups. For example distribute the traversal over time.
		++mGarbageCollectionCounter;
		if (mGarbageCollectionCounter > 100)
		{
			ResourceGroups::iterator iterator = mResourceGroups.begin();
			while (iterator != mResourceGroups.end())
			{
				if (iterator->second->GetRefCount() == 1)
				{
					iterator->second->Release();
					iterator = mResourceGroups.erase(iterator);
				}
				else
				{
					++iterator;
				}
			}
			mGarbageCollectionCounter = 0;
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	RendererResourceManager::~RendererResourceManager()
	{
		// Release manager RHI resource references
		for (auto& pair : mResourceGroups)
		{
			pair.second->Release();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
