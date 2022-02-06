#include "REHeaderTool/Threading/ThreadPool.h"

#include <cassert>

using namespace REHeaderTool;

ThreadPool::ThreadPool(uint32_t threadCount, ETerminationMode	terminationMode) noexcept:
	_destructorCalled{false},
	_workingWorkers{threadCount},
	terminationMode{terminationMode}
{
	assert(threadCount > 0u);

	//Preallocate enough space to avoid reallocations
	_workers.reserve(threadCount);

	for (uint32_t i = 0u; i < threadCount; i++)
	{
		_workers.emplace_back(std::thread(std::bind(&ThreadPool::workerRoutine, this)));
	}
}

ThreadPool::~ThreadPool() noexcept
{
	_taskMutex.lock();
	_destructorCalled = true;
	_taskMutex.unlock();

	//Awake threads so that they can perform necessary tests to exit their routine
	_taskCondition.notify_all();

	//Explicitely join all threads to terminate
	for (std::thread& worker : _workers)
	{
		if (worker.joinable())
		{
			worker.join();
		}
	}
}

void ThreadPool::workerRoutine() noexcept
{
	std::unique_lock lock(_taskMutex);

	while (shouldKeepRunning())
	{
		if (!_tasks.empty() && _isRunning)
		{
			//We own the mutex before grabbing a task
			std::shared_ptr<TaskBase> task = getTask();

			if (task != nullptr)
			{
				//Release the mutex before executing the task to allow other workers to grab tasks during execution
				lock.unlock();

				task->execute();

				lock.lock();
			}
		}

		if (!_destructorCalled && (_tasks.empty() || !_isRunning))
		{
			//A worker is about to sleep, decrement working workers count
			_workingWorkers.fetch_sub(1u);

			_taskCondition.wait(lock);

			//A worker is resuming its activity, increment working workers count
			_workingWorkers.fetch_add(1u);
		}
	}
}

std::shared_ptr<TaskBase> ThreadPool::getTask() noexcept
{
	//Iterate over all tasks
	for (decltype(_tasks)::iterator it = _tasks.begin(); it != _tasks.end(); it++)
	{
		//Get the first ready task
		if ((*it)->isReadyToExecute())
		{
			std::shared_ptr<TaskBase> result = std::move(*it);

			_tasks.erase(it);

			return result;
		}
	}

	return nullptr;
}

void ThreadPool::joinWorkers() noexcept
{
	std::unique_lock lock(_taskMutex);

	if (_destructorCalled)
	{
		lock.unlock();

		//Awake threads so that they can perform necessary tests to exit their routine
		_taskCondition.notify_all();

		//Explicitely join all threads to terminate
		for (std::thread& worker : _workers)
		{
			if (worker.joinable())
			{
				worker.join();
			}
		}
	}
	else
	{
		lock.unlock();

		//Just wait for all workers to be blocked on the _taskCondition
		while (_workingWorkers.load() != 0u || (_isRunning && !_tasks.empty()))
		{
			std::this_thread::yield();
		}
	}
}

bool ThreadPool::shouldKeepRunning() const noexcept
{
	return	!_destructorCalled || (terminationMode == ETerminationMode::FinishAll && _tasks.size() != 0u);
}

void ThreadPool::setIsRunning(bool isRunning) noexcept
{
	std::unique_lock lock(_taskMutex);

	if (_isRunning != isRunning)
	{
		_isRunning = isRunning;

		lock.unlock();

		if (isRunning)
		{
			_taskCondition.notify_all();
		}
	}
}