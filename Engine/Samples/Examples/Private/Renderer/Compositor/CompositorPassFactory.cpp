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
#include "Examples/Renderer/Compositor/CompositorPassFactory.h"
#include "Examples/Renderer/Compositor/CompositorResourcePass.h"
#include "Examples/Renderer/Compositor/CompositorInstancePass.h"


//[-------------------------------------------------------]
//[ Protected virtual RERenderer::ICompositorPassFactory methods ]
//[-------------------------------------------------------]
RERenderer::ICompositorResourcePass* CompositorPassFactory::createCompositorResourcePass(const RERenderer::CompositorTarget& compositorTarget, RERenderer::CompositorPassTypeId compositorPassTypeId) const
{
	// First, let the base implementation try to create an instance
	RERenderer::ICompositorResourcePass* compositorResourcePass = RERenderer::CompositorPassFactory::createCompositorResourcePass(compositorTarget, compositorPassTypeId);

	// Evaluate the compositor pass type
	if (nullptr == compositorResourcePass && CompositorResourcePass::TYPE_ID == compositorPassTypeId)
	{
		compositorResourcePass = new CompositorResourcePass(compositorTarget);
	}

	// Done
	return compositorResourcePass;
}

RERenderer::ICompositorInstancePass* CompositorPassFactory::createCompositorInstancePass(const RERenderer::ICompositorResourcePass& compositorResourcePass, const RERenderer::CompositorNodeInstance& compositorNodeInstance) const
{
	// First, let the base implementation try to create an instance
	RERenderer::ICompositorInstancePass* compositorInstancePass = RERenderer::CompositorPassFactory::createCompositorInstancePass(compositorResourcePass, compositorNodeInstance);

	// Evaluate the compositor pass type
	if (nullptr == compositorInstancePass && compositorResourcePass.getTypeId() == CompositorResourcePass::TYPE_ID)
	{
		compositorInstancePass = new CompositorInstancePass(static_cast<const CompositorResourcePass&>(compositorResourcePass), compositorNodeInstance);
	}

	// Done
	return compositorInstancePass;
}
