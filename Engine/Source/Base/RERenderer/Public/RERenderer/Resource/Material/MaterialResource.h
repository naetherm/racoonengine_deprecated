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
#include "RERenderer/Context.h"
#include <RECore/Resource/IResource.h>
#include "RERenderer/Resource/Material/MaterialProperties.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace RERenderer {
class Renderable;

class MaterialTechnique;

class MaterialResourceLoader;
}
namespace RECore {
	template <class ELEMENT_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class PackedElementManager;
	template <class TYPE, class LOADER_TYPE, typename ID_TYPE, uint32_t MAXIMUM_NUMBER_OF_ELEMENTS> class ResourceManagerTemplate;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32_t MaterialTechniqueId;	///< Material technique identifier, result of hashing the material technique name via "RERenderer::StringId"
	typedef uint32_t MaterialResourceId;	///< POD material resource identifier


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material resource
	*/
	class MaterialResource final : public RECore::IResource
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Renderable;																			// Must be able to attach/detach itself from the material resource
		friend class MaterialResourceLoader;
		friend class MaterialResourceManager;
		friend class MaterialBlueprintResourceManager;
		friend RECore::ResourceManagerTemplate<MaterialResource, MaterialResourceLoader, MaterialResourceId, 4096>;	// Type definition of template class
		friend RECore::PackedElementManager<MaterialResource, MaterialResourceId, 4096>;							// Type definition of template class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<MaterialTechnique*> SortedMaterialTechniqueVector;

