/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <functional>	//std::hash
#include <unordered_map>
#include <unordered_set>

#include <REHeaderTool/CodeGen/Macro/MacroCodeGenModule.h>
#include "REHeaderTool/InfoStructures/EnumInfo.h"
#include "REHeaderTool/InfoStructures/StructClassInfo.h"

#include "REHeaderTool/Properties/InstantiatorPropertyCodeGen.h"
#include "REHeaderTool/Properties/PropertySettingsPropertyCodeGen.h"

namespace REHeaderTool
{
	class ReflectionCodeGenModule : public REHeaderTool::MacroCodeGenModule
	{
		private:
			/** Class global string hasher. */
			inline static std::hash<std::string>		_stringHasher;

			/** Code generator for the CustomInstantiator property. */
			InstantiatorPropertyCodeGen					_instantiatorProperty;

			/** Code generator for the PropertySettings property. */
			PropertySettingsPropertyCodeGen				_propertySettingsProperty;

			/** Dictionnary used to generate properties code. */
			std::unordered_map<std::string, int>		_propertiesCount;

			/** Flag that determines whether the currently generated code is hidden from the parser or not. */
			bool										_isGeneratingHiddenCode;

			/** List of non-public enums. */
			std::unordered_set<REHeaderTool::EnumInfo const*>	_nonPublicEnums;

			/**
			*	@brief Compute the unique id of an entity. The returned string contains an unsigned integer.
			*
			*	@param entity The target entity.
			* 
			*	@return The unique ID of an entity as a string.
			*/
			inline static std::string		getEntityId(REHeaderTool::EntityInfo const& entity)										noexcept;

			/**
			*	@brief Convert the name of a REHeaderTool::EEntityType to its equivalent REHeaderTool::EEntityKind name.
			* 
			*	@param entityType The REHeaderTool::EEntityType value to convert.
			* 
			*	@return The name of the equivalent REHeaderTool::EEntityKind value.
			*/
			inline static std::string		convertEntityTypeToEntityKind(REHeaderTool::EEntityType entityType)						noexcept;

			/**
			*	@brief Compute the REHeaderTool::EFieldFlags value for the provided field.
			*
			*	@param field Field used to compute the REHeaderTool::EFieldFlags value.
			* 
			*	@return The computed REHeaderTool::FieldFlags value as a uint16.
			*/
			inline static uint16_t	computeRefurekuFieldFlags(REHeaderTool::FieldInfo const& field)							noexcept;

			/**
			*	@brief Compute the REHeaderTool::EMethodFlags value for the provided method.
			*
			*	@param method Method used to compute the REHeaderTool::EMethodFlags value.
			* 
			*	@return The computed REHeaderTool::EMethodFlags value as a uint16.
			*/
			static uint16_t		computeRefurekuMethodFlags(REHeaderTool::MethodInfo const& method)						noexcept;

			/**
			*	@brief	Compute the full type of a method pointer.
			*			Ex: void method(int){} becomes void (*)(int)
			*			Ex: class A { void method(int){} }; becomes void (A::*)(int)
			*
			*	@param method Method used to compute the REHeaderTool::EMethodFlags value.
			* 
			*	@return The computed REHeaderTool::EMethodFlags value as a uint16.
			*/
			static std::string			computeFullMethodPointerType(REHeaderTool::StructClassInfo const&	classInfo,
																	 REHeaderTool::MethodInfo const&		method)					noexcept;

			/**
			*	@brief Compute the REHeaderTool::EVarFlags value for the provided variable.
			*
			*	@param variable Variable used to compute the REHeaderTool::EVarFlags value.
			* 
			*	@return The computed REHeaderTool::EVarFlags value as a uint8.
			*/
			static uint8_t		computeRefurekuVariableFlags(REHeaderTool::VariableInfo const& variable)					noexcept;

