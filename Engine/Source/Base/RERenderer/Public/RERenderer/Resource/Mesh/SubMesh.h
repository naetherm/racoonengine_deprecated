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
#include <RECore/Utility/GetInvalid.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32 MaterialResourceId;	///< POD material resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Sub-mesh class
	*/
	class SubMesh final
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline SubMesh() :
			mMaterialResourceId(RECore::getInvalid<MaterialResourceId>()),
			mStartIndexLocation(0),
			mNumberOfIndices(0)
		{
			// Nothing here
		}

		inline SubMesh(MaterialResourceId materialResourceId, RECore::uint32 startIndexLocation, RECore::uint32 numberOfIndices) :
			mMaterialResourceId(materialResourceId),
			mStartIndexLocation(startIndexLocation),
			mNumberOfIndices(numberOfIndices)
		{
			// Nothing here
		}

		inline explicit SubMesh(const SubMesh& subMesh) :
			mMaterialResourceId(subMesh.mMaterialResourceId),
			mStartIndexLocation(subMesh.mStartIndexLocation),
			mNumberOfIndices(subMesh.mNumberOfIndices)
		{
			// Nothing here
		}

		inline ~SubMesh()
		{
			// Nothing here
		}

		inline SubMesh& operator=(const SubMesh& subMesh)
		{
			mMaterialResourceId	= subMesh.mMaterialResourceId;
			mStartIndexLocation = subMesh.mStartIndexLocation;
			mNumberOfIndices	= subMesh.mNumberOfIndices;

			// Done
			return *this;
		}

		[[nodiscard]] inline MaterialResourceId getMaterialResourceId() const
		{
			return mMaterialResourceId;
		}

		inline void setMaterialResourceId(MaterialResourceId materialResourceId)
		{
			mMaterialResourceId = materialResourceId;
		}

		[[nodiscard]] inline RECore::uint32 getStartIndexLocation() const
		{
			return mStartIndexLocation;
		}

		inline void setStartIndexLocation(RECore::uint32 startIndexLocation)
		{
			mStartIndexLocation = startIndexLocation;
		}

		[[nodiscard]] inline RECore::uint32 getNumberOfIndices() const
		{
			return mNumberOfIndices;
		}

		inline void setNumberOfIndices(RECore::uint32 numberOfIndices)
		{
			mNumberOfIndices = numberOfIndices;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialResourceId mMaterialResourceId;	///< Material resource ID, can be set to invalid value
		RECore::uint32		   mStartIndexLocation;
		RECore::uint32		   mNumberOfIndices;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
