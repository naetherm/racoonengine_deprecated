/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

inline EEntityType PropertyCodeGen::getEligibleEntityMask() const noexcept
{
	return _eligibleEntityMask;
}

inline std::string const& PropertyCodeGen::getPropertyName() const noexcept
{
	return _propertyName;
}