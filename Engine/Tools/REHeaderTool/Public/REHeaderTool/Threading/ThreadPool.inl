/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

template <typename Callable, typename>
std::shared_ptr<TaskBase> ThreadPool::submitTask(std::string const& taskName, Callable&& callable, std::vector<std::shared_ptr<TaskBase>>&& deps) noexcept
{
	//Return type of the submitted task
	using ReturnType = typename std::invoke_result_t<Callable, TaskBase*>;

	std::vector<std::shared_ptr<TaskBase>> depsCopy = deps;

	std::shared_ptr<Task<ReturnType>> newTask =
		std::make_shared<Task<ReturnType>>(taskName.data(), std::forward<Callable>(callable), std::forward<std::vector<std::shared_ptr<TaskBase>>>(deps));

	_taskMutex.lock();
	_tasks.emplace_back(newTask);
	_taskMutex.unlock();

	_taskCondition.notify_one();

	return newTask;
}