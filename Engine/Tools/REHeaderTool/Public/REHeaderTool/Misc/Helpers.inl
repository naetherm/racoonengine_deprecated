/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#include <cassert>

namespace REHeaderTool {

constexpr uint16_t Helpers::convertToEntityKind(REHeaderTool::EEntityType entityType) noexcept
{
  switch (entityType)
  {
    case REHeaderTool::EEntityType::Namespace:
      return 1 << 0;

    case REHeaderTool::EEntityType::Class:
      return 1 << 1;

    case REHeaderTool::EEntityType::Struct:
      return 1 << 2;

    case REHeaderTool::EEntityType::Enum:
      return 1 << 3;

    case REHeaderTool::EEntityType::Variable:
      return 1 << 5;

    case REHeaderTool::EEntityType::Field:
      return 1 << 6;

    case REHeaderTool::EEntityType::Function:
      return 1 << 7;

    case REHeaderTool::EEntityType::Method:
      return 1 << 8;

    case REHeaderTool::EEntityType::EnumValue:
      return 1 << 9;

    case REHeaderTool::EEntityType::Undefined:
      [[fallthrough]];
    default:
      assert(false);
  }

  return 0u;
}

inline std::string Helpers::toString(bool value) noexcept {
  return (value) ? "true" : "false";
}

}