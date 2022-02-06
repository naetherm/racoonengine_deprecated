/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <type_traits>
#include <inttypes.h>

namespace REHeaderTool {
/**
*	Enum representing type of an entity
*	This avoids unnecessary dynamic casts and allows EntityInfo to be a non-polymorphic class
*/
enum class EEntityType : uint16_t {
  /** Unset type. */
  Undefined = 0u,

  /** class. */
  Class = 1 << 0,

  /** struct. */
  Struct = 1 << 1,

  /** enum or enum class. */
  Enum = 1 << 2,

  /** Non-member variable. */
  Variable = 1 << 3,

  /** Member-variable. */
  Field = 1 << 4,

  /** Non-member function. */
  Function = 1 << 5,

  /** Member-function. */
  Method = 1 << 6,

  /** enum value or enum class value. */
  EnumValue = 1 << 7,

  /** namespace. */
  Namespace = 1 << 8
};

/**
*	@brief Binary "or" operation between 2 EEntityType masks.
* 
*	@param mask1 First mask.
*	@param mask2 Second mask.
* 
*	@return The binary "or" value between the 2 provided masks.
*/
constexpr EEntityType operator|(EEntityType mask1, EEntityType mask2) noexcept {
  using UnderlyingType = std::underlying_type_t<EEntityType>;

  return static_cast<EEntityType>(static_cast<UnderlyingType>(mask1) | static_cast<UnderlyingType>(mask2));
}

/**
*	@brief Binary "and" operation between 2 EEntityType masks.
* 
*	@param mask1 First mask.
*	@param mask2 Second mask.
* 
*	@return The binary "and" value between the 2 provided masks.
*/
constexpr EEntityType operator&(EEntityType mask1, EEntityType mask2) noexcept {
  using UnderlyingType = std::underlying_type_t<EEntityType>;

  return static_cast<EEntityType>(static_cast<UnderlyingType>(mask1) & static_cast<UnderlyingType>(mask2));
}

/**
*	@brief Check if 2 EEntityType masks overlap.
* 
*	@param mask1 First mask to compare.
*	@param mask2 Second mask to compare.
* 
*	@return true if the 2 masks overlap, else false.
*/
constexpr bool operator&&(EEntityType mask1, EEntityType mask2) noexcept {
  return (mask1 & mask2) != EEntityType::Undefined;
}
}