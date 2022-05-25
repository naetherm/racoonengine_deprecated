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
#include "ExampleBase.h"

#include <RECore/Math/Transform.h>
#include <RERenderer/DebugGui/DebugGuiHelper.h>
#include <RECore/Resource/IResourceListener.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class IController;
typedef struct ini_t ini_t;
namespace REInput
{
	class InputManager;
}
namespace RERenderer
{
	class SceneNode;
	class CameraSceneItem;
	class SunlightSceneItem;
	class DebugDrawSceneItem;
	class SkeletonMeshSceneItem;
	class CompositorWorkspaceInstance;
}


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
namespace RERenderer
{
	typedef RECore::uint32 SceneResourceId;		///< POD scene resource identifier
	typedef RECore::uint32 MaterialResourceId;	///< POD material resource identifier
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene example
*
*  @remarks
*    Demonstrates:
*    - Compositor
*    - Scene
*    - Virtual reality (VR)
*/
class Scene final : public ExampleBase, public RECore::IResourceListener
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Default constructor
	*/
	Scene(Application& cApplication, const RECore::String& name);

	/**
	*  @brief
	*    Destructor
	*/
	virtual ~Scene() override;


//[-------------------------------------------------------]
//[ Public virtual ExampleBase methods                    ]
//[-------------------------------------------------------]
public:
	virtual void onInitialization() override;
	virtual void onDeinitialization() override;
	virtual void onUpdate() override;
	virtual void onDraw(RERHI::RHICommandBuffer& commandBuffer) override;

	[[nodiscard]] inline virtual bool doesCompleteOwnDrawing() const override
	{
		// This example wants complete control of the drawing
		return true;
	}


//[-------------------------------------------------------]
//[ Protected virtual RECore::IResourceListener methods ]
//[-------------------------------------------------------]
protected:
	virtual void onLoadingStateChange(const RECore::IResource& resource) override;


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	explicit Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	void loadIni();
	void saveIni();
	void destroyIni();
	void applyCurrentSettings(RERHI::RHIRenderTarget& mainRenderTarget);
	void createCompositorWorkspace();
	void createDebugGui(RERHI::RHIRenderTarget& mainRenderTarget);
	void trySetCustomMaterialResource();


//[-------------------------------------------------------]
//[ Private definitions                                   ]
//[-------------------------------------------------------]
private:
	enum class Msaa
	{
		NONE,
		TWO,
		FOUR,
		EIGHT
	};
	enum class Compositor
	{
		DEBUG,
		FORWARD,
		DEFERRED,
		VR
	};
	enum class ShadowQuality
	{
		NONE,
		LOW,
		MEDIUM,
		HIGH,
		ULTRA,
		EPIC
	};
	enum class TextureFiltering
	{
		POINT,
		BILINEAR,
		TRILINEAR,
		ANISOTROPIC_2,
		ANISOTROPIC_4,
		ANISOTROPIC_8,
		ANISOTROPIC_16
	};


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
  REInput::InputManager*			   mInputManager;
	RERenderer::CompositorWorkspaceInstance* mCompositorWorkspaceInstance;
	bool								   mFirstFrame;
	RERenderer::SceneResourceId			   mSceneResourceId;
	RERenderer::MaterialResourceId		   mMaterialResourceId;
	RERenderer::MaterialResourceId		   mCloneMaterialResourceId;
	bool								   mCustomMaterialResourceSet;
	IController*						   mController;
	bool								   mDebugDrawEnabled;
	// Crazy raw-pointers to point-of-interest scene stuff
	RERenderer::CameraSceneItem*		 mCameraSceneItem;
	RERenderer::SunlightSceneItem*	 mSunlightSceneItem;
	RERenderer::SkeletonMeshSceneItem* mSkeletonMeshSceneItem;
	RERenderer::DebugDrawSceneItem*	 mDebugDrawSceneItem;
	RERenderer::SceneNode*			 mSceneNode;
	// States for runtime-editing
	RERenderer::DebugGuiHelper::GizmoSettings mGizmoSettings;
	// Video
	bool  mFullscreen;
	bool  mCurrentFullscreen;
	float mResolutionScale;
	bool  mUseVerticalSynchronization;
	bool  mCurrentUseVerticalSynchronization;
	int	  mCurrentMsaa;
	// Graphics
	Compositor	  mInstancedCompositor;
	int			  mCurrentCompositor;
	ShadowQuality mShadowQuality;
	int			  mCurrentShadowQuality;
	bool		  mHighQualityRendering;
	bool		  mHighQualityLighting;
	bool		  mSoftParticles;
	int			  mCurrentTextureFiltering;
	int			  mNumberOfTopTextureMipmapsToRemove;
	int			  mNumberOfTopMeshLodsToRemove;
	int			  mTerrainTessellatedTriangleWidth;
	// Environment
	float mCloudsIntensity;
	float mWindSpeed;
	float mWetSurfaces[4];	// x=wet level, y=hole/cracks flood level, z=puddle flood level, w=rain intensity
	// Post processing
	bool  mPerformFxaa;
	bool  mPerformSharpen;
	bool  mPerformChromaticAberration;
	bool  mPerformOldCrtEffect;
	bool  mPerformFilmGrain;
	bool  mPerformSepiaColorCorrection;
	bool  mPerformVignette;
	float mDepthOfFieldBlurrinessCutoff;
	// Selected material properties
	bool  mUseEmissiveMap;
	float mAlbedoColor[3];
	// Selected scene item
	float mRotationSpeed;
	bool  mShowSkeleton;
	// Scene hot-reloading memory
	bool				mHasCameraTransformBackup;
	RECore::Transform mCameraTransformBackup;
	// Ini settings indices
	std::vector<char> mIniFileContent;	// Defined here to avoid reallocations
	ini_t*			  mIni;
	int				  mMainWindowPositionSizeIniProperty;
	int				  mCameraPositionRotationIniProperty;
	int				  mOpenMetricsWindowIniProperty;
	int				  mDebugDrawEnabledIniProperty;


};
