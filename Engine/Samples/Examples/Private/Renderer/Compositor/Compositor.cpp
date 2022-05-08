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
#include "Examples/Renderer/Compositor/Compositor.h"
#include "Examples/Renderer/Compositor/CompositorPassFactory.h"

#include <RERenderer/IRenderer.h>
#include <RERenderer/Resource/CompositorNode/CompositorNodeResourceManager.h>
#include <RERenderer/Resource/CompositorWorkspace/CompositorWorkspaceInstance.h>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		static const CompositorPassFactory compositorPassFactory;


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void Compositor::onInitialization()
{
	// Get and check the renderer instance
	RERenderer::IRenderer& renderer = getRendererSafe();

	// Set our custom compositor pass factory
	renderer.getCompositorNodeResourceManager().setCompositorPassFactory(&::detail::compositorPassFactory);

	// Create the compositor workspace instance
	mCompositorWorkspaceInstance = new RERenderer::CompositorWorkspaceInstance(renderer, ASSET_ID("Example/CompositorWorkspace/CW_Example"));
}

void Compositor::onDeinitialization()
{
	// TODO(naetherm) Implement decent resource handling
	delete mCompositorWorkspaceInstance;
	mCompositorWorkspaceInstance = nullptr;

	// Be polite and unset our custom compositor pass factory
	getRendererSafe().getCompositorNodeResourceManager().setCompositorPassFactory(nullptr);
}

void Compositor::onDraw(RERHI::RHICommandBuffer&)
{
	// Is there a compositor workspace instance?
	if (nullptr != mCompositorWorkspaceInstance)
	{
		// Get the main render target and ensure there's one
		RERHI::RHIRenderTarget* mainRenderTarget = getMainRenderTarget();
		if (nullptr != mainRenderTarget)
		{
			// Execute the compositor workspace instance
			mCompositorWorkspaceInstance->execute(*mainRenderTarget, nullptr, nullptr);
		}
	}
}
