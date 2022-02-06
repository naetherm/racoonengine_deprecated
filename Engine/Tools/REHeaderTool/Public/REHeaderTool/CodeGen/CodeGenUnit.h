/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <vector>
#include <functional>	//std::function

#include "REHeaderTool/Parsing/ParsingResults/FileParsingResult.h"
#include "REHeaderTool/CodeGen/ETraversalBehaviour.h"
#include "REHeaderTool/CodeGen/CodeGenEnv.h"
#include "REHeaderTool/CodeGen/CodeGenUnitSettings.h"
#include "REHeaderTool/CodeGen/CodeGenModule.h"
#include "REHeaderTool/Misc/ILogger.h"
#include "REHeaderTool/Misc/Filesystem.h"

namespace REHeaderTool
{
	class CodeGenUnit
	{
		private:
			/** Collection of all registered generation modules. */
			std::vector<CodeGenModule*>	_generationModules;

			/** Keep track of either this CodeGenUnit instance was constructed from the copy constructor
			*	or the copy assignement operator.
			*/
			bool						_isCopy	= false;

			/**
			*	@brief Insert a code generator to a sorted vector ordered by generation order.
			* 
			*	@param vector	The collection to insert to.
			*	@param codeGen	The code generator to insert in the vector.
			*/
			static void					sortedInsert(std::vector<ICodeGenerator*>&	vector,
													 ICodeGenerator&				codeGen)													noexcept;

			/**
			*	@brief	Delete all the registered generation modules. If they have been dynamically instantiated,
			*			memory is released correctly.
			*/
			void						clearGenerationModules()																				noexcept;

			/**
			*	@brief Iterate and execute recursively a visitor function on each parsed entity/registered module pair.
			* 
			*	@param visitor	Visitor function to execute on all traversed entities.
			*	@param env		Generation environment structure.
			* 
			*	@return ETraversalBehaviour::Recurse if the traversal completed successfully.
			*			ETraversalBehaviour::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			ETraversalBehaviour::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			ETraversalBehaviour			foreachCodeGenEntityPair(std::function<ETraversalBehaviour(ICodeGenerator&,
																								   EntityInfo const&,
																								   CodeGenEnv&,
																								   void const*)>		visitor,
																 CodeGenEnv&											env)					noexcept;

			/**
			*	@brief	Iterate and execute recursively a visitor function on a namespace and
			*			all its nested entities/registered module pair.
			* 
			*	@param namespace_	Namespace to iterate on.
			*	@param visitor		Visitor function to execute on all traversed entities.
			*	@param env			Generation environment structure.
			* 
			*	@return ETraversalBehaviour::Recurse if the traversal completed successfully.
			*			ETraversalBehaviour::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			ETraversalBehaviour::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			ETraversalBehaviour			foreachCodeGenEntityPairInNamespace(ICodeGenerator&										codeGenerator,
																			NamespaceInfo const&								namespace_,
																			CodeGenEnv&											env,
																			std::function<ETraversalBehaviour(ICodeGenerator&,
																											  EntityInfo const&,
																											  CodeGenEnv&,
																											  void const*)>		visitor)		noexcept;

			/**
			*	@brief	Iterate and execute recursively a visitor function on a struct or class and
			*			all its nested entities/registered module pair.
			* 
			*	@param struct_	Struct/class to iterate on.
			*	@param visitor	Visitor function to execute on all traversed entities.
			*	@param env		Generation environment structure.
			* 
			*	@return ETraversalBehaviour::Recurse if the traversal completed successfully.
			*			ETraversalBehaviour::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			ETraversalBehaviour::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			ETraversalBehaviour			foreachCodeGenEntityPairInStruct(ICodeGenerator&										codeGenerator,
																		 StructClassInfo const&									struct_,
																		 CodeGenEnv&											env,
																		 std::function<ETraversalBehaviour(ICodeGenerator&,
																		 								   EntityInfo const&,
																		 								   CodeGenEnv&,
																		 								   void const*)>		visitor)		noexcept;

			/**
			*	@brief Iterate and execute recursively a visitor function on an enum and all its nested entities.
			* 
			*	@param enum_	Enum to iterate on.
			*	@param visitor	Visitor function to execute on all traversed entities.
			*	@param env		Generation environment structure.
			* 
			*	@return ETraversalBehaviour::Recurse if the traversal completed successfully.
			*			ETraversalBehaviour::AbortWithSuccess if the traversal was aborted prematurely without error.
			*			ETraversalBehaviour::AbortWithFailure if the traversal was aborted prematurely with an error.
			*/
			ETraversalBehaviour			foreachCodeGenEntityPairInEnum(ICodeGenerator&										codeGenerator,
																	   EnumInfo const&										enum_,
																	   CodeGenEnv&											env,
																	   std::function<ETraversalBehaviour(ICodeGenerator&,
																										 EntityInfo const&,
																										 CodeGenEnv&,
																										 void const*)>		visitor)			noexcept;

