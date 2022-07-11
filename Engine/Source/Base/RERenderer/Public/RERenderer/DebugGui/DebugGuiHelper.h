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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERenderer/RERenderer.h"
#include <imgui.h>

namespace RECore {
class Transform;
}

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer
{
	class CameraSceneItem;
	class SkeletonMeshSceneItem;
	class CompositorWorkspaceInstance;
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
	*    Debug GUI helper generating ImGui ( https://github.com/ocornut/imgui ) commands
	*/
	class DebugGuiHelper final
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class DebugGuiManager;	// Calls "RERenderer::DebugGuiHelper::beginFrame()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class GizmoOperation
		{
			TRANSLATE_X      = (1u << 0),
      TRANSLATE_Y      = (1u << 1),
      TRANSLATE_Z      = (1u << 2),
			ROTATE_X      = (1u << 3),
      ROTATE_Y      = (1u << 4),
      ROTATE_Z      = (1u << 5),
      ROTATE_SCREEN      = (1u << 6),
			SCALE_X      = (1u << 7),
      SCALE_Y      = (1u << 8),
      SCALE_Z      = (1u << 9),
      BOUNDS      = (1u << 10),
      TRANSLATE = TRANSLATE_X | TRANSLATE_Y | TRANSLATE_Z,
      ROTATE = ROTATE_X | ROTATE_Y | ROTATE_Z | ROTATE_SCREEN,
      SCALE = SCALE_X | SCALE_Y | SCALE_Z
		};
		enum class GizmoMode
		{
			LOCAL,
			WORLD
		};
		struct GizmoSettings final
		{
			GizmoOperation	currentGizmoOperation = GizmoOperation::TRANSLATE;
			GizmoMode		currentGizmoMode	  = GizmoMode::WORLD;
			bool			useSnap				  = false;
			float			snap[3]					{1.0f, 1.0f, 1.0f};
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void drawText(const char* text, float x, float y, bool drawBackground = true);
		static void drawGizmo(const CameraSceneItem& cameraSceneItem, GizmoSettings& gizmoSettings, RECore::Transform& transform, ImVec2& displaySize, ImVec2& displayPos);	// Using "ImGuizmo" ( https://github.com/CedricGuillemet/ImGuizmo )
		static void drawSkeleton(const CameraSceneItem& cameraSceneItem, const SkeletonMeshSceneItem& skeletonMeshSceneItem, ImVec2& displaySize, ImVec2& displayPos);
		static void drawGrid(const CameraSceneItem& cameraSceneItem, float cellSize = 1.0f, double yPosition = 0.0f);


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		inline static void beginFrame()
		{
			// Reset the draw text counter
			mDrawTextCounter = 0;
		}

		static void drawMetricsWindow(bool& open, CompositorWorkspaceInstance* compositorWorkspaceInstance);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		DebugGuiHelper() = delete;
		~DebugGuiHelper() = delete;
		explicit DebugGuiHelper(const DebugGuiHelper&) = delete;
		DebugGuiHelper& operator=(const DebugGuiHelper&) = delete;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static RECore::uint32 mDrawTextCounter;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