			/**
			*	@brief Compute the REHeaderTool::EFunctionFlags value for the provided function.
			*
			*	@param function Function used to compute the REHeaderTool::EFunctionFlags value.
			* 
			*	@return The computed REHeaderTool::EFunctionFlags value as a uint8.
			*/
			static uint8_t		computeRefurekuFunctionFlags(REHeaderTool::FunctionInfo const& function)					noexcept;

			/**
			*	@brief Compute the static type of a function pointer.
			* 
			*	@param function The target function.
			* 
			*	@return The static type of the function pointer in c++ code.
			*/
			static std::string			computeFunctionPtrType(REHeaderTool::FunctionInfo const& function)						noexcept;

			/**
			*	@brief Compute the name of the getNamespaceFragment function for the given namespace.
			* 
			*	@param namespace_	The target namespace.
			*	@param sourceFile	The path to the file the namespace comes from.
			* 
			*	@return The name of the getNamespaceFragment function for the target namespace.
			*/
			static std::string			computeGetNamespaceFragmentFunctionName(REHeaderTool::NamespaceInfo const&	namespace_,
																				fs::path const&					sourceFile)	noexcept;

			/**
			*	@brief Compute the name of the registerer variable for the given namespace.
			* 
			*	@param namespace_	The target namespace.
			*	@param sourceFile	The path to the file the namespace comes from.
			* 
			*	@return The name of the registerer variable for the target namespace.
			*/
			static std::string			computeNamespaceFragmentRegistererName(REHeaderTool::NamespaceInfo const&	namespace_,
																			   fs::path const&				sourceFile)		noexcept;

			/**
			*	@brief Compute the name of the generated variable for the provided property.
			* 
			*	@param entity	Entity the property is attached to.
			*	@param propertyIndex	Index of the target property in the entity's properties array.
			* 
			*	@return The name of the generated variable name for the target property.
			*/
			static std::string			computePropertyVariableName(REHeaderTool::EntityInfo const&	entity,
																	uint8_t				propertyIndex)				noexcept;

			/**
			*	@brief Compute the name of the generated method to get a non-public nested enum.
			* 
			*	@param nestedEnum The target nested enum.
			* 
			*	@return The name of the generated method.
			*/
			static std::string			computeGetNestedEnumMethodName(REHeaderTool::NestedEnumInfo const& nestedEnum)			noexcept;

			/**
			*	@brief Check if the provided class is accessible from anywhere in the program.
			* 
			*	@param class_ The target class.
			* 
			*	@return true if the class is public, else false.
			*/
			static bool					isPublicClass(REHeaderTool::StructClassInfo const& class_)								noexcept;

			/**
			*	@brief Compute the code that will be evaluated to the provided entity id in the generated code.
			* 
			*	@param class_ The reference class template.
			*	@param entity The target entity.
			* 
			*	@return The code that will be evaluated the provided entity id in the generated code.
			*/
			static std::string			computeClassTemplateEntityId(REHeaderTool::StructClassInfo const& class_,
																	 REHeaderTool::EntityInfo const&		entity)					noexcept;

			/**
			*	@brief	Compute the id of a class nested entity.
			*			The returned string will be evaluated to an id in the generated code.
			* 
			*	@param className	Name of the class.
			*	@param entity		The class nested entity.
			* 
			*	@return The code that will be evaluated to an id in the generated code.
			*/
			static std::string			computeClassNestedEntityId(std::string					className,
																   REHeaderTool::EntityInfo const&	entity)						noexcept;

			/**
			*	@brief	All code generated from this point will be hidden to the parser.
			*			This means the generated code can't be reflected.
			* 
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	beginHiddenGeneratedCode(REHeaderTool::MacroCodeGenEnv&	env,
											 std::string&				inout_result)									noexcept;

			/**
			*	@brief	End the hidden generated code section. If beginHiddenGeneratedCode hasn't been called before,
			*			nothing happens.
			* 
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	endHiddenGeneratedCode(REHeaderTool::MacroCodeGenEnv&	env,
										   std::string&				inout_result)										noexcept;

			/**
			*	@brief Reset the module to begin a clean generation from scratch.
			*/
			void	reset()																								noexcept;

