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
#include "RERenderer/Resource/MaterialBlueprint/BufferManager/PassBufferManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RECore{
class Transform;
}
namespace RERenderer
{
	class IRenderer;
	class MaterialTechnique;
	class CompositorContextData;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract material blueprint resource listener interface
	*/
	class IMaterialBlueprintResourceListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class PassBufferManager;					// Is calling the protected interface methods
		friend class MaterialBlueprintResource;			// Is calling the protected interface methods
		friend class MaterialBufferManager;				// Is calling the protected interface methods
		friend class UniformInstanceBufferManager;		// Is calling the protected interface methods
		friend class TextureInstanceBufferManager;		// Is calling the protected interface methods
		friend class MaterialBlueprintResourceManager;	// Is calling the protected interface methods


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline IMaterialBlueprintResourceListener()
		{
			// Nothing here
		}

		inline virtual ~IMaterialBlueprintResourceListener()
		{
			// Nothing here
		}

		explicit IMaterialBlueprintResourceListener(const IMaterialBlueprintResourceListener&) = delete;
		IMaterialBlueprintResourceListener& operator=(const IMaterialBlueprintResourceListener&) = delete;


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::IMaterialBlueprintResourceListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onStartup(const IRenderer& renderer) = 0;	// Becomes the currently used material blueprint resource listener
		virtual void onShutdown(const IRenderer& renderer) = 0;	// Is no longer the currently used material blueprint resource listener
		virtual void beginFillUnknown() = 0;
		[[nodiscard]] virtual bool fillUnknownValue(RECore::uint32 referenceValue, RECore::uint8* buffer, RECore::uint32 numberOfBytes) = 0;
		virtual void beginFillPass(IRenderer& renderer, const RERHI::RHIRenderTarget* renderTarget, const CompositorContextData& compositorContextData, PassBufferManager::PassData& passData) = 0;	// The render target to render into must be valid for graphics pipeline and must be a null pointer for compute pipeline
		[[nodiscard]] virtual bool fillPassValue(RECore::uint32 referenceValue, RECore::uint8* buffer, RECore::uint32 numberOfBytes) = 0;
		virtual void beginFillMaterial() = 0;
		[[nodiscard]] virtual bool fillMaterialValue(RECore::uint32 referenceValue, RECore::uint8* buffer, RECore::uint32 numberOfBytes) = 0;

		// TODO(naetherm) It might make sense to remove those instance methods from the interface and directly hard-code them for performance reasons. Profiling later on with real world scenes will show.
		virtual void beginFillInstance(const PassBufferManager::PassData& passData, const RECore::Transform& objectSpaceToWorldSpaceTransform, MaterialTechnique& materialTechnique) = 0;
		[[nodiscard]] virtual bool fillInstanceValue(RECore::uint32 referenceValue, RECore::uint8* buffer, RECore::uint32 numberOfBytes, RECore::uint32 instanceTextureBufferStartIndex) = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
