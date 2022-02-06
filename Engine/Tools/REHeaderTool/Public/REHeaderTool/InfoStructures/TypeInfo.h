/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <vector>

#include <clang-c/Index.h>

#include "REHeaderTool/InfoStructures/TypeDescriptor.h"
#include "REHeaderTool/InfoStructures/TemplateParamInfo.h"

namespace REHeaderTool
{
	class TypeInfo
	{
		private:
			/** Internal keywords used for type splitting. */
			static constexpr char const*	_classQualifier		= "class ";
			static constexpr char const*	_structQualifier	= "struct ";
			static constexpr char const*	_constQualifier		= "const";
			static constexpr char const*	_volatileQualifier	= "volatile";
			static constexpr char const*	_restrictQualifier	= "restrict";

			/**
			*	The full name represents the type name, containing all its qualifiers
			*	such as const, volatile or nested info (namespace, outer class).
			*
			*	i.e. const volatile ExampleNamespace::ExampleClass *const*&
			*/
			std::string						_fullName			= "";

			/** The canonical full name is the full name simplified by unwinding all aliases / typedefs. */
			std::string						_canonicalFullName	= "";

			/** List of typenames of the template type, empty if this is not a template type. */
			std::vector<TemplateParamInfo>	_templateParameters;

			/**
			*	@brief Compute a class template full name.
			* 
			*	@param cursor The class template cursor (Cursor_ClassTemplate).
			* 
			*	@return The computed full name.
			*/
			static std::string	computeClassTemplateFullName(CXCursor cursor)					noexcept;

			/**
			*	@brief Remove forward declared class qualifiers from a type string.
			*
			*	@param parsingStr The string we are removing the forward declared class specifier from.
			*/
			static void			removeForwardDeclaredClassQualifier(std::string& parsingStr)	noexcept;

			/**
			*	@brief Remove all namespaces and nested classes appearing before the actual type name.
			*
			*	@param typeString The string we are removing the namespaces and nested classes from.
			*/
			static void			removeNamespacesAndNestedClasses(std::string& typeString)		noexcept;

			/**
			*	@brief Remove the template parameters from a type string.
			* 
			*	@param typeString The string we are removing the template parameters from.
			*/
			static void			removeTemplateParameters(std::string& typeString)				noexcept;

			/** Init all internal flags according to the provided type. */
			void initialize(CXType cursorType)											noexcept;

			/** Init all internal flags according to the provided cursor. */
			void initialize(CXCursor cursor)											noexcept;

			/**
			*	@brief Fill the _templateParameters list with the given cursor.
			* 
			*	@param cursor The template type cursor.
			*/
			void fillTemplateParameters(CXCursor cursor)									noexcept;

			/**
			*	@brief Remove the const qualifier from a type string.
			*
			*	@param typeString The string we are removing the const keyword from.
			*
			*	@return true if a const qualifier has been removed, else false.
			*/
			bool removeConstQualifier(std::string& typeString)					const	noexcept;

			/**
			*	@brief Remove the volatile qualifier from a type string.
			*
			*	@param typeString The string we are removing the volatile keyword from.
			*
			*	@return true if a volatile qualifier has been removed, else false.
			*/
			bool removeVolatileQualifier(std::string& typeString)				const	noexcept;

			/**
			*	@brief Remove the restrict qualifier from a type string.
			*
			*	@param typeString The string we are removing the restrict keyword from.
			*
			*	@return true if a restrict qualifier has been removed, else false.
			*/
			bool removeRestrictQualifier(std::string& typeString)				const	noexcept;

		public:
			/**
			*	This array contains info about each "part" of the type
			*
			*	If the type is SomeType const *const**const*&, the array would be
			*		{ LRef 0, Ptr 0, Const Ptr 0, Ptr 0, Const Ptr 0, Const Value 0 }	(read the type from right to left)
			*	If the type is float[2][3], the array would be
			*		{ CArray 2, CArray 3, Value }	/!\ Array parts ONLY are read from left to right (not right to left)
			*	One more: if the type is int*[2][3], the array would be
			*		{ CArray 2, CArray 3, Ptr, Value }
			*/
			std::vector<TypePart>	typeParts;

			/** Size of this type in bytes. */
			size_t					sizeInBytes			= 0u;

			TypeInfo()					= default;
			TypeInfo(CXType cursorType)	noexcept;
			TypeInfo(CXCursor cursor)	noexcept;
			TypeInfo(TypeInfo const&)	= delete;
			TypeInfo(TypeInfo&&)		= default;

			/**
			*	@brief Check whether a type is a template type or not, by looking for the presence of a < character.
			* 
			*	@param typename_ The typename to check.
			* 
			*	@return true if the typename_ is a template, else false.
			*/
			static bool								isTemplateTypename(std::string const& typename_)									noexcept;

			/**
			*	@brief Get this type name by removing specified qualifiers / namespaces / nested classes.
			*
			*	@param removeQualifiers					Should the const and volatile qualifiers be removed from the type name.
			*	@param removeNamespacesAndNestedClasses Should the namespaces and nested classes be removed from the type name.
			*	@param removeTemplateParameters			Should the part from < to > be removed from the type name.
			*
			*	@return The cleaned type name.
			*/
			std::string								getName(bool removeQualifiers						= false,
															bool shouldRemoveNamespacesAndNestedClasses = false,
															bool removeTemplateParameters				= false)				const	noexcept;

			/**
			*	@brief	Get this type canonical name.
			*			The canonical name is the name simplified by unwinding all aliases and/or typedefs.
			*
			*	@param removeQualifiers Should the const and volatile qualifiers be removed from the type name.
			*	@param removeNamespacesAndNestedClasses Should the namespaces and nested classes be removed from the type name.
			*
			*	@return The cleaned type name.
			*/
			std::string								getCanonicalName(bool removeQualifiers							= false,
																	 bool shouldRemoveNamespacesAndNestedClasses	= false)	const	noexcept;

			/**
			*	@brief Getter for the field _templateParameters.
			* 
			*	@return _templateParameters.
			*/
			std::vector<TemplateParamInfo> const&	getTemplateParameters()														const	noexcept;

			/**
			*	@brief Check whether this type is template or not.
			* 
			*	@return true if the type depends on other types, else false.
			*/
			bool									isTemplateType()															const	noexcept;


			TypeInfo& operator=(TypeInfo const&)	= delete;
			TypeInfo& operator=(TypeInfo&&)			= default;
	};

	std::ostream& operator<<(std::ostream& out_stream, TypeInfo const& typeInfo) noexcept;
}