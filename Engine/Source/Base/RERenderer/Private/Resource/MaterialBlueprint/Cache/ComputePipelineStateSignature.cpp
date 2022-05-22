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
#include "RERenderer/Resource/MaterialBlueprint/Cache/ComputePipelineStateSignature.h"
#include "RERenderer/Resource/MaterialBlueprint/Cache/GraphicsPipelineStateSignature.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResourceManager.h"
#include "RERenderer/Resource/MaterialBlueprint/MaterialBlueprintResource.h"
#include "RERenderer/Resource/ShaderBlueprint/ShaderBlueprintResourceManager.h"
#include "RERenderer/Resource/ShaderBlueprint/ShaderBlueprintResource.h"
#include <RECore/Math/Math.h>
#include "RERenderer/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ComputePipelineStateSignature::ComputePipelineStateSignature(const ComputePipelineStateSignature& computePipelineStateSignature) :
		mMaterialBlueprintResourceId(computePipelineStateSignature.mMaterialBlueprintResourceId),
		mShaderProperties(computePipelineStateSignature.mShaderProperties),
		mComputePipelineStateSignatureId(computePipelineStateSignature.mComputePipelineStateSignatureId),
		mShaderCombinationId(computePipelineStateSignature.mShaderCombinationId)
	{
		// Nothing here
	}

	ComputePipelineStateSignature& ComputePipelineStateSignature::operator=(const ComputePipelineStateSignature& computePipelineStateSignature)
	{
		mMaterialBlueprintResourceId = computePipelineStateSignature.mMaterialBlueprintResourceId;
		mShaderProperties = computePipelineStateSignature.mShaderProperties;
		mComputePipelineStateSignatureId = computePipelineStateSignature.mComputePipelineStateSignatureId;
		mShaderCombinationId = computePipelineStateSignature.mShaderCombinationId;

		// Done
		return *this;
	}

	void ComputePipelineStateSignature::set(const MaterialBlueprintResource& materialBlueprintResource, const ShaderProperties& shaderProperties)
	{
		mMaterialBlueprintResourceId		= materialBlueprintResource.getId();
		mShaderProperties					= shaderProperties;
		mComputePipelineStateSignatureId	= RECore::Math::FNV1a_INITIAL_HASH_32;
		mShaderCombinationId				= RECore::getInvalid<ShaderCombinationId>();

		// Incorporate primitive hashes
		mComputePipelineStateSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&mMaterialBlueprintResourceId), sizeof(RECore::uint32), mComputePipelineStateSignatureId);

		// Incorporate shader related hash
		const ShaderBlueprintResource* shaderBlueprintResource = materialBlueprintResource.getResourceManager<MaterialBlueprintResourceManager>().getRenderer().getShaderBlueprintResourceManager().tryGetById(materialBlueprintResource.getComputeShaderBlueprintResourceId());
		if (nullptr != shaderBlueprintResource)
		{
			const RECore::uint32 hash = mShaderCombinationId = GraphicsPipelineStateSignature::generateShaderCombinationId(*shaderBlueprintResource, mShaderProperties);
			mComputePipelineStateSignatureId = RECore::Math::calculateFNV1a32(reinterpret_cast<const RECore::uint8*>(&hash), sizeof(RECore::uint32), mComputePipelineStateSignatureId);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
