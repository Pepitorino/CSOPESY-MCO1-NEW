#include "CPUSerf.h"
#include "ProcessCommandOutput.h"
#include "Scheduler.h"
#include "ConsoleManager.h"
#include "PagingAllocator.h"

CPUSerf::CPUSerf(int coreId, int RRLimit, uint32_t delay) : ThreadClass() {
	this->coreId = coreId;
	this->process = nullptr; //when constructed, no available process is possible
	this->SerfisRunning = true; //initialized true
	this->CPUCycles = 0;
	this->CPUWaittime = 0;
	this->CPUCyclesCounter = 0;
	this->RRLimit = RRLimit;
	this->delay = delay;
	
	//this->SerfisAvailable = true; //initialized true
	this->start();
}

//to be called by Scheduler when closing the program
void CPUSerf::fireSerf() {
	this->SerfisRunning = false;
}

void CPUSerf::conductProcess() { 
	if (this->process->getNextCommand()->getCommandType() == ICommand::PRINT) {
		String print_output = this->process->getNextCommand()->execute();
		time_t time_now = time(0);
		ProcessCommandOutput output = ProcessCommandOutput(this->coreId, time_now, print_output);
		this->process->addProcessOutput(output);
	}
	//create output in respect to command
	//create ProcessCommandOutput object w/ string output from commandoutput (in this case executePrint() for PRINT)
	//add to ProcessOutputs vector of Process

	//check if process is finished, modify state [Already in CommandExecuted]
	this->process->CommandExecuted(this->coreId);
}

void CPUSerf::ProcessWaitAndGet() {
	//to call a function in the Scheduler to get a process from the processQueue, keeps the 
	// run() function of the CPUSerf running until a process is obtained.

	std::lock_guard<std::mutex> lock(CPUMutex);
	Scheduler::getInstance()->CPUProcessRequest(this->coreId);
}

void CPUSerf::WorkProcess() {
	std::lock_guard<std::mutex> lock(CPUMutex);

	if (delay > 0) {
		if (CPUCycles % delay == 0) {

		}
		else {
			CPUCycles++;
			return;
		}
	}

	this->process->setState(Process::RUNNING);

	// if FCFS
	if (RRLimit == -1)
		conductProcess();
	// if RR
	else if (CPUCyclesCounter < RRLimit) {
		//obtain latest command

		//check if RR limit is reached
		conductProcess();
		CPUCyclesCounter++;

	}

	//this is after CommandExecuted, so we can check if process is finished
	if (this->process->getState() == Process::FINISHED) {
		PagingAllocator::getInstance()->deallocate2(this->process->getPid());
		this->process = nullptr;
		CPUCyclesCounter = 0;
	}
	else if (CPUCyclesCounter == RRLimit) {
		PagingAllocator::getInstance()->visualizeMemory2(this->coreId, this->CPUCycles);
		this->process->setState(Process::WAITING);
		this->process = nullptr;
		CPUCyclesCounter = 0;
	}
	CPUCycles++;
}

void CPUSerf::run() {
	//this->SerfisReady = false;
	while (SerfisRunning) {
		if (ConsoleManager::getInstance()->getRunning() && this->process == nullptr) {
			this->ProcessWaitAndGet();
			if (this->process == nullptr) {
				CPUWaittime++;
				CPUCycles++;
			}
		}
		else if (this->process != nullptr && ConsoleManager::getInstance()->getRunning() && this->process->hasRemainingCommands()) {
			//this means that the process is in another core that is running
			std::unique_lock<std::shared_mutex> lockglobal(ConsoleManager::processListMutex);
			std::unique_lock<std::shared_mutex> lockprocess(this->process->processMutex);
			//if (this->process->getState() == Process::FINISHED) {
			//	this->process = nullptr;
			//	CPUCyclesCounter = 0;
			//}
			if (this->process->getState() == Process::RUNNING && (this->process->getCpuCoreId() != this->coreId)) this->process = nullptr;
			else this->WorkProcess();
			//either FCFS or RR, keep them RUNNING
		}
	}
}

bool CPUSerf::hasProcess() {
	return this->process != nullptr;
}

//to be called by Scheduler whenever CPU is ready to take in a new process
void CPUSerf::switchProcess(std::shared_ptr<Process> process) {
	this->process = process;
}