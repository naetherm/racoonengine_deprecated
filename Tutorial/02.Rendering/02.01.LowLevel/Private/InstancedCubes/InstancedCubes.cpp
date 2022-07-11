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
#include "InstancedCubes/InstancedCubes.h"
#include "InstancedCubes/CubeRendererDrawInstanced/CubeRendererDrawInstanced.h"
#include "InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays.h"
#include "Application.h"
#include <RECore/Color/Color4.h>

#ifdef RENDERER_IMGUI
	#include <RERenderer/IRenderer.h>
	#include <RERenderer/DebugGui/DebugGuiManager.h>
	#include <RERenderer/DebugGui/DebugGuiHelper.h>
#endif

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	PRAGMA_WARNING_DISABLE_MSVC(4668)	// warning C4668: '_M_HYBRID_X86_ARM64' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <glm/gtc/type_ptr.hpp>
	#include <glm/gtc/matrix_transform.hpp>
PRAGMA_WARNING_POP

#include <REInput/Input/InputManager.h>

#include <math.h>
#include <stdio.h>
#include <limits.h>


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
InstancedCubes::InstancedCubes(Application& cApplication, const RECore::String& name)
: ExampleBase(cApplication, name),
	mInputManager(new REInput::InputManager()),
	mCubeRenderer(nullptr),
	mNumberOfCubeInstances(1000),
	mGlobalTimer(0.0f),
	mGlobalScale(1.0f),
	mDisplayStatistics(true),
	mFPSUpdateTimer(0.0f),
	mFramesSinceCheck(0),
	mFramesPerSecond(0.0f)
{
	// Nothing here
}

InstancedCubes::~InstancedCubes()
{
	delete mInputManager;

	// The resources are released within "onDeinitialization()"
}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
void InstancedCubes::onInitialization()
{
	// Get and check the RHI instance
	RERHI::RHIDynamicRHIPtr rhi(getRhi());
	if (nullptr != rhi)
	{
		// Create the cube renderer instance
		// -> Evaluate the feature set of the used RHI
		// TODO(naetherm) This example doesn't support texture buffer emulation, which for OpenGL ES 3 is currently used
		const RERHI::Capabilities& capabilities = rhi->getCapabilities();
		if (capabilities.drawInstanced && capabilities.maximumNumberOf2DTextureArraySlices > 0 && capabilities.maximumTextureBufferSize > 0 && rhi->getNameId() != RERHI::NameId::OPENGLES3)
		{
			// Render cubes by using draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
			mCubeRenderer = new CubeRendererDrawInstanced(*rhi, getMainRenderTarget()->getRenderPass(), NUMBER_OF_TEXTURES, SCENE_RADIUS);
		}
		else if (capabilities.instancedArrays)
		{
			// Render cubes by using instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
			mCubeRenderer = new CubeRendererInstancedArrays(*rhi, getMainRenderTarget()->getRenderPass(), NUMBER_OF_TEXTURES, SCENE_RADIUS);
		}

		// Tell the cube renderer about the number of cubes
		if (nullptr != mCubeRenderer)
		{
			mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
		}
	}
}

void InstancedCubes::onDeinitialization()
{
	// Destroy the cube renderer, in case there's one
	if (nullptr != mCubeRenderer)
	{
		delete mCubeRenderer;
		mCubeRenderer = nullptr;
	}
}

