/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <inttypes.h>

namespace REHeaderTool
{
	enum class ETerminationMode : uint8_t
	{
		/** Finish all tasks submitted to the thread pool before complete destruction of the pool. */
		FinishAll		= 0,

		/** Finish currently running tasks and discard submitted tasks which have not started yet. */
		FinishCurrent	= 1 << 0
	};
}