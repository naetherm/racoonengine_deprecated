#include "REHeaderTool/CodeGen/CodeGenUnit.h"

#include <algorithm>

#include "REHeaderTool/CodeGen/CodeGenHelpers.h"
#include "REHeaderTool/CodeGen/PropertyCodeGen.h"

#define HANDLE_NESTED_ENTITY_ITERATION_RESULT(result)																\
	if (result == ETraversalBehaviour::Break)																		\
	{																												\
		break;																										\
	}																												\
	else if (result == ETraversalBehaviour::AbortWithFailure || result == ETraversalBehaviour::AbortWithSuccess)	\
	{																												\
		return result;																								\
	}

using namespace REHeaderTool;

CodeGenUnit::CodeGenUnit(CodeGenUnit const& other) noexcept:
	_isCopy{true},
	settings{other.settings},
	logger{other.logger}
{
	//Replace each module by a new clone of themself so that
	//each CodeGenUnit instance owns their own modules
	for (size_t i = 0; i < other._generationModules.size(); i++)
	{
		addModule(*static_cast<CodeGenModule*>(other._generationModules[i]->clone()));
	}
}

CodeGenUnit::~CodeGenUnit() noexcept
{
	clearGenerationModules();
}

bool CodeGenUnit::checkSettings() const noexcept
{
	bool result = true;
	bool canLog	= logger != nullptr;

	if (settings == nullptr)
	{
		if (canLog)
		{
			logger->log("CodeGenUnit settings have not been set.", ILogger::ELogSeverity::Error);
		}

		result &= false;
	}
	else
	{
		if (settings->getOutputDirectory().empty())
		{
			if (canLog)
			{
				logger->log("Output directory is empty, it must be specified for the files to be generated.", ILogger::ELogSeverity::Error);
			}

			result &= false;
		}
		else if (!fs::exists(settings->getOutputDirectory()))
		{
			//Before doing anything, make sure the output directory exists
			//If it doesn't, create it

			//Try to create them if it doesn't exist
			try
			{
				fs::create_directories(settings->getOutputDirectory());

				if (canLog)
				{
					logger->log("Specified output directory doesn't exist. Create " + FilesystemHelpers::sanitizePath(settings->getOutputDirectory()).string());
				}
			}
			catch (fs::filesystem_error const& exception)
			{
				if (canLog)
				{
					logger->log("Tried to create directory " + settings->getOutputDirectory().string() + " but failed: " + std::string(exception.what()), ILogger::ELogSeverity::Error);
				}

				//Directory failed to be created, so settings are incorrect
				result &= false;
			}
		}
	}

	return result;
}

bool CodeGenUnit::isFileNewerThan(fs::path const& file, fs::path const& referenceFile) const noexcept
{
	assert(fs::exists(file));
	assert(fs::is_regular_file(file));
	assert(fs::exists(referenceFile));
	assert(fs::is_regular_file(referenceFile));

	return fs::last_write_time(file) > fs::last_write_time(referenceFile);
}

bool CodeGenUnit::generateCode(FileParsingResult const& parsingResult) noexcept
{
	//TODO: Should probably use std::unique_ptr here instead of a raw pointer to be exception-safe
	CodeGenEnv* env = createCodeGenEnv();
	
	//If you assert/crash here, means the createCodeGenEnv method returned nullptr
	//Check the implementation in the CodeGenUnit you use.
	assert(env != nullptr);

	//Pre-generation step
	bool result = preGenerateCode(parsingResult, *env);

	//Generation step (per module/entity pair), runs only if the pre-generation step succeeded
	if (result)
	{
		std::vector<ICodeGenerator*> const& codeGenerators = getSortedCodeGenerators();

		//Call initialGenerateCode on all ICodeGenerators first
		initialGenerateCodeInternal(codeGenerators, *env);

		if (result)
		{
			//Iterate over each module and entity and generate code
			result &= foreachCodeGenEntityPair(std::bind(&CodeGenUnit::generateCodeForEntityInternal, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), *env) != ETraversalBehaviour::AbortWithFailure;

			if (result)
			{
				//Final call to generate code with a nullptr entity
				finalGenerateCodeInternal(codeGenerators, *env);

				//Post-generation step, runs only if all previous steps succeeded
				if (result)
				{
					result &= postGenerateCode(*env);
				}
			}
		}
	}

	delete env;

	return result;
}

