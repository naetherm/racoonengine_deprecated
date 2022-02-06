/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <REHeaderTool/CodeGen/Macro/MacroPropertyCodeGen.h>

namespace REHeaderTool
{
	class PropertySettingsPropertyCodeGen : public REHeaderTool::MacroPropertyCodeGen
	{
		protected:
			virtual bool initialGenerateHeaderFileHeaderCode(REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept override
			{
				inout_result += "#include <RECore/Reflect/TypeInfo/Entity/EEntityKind.h>" + env.getSeparator();

				return true;
			}

			virtual bool generateClassFooterCodeForEntity(REHeaderTool::EntityInfo const&	entity, REHeaderTool::Property const& property, uint8_t /*propertyIndex*/,
														  REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept override
			{
				if (property.arguments.empty())
				{
					if (env.getLogger() != nullptr)
					{
						env.getLogger()->log("The PropertySettings class first constructor parameter is mandatory: " + entity.getFullName(), REHeaderTool::ILogger::ELogSeverity::Error);
					}

					return false;
				}

				//Generate constexpr fields + get overrides
				//generate targetEntityKind
				inout_result += "public: static constexpr RECore::EEntityKind targetEntityKind = " + property.arguments[0] + ";" + env.getSeparator();
				inout_result += env.getExportSymbolMacro() + " virtual RECore::EEntityKind getTargetEntityKind() const noexcept override;" + env.getSeparator();

				if (property.arguments.size() >= 2)
				{
					//generate allowMultiple
					inout_result += "static constexpr bool allowMultiple = " + property.arguments[1] + ";" + env.getSeparator();
					inout_result += env.getExportSymbolMacro() + " virtual bool getAllowMultiple() const noexcept override;" + env.getSeparator();

					if (property.arguments.size() >= 3)
					{
						//generate shouldInherit
						inout_result += "static constexpr bool shouldInherit = " + property.arguments[2] + ";" + env.getSeparator();
						inout_result += env.getExportSymbolMacro() + " virtual bool getShouldInherit() const noexcept override;" + env.getSeparator();
					}
				}

				return true;
			}

			virtual bool initialGenerateSourceFileHeaderCode(REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept override
			{
				inout_result += "#include <type_traits>" + env.getSeparator(); //std::is_base_of

				return true;
			}

			virtual bool generateSourceFileHeaderCodeForEntity(REHeaderTool::EntityInfo const& entity, REHeaderTool::Property const& property, uint8_t /*propertyIndex*/,
															   REHeaderTool::MacroCodeGenEnv& env, std::string& inout_result) noexcept override
			{
				//Define getTargetEntityKind override
				inout_result += "RECore::EEntityKind " + entity.getFullName() + "::getTargetEntityKind() const noexcept { return targetEntityKind; }" + env.getSeparator();

				if (property.arguments.size() >= 2)
				{
					inout_result += "bool " + entity.getFullName() + "::getAllowMultiple() const noexcept { return allowMultiple; }" + env.getSeparator();

					if (property.arguments.size() >= 3)
					{
						inout_result += "bool " + entity.getFullName() + "::getShouldInherit() const noexcept { return shouldInherit; }" + env.getSeparator();
					}
				}

				//Static assert to make sure the property is attached to a class inheriting from REHeaderTool::Property.
				inout_result += "static_assert(std::is_base_of_v<RECore::Property, " + entity.getFullName() + ">, \"[Refureku] Can't attach " + property.name + " property to " + entity.getFullName() + " as it doesn't inherit from RECore::Property.\");" + env.getSeparator();

				return true;
			}

		public:
			PropertySettingsPropertyCodeGen() noexcept:
				REHeaderTool::MacroPropertyCodeGen("RECore::PropertySettings", REHeaderTool::EEntityType::Class | REHeaderTool::EEntityType::Struct)
			{
			}
	};
}