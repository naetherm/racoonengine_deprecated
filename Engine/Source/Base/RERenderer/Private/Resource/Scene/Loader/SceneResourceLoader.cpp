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
#include "RERenderer/Resource/Scene/Loader/SceneResourceLoader.h"
#include "RERenderer/Resource/Scene/Loader/SceneFileFormat.h"
#include "RERenderer/Resource/Scene/Item/ISceneItem.h"
#include "RERenderer/Resource/Scene/SceneResource.h"
#include "RERenderer/IRenderer.h"


// TODO(naetherm) Error handling


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		void itemDeserialization(RECore::IFile& file, RERenderer::SceneResource& sceneResource, RERenderer::SceneNode& sceneNode)
		{
			// Read the scene item header
			RERenderer::v1Scene::ItemHeader itemHeader;
			file.read(&itemHeader, sizeof(RERenderer::v1Scene::ItemHeader));

			// Create the scene item
			RERenderer::ISceneItem* sceneItem = sceneResource.createSceneItem(itemHeader.typeId, sceneNode);
			if (nullptr != sceneItem && 0 != itemHeader.numberOfBytes)
			{
				// Load in the scene item data
				// TODO(naetherm) Get rid of the new/delete in here
				RECore::uint8* data = new RECore::uint8[itemHeader.numberOfBytes];
				file.read(data, itemHeader.numberOfBytes);

				// Deserialize the scene item
				sceneItem->deserialize(itemHeader.numberOfBytes, data);

				// Cleanup
				delete [] data;
			}
			else
			{
				// TODO(naetherm) Error handling
			}
		}

		void nodeDeserialization(RECore::IFile& file, RERenderer::SceneResource& sceneResource)
		{
			// Read in the scene node
			RERenderer::v1Scene::Node node;
			file.read(&node, sizeof(RERenderer::v1Scene::Node));

			// Create the scene node
			RERenderer::SceneNode* sceneNode = sceneResource.createSceneNode(node.transform);
			if (nullptr != sceneNode)
			{
				// Read in the scene items
				for (RECore::uint32 i = 0; i < node.numberOfItems; ++i)
				{
					itemDeserialization(file, sceneResource, *sceneNode);
				}
			}
			else
			{
				// TODO(naetherm) Error handling
			}
		}

		void nodesDeserialization(RECore::IFile& file, RERenderer::SceneResource& sceneResource)
		{
			// Read in the scene nodes
			RERenderer::v1Scene::Nodes nodes;
			file.read(&nodes, sizeof(RERenderer::v1Scene::Nodes));

			// Sanity check
			ASSERT(nodes.numberOfNodes > 0, "Invalid scene asset without any nodes detected")

			// Read in the scene nodes
			for (RECore::uint32 i = 0; i < nodes.numberOfNodes; ++i)
			{
				nodeDeserialization(file, sceneResource);
			}
		}


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Public virtual RECore::IResourceLoader methods      ]
	//[-------------------------------------------------------]
	void SceneResourceLoader::initialize(const RECore::Asset& asset, bool reload, RECore::IResource& resource)
	{
		IResourceLoader::initialize(asset, reload);
		mSceneResource = static_cast<SceneResource*>(&resource);
	}

	bool SceneResourceLoader::onDeserialization(RECore::IFile& file)
	{
		// Tell the memory mapped file about the LZ4 compressed data
		return mMemoryFile.loadLz4CompressedDataFromFile(v1Scene::FORMAT_TYPE, v1Scene::FORMAT_VERSION, file);
	}

	void SceneResourceLoader::onProcessing()
	{
		// Decompress LZ4 compressed data
		mMemoryFile.decompress();

		// Read in the scene header
		// TODO(naetherm) Currently the scene header is unused
		v1Scene::SceneHeader sceneHeader;
		mMemoryFile.read(&sceneHeader, sizeof(v1Scene::SceneHeader));

		// Can we create the RHI resource asynchronous as well?
		// -> For example scene items might create RHI resources, so we have to check for native RHI multithreading support in here
		if (mRenderer.getRhi().getCapabilities().nativeMultithreading)
		{
			// Read in the scene resource nodes
			::detail::nodesDeserialization(mMemoryFile, *mSceneResource);
		}
	}

	bool SceneResourceLoader::onDispatch()
	{
		// Can we create the RHI resource asynchronous as well?
		// -> For example scene items might create RHI resources, so we have to check for native RHI multithreading support in here
		if (!mRenderer.getRhi().getCapabilities().nativeMultithreading)
		{
			// Read in the scene resource nodes
			::detail::nodesDeserialization(mMemoryFile, *mSceneResource);
		}

		// Fully loaded
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
