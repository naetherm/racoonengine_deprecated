#include "REHeaderTool/CodeGen/GeneratedFile.h"

using namespace REHeaderTool;

GeneratedFile::GeneratedFile(fs::path&& generatedFilePath, fs::path const& sourceFilePath) noexcept:
	_path{std::forward<fs::path>(generatedFilePath)},
	_sourceFilePath{sourceFilePath},
	_streamToFile(_path.string(), std::ios::out | std::ios::trunc)
{
}

GeneratedFile::~GeneratedFile() noexcept
{
	_streamToFile.close();
}

void GeneratedFile::writeLine(std::string const& line) noexcept
{
	_streamToFile << line << "\n";
}

void GeneratedFile::writeLine(std::string&& line) noexcept
{
	_streamToFile << std::forward<std::string>(line) << "\n";
}

void GeneratedFile::writeLines(std::string const& line) noexcept
{
	writeLine(line);
}

void GeneratedFile::writeLines(std::string&& line) noexcept
{
	writeLine(std::forward<std::string>(line));
}

void GeneratedFile::expandWriteMacroLines(std::string const& line) noexcept
{
	writeLine(line + "\n");
}

void GeneratedFile::expandWriteMacroLines(std::string&& line) noexcept
{
	writeLine(std::forward<std::string>(line) + "\n");
}

void GeneratedFile::writeMacro(std::string&& macroName) noexcept
{
	writeLine("#define " + std::forward<std::string>(macroName));
}

void GeneratedFile::undefMacro(std::string const& macroName) noexcept
{
	writeLine("#ifdef " + macroName + "\n\t#undef " + macroName + "\n#endif");
}

fs::path const& GeneratedFile::getPath() const noexcept
{
	return _path;
}

fs::path const& GeneratedFile::getSourceFilePath() const noexcept
{
	return _sourceFilePath;
}