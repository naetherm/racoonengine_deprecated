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
#include "RERenderer/Resource/Scene/Item/MaterialSceneItem.h"


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
	*    Particles scene item
	*
	*  @todo
	*    - TODO(naetherm) Particles rendering part is implemented, implement GPU based particles simulation as well
	*/
	class ParticlesSceneItem final : public MaterialSceneItem
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SceneFactory;	// Needs to be able to create scene item instances


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr RECore::uint32 TYPE_ID = STRING_ID("ParticlesSceneItem");
		struct ParticleDataStruct final
		{
			float PositionSize[4];	// Object space particle xyz-position, w = particle size
			float Color[4];			// Linear space RGB particle color and opacity
		};


	//[-------------------------------------------------------]
	//[ Public virtual RERenderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual SceneItemTypeId getSceneItemTypeId() const override
		{
			return TYPE_ID;
		}

		[[nodiscard]] virtual const RenderableManager* getRenderableManager() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::ISceneItem methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void onExecuteOnRendering(const RERHI::RHIRenderTarget& renderTarget, const CompositorContextData& compositorContextData, RERHI::RHICommandBuffer& commandBuffer) const override;


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::MaterialSceneItem methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void initialize() override;
		virtual void onMaterialResourceCreated() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit ParticlesSceneItem(SceneResource& sceneResource);

		inline virtual ~ParticlesSceneItem() override
		{
			// Nothing here
		}

		explicit ParticlesSceneItem(const ParticlesSceneItem&) = delete;
		ParticlesSceneItem& operator=(const ParticlesSceneItem&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RECore::uint32				  mMaximumNumberOfParticles;	///< Maximum number of particles
		RERHI::RHIStructuredBufferPtr mStructuredBufferPtr;			///< Structured buffer the data of the individual particles ("RERenderer::ParticlesSceneItem::ParticleDataStruct")


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
