/// @file	ThreadPool.cpp
///
/// @see	ThreadPool.h
///
/// @author	RDN © Intern :)
///
/// @date    04.12.2024 21:43
///

#include "stdafx.h"
#include "ThreadPool.h"

namespace core
{
	ThreadPool::ThreadPool(size_t count)
		: m_reset(false)
		, m_exit(false)
	{
		for (size_t i = 0; i < count; ++i)
		{
			m_threads.emplace_back([this]()
			{
				while (true)
				{
					task_t task = nullptr;

					{
						std::unique_lock lock(m_mutex);

						// Waiting until there is a task to
						// execute or the pool is stopped
						m_cv.wait(lock, [this]
						{
							return !m_tasks.empty() || m_reset || m_exit;
						});

						// exit the thread in case the pool
						// is stopped and there are no tasks
						if (m_exit)
							return;

						if (!m_reset && !m_tasks.empty())
						{
							// Get the next task from the queue
							task = std::move(m_tasks.front());
							m_tasks.pop();
						}
					}

					if (task)
						task(m_reset);
				}
			});
		}
	}

	ThreadPool::~ThreadPool()
	{
		m_exit = true;
		Clear();
		for (auto & thread : m_threads)
		{
			if (thread.joinable())
				thread.join();
		}
	}

	void ThreadPool::AddTask(task_t&& task)
	{
		m_reset = false;
		{
			std::unique_lock lock(m_mutex);
			m_tasks.emplace(std::move(task));
		}
		m_cv.notify_one();
	}

	void ThreadPool::Clear()
	{
		m_reset = true;
		std::unique_lock lock(m_mutex);
		std::queue<task_t> empty;
		m_tasks.swap(empty);
		m_cv.notify_all();
	}
}
