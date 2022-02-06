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
#include "Examples/Framework/IApplication.h"
#include "Examples/Framework/IApplicationFrontend.h"

#include <RERHI/Rhi.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ExampleBase;
#if defined(RENDERER) && defined(RENDERER_GRAPHICS_DEBUGGER)
	namespace RERenderer
	{
		class IGraphicsDebugger;
	}
#endif
namespace RERHI
{
	class RhiInstance;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    RHI application interface
*/
class IApplicationRhi : public IApplication, public IApplicationFrontend
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Constructor
	*
	*  @param[in] rhiName
	*    Case sensitive ASCII name of the RHI to instance, if null pointer or unknown RHI no RHI will be used.
	*    Example RHI names: "Null", "Vulkan", "OpenGL", "OpenGLES3", "Direct3D9", "Direct3D10", "Direct3D11", "Direct3D12"
	*  @param[in] exampleBase
	*    Reference to an example which should be used
	*/
	IApplicationRhi(const char* rhiName, ExampleBase& exampleBase);

	/**
	*  @brief
	*    Destructor
	*/
	inline virtual ~IApplicationRhi() override
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Public virtual IApplicationFrontend methods           ]
//[-------------------------------------------------------]
public:
	virtual void switchExample(const char* exampleName, const char* rhiName = nullptr) override;

	inline virtual void exit() override
	{
		IApplication::exit();
	}

	[[nodiscard]] inline virtual RERHI::RHIDynamicRHI* getRhi() const override
	{
		return mRhi;
	}

	[[nodiscard]] inline virtual RERHI::RHIRenderTarget* getMainRenderTarget() const override
	{
		return mMainSwapChain;
	}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
public:
	[[nodiscard]] virtual bool onInitialization() override;
	virtual void onDeinitialization() override;
	virtual void onUpdate() override;
	virtual void onResize() override;
	virtual void onToggleFullscreenState() override;
	virtual void onDrawRequest() override;
	virtual void onEscapeKey() override;


//[-------------------------------------------------------]
//[ Protected methods                                     ]
//[-------------------------------------------------------]
protected:
	/**
	*  @brief
	*    Create the RHI instance
	*/
	void createRhi();

	/**
	*  @brief
	*    Destroy the RHI instance
	*/
	void destroyRhi();


//[-------------------------------------------------------]
//[ Protected data                                        ]
//[-------------------------------------------------------]
protected:
	#ifdef RENDERER_GRAPHICS_DEBUGGER
		RERenderer::IGraphicsDebugger* mGraphicsDebugger;	///< Graphics debugger instance, can be a null pointer
	#endif
	ExampleBase& mExampleBase;


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	explicit IApplicationRhi(const IApplicationRhi& source) = delete;
	IApplicationRhi& operator =(const IApplicationRhi& source) = delete;

	/**
	*  @brief
	*    Create a RHI instance
	*
	*  @param[in] rhiName
	*    Case sensitive ASCII name of the RHI to instance, if null pointer nothing happens.
	*    Example RHI names: "Null", "Vulkan", "OpenGL", "OpenGLES3", "Direct3D9", "Direct3D10", "Direct3D11", "Direct3D12"
	*
	*  @return
	*    The created RHI instance, null pointer on error
	*/
	[[nodiscard]] RERHI::RHIDynamicRHI* createRhiInstance(const char* rhiName);


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	char			   mRhiName[32];	///< Case sensitive ASCII name of the RHI to instance
	RERHI::RHIContext*	   mRhiContext;		///< RHI context, can be a null pointer
	RERHI::RhiInstance*  mRhiInstance;	///< RHI instance, can be a null pointer
	RERHI::RHIDynamicRHI*		   mRhi;			///< RHI instance, can be a null pointer, do not destroy the instance
	RERHI::RHISwapChain*   mMainSwapChain;	///< Main swap chain instance, can be a null pointer, release the instance if you no longer need it
	RERHI::RHICommandBuffer mCommandBuffer;	///< Command buffer


};
