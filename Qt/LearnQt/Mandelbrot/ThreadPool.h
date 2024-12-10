/// @file	ThreadPool.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    28.11.2024 22:18
///

#pragma once
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <functional>

namespace core
{
	/*class Task
	{
		std::atomic<bool>* m_needReset = nullptr;
		std::function<void (std::atomic<bool> &)>
	public:
		
		
	};*/

	class ThreadPool
	{
		using task_t = std::function<void(std::atomic<bool>&)>;
	public:
		explicit ThreadPool(size_t count = std::thread::hardware_concurrency());
		~ThreadPool();

		void AddTask(task_t &&);
		void Clear();
	private:
		std::atomic<bool> m_reset;
		bool m_exit;
		std::mutex m_mutex;
		std::condition_variable m_cv;
		std::vector<std::thread> m_threads;
		std::queue<task_t> m_tasks;
	};

}
