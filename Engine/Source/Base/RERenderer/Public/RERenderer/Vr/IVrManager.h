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
#include <RECore/Core/Manager.h>
#include <RECore/String/StringId.h>

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	#include <glm/glm.hpp>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERHI
{
	class RHIRenderTarget;
}
namespace RERenderer
{
	class LightSceneItem;
	class CameraSceneItem;
	class CompositorWorkspaceInstance;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::StringId AssetId;			///< Asset identifier, internally just a POD "RECore::uint32", string ID scheme is "<project name>/<asset directory>/<asset name>"
	typedef RECore::StringId VrManagerTypeId;	///< VR manager identifier, internally just a POD "RECore::uint32"
	typedef RECore::uint32 SceneResourceId;	///< POD scene resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class IVrManager : private RECore::Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererImpl;
		friend class CompositorWorkspaceInstance;	// Calls "RERenderer::IVrManager::executeCompositorWorkspaceInstance()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class VrEye : RECore::int8
		{
			LEFT  =  0,
			RIGHT =  1
		};


	//[-------------------------------------------------------]
	//[ Public virtual RERenderer::IVrManager methods           ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] virtual VrManagerTypeId getVrManagerTypeId() const = 0;

		/**
		*  @brief
		*    Check whether or not a head-mounted display (HMD) is present
		*
		*  @return
		*    "true" if a HMD is present, else "false" (OpenVR shared library not there? OpenVR runtime not installed? HMD not connected?)
		*
		*  @note
		*    - The VR manager is using OpenVR with runtime linking, so you need to ensure the OpenVR shared library
		*      can be loaded ("openvr_api.dll" under Microsoft Windows, "libopenvr_api.so" under Linux)
		*    - Method can also be used when the VR manager is not running
		*/
		[[nodiscard]] virtual bool isHmdPresent() const = 0;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual void setSceneResourceId(SceneResourceId sceneResourceId) = 0;
		[[nodiscard]] virtual bool startup(AssetId vrDeviceMaterialAssetId) = 0;	// If invalid material asset ID, no VR devices will be rendered
		[[nodiscard]] virtual bool isRunning() const = 0;
		virtual void shutdown() = 0;

		//[-------------------------------------------------------]
		//[ Transform (only valid if manager is running)          ]
		//[-------------------------------------------------------]
		virtual void updateHmdMatrixPose(CameraSceneItem* cameraSceneItem) = 0;
		[[nodiscard]] virtual glm::mat4 getHmdViewSpaceToClipSpaceMatrix(VrEye vrEye, float nearZ, float farZ) const = 0;
		[[nodiscard]] virtual glm::mat4 getHmdEyeSpaceToHeadSpaceMatrix(VrEye vrEye) const = 0;
		[[nodiscard]] virtual const glm::mat4& getHmdHeadSpaceToWorldSpaceMatrix() const = 0;
		[[nodiscard]] virtual const glm::mat4& getPreviousHmdHeadSpaceToWorldSpaceMatrix() const = 0;


	//[-------------------------------------------------------]
	//[ Private virtual RERenderer::IVrManager methods          ]
	//[-------------------------------------------------------]
	private:
		//[-------------------------------------------------------]
		//[ Render (only valid if manager is running)             ]
		//[-------------------------------------------------------]
		virtual void executeCompositorWorkspaceInstance(CompositorWorkspaceInstance& compositorWorkspaceInstance, RERHI::RHIRenderTarget& renderTarget, CameraSceneItem* cameraSceneItem, const LightSceneItem* lightSceneItem) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline IVrManager()
		{
			// Nothing here
		}

		inline virtual ~IVrManager()
		{
			// Nothing here
		}

		explicit IVrManager(const IVrManager&) = delete;
		IVrManager& operator=(const IVrManager&) = delete;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
