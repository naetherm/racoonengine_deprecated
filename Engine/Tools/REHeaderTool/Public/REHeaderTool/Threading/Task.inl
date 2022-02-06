/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the REHeaderTool library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

template <typename ReturnType>
Task<ReturnType>::Task(char const* name, std::function<ReturnType(TaskBase*)>&& task, std::vector<std::shared_ptr<TaskBase>>&& deps) noexcept:
	TaskBase(name, std::forward<std::vector<std::shared_ptr<TaskBase>>>(deps)),
	_task{std::forward<std::function<ReturnType(TaskBase*)>>(task)},
	_result{_task.get_future()}
{
}

template <typename ReturnType>
bool Task<ReturnType>::isReadyToExecute() const noexcept
{
	for (std::shared_ptr<TaskBase> const& dependency : dependencies)
	{
		//If any of the dependencies have not finished executing, this task is not ready
		if (!dependency->hasFinished())
		{
			return false;
		}
	}

	return true;
}

template <typename ReturnType>
void Task<ReturnType>::execute() noexcept
{
	_task(this);
}

template <typename ReturnType>
bool Task<ReturnType>::hasFinished() const noexcept
{
	return !_result.valid() || _result.wait_for(std::chrono::nanoseconds(0)) == std::future_status::ready;
}