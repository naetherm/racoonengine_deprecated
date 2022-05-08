/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include "REHeaderTool/Misc/ILogger.h"

namespace REHeaderTool
{
	class DefaultLogger : public ILogger
	{
		protected:
			/** 
			*	@brief Log an info message.
			*	
			*	@param message Message to log.
			*/
			virtual void logInfo(std::string const& message)	noexcept;

			/** 
			*	@brief Log a warning message.
			*	
			*	@param message Message to log.
			*/
			virtual void logWarning(std::string const& message) noexcept;
			
			/** 
			*	@brief Log an error message.
			*	
			*	@param message Message to log.
			*/
			virtual void logError(std::string const& message)	noexcept;

		public:
			virtual void log(std::string const&	message,
							 ELogSeverity		logSeverity = ELogSeverity::Info)	noexcept override;
	};
}