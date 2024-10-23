#include "Scheduler.h"

void Scheduler::hireCPUSerfs(int cores) {
	for (int i = 0; i < cores; i++)
	{
		if (schedulingAlgo == SchedulingAlgo::FCFS) SCHEDULER_FOR_THE_STREETS->cpuList.push_back(std::make_shared<CPUSerf>(i, -1));
		else if (schedulingAlgo == SchedulingAlgo::RR) SCHEDULER_FOR_THE_STREETS->cpuList.push_back(std::make_shared<CPUSerf>(i, quantumCycles));
	}
}

bool Scheduler::CPUProcessRequest(int CPUid) {
	//use lock_guard to lock the mutex
	std::lock_guard<std::mutex> lock(processQueueMutex);
	//create a shared_ptr of Process to be used by CPUSerf
	std::shared_ptr<Process> Process_ToGive = nullptr;

}