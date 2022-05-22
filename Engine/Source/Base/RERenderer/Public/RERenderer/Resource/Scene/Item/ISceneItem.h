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
#include "RERHI/Rhi.h"
#include <RECore/String/StringId.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer
{
	class Context;
	class SceneNode;
	struct SceneItemSet;
	class SceneResource;
	class RenderableManager;
	class CompositorContextData;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::StringId SceneItemTypeId;	///< Scene item type identifier, internally just a POD "RECore::uint32"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class ISceneItem
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SceneNode;						// TODO(naetherm) Remove this
		friend class MeshSceneItem;					// TODO(naetherm) Remove this
		friend class SceneResource;					// Needs to be able to destroy scene items
		friend class CompositorWorkspaceInstance;	// Needs to be able to call "RERenderer::ISceneItem::onExecuteOnRendering()"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] Context& getContext() const;

		[[nodiscard]] inline SceneResource& getSceneResource() const
		{
			return mSceneResource;
		}

		[[nodiscard]] inline bool hasParentSceneNode() const
		{
			return (nullptr != mParentSceneNode);
		}

		[[nodiscard]] inline SceneNode* getParentSceneNode()
		{
			return mParentSceneNode;
		}

		[[nodiscard]] inline const SceneNode* getParentSceneNode() const
		{
			return mParentSceneNode;
		}

		[[nodiscard]] inline SceneNode& getParentSceneNodeSafe()	// Just safe in context known as safe
		{
			ASSERT(nullptr != mParentSceneNode, "Invalid parent scene node")
			return *mParentSceneNode;
		}

		[[nodiscard]] inline const SceneNode& getParentSceneNodeSafe() const	// Just safe in context known as safe
		{
			ASSERT(nullptr != mParentSceneNode, "Invalid parent scene node")
			return *mParentSceneNode;
		}

		[[nodiscard]] inline bool getCallExecuteOnRendering() const
		{
			return mCallExecuteOnRendering;
		}


	//[-------------------------------------------------------]
	//[ Public virtual RERenderer::ISceneItem methods           ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual SceneItemTypeId getSceneItemTypeId() const = 0;
		virtual void deserialize(RECore::uint32 numberOfBytes, const RECore::uint8* data) = 0;

		inline virtual void onAttachedToSceneNode(SceneNode& sceneNode)
		{
			ASSERT(nullptr == mParentSceneNode, "Invalid parent scene node")
			mParentSceneNode = &sceneNode;
		}

		inline virtual void onDetachedFromSceneNode([[maybe_unused]] SceneNode& sceneNode)
		{
			ASSERT(nullptr != mParentSceneNode, "Invalid parent scene node")
			mParentSceneNode = nullptr;
		}

		inline virtual void setVisible([[maybe_unused]] bool visible)
		{
			// Nothing here
		}

		[[nodiscard]] inline virtual const RenderableManager* getRenderableManager() const
		{
			return nullptr;
		}


	//[-------------------------------------------------------]
	//[ Protected virtual RERenderer::ISceneItem methods        ]
	//[-------------------------------------------------------]
	protected:
		// Only called if "RERenderer::ISceneItem::getCallExecuteOnRendering()" returns "true", the default implementation is empty and shouldn't be called
		inline virtual void onExecuteOnRendering([[maybe_unused]] const RERHI::RHIRenderTarget& renderTarget, [[maybe_unused]] const CompositorContextData& compositorContextData, [[maybe_unused]] RERHI::RHICommandBuffer& commandBuffer) const
		{
			ASSERT(true, "Don't call the base implementation of \"RERenderer::ISceneItem::getCallExecuteOnRendering()\"")
		}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		ISceneItem(SceneResource& sceneResource, bool cullable = true);
		virtual ~ISceneItem();
		explicit ISceneItem(const ISceneItem&) = delete;
		ISceneItem& operator=(const ISceneItem&) = delete;

		inline void setCallExecuteOnRendering(bool callExecuteOnRendering)
		{
			mCallExecuteOnRendering = callExecuteOnRendering;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SceneResource& mSceneResource;
		SceneNode*	   mParentSceneNode;		///< Parent scene node, can be a null pointer, don't destroy the instance
		SceneItemSet*  mSceneItemSet;			///< Scene item set, always valid, don't destroy the instance
		RECore::uint32	   mSceneItemSetIndex;		///< Index inside the scene item set
		bool		   mCallExecuteOnRendering;	///< Call execute on rendering? ("RERenderer::ISceneItem::onExecuteOnRendering()") Keep this disabled if not needed not waste performance.


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