			/**
			*	@brief Call ICodeGenerator::initialGenerateCode on all provided code generators.
			* 
			*	@param codeGenerators	List of code generators that should call ICodeGenerator::initialGenerateCode.
			*	@param env				The environment structure.
			* 
			*	@return A combined value of all the ICodeGenerator::initialGeneratorCode calls.
			*/
			bool					initialGenerateCodeInternal(std::vector<ICodeGenerator*> const&	codeGenerators,
																CodeGenEnv&							env)										noexcept;

			/**
			*	@brief Call ICodeGenerator::finalGenerateCode on all provided code generators.
			* 
			*	@param codeGenerators	List of code generators that should call ICodeGenerator::finalGenerateCode.
			*	@param env				The environment structure.
			* 
			*	@return A combined value of all the ICodeGenerator::finalGeneratorCode calls.
			*/
			bool					finalGenerateCodeInternal(std::vector<ICodeGenerator*> const&	codeGenerators,
															  CodeGenEnv&							env)										noexcept;

			/**
			*	@brief	Method called on each entity when CodeGenUnit::generateCode is called.
			* 
			*	@param codeGenerator	The code generator to run for the entity.
			*	@param entity			The entity for which the generate generates code.
			*	@param env				The environment structure.
			*	@param data				Opaque data forwarded to the codeGenerator.generateCode call.
			* 
			*	@return A combined value of all the codeGenerator.generateCode calls.
			*/
			ETraversalBehaviour		generateCodeForEntityInternal(ICodeGenerator&	codeGenerator,
																  EntityInfo const&	entity,
																  CodeGenEnv&		env,
																  void const*		data)														noexcept;

		protected:
			/** Settings used for code generation. */
			CodeGenUnitSettings const*	settings = nullptr;

			/**
			*	@brief	Execute the codeGenModule->generateCode method with the given entity and environment.
			*			The method is made virtual pure to let the implementation control in which string the generated code should be appended.
			*			The implementation is also free to run the generate method multiple times.
			*
			*	@param entity			Target entity for this code generation pass.
			*	@param env				Generation environment structure.
			*/
			virtual void					generateCodeForEntity(EntityInfo const&						entity,
																  CodeGenEnv&							env,
																  std::function<void(EntityInfo const&,
																					 CodeGenEnv&,
																					 std::string&)>		generate)	noexcept	= 0;

			/**
			*	@brief	Execute the codeGenModule->initialGenerateCode method with the given environment.
			*			The method is made virtual pure to let the implementation control in which string the generated code should be appended.
			*			The implementation is also free to run the generate method multiple times.
			* 
			*	@param codeGenModule	Module calling the generateCode method.
			*	@param env				Generation environment structure.
			*/
			virtual void					initialGenerateCode(CodeGenEnv&							env,
																std::function<void(CodeGenEnv&,
																				   std::string&)>	generate)		noexcept	= 0;

			/**
			*	@brief	Execute the codeGenModule->initialGenerateCode method with the given environment.
			*			The method is made virtual pure to let the implementation control in which string the generated code should be appended.
			*			The implementation is also free to run the generate method multiple times.
			* 
			*	@param codeGenModule	Module calling the generateCode method.
			*	@param env				Generation environment structure.
			*/
			virtual void					finalGenerateCode(CodeGenEnv&						env,
															  std::function<void(CodeGenEnv&,
																				 std::string&)>	generate)			noexcept	= 0;

			/**
			*	@brief	Instantiate a CodeGenEnv object (using new).
			*			This method can be overriden to instantiate a child class of CodeGenEnv.
			* 
			*	@return A dynamically instantiated (new) CodeGenEnv object used during the whole generation process.
			*/
			virtual CodeGenEnv*				createCodeGenEnv()												const	noexcept;

