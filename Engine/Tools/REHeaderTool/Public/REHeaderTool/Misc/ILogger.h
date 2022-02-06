/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>

namespace REHeaderTool
{
	class ILogger
	{
		public:
			enum class ELogSeverity : uint8_t
			{
				/** Informative log. */
				Info = 0u,

				/** Warning log. */
				Warning,

				/** Error log. */
				Error
			};

			ILogger()					= default;
			ILogger(ILogger const&)		= default;
			ILogger(ILogger&&)			= default;
			virtual ~ILogger()			= default;

			/**
			*	@brief Log a message.
			*	
			*	@param message		Message to log.
			*	@param logSeverity	Severity level of the message.
			*/
			virtual void log(std::string const&	message,
							 ELogSeverity		logSeverity = ELogSeverity::Info) noexcept = 0;

			ILogger& operator=(ILogger const&)	= default;
			ILogger& operator=(ILogger&&)		= default;
	};
}