			/**
			*	@brief	Check that the endHiddenGeneratedCode has been called correctly if there was any previous
			*			call to beginHiddenGeneratedCode.
			*			If it is not the case, the code will assert.
			*/
			void	checkHiddenGeneratedCodeState()																const	noexcept;

			/**
			*	TODO
			*/
			void	fillEntityProperties(REHeaderTool::EntityInfo const&	entity,
										 REHeaderTool::MacroCodeGenEnv&	env,
										 std::string const&			generatedEntityVarName,
										 std::string&				inout_result)										noexcept;

			/**
			*	@brief Generate the headers inclusion code for the generated header file.
			* 
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	includeHeaderFileHeaders(REHeaderTool::MacroCodeGenEnv&	env,
											 std::string&				inout_result)							const	noexcept;

			/**
			*	@brief Generate the headers inclusion code for the generated source file.
			* 
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	includeSourceFileHeaders(REHeaderTool::MacroCodeGenEnv&	env,
											 std::string&				inout_result)							const	noexcept;



			//Struct / Class code generation
			/**
			*	@brief Generate the friend statements for the provided class.
			* 
			*	@param structClass	Target struct/class.
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	declareFriendClasses(REHeaderTool::StructClassInfo const&	structClass,
										 REHeaderTool::MacroCodeGenEnv&		env,
										 std::string&					inout_result)							const	noexcept;

			/**
			*	@brief Generate the staticGetArchetype method declaration.
			* 
			*	@param structClass	Target struct/class.
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	declareStaticGetArchetypeMethod(REHeaderTool::StructClassInfo const&	structClass,
													REHeaderTool::MacroCodeGenEnv&		env,
													std::string&					inout_result)				const	noexcept;

			/**
			*	@brief Generate the staticGetArchetype method definition.
			* 
			*	@param structClass	Target struct/class.
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	defineStaticGetArchetypeMethod(REHeaderTool::StructClassInfo const&	structClass,
												   REHeaderTool::MacroCodeGenEnv&		env,
												   std::string&					inout_result)							noexcept;

			/**
			*	@brief Generate code for registering the default constructor of a struct or class.
			* 
			*	@param structClass				Target struct/class.
			*	@param env						Code generation environment.
			*	@param generatedClassVarName	Name of the variable holding the class metadata in the generated code.
			*	@param inout_result				String to append the generated code.
			*/
			void	setClassDefaultInstantiator(REHeaderTool::StructClassInfo const&	structClass,
												REHeaderTool::MacroCodeGenEnv&		env,
												std::string const&				generatedClassVarName,
												std::string&					inout_result)							noexcept;

			/**
			*	TODO
			*/
			void	fillClassParents(REHeaderTool::StructClassInfo const&	structClass,
									 REHeaderTool::MacroCodeGenEnv&		env,
									 std::string const&				generatedEntityVarName,
									 std::string&					inout_result)										noexcept;

			/**
			*	TODO
			*/
			void	fillClassFields(REHeaderTool::StructClassInfo const&	structClass,
									REHeaderTool::MacroCodeGenEnv&		env,
									std::string						generatedClassRefExpression,
									std::string&					inout_result)										noexcept;

			/**
			*	TODO
			*/
			void	fillClassMethods(REHeaderTool::StructClassInfo const&	structClass,
									 REHeaderTool::MacroCodeGenEnv&		env,
									 std::string const&				generatedEntityVarName,
									 std::string&					inout_result)										noexcept;

			/**
			*	TODO
			*/
			void	fillClassNestedArchetypes(REHeaderTool::StructClassInfo const&	structClass,
											  REHeaderTool::MacroCodeGenEnv&			env,
											  std::string const&				generatedEntityVarName,
											  std::string&						inout_result)							noexcept;

