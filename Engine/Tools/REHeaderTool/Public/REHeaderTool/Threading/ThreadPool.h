/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include <string>
#include <list>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>		//std::atomic_uint
#include <functional>	//std::bind
#include <memory>		//std::shared_ptr
#include <type_traits>	//std::invoke_result

#include "REHeaderTool/Threading/Task.h"
#include "REHeaderTool/Threading/ETerminationMode.h"

namespace REHeaderTool
{
	class ThreadPool
	{
		private:
			/** Are workers allowed to process queued tasks? */
			bool									_isRunning	= true;

			/** Collection of all workers in this pool. */
			std::vector<std::thread>				_workers;

			/** List of all tasks. */
			std::list<std::shared_ptr<TaskBase>>	_tasks;

			/** Set to true when the ThreadPool destructor has been called. */
			bool									_destructorCalled	= false;

			/** Condition used to notify workers there are tasks to proceed. */
			std::condition_variable					_taskCondition;

			/** Mutex used with taskCondition. */
			std::mutex								_taskMutex;

			/** Number of workers currently running a task. */
			std::atomic_uint						_workingWorkers;

			/**
			*	@brief Routine run by workers.
			*/
			void						workerRoutine()				noexcept;

			/**
			*	@brief	Retrieve a task which is ready to execute.
			*			This method doesn't lock the task mutex so make sure _tasks is safe to access BEFORE the method is called.
			*	
			*	@return A valid shared_ptr pointing to a ready-to-execute task if any, else an empty shared_ptr.
			*/
			std::shared_ptr<TaskBase>	getTask()					noexcept;

			/**
			*	@brief Check whether a worker should keep running or terminate.
			*	
			*	@return true if the worker should continue to poll new tasks, else false.
			*/
			bool						shouldKeepRunning()	const	noexcept;

		public:
			/** Termination mode to apply when this Thread pool will be destroyed. */
			ETerminationMode	terminationMode = ETerminationMode::FinishAll;

			ThreadPool(uint32_t			threadCount		= std::thread::hardware_concurrency(),
					   ETerminationMode	terminationMode = ETerminationMode::FinishAll)			noexcept;
			ThreadPool(ThreadPool const&)														= delete;
			ThreadPool(ThreadPool&&)															= delete;
			~ThreadPool()																		noexcept;

			/**
			*	@brief Submit a task to the thread pool.
			*	
			*	@param taskName	Name of the task to submit to the thread pool.
			*	@param callable	Callable the submitted task should execute. It must take a TaskBase* as parameter.
			*	@param deps		Dependencies of the submitted task.
			*
			*	@return A pointer to the submitted task. It can be used as a dependency when submitting other tasks.
			*/
			template <typename Callable, typename = decltype(std::declval<Callable>()(std::declval<TaskBase*>()))>
			std::shared_ptr<TaskBase>	submitTask(std::string const&						taskName,
												   Callable&&								callable,
												   std::vector<std::shared_ptr<TaskBase>>&& deps = {})	noexcept;

			/**
			*	@brief Join all workers.
			*/
			void						joinWorkers()													noexcept;

			/**
			*	@brief Allow or disallow workers to process tasks.
			* 
			*	@param isRunning true to allow workers to process tasks, else false.
			*/
			void						setIsRunning(bool isRunning)									noexcept;

			ThreadPool& operator=(ThreadPool const&)	= delete;
			ThreadPool& operator=(ThreadPool&&)			= delete;
	};

	#include "REHeaderTool/Threading/ThreadPool.inl"
}