#include "REHeaderTool/Parsing/FileParser.h"

#include <cassert>

#include "REHeaderTool/Misc/Helpers.h"
#include "REHeaderTool/Misc/DisableWarningMacros.h"
#include "REHeaderTool/Misc/TomlUtility.h"

using namespace REHeaderTool;

FileParser::FileParser() noexcept:
	_clangIndex{clang_createIndex(0, 0)},
	_settings{std::make_shared<ParsingSettings>()},
	logger{nullptr}
{
}

FileParser::FileParser(FileParser const& other) noexcept:
	NamespaceParser(other),
	_clangIndex{clang_createIndex(0, 0)},	//Don't copy clang index, create a new one
	_settings{other._settings},
	logger{other.logger}
{
}

FileParser::FileParser(FileParser&& other) noexcept:
	NamespaceParser(std::forward<NamespaceParser>(other)),
	_clangIndex{std::forward<CXIndex>(other._clangIndex)},
	_propertyParser(std::forward<PropertyParser>(other._propertyParser)),
	_settings{other._settings},
	logger{other.logger}
{
	other._clangIndex = nullptr;
}

FileParser::~FileParser() noexcept
{
	if (_clangIndex != nullptr)
	{
		clang_disposeIndex(_clangIndex);
	}
}

bool FileParser::parse(fs::path const& toParseFile, FileParsingResult& out_result) noexcept
{
	assert(_settings.use_count() != 0);

	bool isSuccess = false;

	preParse(toParseFile);

	if (fs::exists(toParseFile) && !fs::is_directory(toParseFile))
	{
		//Fill the parsed file info
		out_result.parsedFile = FilesystemHelpers::sanitizePath(toParseFile);

		//Parse the given file
		CXTranslationUnit translationUnit = clang_parseTranslationUnit(_clangIndex, toParseFile.string().c_str(), _settings->getCompilationArguments().data(), static_cast<int32_t>(_settings->getCompilationArguments().size()), nullptr, 0, CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_Incomplete | CXTranslationUnit_KeepGoing);

		if (translationUnit != nullptr)
		{
			ParsingContext& context = pushContext(translationUnit, out_result);

			if (clang_visitChildren(context.rootCursor, &FileParser::parseNestedEntity, this) || !out_result.errors.empty())
			{
				//ERROR
			}
			else
			{
				//Refresh all outer entities contained in the final result
				refreshOuterEntity(out_result);

				isSuccess = true;
			}

			popContext();

			//There should not have any context left once parsing has finished
			assert(contextsStack.empty());

			if (_settings->shouldLogDiagnostic)
			{
				logDiagnostic(translationUnit);
			}

			clang_disposeTranslationUnit(translationUnit);
		}
		else
		{
			out_result.errors.emplace_back("Failed to initialize translation unit for file: " + toParseFile.string());
		}
	}
	else
	{
		out_result.errors.emplace_back("File " + toParseFile.string() + " doesn't exist.");
	}

	postParse(toParseFile, out_result);

	return isSuccess;
}

CXChildVisitResult FileParser::parseNestedEntity(CXCursor cursor, CXCursor /* parentCursor */, CXClientData clientData) noexcept
{
	FileParser*	parser	= reinterpret_cast<FileParser*>(clientData);

	DISABLE_WARNING_PUSH
	DISABLE_WARNING_UNSCOPED_ENUM
	
	CXChildVisitResult	visitResult = CXChildVisitResult::CXChildVisit_Continue;

	DISABLE_WARNING_POP

	//Parse the given file ONLY, ignore headers
	if (clang_Location_isFromMainFile(clang_getCursorLocation(cursor)))
	{
		switch (cursor.kind)
		{
			case CXCursorKind::CXCursor_Namespace:
				parser->addNamespaceResult(parser->parseNamespace(cursor, visitResult));
				break;

			case CXCursorKind::CXCursor_StructDecl:
				[[fallthrough]];
			case CXCursorKind::CXCursor_ClassDecl:
				parser->addClassResult(parser->parseClass(cursor, visitResult));
				break;

			case CXCursorKind::CXCursor_ClassTemplate:
				parser->addClassResult(parser->parseClass(cursor, visitResult));
				break;

			case CXCursorKind::CXCursor_EnumDecl:
				parser->addEnumResult(parser->parseEnum(cursor, visitResult));
				break;

			case CXCursorKind::CXCursor_FunctionDecl:
				parser->addFunctionResult(parser->parseFunction(cursor, visitResult));
				break;

			case CXCursorKind::CXCursor_VarDecl:
				parser->addVariableResult(parser->parseVariable(cursor, visitResult));
				break;

			default:
				break;
		}
	}

	return visitResult;
}