			/**
			*	@brief Generate the getArchetype method declaration if the provided class inherits from REHeaderTool::Object.
			* 
			*	@param structClass	Target struct/class.
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	declareGetArchetypeMethodIfInheritFromObject(REHeaderTool::StructClassInfo const&	structClass,
																 REHeaderTool::MacroCodeGenEnv&		env,
																 std::string&					inout_result)	const	noexcept;

			/**
			*	@brief Generate the getArchetype method definition if the provided class inherits from REHeaderTool::Object.
			* 
			*	@param structClass	Target struct/class.
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	defineGetArchetypeMethodIfInheritFromObject(REHeaderTool::StructClassInfo const&	structClass,
																REHeaderTool::MacroCodeGenEnv&		env,
																std::string&					inout_result)	const	noexcept;

			/**
			*	TODO
			*/
			void	declareGetArchetypeTemplateSpecialization(REHeaderTool::StructClassInfo const&	structClass,
															  REHeaderTool::MacroCodeGenEnv&			env,
															  std::string&						inout_result)	const	noexcept;

			/**
			*	TODO
			*/
			void	defineGetArchetypeTemplateSpecialization(REHeaderTool::StructClassInfo const&	structClass,
															 REHeaderTool::MacroCodeGenEnv&		env,
															 std::string&					inout_result)		const	noexcept;

			/**
			*	TODO
			*/
			void	declareAndDefineRegisterChildClassMethod(REHeaderTool::StructClassInfo const&	structClass,
															 REHeaderTool::MacroCodeGenEnv&		env,
															 std::string&					inout_result)				noexcept;

			/**
			*	TODO
			*/
			void	declareGetNestedEnumMethods(REHeaderTool::StructClassInfo const&	structClass,
												REHeaderTool::MacroCodeGenEnv&		env,
												std::string&					inout_result)							noexcept;

			/**
			*	TODO
			*/
			void	defineGetNestedEnumMethods(REHeaderTool::StructClassInfo const&	structClass,
											   REHeaderTool::MacroCodeGenEnv&		env,
											   std::string&					inout_result)							noexcept;

			/**
			*	TODO
			*/
			void	declareAndDefineClassRegistererVariable(REHeaderTool::StructClassInfo const&	structClass,
															REHeaderTool::MacroCodeGenEnv&		env,
															std::string&					inout_result)		const	noexcept;

			//Class template code generation
			/**
			*	TODO
			*/
			void	declareAndDefineClassTemplateStaticGetArchetypeMethod(REHeaderTool::StructClassInfo const&	structClass,
																		  REHeaderTool::MacroCodeGenEnv&			env,
																		  std::string&						inout_result)						noexcept;

			/**
			*	TODO
			*/
			void	declareAndDefineClassTemplateGetArchetypeMethodIfInheritFromObject(REHeaderTool::StructClassInfo const&	structClass,
																					   REHeaderTool::MacroCodeGenEnv&			env,
																					   std::string&						inout_result)	const	noexcept;

			/**
			*	TODO
			*/
			void	declareAndDefineClassTemplateRegistererField(REHeaderTool::StructClassInfo const&	structClass,
																 REHeaderTool::MacroCodeGenEnv&		env,
																 std::string&					inout_result)							const	noexcept;

			/**
			*	TODO
			*/
			void	fillClassTemplateArguments(REHeaderTool::StructClassInfo const&	structClass,
											   std::string						generatedEntityVarName,
											   REHeaderTool::MacroCodeGenEnv&			env,
											   std::string&						inout_result)											const	noexcept;

			/**
			*	TODO
			*/
			void	defineClassTemplateGetArchetypeTemplateSpecialization(REHeaderTool::StructClassInfo const&	structClass,
																		  REHeaderTool::MacroCodeGenEnv&			env,
																		  std::string&						inout_result)						noexcept;

