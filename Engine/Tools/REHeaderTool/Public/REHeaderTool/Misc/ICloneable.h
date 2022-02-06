/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

namespace REHeaderTool
{
	class ICloneable
	{
		public:
			ICloneable()					= default;
			ICloneable(ICloneable const&)	= default;
			ICloneable(ICloneable&&)		= default;
			virtual ~ICloneable()			= default;

			/**
			*	@brief Create a dynamically instantiated copy of the class implementing this method.
			* 
			*	@return A pointer on the newly instantiated copy of the implementing class.
			*/
			virtual ICloneable* clone() const noexcept = 0;

			ICloneable& operator=(ICloneable const&)	= default;
			ICloneable& operator=(ICloneable&&)			= default;
	};
}