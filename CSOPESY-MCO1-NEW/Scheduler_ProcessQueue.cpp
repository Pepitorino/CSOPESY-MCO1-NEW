// This file contains the functions for the Scheduler class that are related to the processQueue.
#include "Scheduler.h"

int Scheduler::ProcessWaitingChecker() {
	//call this function to check if there are processes waiting to be added to processQueue
	for (int i = 0; i < processListCopy->size(); i++)
	{
		if (processListCopy->at(i)->getState() == Process::WAITING)
			return i;
	}
	return -1; //no process waiting
}

void Scheduler::ProcessQueuer() {
	//call this function to add process to processQueue
	//check if processCounter < size of vector in processListCopy
	//if so, add the non-accounted shared_ptr<Process> to processQueue
	if (processCounter < processListCopy->size())
	{
		processQueue.push(processListCopy->at(processCounter));
		processListCopy->at(processCounter)->setState(Process::READY);
		processCounter++;
		return;
	}
	int indexOfProcessWaiting = ProcessWaitingChecker();
	if (indexOfProcessWaiting != -1) {
		processQueue.push(processListCopy->at(indexOfProcessWaiting));
		processListCopy->at(indexOfProcessWaiting)->setState(Process::READY);
	}
}