			/**
			*	@brief Generate code to create and add a template parameter to the provided object.
			* 
			*	@param templateParam	Template parameter to create/add.
			*	@param generatedVarName	Name of the variable generated for the template parameter.
			*	@param objVarName		Name of the generated variable to add the template parameter to.
			*	@param addFuncName		Name of the function to use to add the parameter to the object.
			*	@param env				Code generation environment.
			*	@param inout_result		String to append the generated code.
			*	@param recursionLevel	Number of times this function has been recursively called.
			*/
			void	addTemplateParameter(REHeaderTool::TemplateParamInfo const&	templateParam,
										 std::string						generatedVarName,
										 std::string const&					objVarName,
										 std::string						addFuncName,
										 REHeaderTool::MacroCodeGenEnv&			env,
										 std::string&						inout_result,
										 int								recursionLevel = 0)											const	noexcept;

			/**
			*	TODO
			*/
			void	fillClassTemplateParameters(REHeaderTool::StructClassInfo const&	structClass,
												std::string						classTemplateVarName,
												REHeaderTool::MacroCodeGenEnv&		env,
												std::string&					inout_result)											const	noexcept;

			/**
			*	TODO
			*/
			void	declareAndDefineClassTemplateRegistererVariable(REHeaderTool::StructClassInfo const&	structClass,
																	REHeaderTool::MacroCodeGenEnv&		env,
																	std::string&					inout_result)						const	noexcept;

			//Enums code generation
			/**
			*	@brief Check whether the target enum has been registered to the _nonPublicEnum set or not.
			* 
			*	@param nestedEnum the target enum.
			* 
			*	@return true if the nestedEnum is non-public, else false.
			*/
			bool	isRegisteredNonPublicEnum(REHeaderTool::EnumInfo const& nestedEnum)					const	noexcept;

			/**
			*	TODO
			*/
			void	declareGetEnumTemplateSpecialization(REHeaderTool::EnumInfo const&	enum_,
														 REHeaderTool::MacroCodeGenEnv&	env,
														 std::string&				inout_result)	const	noexcept;

			/**
			*	TODO
			*/
			void	defineGetEnumTemplateSpecialization(REHeaderTool::EnumInfo const&		enum_,
														REHeaderTool::MacroCodeGenEnv&	env,
														std::string&				inout_result)			noexcept;

			/**
			*	@brief	Define the content of a get enum method from the { to the }.
			*			Does not include the method prototype.
			* 
			*	TODO
			*/
			void	defineGetEnumContent(REHeaderTool::EnumInfo const&	enum_,
										 REHeaderTool::MacroCodeGenEnv&	env,
										 std::string&				inout_result)							noexcept;

			/**
			*	TODO
			*/
			void	declareAndDefineEnumRegistererVariable(REHeaderTool::EnumInfo const&	enum_,
														   REHeaderTool::MacroCodeGenEnv&	env,
														   std::string&				inout_result)	const	noexcept;


			//Variables code generation
			/**
			*	TODO
			*/
			void	declareGetVariableFunction(REHeaderTool::VariableInfo const&	variable,
											   REHeaderTool::MacroCodeGenEnv&		env,
											   std::string&					inout_result)					noexcept;

			/**
			*	TODO
			*/
			void	defineGetVariableFunction(REHeaderTool::VariableInfo const&	variable,
											  REHeaderTool::MacroCodeGenEnv&		env,
											  std::string&					inout_result)					noexcept;

			/**
			*	TODO
			*/
			void	declareAndDefineVariableRegistererVariable(REHeaderTool::VariableInfo const&	variable,
															   REHeaderTool::MacroCodeGenEnv&		env,
															   std::string&					inout_result)	noexcept;



			//Functions code generation
			/**
			*	TODO
			*/
			void	declareGetFunctionFunction(REHeaderTool::FunctionInfo const&	function,
											   REHeaderTool::MacroCodeGenEnv&		env,
											   std::string&					inout_result)					noexcept;

