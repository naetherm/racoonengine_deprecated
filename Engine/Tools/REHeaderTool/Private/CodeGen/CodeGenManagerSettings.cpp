#include "REHeaderTool/CodeGen/CodeGenManagerSettings.h"

#include "REHeaderTool/Misc/TomlUtility.h"
#include "REHeaderTool/Misc/ILogger.h"

using namespace REHeaderTool;

void CodeGenManagerSettings::sanitizePaths(std::unordered_set<fs::path, PathHash>& set) noexcept
{
	std::unordered_set<fs::path, PathHash>	sanitizedPaths;
	fs::path								sanitizedPath;

	//Modify in 2 for-loops to avoid iterator invalidations by removing/inserting elements
	//Detect all paths that should be sanitized
	for (fs::path const& path : set)
	{
		sanitizedPath = FilesystemHelpers::sanitizePath(path);

		if (!sanitizedPath.empty())
		{
			sanitizedPaths.emplace(sanitizedPath);
		}
		else
		{
			sanitizedPaths.emplace(path);
		}
	}

	set = sanitizedPaths;
}

bool CodeGenManagerSettings::loadSettingsValues(toml::value const& tomlData, ILogger* logger) noexcept
{
	if (tomlData.contains(_tomlSectionName))
	{
		toml::value const& tomlGeneratorSettings = toml::find(tomlData, _tomlSectionName);
		
		loadSupportedFileExtensions(tomlGeneratorSettings, logger);
		loadToProcessFiles(tomlGeneratorSettings, logger);
		loadToProcessDirectories(tomlGeneratorSettings, logger);
		loadIgnoredFiles(tomlGeneratorSettings, logger);
		loadIgnoredDirectories(tomlGeneratorSettings, logger);

		return true;
	}
	else if (logger != nullptr)
	{
		logger->log("Could not find the [" + std::string(_tomlSectionName) + "] section in the TOML file.", ILogger::ELogSeverity::Warning);
	}

	return false;
}

bool CodeGenManagerSettings::addToProcessFile(fs::path const& path) noexcept
{
	bool added = _toProcessFiles.emplace(path).second;

	_toProcessFilesDirtyFlag |= added;

	return added;
}

bool CodeGenManagerSettings::addToProcessDirectory(fs::path const& path) noexcept
{
	bool added = _toProcessDirectories.emplace(path).second;

	_toProcessDirectoriesDirtyFlag |= added;

	return added;
}

bool CodeGenManagerSettings::addIgnoredFile(fs::path const& path) noexcept
{
	bool added = _ignoredFiles.emplace(path).second;

	_ignoredFilesDirtyFlag |= added;

	return added;
}

bool CodeGenManagerSettings::addIgnoredDirectory(fs::path const& path) noexcept
{
	bool added = _ignoredDirectories.emplace(path).second;

	_ignoredDirectoriesDirtyFlag |= added;

	return added;
}

bool CodeGenManagerSettings::addSupportedFileExtension(fs::path const& extension) noexcept
{
	std::string extensionAsString = extension.string();

	if (!extensionAsString.empty() && extensionAsString[0] == '.' && extension.has_stem())
	{
		_supportedFileExtensions.emplace(std::move(extensionAsString));

		return true;
	}

	return false;
}

void CodeGenManagerSettings::removeToProcessFile(fs::path const& path) noexcept
{
	_toProcessFiles.erase(FilesystemHelpers::sanitizePath(path));
}

void CodeGenManagerSettings::removeToProcessDirectory(fs::path const& path) noexcept
{
	_toProcessDirectories.erase(FilesystemHelpers::sanitizePath(path));
}

void CodeGenManagerSettings::removeIgnoredFile(fs::path const& path) noexcept
{
	_ignoredFiles.erase(FilesystemHelpers::sanitizePath(path));
}

void CodeGenManagerSettings::removeSupportedFileExtension(fs::path const& ext) noexcept
{
	_supportedFileExtensions.erase(ext.string());
}

void CodeGenManagerSettings::removeIgnoredDirectory(fs::path const& path) noexcept
{
	_ignoredDirectories.erase(FilesystemHelpers::sanitizePath(path));
}

void CodeGenManagerSettings::clearToProcessFiles() noexcept
{
	_toProcessFiles.clear();
}

void CodeGenManagerSettings::clearToProcessDirectories() noexcept
{
	_toProcessDirectories.clear();
}

void CodeGenManagerSettings::clearIgnoredFiles() noexcept
{
	_ignoredFiles.clear();
}

void CodeGenManagerSettings::clearIgnoredDirectories() noexcept
{
	_ignoredDirectories.clear();
}

void CodeGenManagerSettings::clearSupportedFileExtensions() noexcept
{
	_supportedFileExtensions.clear();
}

bool CodeGenManagerSettings::isSupportedFileExtension(fs::path const& extension) const noexcept
{
	return _supportedFileExtensions.find(extension.string()) != _supportedFileExtensions.end();
}

bool CodeGenManagerSettings::isIgnoredFile(fs::path const& file) noexcept
{
	if (_ignoredFilesDirtyFlag)
	{
		sanitizePaths(_ignoredFiles);
		_ignoredFilesDirtyFlag = false;
	}

	return _ignoredFiles.find(fs::exists(file) ? FilesystemHelpers::sanitizePath(file) : file) != _ignoredFiles.end();
}