bool CodeGenUnit::initialGenerateCodeInternal(std::vector<ICodeGenerator*> const& codeGenerators, CodeGenEnv& env) noexcept
{
	bool result = true;

	for (ICodeGenerator* codeGenerator : codeGenerators)
	{
		auto generateLambda = [&result, codeGenerator](CodeGenEnv& env, std::string& inout_result)
		{
			result &= codeGenerator->initialGenerateCode(env, inout_result);
		};

		//Result will be altered when generateLambda will be called from the CodeGenUnit::initialGenerateCode override
		initialGenerateCode(env, generateLambda);
	}
	
	return result;
}

bool CodeGenUnit::finalGenerateCodeInternal(std::vector<ICodeGenerator*> const& codeGenerators, CodeGenEnv& env) noexcept
{
	bool result = true;

	for (ICodeGenerator* codeGenerator : codeGenerators)
	{
		auto generateLambda = [&result, codeGenerator](CodeGenEnv& env, std::string& inout_result)
		{
			result &= codeGenerator->finalGenerateCode(env, inout_result);
		};

		//Result will be altered when generateLambda will be called from the CodeGenUnit::initialGenerateCode override
		finalGenerateCode(env, generateLambda);
	}

	return result;
}

ETraversalBehaviour	CodeGenUnit::generateCodeForEntityInternal(ICodeGenerator& codeGenerator, EntityInfo const& entity, CodeGenEnv& env, void const* data) noexcept
{
	ETraversalBehaviour result = CodeGenHelpers::leastPrioritizedTraversalBehaviour;

	auto generateLambda = [&result, &codeGenerator, &data](EntityInfo const& entity, CodeGenEnv& env, std::string& inout_result)
	{
		result = CodeGenHelpers::combineTraversalBehaviours(result, codeGenerator.generateCodeForEntity(entity, env, inout_result, data));
	};

	//Result will be altered when generateLambda will be called from the CodeGenUnit::generateCodeForEntity override
	generateCodeForEntity(entity, env, generateLambda);

	return result;
}

void CodeGenUnit::sortedInsert(std::vector<ICodeGenerator*>& vector, ICodeGenerator& codeGen) noexcept
{
	vector.insert
	(
		std::upper_bound(vector.begin(), vector.end(), &codeGen, [](ICodeGenerator const* lhs, ICodeGenerator const* rhs)
		{
			return lhs->getGenerationOrder() < rhs->getGenerationOrder();
		}),
		&codeGen
	);
}

std::vector<ICodeGenerator*> CodeGenUnit::getSortedCodeGenerators() const noexcept
{
	std::vector<ICodeGenerator*> result;

	//Insert all code gen modules
	for (CodeGenModule* codeGenModule : _generationModules)
	{
		sortedInsert(result, *codeGenModule);

		//Insert all property code gens contained in code gen modules
		for (PropertyCodeGen* propertyCodeGen : codeGenModule->getPropertyCodeGenerators())
		{
			sortedInsert(result, *propertyCodeGen);
		}
	}

	return result;
}

CodeGenEnv* CodeGenUnit::createCodeGenEnv() const noexcept
{
	return new CodeGenEnv();
}

bool CodeGenUnit::preGenerateCode(FileParsingResult const& parsingResult, CodeGenEnv& env) noexcept
{
	//Setup generation environment
	env._fileParsingResult	= &parsingResult;
	env._logger				= logger;

	return true;
}

