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
#include <RECore/Resource/IResourceListener.h>

#include <RERHI/Rhi.h>
#include <string.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
typedef struct ini_t ini_t;
class SceneView;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    ImGui example selector
*/
class Gui final : public ExampleBase, public RECore::IResourceListener
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Default constructor
	*/
  Gui();

	/**
	*  @brief
	*    Destructor
	*/
	virtual ~Gui() override;


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
public:
	virtual void onInitialization() override;
	virtual void onDeinitialization() override;
	virtual void onDraw(RERHI::RHICommandBuffer& commandBuffer) override;


//[-------------------------------------------------------]
//[ Protected virtual RECore::IResourceListener methods ]
//[-------------------------------------------------------]
protected:
  virtual void onLoadingStateChange(const RECore::IResource& resource) override;


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	void loadIni();
	void saveIni();
	void destroyIni();
	void createDebugGui();
  void createSceneView();
  void createAssetBrowser();
  void fillCommandBuffer();


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
  RERHI::RHIBufferManagerPtr			mBufferManager;			///< Buffer manager, can be a null pointer
  RERHI::RHITextureManagerPtr			mTextureManager;		///< Texture manager, can be a null pointer
  RERHI::RHIRootSignaturePtr			mRootSignature;			///< Root signature, can be a null pointer
  RERHI::RHIFramebufferPtr			mFramebuffer;			///< Framebuffer object (FBO), can be a null pointer
  RERHI::RHIResourceGroupPtr			mTextureGroup;			///< Texture group, can be a null pointer
  RERHI::RHIResourceGroupPtr			mSamplerStateGroup;		///< Sampler state resource group, can be a null pointer
  RERHI::RHIGraphicsPipelineStatePtr	mGraphicsPipelineState;	///< Graphics pipeline state object (PSO), can be a null pointer
  RERHI::RHIVertexArrayPtr			mVertexArray;
  RERHI::RHITexture2DPtr mTexture2DPtr;

  RERHI::RHICommandBuffer				mFBCommandBuffer;			///< Command buffer
  RERHI::RHICommandBuffer mCommandBuffer;	///< RHI command buffer
	// Current selection
	std::string mSelectedRhiName;
	std::string mSelectedExampleName;
	// Ini settings indices
	std::vector<char> mIniFileContent;	// Defined here to avoid reallocations
	ini_t*			  mIni;
	int				  mSelectedRhiNameIndex;
	int				  mSelectedExampleNameIndex;
  bool mShowDemoWindow;

  SceneView* mSceneView;
  bool mSceneViewOnFocus;

  char m_filter[128];

};
