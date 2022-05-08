/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

inline std::string ReflectionCodeGenModule::getEntityId(REHeaderTool::EntityInfo const& entity) noexcept
{
	return std::to_string(ReflectionCodeGenModule::_stringHasher(entity.id)) + "u";
}

ReflectionCodeGenModule::ReflectionCodeGenModule() noexcept:
	_isGeneratingHiddenCode{false}
{
	addPropertyCodeGen(_instantiatorProperty);
	addPropertyCodeGen(_propertySettingsProperty);
}

ReflectionCodeGenModule::ReflectionCodeGenModule(ReflectionCodeGenModule const&) noexcept:
	ReflectionCodeGenModule()
{
}

ReflectionCodeGenModule* ReflectionCodeGenModule::clone() const noexcept
{
	return new ReflectionCodeGenModule(*this);
}

bool ReflectionCodeGenModule::initialGenerateHeaderFileHeaderCode(REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	reset();

	includeHeaderFileHeaders(env, inout_result);

	return true;
}

REHeaderTool::ETraversalBehaviour	ReflectionCodeGenModule::generateHeaderFileHeaderCodeForEntity(REHeaderTool::EntityInfo const& entity, REHeaderTool::MacroCodeGenEnv& env, std::string& /* inout_result */) noexcept
{
	REHeaderTool::ETraversalBehaviour result = REHeaderTool::ETraversalBehaviour::Recurse;

	switch (entity.entityType)
	{
		case REHeaderTool::EEntityType::Struct:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Class:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Enum:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Variable:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Function:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Field:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Method:
			[[fallthrough]];
		case REHeaderTool::EEntityType::EnumValue:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Namespace:
			result = REHeaderTool::ETraversalBehaviour::Break;
			break;

		case REHeaderTool::EEntityType::Undefined:
			[[fallthrough]];
		default:
			assert(false); //This should never happen
			env.getLogger()->log("The entity " + entity.getFullName() + " has an undefined type. Abort.", REHeaderTool::ILogger::ELogSeverity::Error);

			return REHeaderTool::ETraversalBehaviour::AbortWithFailure;
	}

	checkHiddenGeneratedCodeState();

	return result; 
}