bool CodeGenUnit::postGenerateCode(CodeGenEnv& /* env */) noexcept
{
	//Default implementation does nothing
	return true;
}
ETraversalBehaviour CodeGenUnit::foreachCodeGenEntityPair(std::function<ETraversalBehaviour(ICodeGenerator&, EntityInfo const&, CodeGenEnv&, void const*)> visitor, CodeGenEnv& env) noexcept
{
	assert(visitor != nullptr);

	ETraversalBehaviour result;

	//Call visitor on all code generators
	for (ICodeGenerator* codeGenerator : getSortedCodeGenerators())
	{
		for (NamespaceInfo const& namespace_ : env.getFileParsingResult()->namespaces)
		{
			result = foreachCodeGenEntityPairInNamespace(*codeGenerator, namespace_, env, visitor);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (StructClassInfo const& struct_ : env.getFileParsingResult()->structs)
		{
			result = foreachCodeGenEntityPairInStruct(*codeGenerator, struct_, env, visitor);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (StructClassInfo const& class_ : env.getFileParsingResult()->classes)
		{
			result = foreachCodeGenEntityPairInStruct(*codeGenerator, class_, env, visitor);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (EnumInfo const& enum_ : env.getFileParsingResult()->enums)
		{
			result = foreachCodeGenEntityPairInEnum(*codeGenerator, enum_, env, visitor);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (VariableInfo const& variable : env.getFileParsingResult()->variables)
		{
			result = codeGenerator->callVisitorOnEntity(variable, env, visitor);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}

		for (FunctionInfo const& function : env.getFileParsingResult()->functions)
		{
			result = codeGenerator->callVisitorOnEntity(function, env, visitor);

			HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
		}
	}

	return ETraversalBehaviour::Recurse;
}

ETraversalBehaviour CodeGenUnit::foreachCodeGenEntityPairInNamespace(ICodeGenerator& codeGenerator, NamespaceInfo const& namespace_, CodeGenEnv& env,
																	 std::function<ETraversalBehaviour(ICodeGenerator&, EntityInfo const&, CodeGenEnv&, void const*)> visitor) noexcept
{
	assert(visitor != nullptr);

	//Execute the visitor function on the current namespace
	ETraversalBehaviour result = codeGenerator.callVisitorOnEntity(namespace_, env, visitor);

	if (result != ETraversalBehaviour::Recurse)
	{
		return result;
	}

	//Iterate and execute the provided visitor function recursively on all nested entities
	for (NamespaceInfo const& nestedNamespace : namespace_.namespaces)
	{
		result = foreachCodeGenEntityPairInNamespace(codeGenerator, nestedNamespace, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (StructClassInfo const& struct_ : namespace_.structs)
	{
		result = foreachCodeGenEntityPairInStruct(codeGenerator, struct_, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (StructClassInfo const& class_ : namespace_.classes)
	{
		result = foreachCodeGenEntityPairInStruct(codeGenerator, class_, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (EnumInfo const& enum_ : namespace_.enums)
	{
		result = foreachCodeGenEntityPairInEnum(codeGenerator, enum_, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (VariableInfo const& variable : namespace_.variables)
	{
		result = codeGenerator.callVisitorOnEntity(variable, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (FunctionInfo const& function : namespace_.functions)
	{
		result = codeGenerator.callVisitorOnEntity(function, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	return ETraversalBehaviour::Recurse;
}

ETraversalBehaviour CodeGenUnit::foreachCodeGenEntityPairInStruct(ICodeGenerator& codeGenerator, StructClassInfo const& struct_, CodeGenEnv& env,
																  std::function<ETraversalBehaviour(ICodeGenerator&, EntityInfo const&, CodeGenEnv&, void const*)> visitor) noexcept
{
	assert(visitor != nullptr);

	//Execute the visitor function on the current struct/class
	ETraversalBehaviour result = codeGenerator.callVisitorOnEntity(struct_, env, visitor);

	if (result != ETraversalBehaviour::Recurse)
	{
		return result;
	}

	//Iterate and execute the provided visitor function recursively on all nested entities
	for (std::shared_ptr<NestedStructClassInfo> const& nestedStruct : struct_.nestedStructs)
	{
		result = foreachCodeGenEntityPairInStruct(codeGenerator, *nestedStruct, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (std::shared_ptr<NestedStructClassInfo> const& nestedClass : struct_.nestedClasses)
	{
		result = foreachCodeGenEntityPairInStruct(codeGenerator, *nestedClass, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (NestedEnumInfo const& nestedEnum : struct_.nestedEnums)
	{
		result = foreachCodeGenEntityPairInEnum(codeGenerator, nestedEnum, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (FieldInfo const& field : struct_.fields)
	{
		result = codeGenerator.callVisitorOnEntity(field, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}

	for (MethodInfo const& method : struct_.methods)
	{
		result = codeGenerator.callVisitorOnEntity(method, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	return ETraversalBehaviour::Recurse;
}

ETraversalBehaviour CodeGenUnit::foreachCodeGenEntityPairInEnum(ICodeGenerator& codeGenerator, EnumInfo const& enum_, CodeGenEnv& env,
																std::function<ETraversalBehaviour(ICodeGenerator&, EntityInfo const&, CodeGenEnv&, void const*)> visitor) noexcept
{
	assert(visitor != nullptr);

	//Execute the visitor function on the current enum
	ETraversalBehaviour result = codeGenerator.callVisitorOnEntity(enum_, env, visitor);

	if (result != ETraversalBehaviour::Recurse)
	{
		return result;
	}

	//Iterate and execute the provided visitor function recursively on all enum values
	for (EnumValueInfo const& enumValue : enum_.enumValues)
	{
		result = codeGenerator.callVisitorOnEntity(enumValue, env, visitor);

		HANDLE_NESTED_ENTITY_ITERATION_RESULT(result);
	}
	
	return ETraversalBehaviour::Recurse;
}

void CodeGenUnit::clearGenerationModules() noexcept
{
	if (_isCopy)
	{
		//Free copied modules
		for (CodeGenModule* codeGenModule : _generationModules)
		{
			delete codeGenModule;
		}
	}

	_generationModules.clear();
}

void CodeGenUnit::addModule(CodeGenModule& generationModule) noexcept
{
	//Add modules sorted by generation order
	_generationModules.emplace_back(&generationModule);
}

bool CodeGenUnit::removeModule(CodeGenModule const& generationModule) noexcept
{
	auto it = std::find(_generationModules.cbegin(), _generationModules.cend(), &generationModule);

	if (it != _generationModules.cend())
	{
		_generationModules.erase(it);

		return true;
	}

	return false;
}

CodeGenUnitSettings const* CodeGenUnit::getSettings() const noexcept
{
	return settings;
}

uint8_t CodeGenUnit::getIterationCount() const noexcept
{
	if (_generationModules.empty())
	{
		return 1u;
	}
	else
	{
		auto it = std::max_element(_generationModules.cbegin(), _generationModules.cend(),
								   [](CodeGenModule* const& lhs, CodeGenModule* const& rhs)
								   {
									   return lhs->getIterationCount() < rhs->getIterationCount();
								   });

		return (*it)->getIterationCount();
	}
}

std::vector<CodeGenModule*>	const& CodeGenUnit::getRegisteredCodeGenModules() const noexcept
{
	return _generationModules;
}

CodeGenUnit& CodeGenUnit::operator=(CodeGenUnit const& other) noexcept
{
	settings = other.settings;
	logger = other.logger;

	//Correctly release memory if the instance is already a copy
	if (_isCopy)
	{
		clearGenerationModules();
	}
	else
	{
		_isCopy = true;
	}

	//At this point, there should be no registered module
	assert(_generationModules.empty());

	//Replace each module by a new clone of themself so that
	//each CodeGenUnit instance owns their own modules
	for (size_t i = 0; i < other._generationModules.size(); i++)
	{
		addModule(*static_cast<CodeGenModule*>(other._generationModules[i]->clone()));
	}

	return *this;
}