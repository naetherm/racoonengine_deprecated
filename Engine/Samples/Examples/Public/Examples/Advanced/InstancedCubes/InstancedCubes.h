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
#include "Examples/Framework/ExampleBase.h"

#ifdef RENDERER
	#include <RECore/Time/Stopwatch.h>
#endif
#include <RERHI/Rhi.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace REInput
{
	class InputManager;
}
class ICubeRenderer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Cube instancing application class
*
*  @remarks
*    Demonstrates:
*    - Vertex buffer object (VBO)
*    - Index buffer object (IBO)
*    - Vertex array object (VAO)
*    - Texture buffer object (TBO)
*    - Uniform buffer object (UBO)
*    - 2D texture
*    - 2D texture array
*    - Sampler state object (SO)
*    - Vertex shader (VS) and fragment shader (FS)
*    - Root signature
*    - Graphics pipeline state object (PSO)
*    - Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
*    - Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
*/
class InstancedCubes final : public ExampleBase
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Default constructor
	*/
	InstancedCubes();

	/**
	*  @brief
	*    Destructor
	*/
	virtual ~InstancedCubes() override;


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
public:
	virtual void onInitialization() override;
	virtual void onDeinitialization() override;
	virtual void onUpdate() override;
	virtual void onDraw(RERHI::RHICommandBuffer& commandBuffer) override;


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
private:
	static constexpr RECore::uint32 NUMBER_OF_CHANGED_CUBES = 1000;	///< Number of changed cubes on key interaction
	static constexpr RECore::uint32 NUMBER_OF_TEXTURES	  = 8;		///< Number of textures
	static constexpr RECore::uint32 SCENE_RADIUS			  = 10;		///< Scene radius


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
  REInput::InputManager*	mInputManager;			///< Input manager instance, always valid
	ICubeRenderer*				mCubeRenderer;			///< Cube renderer instance, can be a null pointer
	RECore::uint32					mNumberOfCubeInstances;	///< Number of cube instances
	RERHI::RHICommandBuffer			mCommandBuffer;			///< Command buffer
	// The rest is for timing and statistics
	#ifdef RENDERER
		RECore::Stopwatch mStopwatch;	///< Stopwatch instance
	#endif
	float mGlobalTimer;			///< Global timer
	float mGlobalScale;			///< Global scale
	bool  mDisplayStatistics;	///< Display statistics?
	float mFPSUpdateTimer;		///< Timer for FPS update
	int   mFramesSinceCheck;	///< Number of frames since last FPS update
	float  mFramesPerSecond;	///< Current frames per second


};
