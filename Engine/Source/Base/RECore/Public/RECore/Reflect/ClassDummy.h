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
*    Dummy 'Class' implementation
*/
class ClassDummy : public ClassImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ClassManager;


	//[-------------------------------------------------------]
	//[ Protected virtual ClassImpl functions                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool isDummy() const override;
		virtual void initClass() const override;
		virtual void deInitClass() const override;
		virtual const std::list<VarDesc*> &getAttributes() const override;
		virtual const VarDesc *getAttribute(const String &sName) const override;
		virtual const std::list<FuncDesc*> &getMethods() const override;
		virtual const FuncDesc *getMethod(const String &sName) const override;
		virtual const std::list<EventDesc*> &getSignals() const override;
		virtual const EventDesc *getSignal(const String &sName) const override;
		virtual const std::list<EventHandlerDesc*> &getSlots() const override;
		virtual const EventHandlerDesc *getSlot(const String &sName) const override;
		virtual bool hasConstructor() const override;
		virtual bool hasDefaultConstructor() const override;
		virtual const std::list<ConstructorDesc*> &getConstructors() const override;
		virtual const ConstructorDesc *getConstructor(const String &sName) const override;
		virtual Object *create() const override;
		virtual Object *create(const DynParams &cParams) const override;
		virtual Object *create(const String &sName, const DynParams &cParams) const override;
		virtual Object *create(const String &sName, const String &sParams) const override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
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
		*  @param[in] bHasConstructor
		*    Class has any constructors?
		*  @param[in] bHasDefaultConstructor
		*    Class has a default constructor?
		*/
		ClassDummy(uint32 nModuleID, const String &sName, const String &sDescription, const String &sNamespace, const String &sBaseClass, bool bHasConstructor, bool bHasDefaultConstructor);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ClassDummy();

		/*
		*  @brief
		*    Requests the real class implementation
		*
		*  @return
		*    The real class implementation if all went fine, else a null pointer
		*
		*  @note
		*    - Please note that it's not valid to use this dummy class implementation after this method was call because it may have been killed!
		*/
		ClassImpl *GetRealClassImpl() const;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		// Member lists (also including the members from base classes) - static because those are just dummies in here
		static std::map<String, MemberDesc*> m_mapMembers;		/**< Hash map of names -> members */
		static std::list<VarDesc*>			    m_lstAttributes;	/**< List of attributes */
		static std::list<FuncDesc*>				m_lstMethods;		/**< List of methods */
		static std::list<EventDesc*>				m_lstSignals;		/**< List of signals */
		static std::list<EventHandlerDesc*>		m_lstSlots;			/**< List of slots */
		static std::list<ConstructorDesc*>		m_lstConstructors;	/**< List of constructors */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool m_bHasConstructor;			/**< Class has any constructors? */
		bool m_bHasDefaultConstructor;	/**< Class has a default constructor? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
