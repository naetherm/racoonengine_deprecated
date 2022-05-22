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
#include <RERenderer/Resource/CompositorNode/Pass/ICompositorResourcePass.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class CompositorResourcePass final : public RERenderer::ICompositorResourcePass
{


//[-------------------------------------------------------]
//[ Friends                                               ]
//[-------------------------------------------------------]
	friend class CompositorPassFactory;	// The only one allowed to create instances of this class


//[-------------------------------------------------------]
//[ Public definitions                                    ]
//[-------------------------------------------------------]
public:
	static constexpr RECore::uint32 TYPE_ID = STRING_ID("CompositorExample");


//[-------------------------------------------------------]
//[ Public virtual RERenderer::ICompositorResourcePass methods ]
//[-------------------------------------------------------]
public:
	[[nodiscard]] inline virtual RERenderer::CompositorPassTypeId getTypeId() const override
	{
		return TYPE_ID;
	}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	inline explicit CompositorResourcePass(const RERenderer::CompositorTarget& compositorTarget) :
		RERenderer::ICompositorResourcePass(compositorTarget)
	{
		// Nothing here
	}

	inline virtual ~CompositorResourcePass() override
	{
		// Nothing here
	}

	explicit CompositorResourcePass(const CompositorResourcePass&) = delete;
	CompositorResourcePass& operator=(const CompositorResourcePass&) = delete;


};
