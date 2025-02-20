// Includes from project
#include "../src/orchestrator.hpp"

// Includes from STL
#include <iostream>
#include <thread>


Orchestrator::Orchestrator(const size_t  numberOfThreads) : m_numberOfThreads(numberOfThreads)
{
	// Launch the worker threads
	start();
}


Orchestrator::~Orchestrator()
{
	// Stop the orchestrator thread
	stop();

	// Join the worker threads
	for (auto& workerThread : m_workerThreads)
	{
		if (workerThread.joinable())
		{
			workerThread.join();
		}
	}
}


/*
* The orchestrator class is a singleton
* This method return it's only instance
* Instance is created with the number of threads corresponding to CPU cores at the first call
* 
* @return Orchestrator& The orchestrator instance
*/
Orchestrator& Orchestrator::getInstance()
{
	unsigned int cores = std::thread::hardware_concurrency();
	//std::cout << "Number of CPU cores: " << cores << std::endl;

	static Orchestrator instance(cores);
	return instance;
}


/*
* Create and run the worker threads to execute the tasks from the task queue.
* 
* @return void
*/
void Orchestrator::start()
{
	m_workerRunning = true;

	if (m_workerThreads.size() == 0)
	{
		// Worker thread lambda function
		// This just endlessley loops to gets and execute tasks from the task queue
		auto workerThreadLambda = [this]()
			{
				while (m_workerRunning)
				{
					std::function<void()> task;
					m_taskQueue.getTask(task);
					if (task)
					{
						task();
						m_taskQueue.markTaskAsDone();
					}
				}
			};

		// Create the worker threads
		for (size_t i = 0; i < m_numberOfThreads; ++i)
		{
			m_workerThreads.push_back(std::thread(workerThreadLambda));
		}
	}

	// Initialize the last update time
	m_lastUpdateTime = std::chrono::steady_clock::now();
}

/*
* Stop the orchestrator by joining the orchestrator thread
* 
* @return void
*/
void Orchestrator::stop()
{
	// Tell the workier threads to stop and release all the tasks by clearing the task queue
	m_workerRunning = false;

	// Stop the worker threads
	for (auto& workerThread : m_workerThreads)
	{
		if (workerThread.joinable())
		{
			m_taskQueue.releaseAll(m_numberOfThreads);
			workerThread.join();
		}
	}
	m_workerThreads.clear();

	m_taskQueue.clearTaskQueue();
}