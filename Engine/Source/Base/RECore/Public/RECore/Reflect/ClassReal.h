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
#include "RECore/Reflect/ClassImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Real 'Class' implementation
*
*  @note
*    - The class instance is created and destroyed automatically by the RTTI system (see "re_class"-macro)
*/
class ClassReal : public ClassImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MemberDesc;
	friend class ClassManager;


	//[-------------------------------------------------------]
	//[ Protected virtual ClassImpl functions                 ]
	//[-------------------------------------------------------]
	protected:
		RECORE_API virtual bool isDummy() const override;
		RECORE_API virtual void initClass() const override;
		RECORE_API virtual void deInitClass() const override;
		RECORE_API virtual const std::list<VarDesc*> &getAttributes() const override;
		RECORE_API virtual const VarDesc *getAttribute(const String &sName) const override;
		RECORE_API virtual const std::list<FuncDesc*> &getMethods() const override;
		RECORE_API virtual const FuncDesc *getMethod(const String &sName) const override;
		RECORE_API virtual const std::list<EventDesc*> &getSignals() const override;
		RECORE_API virtual const EventDesc *getSignal(const String &sName) const override;
		RECORE_API virtual const std::list<EventHandlerDesc*> &getSlots() const override;
		RECORE_API virtual const EventHandlerDesc *getSlot(const String &sName) const override;
		RECORE_API virtual bool hasConstructor() const override;
		RECORE_API virtual bool hasDefaultConstructor() const override;
		RECORE_API virtual const std::list<ConstructorDesc*> &getConstructors() const override;
		RECORE_API virtual const ConstructorDesc *getConstructor(const String &sName) const override;
		RECORE_API virtual Object *create() const override;
		RECORE_API virtual Object *create(const DynParams &cParams) const override;
		RECORE_API virtual Object *create(const String &sName, const DynParams &cParams) const override;
		RECORE_API virtual Object *create(const String &sName, const String &sParams) const override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nModuleID
		*    ID of owner module
		*  @param[in] sName
		*    Name
		*  @param[in] sDescription
		*    Description
		*  @param[in] sNamespace
		*    Namespace
		*  @param[in] sBaseClass
		*    Base class
		*/
		RECORE_API ClassReal(uint32 nModuleID, const String &sName, const String &sDescription, const String &sNamespace, const String &sBaseClass);

		/**
		*  @brief
		*    Destructor
		*/
		RECORE_API virtual ~ClassReal();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Add member
		*
		*  @param[in] pMemberDesc
		*    Member descriptor
		*/
		RECORE_API void addMember(MemberDesc *pMemberDesc);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Own data (does not include data from base classes)
		std::list<MemberDesc*>						m_lstOwnMembers;	/**< std::list of members */
		// Member lists (also including the members from base classes)
		mutable std::map<String, MemberDesc*>	m_mapMembers;		/**< Hash map of names -> members */
		mutable std::list<VarDesc*>					m_lstAttributes;	/**< std::list of attributes */
		mutable std::list<FuncDesc*>					m_lstMethods;		/**< std::list of methods */
		mutable std::list<EventDesc*>				m_lstSignals;		/**< std::list of signals */
		mutable std::list<EventHandlerDesc*>			m_lstSlots;			/**< std::list of slots */
		mutable std::list<ConstructorDesc*>			m_lstConstructors;	/**< std::list of constructors */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore