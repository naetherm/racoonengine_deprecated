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
#include <RERenderer/Resource/CompositorNode/Pass/CompositorPassFactory.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class CompositorPassFactory final : public RERenderer::CompositorPassFactory
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	inline CompositorPassFactory()
	{
		// Nothing here
	}

	inline virtual ~CompositorPassFactory() override
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Protected virtual RERenderer::ICompositorPassFactory methods ]
//[-------------------------------------------------------]
protected:
	[[nodiscard]] virtual RERenderer::ICompositorResourcePass* createCompositorResourcePass(const RERenderer::CompositorTarget& compositorTarget, RERenderer::CompositorPassTypeId compositorPassTypeId) const override;
	[[nodiscard]] virtual RERenderer::ICompositorInstancePass* createCompositorInstancePass(const RERenderer::ICompositorResourcePass& compositorResourcePass, const RERenderer::CompositorNodeInstance& compositorNodeInstance) const override;


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	explicit CompositorPassFactory(const CompositorPassFactory&) = delete;
	CompositorPassFactory& operator=(const CompositorPassFactory&) = delete;


};
