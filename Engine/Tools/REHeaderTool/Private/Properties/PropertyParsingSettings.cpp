#include "REHeaderTool/Properties/PropertyParsingSettings.h"

#include "REHeaderTool/Misc/TomlUtility.h"
#include "REHeaderTool/Misc/ILogger.h"

using namespace REHeaderTool;

bool PropertyParsingSettings::loadSettingsValues(toml::value const& tomlData, ILogger* logger) noexcept
{
	TomlUtility::updateSetting(tomlData, "propertySeparator", propertySeparator, logger);
	TomlUtility::updateSetting(tomlData, "argumentSeparator", argumentSeparator, logger);
	TomlUtility::updateSetting(tomlData, "argumentStartEncloser", argumentEnclosers[0], logger);
	TomlUtility::updateSetting(tomlData, "argumentEndEncloser", argumentEnclosers[1], logger);

	TomlUtility::updateSetting(tomlData, "namespaceMacroName", namespaceMacroName, logger);
	TomlUtility::updateSetting(tomlData, "classMacroName", classMacroName, logger);
	TomlUtility::updateSetting(tomlData, "structMacroName", structMacroName, logger);
	TomlUtility::updateSetting(tomlData, "variableMacroName", variableMacroName, logger);
	TomlUtility::updateSetting(tomlData, "fieldMacroName", fieldMacroName, logger);
	TomlUtility::updateSetting(tomlData, "functionMacroName", functionMacroName, logger);
	TomlUtility::updateSetting(tomlData, "methodMacroName", methodMacroName, logger);
	TomlUtility::updateSetting(tomlData, "enumMacroName", enumMacroName, logger);
	TomlUtility::updateSetting(tomlData, "enumValueMacroName", enumValueMacroName, logger);

	return true;
}