			/**
			*	@brief	Called just before CodeGenUnit::foreachModuleEntityPair.
			*			Perform all registered modules initialization and initialize CodeGenEnv fields (logger, FileParsingResult...).
			*			The whole generation process is aborted if the method returns false.
			*			/!\ Overrides MUST call this base implementation as well through CodeGenUnit::preGenerateCode(parsingResult, env) /!\
			* 
			*	@param parsingResult	Result of a file parsing used to generate code.
			*	@param env				Generation environment structure.
			* 
			*	@return true if the method completed successfully, else false.
			*/
			virtual bool					preGenerateCode(FileParsingResult const&	parsingResult,
															CodeGenEnv&					env)						noexcept;

			/**
			*	@brief	Called just after CodeGenUnit::foreachModuleEntityPair.
			*			Can be used to perform any post-generation tasks or cleanup.
			*			If the unit needs to write the generated code in files, this is typically here
			*			that files are created and written to.
			*
			*	@param env Generation environment structure after the preGenerateCode and generateCodeInternal have run.
			* 
			*	@return true if the method completed successfully, else false.
			*/
			virtual bool					postGenerateCode(CodeGenEnv& env)										noexcept;

			/**
			*	@brief Check if file last write time is newer than reference file last write time.
			*			The method will assert if a path is invalid or is not a file.
			* 
			*	@param file				Path to the file to compare.
			*	@param referenceFile	Path to the reference file to compare.
			* 
			*	@return true if file last write time is newer than referenceFile's, else false.
			*/
			bool							isFileNewerThan(fs::path const& file,
															fs::path const& referenceFile)					const	noexcept;

			/**
			*	@brief Compute the list of all generators nested in this CodeGenUnit sorted by ascending generation order.
			* 
			*	@return The list of sorted code generators.
			*/
			std::vector<ICodeGenerator*>	getSortedCodeGenerators()										const	noexcept;

		public:
			/** Logger used to issue logs from this CodeGenUnit. */
			ILogger*	logger	= nullptr;

			CodeGenUnit()					= default;
			CodeGenUnit(CodeGenUnit const&)	noexcept;
			CodeGenUnit(CodeGenUnit&&)		= default;
			virtual ~CodeGenUnit()			noexcept;

			/**
			*	@brief Check whether the generated code for a given source file is up-to-date or not.
			* 
			*	@param sourceFile Path to the source file.
			*
			*	@return true if the code generated for sourceFile is up-to-date, else false.
			*/
			virtual bool				isUpToDate(fs::path const& sourceFile)			const	noexcept = 0;

			/**
			*	@brief	Check whether all settings are setup correctly for this unit to work.
			*			If output directory path is valid but doesn't exist yet, it is created.
			*			This method is internally called by FileGenerator::generateFiles.
			* 
			*	@return	true if all settings are valid, else false.
			*			Note that the method will return false if the output directory failed to be created (only if it didn't exist).
			*/
			virtual bool				checkSettings()									const	noexcept;

			/**
			*	@brief	Calls preGenerateCode, foreachModuleEntityPair, and postGenerateCode in that order.
			*			If any of the previously mentioned method returns false, the generation aborts (next methods
			*			will not be called).
			*
			*			ex: If preGenerateCode returns false, both foreachModuleEntityPair and postGenerateCode calls will be skipped.
			*			
			*	@param parsingResult	Result of a file parsing used to generate code.
			* 
			*	@return true if preGenerateCode, foreachModuleEntityPair and postGenerateCode calls have succeeded, else false.
			*/
			bool						generateCode(FileParsingResult const& parsingResult)	noexcept;

			/**
			*	@brief Add a module to the internal list of generation modules.
			* 
			*	@param generationModule The generation module to add.
			*/
			void						addModule(CodeGenModule& generationModule)				noexcept;

			/**
			*	@brief Remove a module from the internal list of generation modules.
			* 
			*	@param generationModule The generation module to remove.
			* 
			*	@return true if a module has been successfully removed, else false.
			*/
			bool						removeModule(CodeGenModule const& generationModule)		noexcept;
			
			/**
			*	@brief Getter for settings field.
			* 
			*	@return settings.
			*/
			CodeGenUnitSettings const*			getSettings()							const	noexcept;

			/**
			*	@brief	Get the highest iteration count between all registered modules.
			*/
			uint8_t								getIterationCount()						const	noexcept;

			/**
			*	@brief Getter for _generationModules field.
			* 
			*	@return _generationModules.
			*/
			std::vector<CodeGenModule*>	const&	getRegisteredCodeGenModules()			const	noexcept;

			CodeGenUnit&	operator=(CodeGenUnit const&)	noexcept;
			CodeGenUnit&	operator=(CodeGenUnit&&)		= default;
	};
}