			/**
			*	TODO
			*/
			void	defineGetFunctionFunction(REHeaderTool::FunctionInfo const&	function,
											  REHeaderTool::MacroCodeGenEnv&		env,
											  std::string&					inout_result)					noexcept;

			/**
			*	TODO
			*/
			void	declareAndDefineFunctionRegistererVariable(REHeaderTool::FunctionInfo const&	function,
															   REHeaderTool::MacroCodeGenEnv&		env,
															   std::string&					inout_result)	noexcept;



			//Namespaces code generation
			/**
			*	@brief Declare and define the getNamespaceFragment function for the provided namespace.
			*
			*	@param namespace_	Target namespace.
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	declareAndDefineGetNamespaceFragmentFunction(REHeaderTool::NamespaceInfo const&	namespace_,
																 REHeaderTool::MacroCodeGenEnv&		env,
																 std::string&					inout_result)				noexcept;

			/**
			*	@brief Define the namespace fragment registerer variable for the provided namespace.
			* 
			*	@param namespace_	Target namespace.
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	declareAndDefineNamespaceFragmentRegistererVariable(REHeaderTool::NamespaceInfo const&	namespace_,
																		REHeaderTool::MacroCodeGenEnv&		env,
																		std::string&					inout_result)		noexcept;

			/**
			*	@brief	Recursively declare and define the get namespace fragment function and the namespace registerer variable
			*			for the provided namespace and all its sub namespaces.
			*			Function and registerers are defined in reversed order, so the most inner namespace metadata will be defined first and the most
			*			outer ones at the end.
			*			/!\ This method only generates code when a top-level (without outer entity) namespace is provided.
			*				In all other cases, the method doesn't generate any code. /!\
			*
			*	@param namespace_	Top level namespace.
			*	@param env			Code generation environment.
			*	@param inout_result	String to append the generated code.
			*/
			void	declareAndDefineGetNamespaceFragmentAndRegistererRecursive(REHeaderTool::NamespaceInfo const&	namespace_,
																			   REHeaderTool::MacroCodeGenEnv&		env,
																			   std::string&					inout_result)	noexcept;


		protected:
			virtual ReflectionCodeGenModule*	clone()																		const	noexcept	override;

			virtual bool						initialGenerateHeaderFileHeaderCode(REHeaderTool::MacroCodeGenEnv&		env,
																					std::string&					inout_result)	noexcept	override;

			virtual REHeaderTool::ETraversalBehaviour	generateHeaderFileHeaderCodeForEntity(REHeaderTool::EntityInfo const&	entity,
																					  REHeaderTool::MacroCodeGenEnv&	env,
																					  std::string&				inout_result)		noexcept	override;

			virtual REHeaderTool::ETraversalBehaviour	generateClassFooterCodeForEntity(REHeaderTool::EntityInfo const&	entity,
																				 REHeaderTool::MacroCodeGenEnv&	env,
																				 std::string&				inout_result)			noexcept	override;

			virtual REHeaderTool::ETraversalBehaviour	generateHeaderFileFooterCodeForEntity(REHeaderTool::EntityInfo const&	entity,
																					  REHeaderTool::MacroCodeGenEnv&	env,
																					  std::string&				inout_result)		noexcept	override;

			virtual bool						initialGenerateSourceFileHeaderCode(REHeaderTool::MacroCodeGenEnv&		env,
																					std::string&					inout_result)	noexcept	override;

			virtual REHeaderTool::ETraversalBehaviour	generateSourceFileHeaderCodeForEntity(REHeaderTool::EntityInfo const&	entity,
																					  REHeaderTool::MacroCodeGenEnv&	env,
																					  std::string&				inout_result)		noexcept	override;

		public:
			ReflectionCodeGenModule()								noexcept;
			ReflectionCodeGenModule(ReflectionCodeGenModule const&)	noexcept;
	};

	#include "REHeaderTool/CodeGen/ReflectionCodeGenModule.inl"
}