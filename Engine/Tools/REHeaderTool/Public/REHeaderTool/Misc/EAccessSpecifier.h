/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <inttypes.h>

namespace REHeaderTool {
enum class EAccessSpecifier : uint8_t {
  Invalid = 0u,
  Public = 1u,
  Protected = 2u,
  Private = 3u
};

std::string toString(EAccessSpecifier error) noexcept;
}