void InstancedCubes::onUpdate()
{
	// Stop the stopwatch
	#ifdef RENDERER
		mStopwatch.stop();
	#endif

	// Update the global timer (FPS independent movement)
	#ifdef RENDERER
		const float timeDifference = mStopwatch.getMilliseconds();
	#else
		const float timeDifference = 0.0f;
	#endif
	mGlobalTimer += timeDifference;

	// Calculate the current FPS
	++mFramesSinceCheck;
	mFPSUpdateTimer += timeDifference;
	if (mFPSUpdateTimer > 1000.0f)
	{
		mFramesPerSecond   = static_cast<float>(mFramesSinceCheck) / (mFPSUpdateTimer / 1000.0f);
		mFPSUpdateTimer   -= 1000.0f;
		mFramesSinceCheck  = 0;
	}

	// Start the stopwatch
	#ifdef RENDERER
		mStopwatch.start();
	#endif

	{ // Input
    REInput::Keyboard* keyboard = mInputManager->getKeyboard();
		if (keyboard->hasChanged())
		{
			// Add a fixed number of cubes
			if (keyboard->NumpadAdd.checkHitAndReset() || keyboard->Add.checkHitAndReset())
			{
				// Upper limit, just in case someone tries something nasty
				if (mNumberOfCubeInstances < UINT_MAX - NUMBER_OF_CHANGED_CUBES)
				{
					// Update the number of cubes
					mNumberOfCubeInstances += NUMBER_OF_CHANGED_CUBES;

					// Tell the cube renderer about the number of cubes
					if (nullptr != mCubeRenderer)
					{
						mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
					}
				}
			}

			// Subtract a fixed number of cubes
			if (keyboard->NumpadSubtract.checkHitAndReset() || keyboard->Subtract.checkHitAndReset())
			{
				// Lower limit
				if (mNumberOfCubeInstances > 1)
				{
					// Update the number of cubes
					if (mNumberOfCubeInstances > NUMBER_OF_CHANGED_CUBES)
					{
						mNumberOfCubeInstances -= NUMBER_OF_CHANGED_CUBES;
					}
					else
					{
						mNumberOfCubeInstances = 1;
					}

					// Tell the cube renderer about the number of cubes
					if (nullptr != mCubeRenderer)
					{
						mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
					}
				}
			}

			// Scale cubes up (change the size of all cubes at the same time)
			if (keyboard->Up.checkHitAndReset())
			{
				mGlobalScale += 0.1f;
			}

			// Scale cubes down (change the size of all cubes at the same time)
			if (keyboard->Down.checkHitAndReset())
			{
				mGlobalScale -= 0.1f;	// No need to check for negative values, results in entertaining inversed backface culling
			}

			// Show/hide statistics
			if (keyboard->Space.checkHitAndReset())
			{
				// Toggle display of statistics
				mDisplayStatistics = !mDisplayStatistics;
			}
		}
		mInputManager->update();
	}
}

void InstancedCubes::onDraw(RERHI::RHICommandBuffer& commandBuffer)
{
	// Clear the graphics color buffer of the current render target with gray, do also clear the depth buffer
	RERHI::Command::ClearGraphics::create(mCommandBuffer, RERHI::ClearFlag::COLOR_DEPTH, RECore::Color4::GRAY);

	// Draw the cubes
	if (nullptr != mCubeRenderer)
	{
		mCubeRenderer->fillCommandBuffer(mGlobalTimer, mGlobalScale, sin(mGlobalTimer * 0.001f) * SCENE_RADIUS, sin(mGlobalTimer * 0.0005f) * SCENE_RADIUS, cos(mGlobalTimer * 0.0008f) * SCENE_RADIUS, mCommandBuffer);
	}

	// Display statistics
	#ifdef RENDERER_IMGUI
		if (mDisplayStatistics && nullptr != getMainRenderTarget() && nullptr != getRenderer())
		{
			RERenderer::DebugGuiManager& debugGuiManager = getRendererSafe().getDebugGuiManager();
			debugGuiManager.newFrame(*getMainRenderTarget());

			// Is there a cube renderer instance?
			if (nullptr != mCubeRenderer)
			{
				char text[128];

				// Number of cubes
				snprintf(text, GLM_COUNTOF(text), "Number of cubes: %u", mNumberOfCubeInstances);
				RERenderer::DebugGuiHelper::drawText(text, 10.0f, 10.0f);

				// Frames per second
				snprintf(text, GLM_COUNTOF(text), "Frames per second: %.2f", mFramesPerSecond);
				RERenderer::DebugGuiHelper::drawText(text, 10.0f, 40.0f);

				// Cubes per second
				// -> In every frame we draw n-cubes...
				// -> TODO(naetherm) This number can get huge... had over 1 million cubes with >25 FPS... million cubes at ~2.4 FPS...
				snprintf(text, GLM_COUNTOF(text), "Cubes per second: %u", static_cast<RECore::uint32>(mFramesPerSecond) * mNumberOfCubeInstances);
				RERenderer::DebugGuiHelper::drawText(text, 10.0f, 70.0f);
			}
			else
			{
				RERenderer::DebugGuiHelper::drawText("No cube renderer instance", 10.0f, 10.0f);
			}
			debugGuiManager.fillGraphicsCommandBufferUsingFixedBuildInRhiConfiguration(mCommandBuffer);
		}
	#endif

	// Append command buffer to the given command buffer
	mCommandBuffer.appendToCommandBufferAndClear(commandBuffer);
}
