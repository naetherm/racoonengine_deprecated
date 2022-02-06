/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <vector>
#include <string>
#include <memory>	//std::shared_ptr

namespace REHeaderTool
{
	class TaskBase
	{
		friend class TaskHelper;

		private:
			/** Name of the task. */
			std::string	_name;

		protected:
			/** Dependent tasks which must terminate before this task is executed. */
			std::vector<std::shared_ptr<TaskBase>>	dependencies;

		public:
			TaskBase()														= delete;
			TaskBase(char const*								name,
					 std::vector<std::shared_ptr<TaskBase>>&&	deps = {})	noexcept;
			TaskBase(TaskBase const&)										= default;
			TaskBase(TaskBase&&)											= default;
			virtual ~TaskBase()												= default;

			/**
			*	@brief	Check if this task is ready to execute, i.e. it has no dependency or
			*			all its dependencies have finished their execution.
			*	
			*	@return true if this task is ready to execute, else false.
			*/
			virtual bool		isReadyToExecute()	const	noexcept = 0;

			/**
			*	@brief Execute the underlying task.
			*/
			virtual void		execute()					noexcept = 0;

			/**
			*	@brief Check whether this task has finished executing or not.
			*	
			*	@return true if this task has finished, else false.
			*/
			virtual bool		hasFinished()		const	noexcept = 0;

			/**
			*	@brief Getter for _name field.
			* 
			*	@return _name field.
			*/
			std::string const&	getName()			const	noexcept;

			TaskBase& operator=(TaskBase const&)	= default;
			TaskBase& operator=(TaskBase&&)			= default;
	};
}