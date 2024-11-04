// This file contains the functions for the Scheduler class that are related to the processQueue.
#include "Scheduler.h"
#include "ConsoleManager.h"

void Scheduler::processListCopyUpdater() {
	std::unique_lock<std::shared_mutex> lockglobal(ConsoleManager::processListMutex);
	std::lock_guard<std::mutex> lock(processQueueMutex);
	SCHEDULER_FOR_THE_STREETS->processListCopy = ConsoleManager::getInstance()->giveProcess_InOrderVectorToScheduler();
}

int Scheduler::ProcessWaitingChecker() {
	//call this function to check if there are processes waiting to be added to processQueue
	std::unique_lock<std::shared_mutex> lockglobal(ConsoleManager::processListMutex);
	std::lock_guard<std::mutex> lock(processQueueMutex);
	for (int i = 0; i < processListCopy->size(); i++) 
	{
		//create copy of process at i
		
		if (processListCopy->at(i) != nullptr) {
			std::unique_lock<std::shared_mutex> lockprocess(processListCopy->at(i)->processMutex);
			Process* process = processListCopy->at(i).get();
			
			if (process->state != Process::FINISHED
				&& process->state == Process::WAITING)
				return i;
		}
	}
	return -1; //no process waiting
}

void Scheduler::ProcessQueuer() {
	//call this function to add process to processQueue
	//check if processCounter < size of vector in processListCopy
	//if so, add the non-accounted shared_ptr<Process> to processQueue
	if (processListCopy->size() == 0) return;
	if (processCounter < processListCopy->size())
	{
		std::lock_guard<std::mutex> lock(processQueueMutex);
		processQueue.push(processListCopy->at(processCounter));
		processListCopy->at(processCounter)->setState(Process::READY);
		processCounter++;
		return;
	}

	
	int indexOfProcessWaiting = ProcessWaitingChecker();
	if (indexOfProcessWaiting != -1) {
		std::unique_lock<std::shared_mutex> lockglobal(ConsoleManager::processListMutex);
		std::lock_guard<std::mutex> lock(processQueueMutex);
		processQueue.push(processListCopy->at(indexOfProcessWaiting));
		processListCopy->at(indexOfProcessWaiting)->setState(Process::READY);
	}
}

void Scheduler::ProcessGiver() {
	if (running) {
		std::lock_guard<std::mutex> lock(processQueueMutex);
		//create a shared_ptr of Process to be used by CPUSerf

		//check if processQueue is empty
		if (processQueue.empty()) {
			return;
		}
		for (int i = 0; i < cpuList.size(); i++) {
			if (!(cpuList.at(i)->hasProcess())) {
				//std::shared_ptr<Process> Process_ToGive = processQueue.front();
				//processQueue.pop();
				SCHEDULER_FOR_THE_STREETS->cpuList.at(i)->switchProcess(processQueue.front());
				processQueue.pop();
				//pop the process from the processQueue
				return;
			}
		}
		return;
	}
}