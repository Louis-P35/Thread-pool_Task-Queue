#pragma once

// Includes from project
#include "../src/taskQueue.hpp"

// Includes from STL
#include <thread>
#include <atomic>
#include <memory>
#include <chrono>



/*
* Class Orchestrator
* This class is a singleton that orchestrates the thread pool
*/
class Orchestrator
{
private:
	std::vector<std::thread> m_workerThreads;
	
	std::chrono::steady_clock::time_point m_lastUpdateTime;
	size_t m_numberOfThreads;
	std::atomic<bool> m_workerRunning = false;

public:
	TaskQueue m_taskQueue;

public:
	Orchestrator(const size_t numberOfThreads);
	static Orchestrator& getInstance();
	void start();
	void stop();

	// Delete copy constructor and assignment operator
	Orchestrator(const Orchestrator&) = delete;
	Orchestrator& operator=(const Orchestrator&) = delete;

private:
	~Orchestrator(); // Private destructor
};