bool CodeGenManagerSettings::isIgnoredDirectory(fs::path const& directory) noexcept
{
	if (_ignoredDirectoriesDirtyFlag)
	{
		sanitizePaths(_ignoredDirectories);
		_ignoredDirectoriesDirtyFlag = false;
	}

	return _ignoredDirectories.find(fs::exists(directory) ? FilesystemHelpers::sanitizePath(directory) : directory) != _ignoredDirectories.end();
}

void CodeGenManagerSettings::loadSupportedFileExtensions(toml::value const& generationSettings, ILogger* logger) noexcept
{
	//Clear supported extensions before loading
	_supportedFileExtensions.clear();

	std::unordered_set<std::string> loadedExtensions;
	if (TomlUtility::updateSetting(generationSettings, "supportedFileExtensions", loadedExtensions, logger) && logger != nullptr)
	{
		for (std::string const& extension : loadedExtensions)
		{
			if (addSupportedFileExtension(extension))
			{
				if (logger != nullptr)
				{
					logger->log("[TOML] Load new supported file extension: " + extension);
				}
			}
			else if (logger != nullptr)
			{
				if (logger != nullptr)
				{
					logger->log("[TOML] Failed to add supported extension: " + extension);
				}
			}
		}
	}
}

void CodeGenManagerSettings::loadToProcessFiles(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::unordered_set<fs::path, PathHash> toParseFiles;

	clearToProcessFiles();

	if (TomlUtility::updateSetting(generationSettings, "toProcessFiles", toParseFiles, logger))
	{
		bool success;

		for (fs::path const& path : toParseFiles)
		{
			success = addToProcessFile(path);

			if (logger != nullptr)
			{
				if (success)
				{
					logger->log("[TOML] Load new file to parse: " + FilesystemHelpers::sanitizePath(path).string());
				}
				else
				{
					logger->log("[TOML] Failed to add toParseFile as it doesn't exist, is not a file or is already part of the list of files to parse: " + path.string(), ILogger::ELogSeverity::Warning);
				}
			}
		}
	}
}

void CodeGenManagerSettings::loadToProcessDirectories(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::unordered_set<fs::path, PathHash> toParseDirectories;

	clearToProcessDirectories();

	if (TomlUtility::updateSetting(generationSettings, "toProcessDirectories", toParseDirectories, logger))
	{
		bool success;

		for (fs::path const& path : toParseDirectories)
		{
			success = addToProcessDirectory(path);

			if (logger != nullptr)
			{
				if (success)
				{
					logger->log("[TOML] Load new directory to parse: " + FilesystemHelpers::sanitizePath(path).string());
				}
				else
				{
					logger->log("[TOML] Failed to add toParseDirectory as it doesn't exist, is not a directory or is already part of the list of directories to parse: " + path.string(), ILogger::ELogSeverity::Warning);
				}
			}
		}
	}
}

void CodeGenManagerSettings::loadIgnoredFiles(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::unordered_set<fs::path, PathHash> ignoredFiles;

	clearIgnoredFiles();

	if (TomlUtility::updateSetting(generationSettings, "ignoredFiles", ignoredFiles, logger))
	{
		bool success;

		for (fs::path const& path : ignoredFiles)
		{
			success = addIgnoredFile(path);

			if (logger != nullptr)
			{
				if (success)
				{
					logger->log("[TOML] Load new ignored file: " + FilesystemHelpers::sanitizePath(path).string());
				}
				else
				{
					logger->log("[TOML] Failed to add ignoredFile as it doesn't exist, is not a file or is already part of the list of ignored files: " + path.string(), ILogger::ELogSeverity::Warning);
				}
			}
		}
	}
}

void CodeGenManagerSettings::loadIgnoredDirectories(toml::value const& generationSettings, ILogger* logger) noexcept
{
	std::unordered_set<fs::path, PathHash> ignoredDirectories;

	clearIgnoredDirectories();

	if (TomlUtility::updateSetting(generationSettings, "ignoredDirectories", ignoredDirectories, logger))
	{
		bool success;

		for (fs::path const& path : ignoredDirectories)
		{
			success = addIgnoredDirectory(path);

			if (logger != nullptr)
			{
				if (success)
				{
					logger->log("[TOML] Load new ignored directory: " + FilesystemHelpers::sanitizePath(path).string());
				}
				else
				{
					logger->log("[TOML] Failed to add ignoredDirectory as it doesn't exist, is not a directory or is already part of the list of ignored directories: " + path.string(), ILogger::ELogSeverity::Warning);
				}
			}
		}
	}
}

std::unordered_set<fs::path, PathHash> const& CodeGenManagerSettings::getToProcessFiles() const noexcept
{
	return _toProcessFiles;
}

std::unordered_set<fs::path, PathHash> const& CodeGenManagerSettings::getToProcessDirectories() const noexcept
{
	return _toProcessDirectories;
}

std::unordered_set<fs::path, PathHash> const& CodeGenManagerSettings::getIgnoredFiles() const noexcept
{
	return _ignoredFiles;
}

std::unordered_set<fs::path, PathHash> const& CodeGenManagerSettings::getIgnoredDirectories() const noexcept
{
	return _ignoredDirectories;
}

std::unordered_set<std::string> const& CodeGenManagerSettings::getSupportedExtensions() const noexcept
{
	return _supportedFileExtensions;
}