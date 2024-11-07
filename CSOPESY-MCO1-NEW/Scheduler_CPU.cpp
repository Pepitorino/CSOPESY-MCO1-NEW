#include "Scheduler.h"

/*
	Scheduler_CPU code division from top to bottom, indicated by "// <--- [(division name)] --->" :
		1. ConsoleManager purpose functions
		2. CPU related functions
*/

// <--- [for ConsoleManager] --->
//used std::tuple instead of int* for automatic deallocation
// std::tuple<> is destroyed out of scope
std::tuple<float, int, int> Scheduler::findCoresUsed() {
	int numCores = cpuList.size();
	int coresUsed = 0;
	for (int i = 0; i < cpuList.size(); i++) 
		if (cpuList.at(i)->hasProcess()) coresUsed++;
	
	float CPUUsePercent = (float)coresUsed / (float)numCores * 100;
	// coresUsed; //cores used
	numCores = numCores - coresUsed; //cores available

	return std::make_tuple(CPUUsePercent, coresUsed, numCores);
}

// <--- [for CPUs] --->
void Scheduler::hireCPUSerfs(int cores) {
	for (int i = 0; i < cores; i++)
	{
		if (schedulingAlgo == SchedulingAlgo::FCFS) SCHEDULER_FOR_THE_STREETS->cpuList.push_back(std::make_shared<CPUSerf>(i, -1, this->delays));
		else if (schedulingAlgo == SchedulingAlgo::RR) SCHEDULER_FOR_THE_STREETS->cpuList.push_back(std::make_shared<CPUSerf>(i, quantumCycles, this->delays));
	}
}

//to be called by CPUs whenever they're ready to take in a new process
void Scheduler::CPUProcessRequest(int CPUid) {
	//use lock_guard to lock the mutex
	if (running) {
		std::lock_guard<std::mutex> lock(processQueueMutex);
		//create a shared_ptr of Process to be used by CPUSerf
		std::shared_ptr<Process> Process_ToGive = nullptr;

		//check if processQueue is empty
		if (processQueue.empty()) {
			SCHEDULER_FOR_THE_STREETS->cpuList.at(CPUid)->switchProcess(Process_ToGive);
			return;
		}
		//if not empty, get the front process
		Process_ToGive = processQueue.front();
		//pop the process from the processQueue
		processQueue.pop();

		// check if the process is in memory, then pass to CPUSerf
		if (PagingAllocator::getInstance()->IsProcessInMemory(Process_ToGive->getPid())) {
			//give the process to the CPUSerf
			SCHEDULER_FOR_THE_STREETS->cpuList.at(CPUid)->switchProcess(Process_ToGive);
		}
		//if process is not in memory, check if there is enough memory to allocate

		else {
			if (PagingAllocator::getInstance()->IsMemoryAvailable(Process_ToGive->getMemorySize())) {
				//if not in memory, add to memory
				PagingAllocator::getInstance()->allocate2(Process_ToGive->getPid(), Process_ToGive->getMemorySize());
			}
			else {
				//if not enough memory, add the process back to the processQueue (no backing store yet)
				processQueue.push(Process_ToGive);
			}
		}
	}
}

//to be called when shutting down
void Scheduler::fireSerfs() {
	for (std::shared_ptr<CPUSerf>& serf : cpuList) {
		serf->fireSerf();
	}
}