REHeaderTool::ETraversalBehaviour	ReflectionCodeGenModule::generateClassFooterCodeForEntity(REHeaderTool::EntityInfo const& entity, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	REHeaderTool::ETraversalBehaviour result = REHeaderTool::ETraversalBehaviour::Recurse;

	switch (entity.entityType)
	{
		case REHeaderTool::EEntityType::Struct:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Class:
			//Do not generate code for forward declarations
			if (static_cast<REHeaderTool::StructClassInfo const&>(entity).isForwardDeclaration)
			{
				return REHeaderTool::ETraversalBehaviour::Continue;
			}

			beginHiddenGeneratedCode(env, inout_result);

			declareFriendClasses(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);

			declareAndDefineRegisterChildClassMethod(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
			declareGetNestedEnumMethods(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);

			if (static_cast<REHeaderTool::StructClassInfo const&>(entity).type.isTemplateType())
			{
				//Declaration AND definition must appear in the class footer for class templates
				declareAndDefineClassTemplateStaticGetArchetypeMethod(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
				declareAndDefineClassTemplateGetArchetypeMethodIfInheritFromObject(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
				declareAndDefineClassTemplateRegistererField(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
			}
			else
			{
				declareStaticGetArchetypeMethod(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
				declareGetArchetypeMethodIfInheritFromObject(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
			}

			endHiddenGeneratedCode(env, inout_result);

			result = REHeaderTool::ETraversalBehaviour::Recurse;
			break;

		case REHeaderTool::EEntityType::Enum:
			result = REHeaderTool::ETraversalBehaviour::Continue; //Go to next enum
			break;

		case REHeaderTool::EEntityType::Field:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Method:
			result = REHeaderTool::ETraversalBehaviour::Break; //Don't need to iterate over those individual entities
			break;

			//All those cases should not be able to be called in generateClassFooterCode
		case REHeaderTool::EEntityType::Variable:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Function:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Namespace:
			[[fallthrough]];
		case REHeaderTool::EEntityType::EnumValue:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Undefined:
			[[fallthrough]];
		default:
			assert(false); //This should never happen
			env.getLogger()->log("The entity " + entity.getFullName() + " has an undefined type. Abort.", REHeaderTool::ILogger::ELogSeverity::Error);

			return REHeaderTool::ETraversalBehaviour::AbortWithFailure;
	}

	checkHiddenGeneratedCodeState();

	return result;
}

REHeaderTool::ETraversalBehaviour ReflectionCodeGenModule::generateHeaderFileFooterCodeForEntity(REHeaderTool::EntityInfo const& entity, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	REHeaderTool::ETraversalBehaviour result = REHeaderTool::ETraversalBehaviour::Recurse;

	switch (entity.entityType)
	{
		case REHeaderTool::EEntityType::Struct:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Class:
			//Do not generate code for forward declarations
			if (reinterpret_cast<REHeaderTool::StructClassInfo const&>(entity).isForwardDeclaration)
			{
				return REHeaderTool::ETraversalBehaviour::Continue;
			}

			declareGetArchetypeTemplateSpecialization(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);

			result = REHeaderTool::ETraversalBehaviour::Recurse;
			break;

		case REHeaderTool::EEntityType::Enum:
			declareGetEnumTemplateSpecialization(static_cast<REHeaderTool::EnumInfo const&>(entity), env, inout_result);

			result = REHeaderTool::ETraversalBehaviour::Continue; //Go to next enum
			break;

		case REHeaderTool::EEntityType::Variable:
			if (static_cast<REHeaderTool::VariableInfo const&>(entity).isStatic)
			{
				defineGetVariableFunction(static_cast<REHeaderTool::VariableInfo const&>(entity), env, inout_result);
			}
			else
			{
				declareGetVariableFunction(static_cast<REHeaderTool::VariableInfo const&>(entity), env, inout_result);
			}

			result = REHeaderTool::ETraversalBehaviour::Continue; //Go to next variable
			break;

		case REHeaderTool::EEntityType::Function:
			if (static_cast<REHeaderTool::FunctionInfo const&>(entity).isStatic)
			{
				defineGetFunctionFunction(static_cast<REHeaderTool::FunctionInfo const&>(entity), env, inout_result);
			}
			else
			{
				declareGetFunctionFunction(static_cast<REHeaderTool::FunctionInfo const&>(entity), env, inout_result);
			}

			result = REHeaderTool::ETraversalBehaviour::Continue; //Go to next function
			break;

		case REHeaderTool::EEntityType::Field:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Method:
			[[fallthrough]];
		case REHeaderTool::EEntityType::EnumValue:
			result =  REHeaderTool::ETraversalBehaviour::Break; //Don't need to iterate over those individual entities
			break;

		case REHeaderTool::EEntityType::Namespace:
			result = REHeaderTool::ETraversalBehaviour::Recurse;
			break;

		case REHeaderTool::EEntityType::Undefined:
			[[fallthrough]];
		default:
			assert(false); //This should never happen
			env.getLogger()->log("The entity " + entity.getFullName() + " has an undefined type. Abort.", REHeaderTool::ILogger::ELogSeverity::Error);

			return REHeaderTool::ETraversalBehaviour::AbortWithFailure;
	}

	checkHiddenGeneratedCodeState();

	return result;
}

bool ReflectionCodeGenModule::initialGenerateSourceFileHeaderCode(REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	includeSourceFileHeaders(env, inout_result);

	return true;
}

REHeaderTool::ETraversalBehaviour ReflectionCodeGenModule::generateSourceFileHeaderCodeForEntity(REHeaderTool::EntityInfo const& entity, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	REHeaderTool::ETraversalBehaviour result = REHeaderTool::ETraversalBehaviour::Recurse;

	switch (entity.entityType)
	{
		case REHeaderTool::EEntityType::Struct:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Class:
			//Do not generate code for forward declarations
			if (reinterpret_cast<REHeaderTool::StructClassInfo const&>(entity).isForwardDeclaration)
			{
				return REHeaderTool::ETraversalBehaviour::Continue;
			}

			if (static_cast<REHeaderTool::StructClassInfo const&>(entity).type.isTemplateType())
			{
				defineClassTemplateGetArchetypeTemplateSpecialization(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
				declareAndDefineClassTemplateRegistererVariable(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
			}
			else
			{
				declareAndDefineClassRegistererVariable(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
				defineStaticGetArchetypeMethod(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
				defineGetArchetypeMethodIfInheritFromObject(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
				defineGetArchetypeTemplateSpecialization(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
				defineGetNestedEnumMethods(static_cast<REHeaderTool::StructClassInfo const&>(entity), env, inout_result);
			}

			result = REHeaderTool::ETraversalBehaviour::Recurse;
			break;

		case REHeaderTool::EEntityType::Enum:
			defineGetEnumTemplateSpecialization(static_cast<REHeaderTool::EnumInfo const&>(entity), env, inout_result);
			declareAndDefineEnumRegistererVariable(static_cast<REHeaderTool::EnumInfo const&>(entity), env, inout_result);

			result = REHeaderTool::ETraversalBehaviour::Continue; //Go to next enum
			break;

		case REHeaderTool::EEntityType::Variable:
			if (!static_cast<REHeaderTool::VariableInfo const&>(entity).isStatic)
			{
				defineGetVariableFunction(static_cast<REHeaderTool::VariableInfo const&>(entity), env, inout_result);
			}

			declareAndDefineVariableRegistererVariable(static_cast<REHeaderTool::VariableInfo const&>(entity), env, inout_result);

			result = REHeaderTool::ETraversalBehaviour::Continue; //Go to next variable
			break;

		case REHeaderTool::EEntityType::Function:
			if (!static_cast<REHeaderTool::FunctionInfo const&>(entity).isStatic)
			{
				defineGetFunctionFunction(static_cast<REHeaderTool::FunctionInfo const&>(entity), env, inout_result);
			}

			declareAndDefineFunctionRegistererVariable(static_cast<REHeaderTool::FunctionInfo const&>(entity), env, inout_result);

			result = REHeaderTool::ETraversalBehaviour::Continue; //Go to next function
			break;

		case REHeaderTool::EEntityType::Field:
			[[fallthrough]];
		case REHeaderTool::EEntityType::Method:
			[[fallthrough]];
		case REHeaderTool::EEntityType::EnumValue:
			result = REHeaderTool::ETraversalBehaviour::Break; //Don't need to iterate over those individual entities
			break;

		case REHeaderTool::EEntityType::Namespace:
			declareAndDefineGetNamespaceFragmentAndRegistererRecursive(static_cast<REHeaderTool::NamespaceInfo const&>(entity), env, inout_result);

			result = REHeaderTool::ETraversalBehaviour::Recurse;
			break;

		case REHeaderTool::EEntityType::Undefined:
			[[fallthrough]];

		default:
			assert(false); //This should never happen
			env.getLogger()->log("The entity " + entity.getFullName() + " has an undefined type. Abort.", REHeaderTool::ILogger::ELogSeverity::Error);

			return REHeaderTool::ETraversalBehaviour::AbortWithFailure;
	}

	checkHiddenGeneratedCodeState();

	return result;
}

void ReflectionCodeGenModule::includeHeaderFileHeaders(REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	inout_result += "#include <string>" + env.getSeparator() +
		"#include <RECore/Reflect/Misc/CodeGenerationHelpers.h>" + env.getSeparator() +
		"#include <RECore/Reflect/Misc/DisableWarningMacros.h>" + env.getSeparator() +
		"#include <RECore/Reflect/TypeInfo/Functions/Method.h>" + env.getSeparator() +
		"#include <RECore/Reflect/TypeInfo/Functions/StaticMethod.h>" + env.getSeparator() +
		"#include <RECore/Reflect/TypeInfo/Variables/Field.h>" + env.getSeparator() +
		"#include <RECore/Reflect/TypeInfo/Variables/StaticField.h>" + env.getSeparator() +
		"#include <RECore/Reflect/TypeInfo/Archetypes/Enum.h>" + env.getSeparator() +
		"#include <RECore/Reflect/TypeInfo/Archetypes/EnumValue.h>" + env.getSeparator() +
		"#include <RECore/Reflect/TypeInfo/Variables/Variable.h>" + env.getSeparator() +											//TODO: Only if there is a variable
		"#include <RECore/Reflect/TypeInfo/Functions/Function.h>" + env.getSeparator() +											//TODO: Only if there is a function
		"#include <RECore/Reflect/TypeInfo/Archetypes/Template/ClassTemplate.h>" + env.getSeparator() +							//TODO: Only when there is a template class
		"#include <RECore/Reflect/TypeInfo/Archetypes/Template/ClassTemplateInstantiation.h>" + env.getSeparator() +				//TODO: Only when there is a template class
		"#include <RECore/Reflect/TypeInfo/Archetypes/Template/ClassTemplateInstantiationRegisterer.h>" + env.getSeparator() +	//TODO: Only when there is a non-nested template class
		"#include <RECore/Reflect/TypeInfo/Archetypes/Template/TypeTemplateArgument.h>" + env.getSeparator() +					//TODO: Only when there is a template class
		"#include <RECore/Reflect/TypeInfo/Archetypes/Template/NonTypeTemplateArgument.h>" + env.getSeparator() +					//TODO: Only when there is a template class
		"#include <RECore/Reflect/TypeInfo/Archetypes/Template/TemplateTemplateArgument.h>" + env.getSeparator() +				//TODO: Only when there is a template class
		env.getSeparator();
}

void ReflectionCodeGenModule::includeSourceFileHeaders(REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	inout_result += "#include <RECore/Reflect/TypeInfo/Entity/DefaultEntityRegisterer.h>" + env.getSeparator() +					//TODO: Only if there is a variable or function
		"#include <RECore/Reflect/TypeInfo/Archetypes/ArchetypeRegisterer.h>" + env.getSeparator() +
		"#include <RECore/Reflect/TypeInfo/Namespace/Namespace.h>" + env.getSeparator() +								//TODO: Only if there is a namespace
		"#include <RECore/Reflect/TypeInfo/Namespace/NamespaceFragment.h>" + env.getSeparator() +						//TODO: Only if there is a namespace
		"#include <RECore/Reflect/TypeInfo/Namespace/NamespaceFragmentRegisterer.h>" + env.getSeparator() +			//TODO: Only if there is a namespace
		"#include <RECore/Reflect/TypeInfo/Archetypes/Template/TypeTemplateParameter.h>" + env.getSeparator() +		//TODO: Only if there is a template class in the parsed data
		"#include <RECore/Reflect/TypeInfo/Archetypes/Template/NonTypeTemplateParameter.h>" + env.getSeparator() +	//TODO: Only if there is a template class in the parsed data
		"#include <RECore/Reflect/TypeInfo/Archetypes/Template/TemplateTemplateParameter.h>" + env.getSeparator() +	//TODO: Only if there is a template class in the parsed data
		env.getSeparator();
}

void ReflectionCodeGenModule::declareFriendClasses(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	inout_result += "friend RECore::internal::CodeGenerationHelpers;" + env.getSeparator();
	inout_result += "friend implements_template1__RECore_registerChildClass<" + structClass.name + ", void, void(RECore::Struct&)>; " + env.getSeparator() + env.getSeparator();
}

void ReflectionCodeGenModule::declareStaticGetArchetypeMethod(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	inout_result += "public: " + env.getExportSymbolMacro() + " static ";
	inout_result += (structClass.isClass()) ? "RECore::Struct" : "RECore::Struct";
	inout_result += " const& staticGetArchetype() noexcept;" + env.getSeparator() + env.getSeparator();
}

void ReflectionCodeGenModule::declareGetArchetypeMethodIfInheritFromObject(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	if (env.getFileParsingResult()->structClassTree.isBaseOf("RECore::Object", structClass.getFullName()))
	{
		inout_result += "public: " + env.getExportSymbolMacro() + " virtual ";
		inout_result += (structClass.isClass()) ? "RECore::Struct" : "RECore::Struct";
		inout_result += " const& getArchetype() const noexcept override;" + env.getSeparator() + env.getSeparator();
	}
}

void ReflectionCodeGenModule::defineStaticGetArchetypeMethod(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	std::string returnType = (structClass.isClass()) ? "RECore::Class" : "RECore::Struct";

	inout_result += returnType + " const& " + structClass.type.getCanonicalName() + "::staticGetArchetype() noexcept {" + env.getSeparator() +
		"static bool initialized = false;" + env.getSeparator() +
		"static " + returnType + " type(\"" + structClass.name + "\", " +
		getEntityId(structClass) + ", "
		"sizeof(" + structClass.name + "), " +
		std::to_string(structClass.isClass()) +
		");" + env.getSeparator() +
		"if (!initialized) {" + env.getSeparator() +
		"initialized = true;" + env.getSeparator();

	//Inside the if statement, initialize the Struct metadata
	fillEntityProperties(structClass, env, "type.", inout_result);
	fillClassParents(structClass, env, "type.", inout_result);
	fillClassFields(structClass, env, "type", inout_result);

	//Set the default instantiator BEFORE filling the class methods since methods can overwrite the custom instantiator
	setClassDefaultInstantiator(structClass, env, "type.", inout_result);
	fillClassMethods(structClass, env, "type.", inout_result);
	fillClassNestedArchetypes(structClass, env, "type.", inout_result);

	//End of the initialization if statement
	inout_result += "}" + env.getSeparator();


	inout_result += "return type; }" + env.getSeparator() + env.getSeparator();
}

std::string ReflectionCodeGenModule::computePropertyVariableName(REHeaderTool::EntityInfo const& entity, uint8_t propertyIndex) noexcept
{
	return "property_" + getEntityId(entity) + "_" + std::to_string(propertyIndex);
}

std::string ReflectionCodeGenModule::computeGetNestedEnumMethodName(REHeaderTool::NestedEnumInfo const& nestedEnum) noexcept
{
	return "_RECore_getNestedEnum_" + nestedEnum.name;
}

bool ReflectionCodeGenModule::isPublicClass(REHeaderTool::StructClassInfo const& class_) noexcept
{
	if (class_.outerEntity == nullptr)
	{
		return true;
	}
	else
	{
		//class outer entity can only be a namespace or another struct/class
		if (class_.outerEntity->entityType == REHeaderTool::EEntityType::Namespace)
		{
			//Namespace can't be private
			return true;
		}
		else
		{
			//A class outer entity must be another struct or class
			assert(class_.outerEntity->entityType && (REHeaderTool::EEntityType::Class | REHeaderTool::EEntityType::Struct));

			return	static_cast<REHeaderTool::NestedStructClassInfo const&>(class_).accessSpecifier == REHeaderTool::EAccessSpecifier::Public &&
				isPublicClass(static_cast<REHeaderTool::StructClassInfo const&>(*class_.outerEntity));
		}
	}
}

std::string ReflectionCodeGenModule::computeClassTemplateEntityId(REHeaderTool::StructClassInfo const& class_, REHeaderTool::EntityInfo const& entity) noexcept
{
	return computeClassNestedEntityId(class_.getFullName(), entity);
}

std::string ReflectionCodeGenModule::computeClassNestedEntityId(std::string className, REHeaderTool::EntityInfo const& entity) noexcept
{
	return "std::hash<std::string>()(std::string(\"" + entity.id + "\") + RECore::internal::getTypename<" + std::move(className) + ">())";
}

void ReflectionCodeGenModule::beginHiddenGeneratedCode(REHeaderTool::MacroCodeGenEnv& /*env*/, std::string& inout_result) noexcept
{
	if (!_isGeneratingHiddenCode)
	{
		_isGeneratingHiddenCode = true;
		inout_result += "RFK_UNPACK_IF_NOT_PARSING(";
	}
}

void ReflectionCodeGenModule::endHiddenGeneratedCode(REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	if (_isGeneratingHiddenCode)
	{
		_isGeneratingHiddenCode = false;
		inout_result += ")" + env.getSeparator();
	}
}

void ReflectionCodeGenModule::reset() noexcept
{
	_isGeneratingHiddenCode = false;
	_nonPublicEnums.clear();
}

void ReflectionCodeGenModule::checkHiddenGeneratedCodeState() const noexcept
{
	assert(!_isGeneratingHiddenCode);
}

void ReflectionCodeGenModule::fillEntityProperties(REHeaderTool::EntityInfo const& entity, REHeaderTool::MacroCodeGenEnv& env, std::string const& generatedEntityVarName, std::string& inout_result) noexcept
{
	auto generatePropertyStaticAsserts = [this, &entity](REHeaderTool::Property const& property) -> std::string
	{
		std::string result;

		//Generate AllowMultiple assert only if a property apprears for the second time
		auto it = _propertiesCount.find(property.name);

		if (it != _propertiesCount.end())
		{
			if (it->second == 1)
			{
				//Second apparition of the property, generate
				result += "static_assert(" + property.name + "::allowMultiple, \"[Refureku] " + entity.getFullName() + ": " + property.name + " can't be attached multiple times to a single entity.\"); ";
			}

			++it->second;
		}
		else
		{
			_propertiesCount[property.name] = 1;
		}

		//Generate entity kind check
		std::string entityKindName = convertEntityTypeToEntityKind(entity.entityType);
		result += "static_assert((" + property.name + "::targetEntityKind & " + entityKindName + ") != " + convertEntityTypeToEntityKind(REHeaderTool::EEntityType::Undefined) +
			", \"[Refureku] " + property.name + " can't be applied to a " + entityKindName + "\");";

		return result;
	};

	std::string generatedPropertyVariableName;

	if (!entity.properties.empty())
	{
		//Reserve space to avoid reallocation
		inout_result += generatedEntityVarName + "setPropertiesCapacity(" + std::to_string(entity.properties.size()) + ");" + env.getSeparator();

		//Add all properties
		_propertiesCount.clear();
		for (uint8_t i = 0; i < entity.properties.size(); i++)
		{
			generatedPropertyVariableName = computePropertyVariableName(entity, i);

			//Generate static_asserts bound to this property
			inout_result += generatePropertyStaticAsserts(entity.properties[i]);

			//Declare property
			inout_result += "static " + entity.properties[i].name + " " + generatedPropertyVariableName;

			//Construct the property with the provided arguments
			if (!entity.properties[i].arguments.empty())
			{
				inout_result += "{";

				for (std::string const& argument : entity.properties[i].arguments)
				{
					inout_result += argument + ",";
				}

				inout_result.back() = '}';	//Replace the last , by a }
			}

			inout_result.push_back(';');

			inout_result += generatedEntityVarName + "addProperty(" + generatedPropertyVariableName + ");" + env.getSeparator();
		}
	}
}

void ReflectionCodeGenModule::setClassDefaultInstantiator(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env,
														  std::string const& generatedClassVarName, std::string& inout_result) noexcept
{
	inout_result += "static RECore::StaticMethod defaultSharedInstantiator(\"\", 0u, RECore::getType<RECore::SharedPtr<" + structClass.name +">>(),"
		"new RECore::NonMemberFunction<RECore::SharedPtr<" + structClass.name + ">()>(&RECore::internal::CodeGenerationHelpers::defaultSharedInstantiator<" + structClass.name + ">),"
		"RECore::EMethodFlags::Default, nullptr);" + env.getSeparator();

	inout_result += generatedClassVarName + "addSharedInstantiator(defaultSharedInstantiator);" + env.getSeparator();
}

void ReflectionCodeGenModule::fillClassParents(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env,
											   std::string const& generatedEntityVarName, std::string& inout_result) noexcept
{
	if (!structClass.parents.empty())
	{
		inout_result += generatedEntityVarName + "setDirectParentsCapacity(" + std::to_string(structClass.parents.size()) + ");" + env.getSeparator();

		for (REHeaderTool::StructClassInfo::ParentInfo const& parent : structClass.parents)
		{
			inout_result += generatedEntityVarName + "addDirectParent("
				"RECore::getArchetype<" + parent.type.getName(true) + ">(), "
				"static_cast<RECore::EAccessSpecifier>(" + std::to_string(static_cast<uint8_t>(parent.inheritanceAccess)) + "));" + env.getSeparator();
		}
	}
}

void ReflectionCodeGenModule::fillClassFields(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string generatedClassRefExpression, std::string& inout_result) noexcept
{
	inout_result += structClass.name + "::_RECore_registerChildClass<" + structClass.name + ">(" + std::move(generatedClassRefExpression) + ");" + env.getSeparator();
}

void ReflectionCodeGenModule::fillClassMethods(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string const& generatedEntityVarName, std::string& inout_result) noexcept
{
	if (!structClass.methods.empty())
	{
		inout_result += "[[maybe_unused]] RECore::Method* method = nullptr; [[maybe_unused]] RECore::StaticMethod* staticMethod = nullptr;" + env.getSeparator();
	}

	std::string generatedCode;
	std::string currentMethodVariable;
	for (REHeaderTool::MethodInfo const& method : structClass.methods)
	{
		if (method.isStatic)
		{
			inout_result += "staticMethod = " + generatedEntityVarName + "addStaticMethod(\"" + method.name + "\", " +
				(structClass.type.isTemplateType() ? computeClassTemplateEntityId(structClass, method) : std::to_string(_stringHasher(method.id)) + "u") + ", "
				"RECore::getType<" + method.returnType.getName() + ">(), "
				"new RECore::NonMemberFunction<" + method.getPrototype(true) + ">(& " + structClass.name + "::" + method.name + "), "
				"static_cast<RECore::EMethodFlags>(" + std::to_string(computeRefurekuMethodFlags(method)) + "));" + env.getSeparator();

			currentMethodVariable = "staticMethod";
		}
		else
		{
			inout_result += "method = " + generatedEntityVarName + "addMethod(\"" + method.name + "\", " +
				(structClass.type.isTemplateType() ? computeClassTemplateEntityId(structClass, method) : std::to_string(_stringHasher(method.id)) + "u") + ", "
				"RECore::getType<" + method.returnType.getName() + ">(), "
				"new RECore::MemberFunction<" + structClass.name + ", " + method.getPrototype(true) + ">(static_cast<" + computeFullMethodPointerType(structClass, method) + ">(& " + structClass.name + "::" + method.name + ")), "
				"static_cast<RECore::EMethodFlags>(" + std::to_string(computeRefurekuMethodFlags(method)) + "));" + env.getSeparator();

			currentMethodVariable = "method";
		}

		//Setup parameters
		if (!method.parameters.empty())
		{
			//Add all parameters in a single string
			generatedCode = currentMethodVariable + "->setParametersCapacity(" + std::to_string(method.parameters.size()) + "); " + env.getSeparator();

			for (REHeaderTool::FunctionParamInfo const& param : method.parameters)
			{
				generatedCode += currentMethodVariable + "->addParameter(\"" + param.name + "\", 0u, RECore::getType<" + param.type.getName() + ">());" + env.getSeparator();	//TODO: Build Id for parameters
			}

			//Write generated parameters string to file
			inout_result += generatedCode + env.getSeparator();
		}

		//Add properties after the method has been fully setup
		//Parameters have been added at this point, so properties generated code can safely add additional checks
		if (method.isStatic)
		{
			fillEntityProperties(method, env, "staticMethod->", inout_result);

			//Generate specific code for the Instantiator property here
			for (uint8_t i = 0; i < method.properties.size(); i++)
			{
				if (_instantiatorProperty.shouldGenerateCodeForEntity(method, method.properties[i], i))
				{
					_instantiatorProperty.addInstantiatorToClass(method.properties[i], generatedEntityVarName, "staticMethod", inout_result);
					break;
				}
			}
		}
		else
		{
			fillEntityProperties(method, env, "method->", inout_result);

			//Base method properties must be inherited AFTER this method properties have been added
			if (method.isOverride)
			{
				inout_result += "method->inheritBaseMethodProperties();" + env.getSeparator();
			}
		}
	}
}

void ReflectionCodeGenModule::fillClassNestedArchetypes(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string const& generatedEntityVarName, std::string& inout_result) noexcept
{
	std::size_t nestedArchetypesCount = structClass.nestedStructs.size() + structClass.nestedClasses.size() + structClass.nestedEnums.size();

	//No code to generate if there are no nested entities
	if (nestedArchetypesCount == 0u)
	{
		return;
	}

	//Reserve memory for the correct number of nested entities
	inout_result += generatedEntityVarName + "setNestedArchetypesCapacity(" + std::to_string(nestedArchetypesCount) + ");" + env.getSeparator();

	auto addNestedStructClassLambda = [&inout_result, &generatedEntityVarName, &env](std::shared_ptr<REHeaderTool::NestedStructClassInfo> const& structClass)
	{
		inout_result += generatedEntityVarName + "addNestedArchetype(RECore::getArchetype<" + structClass->type.getCanonicalName() + ">(), "
			"static_cast<RECore::EAccessSpecifier>(" + std::to_string(static_cast<uint8_t>(structClass->accessSpecifier)) + "));" + env.getSeparator();
	};

	//Add nested structs
	for (std::shared_ptr<REHeaderTool::NestedStructClassInfo> const& nestedStruct : structClass.nestedStructs)
	{
		addNestedStructClassLambda(nestedStruct);
	}

	//Add nested classes
	for (std::shared_ptr<REHeaderTool::NestedStructClassInfo> const& nestedClass : structClass.nestedClasses)
	{
		addNestedStructClassLambda(nestedClass);
	}

	//Add nested enums
	for (REHeaderTool::NestedEnumInfo const& nestedEnum : structClass.nestedEnums)
	{
		inout_result += generatedEntityVarName + "addNestedArchetype(" + 
			(isRegisteredNonPublicEnum(nestedEnum) ? computeGetNestedEnumMethodName(nestedEnum) : "RECore::getEnum<" + nestedEnum.type.getCanonicalName() + ">") + "(), "
			"static_cast<RECore::EAccessSpecifier>(" + std::to_string(static_cast<uint8_t>(nestedEnum.accessSpecifier)) + "));" + env.getSeparator();
	}
}

void ReflectionCodeGenModule::defineGetArchetypeMethodIfInheritFromObject(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	if (env.getFileParsingResult()->structClassTree.isBaseOf("RECore::Object", structClass.getFullName()))
	{
		std::string returnType = (structClass.isClass()) ? "RECore::Class" : "RECore::Struct";

		inout_result += std::move(returnType) + " const& " + structClass.type.getCanonicalName() + "::getArchetype() const noexcept { return " + structClass.name + "::staticGetArchetype(); }" + env.getSeparator() + env.getSeparator();
	}
}

void ReflectionCodeGenModule::declareGetArchetypeTemplateSpecialization(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	if (isPublicClass(structClass))
	{
		inout_result += "template <> " + env.getExportSymbolMacro() + " RECore::Archetype const* RECore::getArchetype<" + structClass.type.getName() + ">() noexcept;" + env.getSeparator();
	}
}

void ReflectionCodeGenModule::defineGetArchetypeTemplateSpecialization(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	//Generate the getArchetype specialization only if the class is non-nested, namespace nested or publicly nested in a struct/class
	if (isPublicClass(structClass))
	{
		inout_result += "template <> RECore::Archetype const* RECore::getArchetype<" + structClass.getFullName() + ">() noexcept { " +
			"return &" + structClass.getFullName() + "::staticGetArchetype(); }" + env.getSeparator() + env.getSeparator();
	}
}

void ReflectionCodeGenModule::declareAndDefineRegisterChildClassMethod(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	bool isGeneratingHiddenCode = _isGeneratingHiddenCode;

	inout_result += "private: template <typename ChildClass> static void _RECore_registerChildClass(RECore::Struct& childClass) noexcept {" + env.getSeparator();

	//Propagate the child class registration to parent classes too
	for (REHeaderTool::StructClassInfo::ParentInfo const& parent : structClass.parents)
	{
		inout_result += "RECore::internal::CodeGenerationHelpers::registerChildClass<" + parent.type.getName(true) + ", ChildClass>(childClass);" + env.getSeparator();
	}

	inout_result += "RECore::Struct const& thisClass = staticGetArchetype();" + env.getSeparator();

	//Register the child to the subclasses list
	inout_result += "if constexpr (!std::is_same_v<ChildClass, " + structClass.name + ">) const_cast<RECore::Struct&>(thisClass).addSubclass(childClass);" + env.getSeparator();

	//Make the child class inherit from the parents class fields
	if (!structClass.fields.empty())
	{
		inout_result += "[[maybe_unused]] RECore::Field* field = nullptr; [[maybe_unused]] RECore::StaticField* staticField = nullptr;" + env.getSeparator();

		//Trick to have the pragma statement outside of the UNPACK_IF_NOT_PARSING macro
		//If not doing that, the pragma is ignored and offsetof warnings are issued on gcc & clang.
		if (isGeneratingHiddenCode)
		{
			endHiddenGeneratedCode(env, inout_result);
			inout_result += "__RFK_DISABLE_WARNING_PUSH " + env.getSeparator() + "__RFK_DISABLE_WARNING_OFFSETOF " + env.getSeparator();	//Disable offsetof usage warnings
			beginHiddenGeneratedCode(env, inout_result);
		}
		else
		{
			inout_result += "__RFK_DISABLE_WARNING_PUSH " + env.getSeparator() + "__RFK_DISABLE_WARNING_OFFSETOF " + env.getSeparator();	//Disable offsetof usage warnings
		}

		//Iterate over fields
		std::string properties;
		std::string currentFieldVariable;
		for (REHeaderTool::FieldInfo const& field : structClass.fields)
		{
			if (field.isStatic)
			{
				inout_result += "staticField = childClass.addStaticField(\"" + field.name + "\", " +
					(structClass.type.isTemplateType() ? computeClassTemplateEntityId(structClass, field) : computeClassNestedEntityId("ChildClass", field)) + ", " +
					"RECore::getType<" + field.type.getName() + ">(), "
					"static_cast<RECore::EFieldFlags>(" + std::to_string(computeRefurekuFieldFlags(field)) + "), "
					"&" + structClass.name + "::" + field.name + ", "
					"&thisClass);" + env.getSeparator();

				currentFieldVariable = "staticField->";
			}
			else
			{
				inout_result += "field = childClass.addField(\"" + field.name + "\", " +
					(structClass.type.isTemplateType() ? computeClassTemplateEntityId(structClass, field) : computeClassNestedEntityId("ChildClass", field)) + ", " +
					"RECore::getType<" + field.type.getName() + ">(), "
					"static_cast<RECore::EFieldFlags>(" + std::to_string(computeRefurekuFieldFlags(field)) + "), "
					"offsetof(ChildClass, " + field.name + "), "
					"&thisClass);" + env.getSeparator();

				currentFieldVariable = "field->";
			}

			//Add properties
			fillEntityProperties(field, env, currentFieldVariable, inout_result);
		}

		//Trick to have the pragma statement outside of the UNPACK_IF_NOT_PARSING macro
		//If not doing that, the pragma is ignored and offsetof warnings are issued on gcc & clang.
		if (isGeneratingHiddenCode)
		{
			endHiddenGeneratedCode(env, inout_result);
			inout_result += "__RFK_DISABLE_WARNING_POP " + env.getSeparator();
			beginHiddenGeneratedCode(env, inout_result);
		}
		else
		{
			inout_result += "__RFK_DISABLE_WARNING_POP " + env.getSeparator();
		}
	}

	inout_result += "}" + env.getSeparator() + env.getSeparator();
}

void ReflectionCodeGenModule::declareGetNestedEnumMethods(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	bool publicClass = isPublicClass(structClass);

	for (REHeaderTool::NestedEnumInfo const& nestedEnum : structClass.nestedEnums)
	{
		//Generate a method on non-public enums
		//Public nested enums generated code is handled like non-nested enums
		if (!publicClass || nestedEnum.accessSpecifier != REHeaderTool::EAccessSpecifier::Public)
		{
			_nonPublicEnums.emplace(&nestedEnum);

			beginHiddenGeneratedCode(env, inout_result);

			inout_result += "private: static RECore::Enum const* " + computeGetNestedEnumMethodName(nestedEnum) + "() noexcept;" + env.getSeparator();

			endHiddenGeneratedCode(env, inout_result);
		}
	}

	inout_result += env.getSeparator();
}

void ReflectionCodeGenModule::defineGetNestedEnumMethods(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	for (REHeaderTool::NestedEnumInfo const& nestedEnum : structClass.nestedEnums)
	{
		if (isRegisteredNonPublicEnum(nestedEnum))
		{
			inout_result += "RECore::Enum const* " + structClass.getFullName() + "::" + computeGetNestedEnumMethodName(nestedEnum) + "() noexcept" + env.getSeparator();
			defineGetEnumContent(nestedEnum, env, inout_result);
		}
	}
}

void ReflectionCodeGenModule::declareAndDefineClassRegistererVariable(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	//Define the registrator only when there is no outer entity.
	//If there is an outer entity, it will register its nested entities to the database itself.
	if (structClass.outerEntity == nullptr)
	{
		inout_result += "namespace RECore::generated { static RECore::ArchetypeRegisterer const registerer_" + getEntityId(structClass) + " = " +
			structClass.getFullName() + "::staticGetArchetype(); }" + env.getSeparator() + env.getSeparator();
	}
}

void ReflectionCodeGenModule::declareAndDefineClassTemplateStaticGetArchetypeMethod(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	inout_result += "public: static RECore::ClassTemplateInstantiation const& staticGetArchetype() noexcept {" + env.getSeparator();
	inout_result += "static bool initialized = false;" + env.getSeparator();
	inout_result += "static RECore::ClassTemplateInstantiation type(\"" + structClass.type.getName(false, true) + "\"," +
		computeClassTemplateEntityId(structClass, structClass) + ", " +
		"sizeof(" + structClass.getFullName() + "), " + 
		std::to_string(structClass.isClass()) + ", "
		"*RECore::getArchetype<::" + structClass.type.getName() + ">());" + env.getSeparator();

	//Init content
	inout_result += "if (!initialized) {" + env.getSeparator();
	inout_result += "initialized = true;" + env.getSeparator();

	//Inside the if statement, initialize the Struct metadata
	fillClassTemplateArguments(structClass, "type.", env, inout_result);
	fillEntityProperties(structClass, env, "type.", inout_result);
	fillClassParents(structClass, env, "type.", inout_result);
	fillClassFields(structClass, env, "type", inout_result);

	//Set the default instantiator BEFORE filling the class methods since methods can overwrite the custom instantiator
	setClassDefaultInstantiator(structClass, env, "type.", inout_result);
	fillClassMethods(structClass, env, "type.", inout_result);
	fillClassNestedArchetypes(structClass, env, "type.", inout_result);

	//End init
	inout_result += "}" + env.getSeparator();

	inout_result += "return type; }" + env.getSeparator() + env.getSeparator();
}

void ReflectionCodeGenModule::declareAndDefineClassTemplateGetArchetypeMethodIfInheritFromObject(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	if (env.getFileParsingResult()->structClassTree.isBaseOf("RECore::Object", structClass.getFullName()))
	{
		inout_result += "virtual RECore::ClassTemplateInstantiation const& getArchetype() const noexcept override { return staticGetArchetype(); }" + env.getSeparator() + env.getSeparator();
	}
}

void ReflectionCodeGenModule::declareAndDefineClassTemplateRegistererField(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	//Define the registrator only when there is no outer entity.
	//If there is an outer entity, it will register its nested entities to the database itself.
	if (structClass.outerEntity == nullptr)
	{
		inout_result += "private: static inline RECore::ClassTemplateInstantiationRegisterer const _RECore_registerer = staticGetArchetype(); RECore::internal::ForceGenerateSymbol<&_RECore_registerer> _RECore_forceRegister() = delete;" + env.getSeparator() + env.getSeparator();
	}
}

void ReflectionCodeGenModule::fillClassTemplateArguments(REHeaderTool::StructClassInfo const& structClass, std::string generatedEntityVarName, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	std::vector<REHeaderTool::TemplateParamInfo> const& templateParameters = structClass.type.getTemplateParameters();
	std::string boundParameterIndex;
	std::string argVarName;

	for (std::size_t i = 0; i < templateParameters.size(); i++)
	{
		boundParameterIndex = std::to_string(i);
		argVarName = "arg" + boundParameterIndex;

		//TODO: this doesn't support auto
		switch (templateParameters[i].kind)
		{
			case REHeaderTool::ETemplateParameterKind::TypeTemplateParameter:
				inout_result += "static RECore::TypeTemplateArgument " + argVarName + "(RECore::getArchetype<" + templateParameters[i].name + ">());" + env.getSeparator();
				break;

			case REHeaderTool::ETemplateParameterKind::NonTypeTemplateParameter:
				inout_result += "static constexpr auto const " + argVarName + "Value = " + templateParameters[i].name + ";" + env.getSeparator();
				inout_result += "static RECore::NonTypeTemplateArgument " + argVarName + "(RECore::getArchetype<decltype(" + templateParameters[i].name + ")>(), "
					"&" + argVarName + "Value);" + env.getSeparator();
				break;

			case REHeaderTool::ETemplateParameterKind::TemplateTemplateParameter:
				inout_result += "static RECore::TemplateTemplateArgument " + argVarName + "(reinterpret_cast<RECore::ClassTemplate const*>(RECore::getArchetype<" + templateParameters[i].name + ">()));" + env.getSeparator();
				break;

			default:
				assert(false);
				break;
		}

		inout_result += "" + generatedEntityVarName + "addTemplateArgument(" + argVarName + ");" + env.getSeparator();
	}
}

void ReflectionCodeGenModule::defineClassTemplateGetArchetypeTemplateSpecialization(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	assert(structClass.type.isTemplateType());

	inout_result += "template <> " + env.getExportSymbolMacro() + " RECore::Archetype const* RECore::getArchetype<" + structClass.type.getName() + ">() noexcept {" + env.getSeparator();
	inout_result += "static bool initialized = false;" + env.getSeparator();
	inout_result += "static RECore::ClassTemplate type(\"" + structClass.type.getName(false, true) + "\", " +
		std::to_string(_stringHasher(structClass.id)) + "u, " +
		std::to_string(structClass.isClass()) + 
		");" + env.getSeparator();

	//Init class template content
	inout_result += "if (!initialized) {" + env.getSeparator();
	inout_result += "initialized = true;" + env.getSeparator();

	fillEntityProperties(structClass, env, "type.", inout_result);

	//Class template has no fields / methods until it is instantiated (no memory address).
	//Likewise, the parent type can depend on the template params which are not accessible from this method, so omit them
	fillClassTemplateParameters(structClass, "type.", env, inout_result);

	//End init if
	inout_result += "}";

	inout_result += "return &type; }" + env.getSeparator() + env.getSeparator();
}

void ReflectionCodeGenModule::addTemplateParameter(REHeaderTool::TemplateParamInfo const& templateParam, std::string generatedVarName, std::string const& objVarName,
												   std::string addFuncName, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result, int recursionLevel) const noexcept
{
	inout_result += "{ " + env.getSeparator();

	switch (templateParam.kind)
	{
		case REHeaderTool::ETemplateParameterKind::NonTypeTemplateParameter:
			//TODO: this does not support auto
			inout_result += "static RECore::NonTypeTemplateParameter " + generatedVarName + "(\"" + templateParam.name + "\", RECore::getArchetype<" + templateParam.type->getName() + ">());" + env.getSeparator();
			break;

		case REHeaderTool::ETemplateParameterKind::TypeTemplateParameter:
			inout_result += "static RECore::TypeTemplateParameter " + generatedVarName + "(\"" + templateParam.name + "\");" + env.getSeparator();
			break;

		case REHeaderTool::ETemplateParameterKind::TemplateTemplateParameter:
			inout_result += "static RECore::TemplateTemplateParameter " + generatedVarName + "(\"" + templateParam.name + "\");" + env.getSeparator();
			for (REHeaderTool::TemplateParamInfo const& param : templateParam.type->getTemplateParameters())
			{
				addTemplateParameter(param, generatedVarName + std::to_string(recursionLevel), generatedVarName + ".", "addTemplateParameter", env, inout_result, recursionLevel + 1);
			}
			break;

		default:
			//Should never reach this point
			assert(false);
	}

	inout_result += objVarName + std::move(addFuncName) + "(" + std::move(generatedVarName) + ");" + env.getSeparator();
	inout_result += "}" + env.getSeparator();
}

void ReflectionCodeGenModule::fillClassTemplateParameters(REHeaderTool::StructClassInfo const& structClass, std::string classTemplateVarName, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	for (REHeaderTool::TemplateParamInfo const& templateParam : structClass.type.getTemplateParameters())
	{
		addTemplateParameter(templateParam, "templateParameter", std::move(classTemplateVarName), "addTemplateParameter", env, inout_result);
	}
}

void ReflectionCodeGenModule::declareAndDefineClassTemplateRegistererVariable(REHeaderTool::StructClassInfo const& structClass, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	assert(structClass.type.isTemplateType());

	//Define the registrator only when there is no outer entity.
	//If there is an outer entity, it will register its nested entities to the database itself.
	if (structClass.outerEntity == nullptr)
	{
		inout_result += "namespace RECore::generated { static RECore::ArchetypeRegisterer const register_" + getEntityId(structClass) +
			" = *RECore::getArchetype<::" + structClass.type.getName(false, false, true) + ">(); }" + env.getSeparator() + env.getSeparator();
	}
}

std::string ReflectionCodeGenModule::convertEntityTypeToEntityKind(REHeaderTool::EEntityType entityType) noexcept
{
	switch (entityType)
	{
		case REHeaderTool::EEntityType::Struct:
			return "RECore::EEntityKind::Struct";

		case REHeaderTool::EEntityType::Class:
			return "RECore::EEntityKind::Class";

		case REHeaderTool::EEntityType::Enum:
			return "RECore::EEntityKind::Enum";

		case REHeaderTool::EEntityType::EnumValue:
			return "RECore::EEntityKind::EnumValue";

		case REHeaderTool::EEntityType::Field:
			return "RECore::EEntityKind::Field";

		case REHeaderTool::EEntityType::Method:
			return "RECore::EEntityKind::Method";

		case REHeaderTool::EEntityType::Variable:
			return "RECore::EEntityKind::Variable";

		case REHeaderTool::EEntityType::Function:
			return "RECore::EEntityKind::Function";

		case REHeaderTool::EEntityType::Namespace:
			return "RECore::EEntityKind::Namespace";

		case REHeaderTool::EEntityType::Undefined:
			return "RECore::EEntityKind::Undefined";
			break;
	}

	//Should never reach this point
	assert(false);

	return "";
}

uint16_t ReflectionCodeGenModule::computeRefurekuFieldFlags(REHeaderTool::FieldInfo const& field) noexcept
{
	uint16_t result = 0;

	switch (field.accessSpecifier)
	{
		case REHeaderTool::EAccessSpecifier::Public:
			result |= 1 << 0;
			break;

		case REHeaderTool::EAccessSpecifier::Protected:
			result |= 1 << 1;
			break;

		case REHeaderTool::EAccessSpecifier::Private:
			result |= 1 << 2;
			break;

		default:
			break;
	}

	if (field.isStatic)
		result |= 1 << 3;

	if (field.isMutable)
		result |= 1 << 4;

	return result;
}

uint16_t ReflectionCodeGenModule::computeRefurekuMethodFlags(REHeaderTool::MethodInfo const& method) noexcept
{
	uint16_t result = 0;

	switch (method.accessSpecifier)
	{
		case REHeaderTool::EAccessSpecifier::Public:
			result |= 1 << 0;
			break;

		case REHeaderTool::EAccessSpecifier::Protected:
			result |= 1 << 1;
			break;

		case REHeaderTool::EAccessSpecifier::Private:
			result |= 1 << 2;
			break;

		default:
			break;
	}

	if (method.isStatic)
		result |= 1 << 3;

	if (method.isInline)
		result |= 1 << 4;

	if (method.isVirtual)
		result |= 1 << 5;

	if (method.isPureVirtual)
		result |= 1 << 6;

	if (method.isOverride)
		result |= 1 << 7;

	if (method.isFinal)
		result |= 1 << 8;

	if (method.isConst)
		result |= 1 << 9;

	return result;
}

std::string ReflectionCodeGenModule::computeFullMethodPointerType(REHeaderTool::StructClassInfo const& classInfo, REHeaderTool::MethodInfo const& method) noexcept
{
	std::string result = method.getPrototype();

	if (method.isStatic)
	{
		//Add the ptr on non-member (*) to the type
		result.insert(result.find_first_of('('), "(*)");
	}
	else
	{
		//Add the ptr on member (Class::*) to the type
		result.insert(result.find_first_of('('), "(" + classInfo.name + "::*)");
	}

	return result;
}

bool ReflectionCodeGenModule::isRegisteredNonPublicEnum(REHeaderTool::EnumInfo const& nestedEnum) const noexcept
{
	return _nonPublicEnums.find(&nestedEnum) != _nonPublicEnums.cend();
}

void ReflectionCodeGenModule::declareGetEnumTemplateSpecialization(REHeaderTool::EnumInfo const& enum_, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	//Don't generate template specialization code on non-public enums
	//Code is generated by the outer class itself
	if (!isRegisteredNonPublicEnum(enum_))
	{
		inout_result += "namespace RECore { template <> " + env.getExportSymbolMacro() + " RECore::Enum const* getEnum<" + enum_.type.getCanonicalName() + ">() noexcept; }" + env.getSeparator();
	}
}

void ReflectionCodeGenModule::defineGetEnumTemplateSpecialization(REHeaderTool::EnumInfo const& enum_, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	//Don't generate template specialization code on non-public enums
	//Code is generated by the outer class itself
	if (!isRegisteredNonPublicEnum(enum_))
	{
		std::string typeName = enum_.type.getCanonicalName();

		inout_result += "template <> RECore::Enum const* RECore::getEnum<" + typeName + ">() noexcept" + env.getSeparator();
		defineGetEnumContent(enum_, env, inout_result);
	}
}

void ReflectionCodeGenModule::defineGetEnumContent(REHeaderTool::EnumInfo const& enum_, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	inout_result += "{" + env.getSeparator() +
		"static bool initialized = false;" + env.getSeparator() +
		"static RECore::Enum type(\"" + enum_.name + "\", " +
		getEntityId(enum_) + ", "
		"RECore::getArchetype<" + enum_.underlyingType.getCanonicalName() + ">());" + env.getSeparator();

	//Initialize the enum metadata
	inout_result += "if (!initialized) {" + env.getSeparator() +
		"initialized = true;" + env.getSeparator();

	fillEntityProperties(enum_, env, "type.", inout_result);

	if (!enum_.enumValues.empty())
	{
		inout_result += "RECore::EnumValue* enumValue = nullptr;" + env.getSeparator() +
			"type.setEnumValuesCapacity(" + std::to_string(enum_.enumValues.size()) + ");" + env.getSeparator();

		for (REHeaderTool::EnumValueInfo const& enumValue : enum_.enumValues)
		{
			inout_result += "enumValue = type.addEnumValue(\"" + enumValue.name + "\", " + getEntityId(enumValue) + ", " + std::to_string(enumValue.value) + ");" + env.getSeparator();

			//Fill enum value properties
			fillEntityProperties(enumValue, env, "enumValue->", inout_result);
		}
	}

	//End initialization if
	inout_result += "}" + env.getSeparator();


	inout_result += "return &type; }" + env.getSeparator();
}

void ReflectionCodeGenModule::declareAndDefineEnumRegistererVariable(REHeaderTool::EnumInfo const& enum_, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) const noexcept
{
	if (enum_.outerEntity == nullptr)
	{
		inout_result += "namespace RECore::generated { static RECore::ArchetypeRegisterer const registerer_" + getEntityId(enum_) + " = *RECore::getEnum<" + enum_.type.getCanonicalName() + ">(); }" + env.getSeparator();
	}
}

void ReflectionCodeGenModule::declareGetVariableFunction(REHeaderTool::VariableInfo const& variable, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	beginHiddenGeneratedCode(env, inout_result);

	inout_result += "template <> RECore::Variable const* RECore::getVariable<&" + variable.getFullName() + ">() noexcept; " + env.getSeparator();

	endHiddenGeneratedCode(env, inout_result);
}

void ReflectionCodeGenModule::defineGetVariableFunction(REHeaderTool::VariableInfo const& variable, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	std::string fullName = variable.getFullName();

	inout_result += "template <> RECore::Variable const* RECore::getVariable<&" + variable.getFullName() + ">() noexcept {" + env.getSeparator() +
		"static bool initialized = false;" + env.getSeparator() + 
		"static RECore::Variable variable(\"" + variable.name + "\", " +
		getEntityId(variable) + ", "
		"RECore::getType<decltype(" + fullName + ")>(), "
		"&" + fullName + ", "
		"static_cast<RECore::EVarFlags>(" + std::to_string(computeRefurekuVariableFlags(variable)) + ")"
		");" + env.getSeparator();

	//Initialize variable metadata
	inout_result += "if (!initialized) {" + env.getSeparator() +
		"initialized = true;" + env.getSeparator();

	fillEntityProperties(variable, env, "variable.", inout_result);

	//End initialization if
	inout_result += "}";

	inout_result += "return &variable; }" + env.getSeparator();
}

void ReflectionCodeGenModule::declareAndDefineVariableRegistererVariable(REHeaderTool::VariableInfo const& variable, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	if (variable.outerEntity == nullptr)
	{
		inout_result += "namespace RECore::generated { static RECore::DefaultEntityRegisterer const registerer_" + getEntityId(variable) +
			" = *RECore::getVariable<&" + variable.getFullName() + ">(); }" + env.getSeparator();
	}
}

uint8_t ReflectionCodeGenModule::computeRefurekuVariableFlags(REHeaderTool::VariableInfo const& variable) noexcept
{
	uint8_t result = 0u;

	if (variable.isStatic)
	{
		result |= 1 << 0;
	}

	return result;
}

void ReflectionCodeGenModule::declareGetFunctionFunction(REHeaderTool::FunctionInfo const& function, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	beginHiddenGeneratedCode(env, inout_result);

	inout_result += "template <> RECore::Function const* RECore::getFunction<static_cast<" + computeFunctionPtrType(function) + ">(&" + function.getFullName() + ")>() noexcept; ";

	endHiddenGeneratedCode(env, inout_result);
}

void ReflectionCodeGenModule::defineGetFunctionFunction(REHeaderTool::FunctionInfo const& function, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	inout_result += "template <> RECore::Function const* RECore::getFunction<static_cast<" + computeFunctionPtrType(function) + ">(&" + function.getFullName() + ")>() noexcept {" + env.getSeparator() +
		"static bool initialized = false;" + env.getSeparator() + 
		"static RECore::Function function(\"" + function.name + "\", " +
		getEntityId(function) + ", "
		"RECore::getType<" + function.returnType.getCanonicalName() + ">(), "
		"new RECore::NonMemberFunction<" + function.getPrototype(true) + ">(&" + function.getFullName() + "), "
		"static_cast<RECore::EFunctionFlags>(" + std::to_string(computeRefurekuFunctionFlags(function)) + ")"
		");" + env.getSeparator();

	//Initialize variable metadata
	inout_result += "if (!initialized) {" + env.getSeparator() +
		"initialized = true;" + env.getSeparator();

	fillEntityProperties(function, env, "function.", inout_result);

	//Setup parameters
	if (!function.parameters.empty())
	{
		inout_result += "function.setParametersCapacity(" + std::to_string(function.parameters.size()) + ");" + env.getSeparator();

		for (REHeaderTool::FunctionParamInfo const& param : function.parameters)
		{
			inout_result += "function.addParameter(\"" + param.name + "\", 0u, RECore::getType<" + param.type.getName() + ">());" + env.getSeparator();	//TODO: Build an id for the parameter
		}

		inout_result += ";" + env.getSeparator();
	}

	//End initialization if
	inout_result += "}";

	inout_result += "return &function; }" + env.getSeparator();
}

void ReflectionCodeGenModule::declareAndDefineFunctionRegistererVariable(REHeaderTool::FunctionInfo const& function, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	if (function.outerEntity == nullptr)
	{
		inout_result += "namespace RECore::generated { static RECore::DefaultEntityRegisterer const registerer" + getEntityId(function) +
			" = *RECore::getFunction<static_cast<" + computeFunctionPtrType(function) + ">(&" + function.getFullName() + ")>(); }" + env.getSeparator();
	}
}

uint8_t ReflectionCodeGenModule::computeRefurekuFunctionFlags(REHeaderTool::FunctionInfo const& function) noexcept
{
	uint8_t result = 0u;

	if (function.isStatic)
	{
		result |= 1 << 0;
	}

	if (function.isInline)
	{
		result |= 1 << 1;
	}

	return result;
}

std::string ReflectionCodeGenModule::computeFunctionPtrType(REHeaderTool::FunctionInfo const& function) noexcept
{
	return function.returnType.getCanonicalName() + "(*)(" + function.getParameterTypes() + ")";
}

void ReflectionCodeGenModule::declareAndDefineGetNamespaceFragmentFunction(REHeaderTool::NamespaceInfo const& namespace_, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	inout_result += env.getInternalSymbolMacro() + " static RECore::NamespaceFragment const& " + computeGetNamespaceFragmentFunctionName(namespace_, env.getFileParsingResult()->parsedFile) + "() noexcept {" + env.getSeparator() +
		"static RECore::NamespaceFragment fragment(\"" + namespace_.name + "\", " + getEntityId(namespace_) + ");" + env.getSeparator() +
		"static bool initialized = false;" + env.getSeparator();


	//Initialize namespace metadata
	inout_result += "if (!initialized) {" + env.getSeparator() +
		"initialized = true;" + env.getSeparator();

	fillEntityProperties(namespace_, env, "fragment.", inout_result);

	size_t nestedEntityCount =	namespace_.namespaces.size() + namespace_.structs.size() + namespace_.classes.size() +
		namespace_.enums.size() + namespace_.variables.size() + namespace_.functions.size();

	if (nestedEntityCount > 0u)
	{
		//Reserve space first
		inout_result += "fragment.setNestedEntitiesCapacity(" + std::to_string(nestedEntityCount) + "u);" + env.getSeparator();

		//Nested...
		//Namespaces
		for (REHeaderTool::NamespaceInfo const& nestedNamespace : namespace_.namespaces)
		{
			inout_result += "fragment.addNestedEntity(RECore::generated::" + computeGetNamespaceFragmentFunctionName(nestedNamespace, env.getFileParsingResult()->parsedFile) + "());" + env.getSeparator();
		}

		//Structs
		for (REHeaderTool::StructClassInfo const& nestedStruct : namespace_.structs)
		{
			inout_result += "fragment.addNestedEntity(*RECore::getArchetype<" + nestedStruct.type.getCanonicalName() + ">());" + env.getSeparator();
		}

		//Classes
		for (REHeaderTool::StructClassInfo const& nestedClass : namespace_.classes)
		{
			inout_result += "fragment.addNestedEntity(*RECore::getArchetype<" + nestedClass.type.getCanonicalName() + ">());" + env.getSeparator();
		}

		//Enums
		for (REHeaderTool::EnumInfo const& nestedEnum : namespace_.enums)
		{
			inout_result += "fragment.addNestedEntity(*RECore::getEnum<" + nestedEnum.type.getCanonicalName() + ">());" + env.getSeparator();
		}

		//Variables
		for (REHeaderTool::VariableInfo const& variable : namespace_.variables)
		{
			inout_result += "fragment.addNestedEntity(*RECore::getVariable<&" + variable.getFullName() + ">()); " + env.getSeparator();
		}

		//Functions
		for (REHeaderTool::FunctionInfo const& function : namespace_.functions)
		{
			inout_result += "fragment.addNestedEntity(*RECore::getFunction<static_cast<" + computeFunctionPtrType(function) + ">(&" + function.getFullName() + ")>());" + env.getSeparator();
		}
	}

	//End initialization if
	inout_result += "}" + env.getSeparator();

	inout_result += "return fragment; }" + env.getSeparator();
}

void ReflectionCodeGenModule::declareAndDefineNamespaceFragmentRegistererVariable(REHeaderTool::NamespaceInfo const& namespace_, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	assert(namespace_.outerEntity == nullptr);

	inout_result += "static RECore::NamespaceFragmentRegisterer const " + computeNamespaceFragmentRegistererName(namespace_, env.getFileParsingResult()->parsedFile) +
		"(RECore::generated::" + computeGetNamespaceFragmentFunctionName(namespace_, env.getFileParsingResult()->parsedFile) + "());" + env.getSeparator();
}

void ReflectionCodeGenModule::declareAndDefineGetNamespaceFragmentAndRegistererRecursive(REHeaderTool::NamespaceInfo const& namespace_, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept
{
	//Use a lambda since since this portion of code should not be accessible to other methods
	auto const defineAndDeclareGetNamespaceFragmentAndRegistererLambda = [this](REHeaderTool::NamespaceInfo const& namespace_, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) -> void
	{
		//Nested lambda to make a recursive lambda call
		auto const defineAndDeclareGetNamespaceFragmentAndRegistererLambdaInternal = [this](REHeaderTool::NamespaceInfo const& namespace_, REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result,
																							auto const& defineNamespaceFragmentRegistererVariableInternalRef) -> void
		{
			for (REHeaderTool::NamespaceInfo const& nestedNamespace : namespace_.namespaces)
			{
				defineNamespaceFragmentRegistererVariableInternalRef(nestedNamespace, env, inout_result, defineNamespaceFragmentRegistererVariableInternalRef);
			}

			declareAndDefineGetNamespaceFragmentFunction(namespace_, env, inout_result);
		};

		defineAndDeclareGetNamespaceFragmentAndRegistererLambdaInternal(namespace_, env, inout_result, defineAndDeclareGetNamespaceFragmentAndRegistererLambdaInternal);
	};

	//Generate code only if it is a top-level namespace
	if (namespace_.outerEntity == nullptr)
	{
		inout_result += "namespace RECore::generated { " + env.getSeparator();
		defineAndDeclareGetNamespaceFragmentAndRegistererLambda(namespace_, env, inout_result);
		declareAndDefineNamespaceFragmentRegistererVariable(namespace_, env, inout_result);
		inout_result += " }" + env.getSeparator();
	}
}

std::string ReflectionCodeGenModule::computeGetNamespaceFragmentFunctionName(REHeaderTool::NamespaceInfo const& namespace_, fs::path const& sourceFile) noexcept
{
	return "getNamespaceFragment_" + getEntityId(namespace_) + "_" + std::to_string(_stringHasher(sourceFile.string()));
}

std::string ReflectionCodeGenModule::computeNamespaceFragmentRegistererName(REHeaderTool::NamespaceInfo const& namespace_, fs::path const& sourceFile) noexcept
{
	return "namespaceFragmentRegisterer_" + getEntityId(namespace_) + "_" + std::to_string(_stringHasher(sourceFile.string()));
}