ParsingContext& FileParser::pushContext(CXTranslationUnit const& translationUnit, FileParsingResult& out_result) noexcept
{
	_propertyParser.setup(_settings->propertyParsingSettings);

	ParsingContext newContext;

	newContext.parentContext	= nullptr;
	newContext.rootCursor		= clang_getTranslationUnitCursor(translationUnit);
	newContext.propertyParser	= &_propertyParser;
	newContext.parsingSettings	= _settings.get();
	newContext.structClassTree	= &out_result.structClassTree;
	newContext.parsingResult	= &out_result;

	contextsStack.push(std::move(newContext));

	return getContext();
}

void FileParser::addNamespaceResult(NamespaceParsingResult&& result) noexcept
{
	if (result.parsedNamespace.has_value())
	{
		getParsingResult()->namespaces.emplace_back(std::move(result.parsedNamespace).value());
	}

	getParsingResult()->appendResultErrors(result);
}

void FileParser::addClassResult(ClassParsingResult&& result) noexcept
{
	if (result.parsedClass.has_value())
	{
		switch (result.parsedClass->entityType)
		{
			case EEntityType::Struct:
				getParsingResult()->structs.emplace_back(std::move(result.parsedClass).value());
				break;

			case EEntityType::Class:
				getParsingResult()->classes.emplace_back(std::move(result.parsedClass).value());
				break;

			default:
				assert(false);	//Should never reach this line
				break;
		}
	}

	getParsingResult()->appendResultErrors(result);
}

void FileParser::addEnumResult(EnumParsingResult&& result) noexcept
{
	if (result.parsedEnum.has_value())
	{
		getParsingResult()->enums.emplace_back(std::move(result.parsedEnum).value());
	}

	getParsingResult()->appendResultErrors(result);
}

void FileParser::addVariableResult(VariableParsingResult&& result) noexcept
{
	if (result.parsedVariable.has_value())
	{
		getParsingResult()->variables.emplace_back(std::move(result.parsedVariable).value());
	}

	getParsingResult()->appendResultErrors(result);
}

void FileParser::addFunctionResult(FunctionParsingResult&& result) noexcept
{
	if (result.parsedFunction.has_value())
	{
		getParsingResult()->functions.emplace_back(std::move(result.parsedFunction).value());
	}

	getParsingResult()->appendResultErrors(result);
}

void FileParser::refreshOuterEntity(FileParsingResult& out_result) const noexcept
{
	for (NamespaceInfo& namespaceInfo : out_result.namespaces)
	{
		namespaceInfo.refreshOuterEntity();
	}

	for (StructClassInfo& structInfo : out_result.structs)
	{
		structInfo.refreshOuterEntity();
	}

	for (StructClassInfo& classInfo : out_result.classes)
	{
		classInfo.refreshOuterEntity();
	}

	for (EnumInfo& enumInfo : out_result.enums)
	{
		enumInfo.refreshOuterEntity();
	}
}

void FileParser::preParse(fs::path const& parseFile) noexcept
{
	/**
	*	Default implementation does nothing special
	*/
  if (logger != nullptr)
  {
    logger->log("Start parsing: " + parseFile.string(), ILogger::ELogSeverity::Info);
  }
}

void FileParser::postParse(fs::path const& parseFile, REHeaderTool::FileParsingResult const& result) noexcept
{
	/**
	*	Default implementation does nothing special
	*/
  if (logger != nullptr)
  {
    for (REHeaderTool::ParsingError const& parsingError : result.errors)
    {
      logger->log(parsingError.toString(), REHeaderTool::ILogger::ELogSeverity::Error);
    }

    logger->log(parseFile.string() + ": Found " + std::to_string(result.namespaces.size()) + " namespace(s), " +
                std::to_string(result.structs.size()) + " struct(s), " +
                std::to_string(result.classes.size()) + " classe(s) and " +
                std::to_string(result.enums.size()) + " enum(s).", REHeaderTool::ILogger::ELogSeverity::Info);
  }
}

bool FileParser::logDiagnostic(CXTranslationUnit const& translationUnit) const noexcept
{
	if (logger != nullptr)
	{
		CXDiagnosticSet diagnostics = clang_getDiagnosticSetFromTU(translationUnit);

		unsigned int diagnosticsCount = clang_getNumDiagnosticsInSet(diagnostics);

		//Log only if there is at least 1 diagnostic entry
		if (diagnosticsCount > 0)
		{
			logger->log("Start diagnostic...", ILogger::ELogSeverity::Info);

			for (unsigned i = 0u; i < diagnosticsCount; i++)
			{
				CXDiagnostic diagnostic(clang_getDiagnosticInSet(diagnostics, i));

				logger->log(Helpers::getString(clang_formatDiagnostic(diagnostic, clang_defaultDiagnosticDisplayOptions())), ILogger::ELogSeverity::Warning);

				clang_disposeDiagnostic(diagnostic);
			}

			logger->log("End diagnostic...", ILogger::ELogSeverity::Info);
		}

		clang_disposeDiagnosticSet(diagnostics);

		return true;
	}
	else
	{
		return false;
	}
}