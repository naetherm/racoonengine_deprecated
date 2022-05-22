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
#include "Examples/Basics/Queries/Queries.h"
#include <RECore/Color/Color4.h>


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void Queries::onInitialization()
{
	// Call the base implementation
	Triangle::onInitialization();

	// Get and check the RHI instance
	RERHI::RHIDynamicRHIPtr rhi(getRhi());
	if (nullptr != rhi)
	{
		// Create the queries
		mOcclusionQueryPool = rhi->createQueryPool(RERHI::QueryType::OCCLUSION);
		mPipelineStatisticsQueryPool = rhi->createQueryPool(RERHI::QueryType::PIPELINE_STATISTICS);
		mTimestampQueryPool = rhi->createQueryPool(RERHI::QueryType::TIMESTAMP, 2);

		// Since we're always dispatching the same commands to the RHI, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
		mCommandBuffer.clear();	// Throw away "Triangle"-stuff
		fillCommandBuffer();
	}
}

void Queries::onDeinitialization()
{
	// Release the used resources
	mOcclusionQueryPool = nullptr;
	mPipelineStatisticsQueryPool = nullptr;
	mTimestampQueryPool = nullptr;

	// Call the base implementation
	Triangle::onDeinitialization();
}

void Queries::onDraw(RERHI::RHICommandBuffer& commandBuffer)
{
	// Call the base implementation
	Triangle::onDraw(commandBuffer);

	// Get query results
	RERHI::RHIDynamicRHIPtr rhi(getRhi());
	if (nullptr != rhi)
	{
		// Sanity checks
		RHI_ASSERT(rhi->getContext(), nullptr != mOcclusionQueryPool, "Invalid occlusion query pool")
		RHI_ASSERT(rhi->getContext(), nullptr != mPipelineStatisticsQueryPool, "Invalid pipeline statistics query pool")
		RHI_ASSERT(rhi->getContext(), nullptr != mTimestampQueryPool, "Invalid timestamp query pool")

		{ // Occlusion query pool
			RECore::uint64 numberOfSamples = 0;
			if (rhi->getQueryPoolResults(*mOcclusionQueryPool, sizeof(RECore::uint64), reinterpret_cast<RECore::uint8*>(&numberOfSamples)))
			{
				NOP;	// TODO(naetherm) Process result
			}
		}

		{ // Pipeline statistics query pool
			RERHI::PipelineStatisticsQueryResult pipelineStatisticsQueryResult = {};
			if (rhi->getQueryPoolResults(*mPipelineStatisticsQueryPool, sizeof(RERHI::PipelineStatisticsQueryResult), reinterpret_cast<RECore::uint8*>(&pipelineStatisticsQueryResult)))
			{
				NOP;	// TODO(naetherm) Process result
			}
		}

		{ // Timestamp query pool
			RECore::uint64 timestamp[2] = {};
			if (rhi->getQueryPoolResults(*mTimestampQueryPool, sizeof(RECore::uint64) * 2, reinterpret_cast<RECore::uint8*>(&timestamp), 0, 2, sizeof(RECore::uint64)))
			{
				NOP;	// TODO(naetherm) Process result
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void Queries::fillCommandBuffer()
{
	// Sanity checks
	ASSERT(nullptr != getRhi(), "Invalid RHI instance")
	RHI_ASSERT(getRhi()->getContext(), mCommandBuffer.isEmpty(), "Command buffer is already filled")
	RHI_ASSERT(getRhi()->getContext(), nullptr != mRootSignature, "Invalid root signature")
	RHI_ASSERT(getRhi()->getContext(), nullptr != mGraphicsPipelineState, "Invalid graphics pipeline state")
	RHI_ASSERT(getRhi()->getContext(), nullptr != mVertexArray, "Invalid vertex array")
	RHI_ASSERT(getRhi()->getContext(), nullptr != mOcclusionQueryPool, "Invalid occlusion query pool")
	RHI_ASSERT(getRhi()->getContext(), nullptr != mPipelineStatisticsQueryPool, "Invalid pipeline statistics query pool")
	RHI_ASSERT(getRhi()->getContext(), nullptr != mTimestampQueryPool, "Invalid timestamp query pool")

	// Scoped debug event
	COMMAND_SCOPED_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	// Reset and begin queries
	RERHI::Command::ResetQueryPool::create(mCommandBuffer, *mTimestampQueryPool, 0, 2);
	RERHI::Command::WriteTimestampQuery::create(mCommandBuffer, *mTimestampQueryPool, 0);
	RERHI::Command::ResetAndBeginQuery::create(mCommandBuffer, *mOcclusionQueryPool);
	RERHI::Command::ResetAndBeginQuery::create(mCommandBuffer, *mPipelineStatisticsQueryPool, 0, RERHI::QueryControlFlags::PRECISE);

	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	RERHI::Command::ClearGraphics::create(mCommandBuffer, RERHI::ClearFlag::COLOR_DEPTH, RECore::Color4::GRAY);

	// Set the used graphics root signature
	RERHI::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Set the used graphics pipeline state object (PSO)
	RERHI::Command::SetGraphicsPipelineState::create(mCommandBuffer, mGraphicsPipelineState);

	// Input assembly (IA): Set the used vertex array
	RERHI::Command::SetGraphicsVertexArray::create(mCommandBuffer, mVertexArray);

	// Set debug marker
	// -> Debug methods: When using Direct3D <11.1, these methods map to the Direct3D 9 PIX functions
	//    (D3DPERF_* functions, also works directly within VisualStudio 2017 out-of-the-box)
	COMMAND_SET_DEBUG_MARKER(mCommandBuffer, "Everyone ready for the upcoming triangle?")

	{
		// Scoped debug event
		COMMAND_SCOPED_DEBUG_EVENT(mCommandBuffer, "Drawing the fancy triangle")

		// Render the specified geometric primitive, based on an array of vertices
		RERHI::Command::DrawGraphics::create(mCommandBuffer, 3);
	}

	// End queries
	RERHI::Command::EndQuery::create(mCommandBuffer, *mOcclusionQueryPool);
	RERHI::Command::EndQuery::create(mCommandBuffer, *mPipelineStatisticsQueryPool);
	RERHI::Command::WriteTimestampQuery::create(mCommandBuffer, *mTimestampQueryPool, 1);
}
