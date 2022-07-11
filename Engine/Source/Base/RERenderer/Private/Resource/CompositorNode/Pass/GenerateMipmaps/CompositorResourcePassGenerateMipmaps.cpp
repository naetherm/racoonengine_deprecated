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
#include "RERenderer/Resource/CompositorNode/Pass/GenerateMipmaps/CompositorResourcePassGenerateMipmaps.h"
#include "RERenderer/Resource/CompositorNode/Loader/CompositorNodeFileFormat.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public virtual RERenderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	void CompositorResourcePassGenerateMipmaps::deserialize([[maybe_unused]] RECore::uint32 numberOfBytes, const RECore::uint8* data)
	{
		// Sanity check
		ASSERT(sizeof(v1CompositorNode::PassGenerateMipmaps) == numberOfBytes, "Invalid number of bytes")

		// Call the base implementation
		ICompositorResourcePass::deserialize(sizeof(v1CompositorNode::Pass), data);

		// Read data
		const v1CompositorNode::PassGenerateMipmaps* passGenerateMipmaps = reinterpret_cast<const v1CompositorNode::PassGenerateMipmaps*>(data);
		mTextureAssetId = passGenerateMipmaps->textureAssetId;
		mMaterialBlueprintAssetId = passGenerateMipmaps->materialBlueprintAssetId;
		mTextureMaterialBlueprintProperty = passGenerateMipmaps->textureMaterialBlueprintProperty;

		// Sanity check
		ASSERT((RECore::isInvalid(mMaterialBlueprintAssetId) && RECore::isInvalid(mTextureMaterialBlueprintProperty)) || (RECore::isValid(mMaterialBlueprintAssetId) && RECore::isValid(mTextureMaterialBlueprintProperty)), "Invalid material blueprint asset")
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
