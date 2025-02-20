// Includes from project
#include "../src/orchestrator.hpp"

// Include from STL
#include <iostream>


int main(int argc [[maybe_unused]], char** argv [[maybe_unused]])
{
	std::cout << "Hello World!" << std::endl;

	Orchestrator::getInstance().m_taskQueue.addTask([]() { std::cout << "Task 1" << std::endl; });
	Orchestrator::getInstance().m_taskQueue.addTask([]() { std::cout << "Task 2" << std::endl; });
	Orchestrator::getInstance().m_taskQueue.addTask([]() { std::cout << "Task 3" << std::endl; });
	Orchestrator::getInstance().m_taskQueue.addTask([]() { std::cout << "Task 4" << std::endl; });
	Orchestrator::getInstance().m_taskQueue.addTask([]() { std::cout << "Task 5" << std::endl; });

	Orchestrator::getInstance().m_taskQueue.waitUntilEmpty();

	return 0;
}