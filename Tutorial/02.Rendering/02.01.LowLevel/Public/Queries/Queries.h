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
#include "Triangle/Triangle.h"

#include <RERHI/Rhi.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Shows how to use asynchronous queries
*
*  @remarks
*    Demonstrates:
*    - Everything from "Triangle"
*    - Asynchronous queries
*/
class Queries final : public Triangle
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Default constructor
	*/
	inline Queries(Application& cApplication, const RECore::String& name)
    : Triangle(cApplication, name)
	{
		// Nothing here
	}

	/**
	*  @brief
	*    Destructor
	*/
	inline virtual ~Queries() override
	{
		// The resources are released within "onDeinitialization()"
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Public virtual IApplication methods                   ]
//[-------------------------------------------------------]
public:
	virtual void onInitialization() override;
	virtual void onDeinitialization() override;
	virtual void onDraw(RERHI::RHICommandBuffer& commandBuffer) override;


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	void fillCommandBuffer();


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	RERHI::RHIQueryPoolPtr mOcclusionQueryPool;				///< Asynchronous occlusion query pool, can be a null pointer
	RERHI::RHIQueryPoolPtr mPipelineStatisticsQueryPool;	///< Asynchronous pipeline statistics query pool, can be a null pointer
	RERHI::RHIQueryPoolPtr mTimestampQueryPool;				///< Asynchronous timestamp query pool, can be a null pointer


};