		// Fixed build in material properties
		static constexpr uint32_t RENDER_QUEUE_INDEX_PROPERTY_ID  = STRING_ID("RenderQueueIndex");	///< "RenderQueueIndex", value type = "INTEGER" with usage = "STATIC" and value range = [0, 255]
		static constexpr uint32_t CAST_SHADOWS_PROPERTY_ID		  = STRING_ID("CastShadows");		///< "CastShadows", value type = "BOOLEAN" with usage = "STATIC"
		static constexpr uint32_t USE_ALPHA_MAP_PROPERTY_ID		  = STRING_ID("UseAlphaMap");		///< "UseAlphaMap", value type = "BOOLEAN" with usage = "SHADER_COMBINATION"
		static constexpr uint32_t LOCAL_COMPUTE_SIZE_PROPERTY_ID  = STRING_ID("LocalComputeSize");	///< "LocalComputeSize", value type = "INTEGER_3" with usage = "STATIC" and value e.g. "32 32 1"
		static constexpr uint32_t GLOBAL_COMPUTE_SIZE_PROPERTY_ID = STRING_ID("GlobalComputeSize");	/** "GlobalComputeSize":
																										 - Static value example: value type = "INTEGER_3" with usage = "STATIC" and value e.g. "1920 1080 1"
																										 - Dynamic value example: value type = "INTEGER_3" with usage = "MATERIAL_REFERENCE" and value e.g. "@OutputTexture2D" (while material property "OutputTexture2D" has value type = "TEXTURE_ASSET_ID" with usage = "TEXTURE_REFERENCE" and value e.g. "RacoonEngine/Texture/DynamicByCode/BlackMap2D"), results in texture size as value
																									*/


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] Context& getContext() const;

		/**
		*  @brief
		*    Return the parent material resource ID
		*
		*  @return
		*    The parent material resource ID, invalid if there's no parent
		*/
		[[nodiscard]] inline MaterialResourceId getParentMaterialResourceId() const
		{
			return mParentMaterialResourceId;
		}

		/**
		*  @brief
		*    Set the parent material resource ID
		*
		*  @param[in] parentMaterialResourceId
		*    Parent material resource ID, can be invalid
		*
		*  @note
		*    - Parent material resource must be fully loaded
		*    - All property values will be reset
		*/
		void setParentMaterialResourceId(MaterialResourceId parentMaterialResourceId);

		/**
		*  @brief
		*    Return the sorted material technique vector
		*
		*  @return
		*    The sorted material technique vector
		*/
		[[nodiscard]] inline SortedMaterialTechniqueVector& getSortedMaterialTechniqueVector()
		{
			return mSortedMaterialTechniqueVector;
		}
		[[nodiscard]] inline const SortedMaterialTechniqueVector& getSortedMaterialTechniqueVector() const
		{
			return mSortedMaterialTechniqueVector;
		}

		/**
		*  @brief
		*    Return a material technique by ID
		*
		*  @param[in] materialTechniqueId
		*    ID of the material technique to return
		*
		*  @return
		*    The requested material technique, null pointer on error, don't destroy the returned instance
		*/
		[[nodiscard]] MaterialTechnique* getMaterialTechniqueById(MaterialTechniqueId materialTechniqueId) const;

		/**
		*  @brief
		*    Destroy all material techniques
		*/
		void destroyAllMaterialTechniques();

		//[-------------------------------------------------------]
		//[ Property                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the material properties
		*
		*  @return
		*    The material properties
		*/
		[[nodiscard]] inline const MaterialProperties& getMaterialProperties() const
		{
			return mMaterialProperties;
		}

		/**
		*  @brief
		*    Return the material properties as sorted vector
		*
		*  @return
		*    The material properties as sorted vector
		*/
		[[nodiscard]] inline const MaterialProperties::SortedPropertyVector& getSortedPropertyVector() const
		{
			return mMaterialProperties.getSortedPropertyVector();
		}

		/**
		*  @brief
		*    Remove all material properties
		*/
		inline void removeAllProperties()
		{
			return mMaterialProperties.removeAllProperties();
		}

		/**
		*  @brief
		*    Return a material property by ID
		*
		*  @param[in] materialPropertyId
		*    ID of the material property to return
		*
		*  @return
		*    The requested material property, null pointer on error, don't destroy the returned instance
		*/
		[[nodiscard]] inline const MaterialProperty* getPropertyById(MaterialPropertyId materialPropertyId) const
		{
			return mMaterialProperties.getPropertyById(materialPropertyId);
		}

		/**
		*  @brief
		*    Set a material property value by ID
		*
		*  @param[in] materialPropertyId
		*    ID of the material property to set the value from
		*  @param[in] materialPropertyValue
		*    The material property value to set
		*  @param[in] materialPropertyUsage
		*    The material property usage
		*
		*  @return
		*    "true" if a material property change has been detected, else "false"
		*/
		inline bool setPropertyById(MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, MaterialProperty::Usage materialPropertyUsage = MaterialProperty::Usage::UNKNOWN)
		{
			return setPropertyByIdInternal(materialPropertyId, materialPropertyValue, materialPropertyUsage, true);
		}

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		// TODO(naetherm)
		void releaseTextures();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline MaterialResource() :
			mParentMaterialResourceId(RECore::getInvalid<MaterialResourceId>())
		{
			// Nothing here
		}

		virtual ~MaterialResource() override;
		explicit MaterialResource(const MaterialResource&) = delete;
		MaterialResource& operator=(const MaterialResource&) = delete;
		MaterialResource& operator=(MaterialResource&& materialResource);

		//[-------------------------------------------------------]
		//[ "RERenderer::PackedElementManager" management           ]
		//[-------------------------------------------------------]
		inline void initializeElement(MaterialResourceId materialResourceId)
		{
			// Sanity checks
			RHI_ASSERT(RECore::isInvalid(mParentMaterialResourceId), "Invalid parent material resource ID")
			RHI_ASSERT(mSortedChildMaterialResourceIds.empty(), "Invalid sorted child material resource IDs")
			RHI_ASSERT(mSortedMaterialTechniqueVector.empty(), "Invalid sorted material technique vector")
			RHI_ASSERT(mMaterialProperties.getSortedPropertyVector().empty(), "Invalid material properties")

			// Call base implementation
			IResource::initializeElement(materialResourceId);
		}

		void deinitializeElement();

		/**
		*  @brief
		*    Set a material property value by ID
		*
		*  @param[in] materialPropertyId
		*    ID of the material property to set the value from
		*  @param[in] materialPropertyValue
		*    The material property value to set
		*  @param[in] materialPropertyUsage
		*    The material property usage
		*  @param[in] changeOverwrittenState
		*    Change overwritten state?
		*
		*  @return
		*    Pointer to the added or changed property, null pointer if no material property change has been detected, don't destroy the returned instance
		*/
		bool setPropertyByIdInternal(MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, MaterialProperty::Usage materialPropertyUsage, bool changeOverwrittenState);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<MaterialResourceId> SortedChildMaterialResourceIds;
		typedef std::vector<Renderable*>		AttachedRenderables;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialResourceId			   mParentMaterialResourceId;
		SortedChildMaterialResourceIds mSortedChildMaterialResourceIds;
		SortedMaterialTechniqueVector  mSortedMaterialTechniqueVector;
		MaterialProperties			   mMaterialProperties;
		AttachedRenderables			   mAttachedRenderables;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
