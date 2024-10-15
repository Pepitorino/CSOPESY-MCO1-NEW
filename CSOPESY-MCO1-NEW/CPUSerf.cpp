#include "CPUSerf.h"
#include "ProcessCommandOutput.h"

CPUSerf::CPUSerf(int coreId) {
	this->coreId = coreId;
	this->process = nullptr; //when constructed, no available process is possible
	this->SerfisReady = true; //initialized true
	//this->SerfisAvailable = true; //initialized true
}

//if we made the checking here, then we'd have another function to return this->isAvailable to the Scheduler

// to be called by a function in the Scheduler to check if CPU is available
bool CPUSerf::SerfisAvailable() {
	return (process == nullptr || process->getState() == Process::FINISHED);
}

// These guys can have no process and still be ready. This is for the case that when there are no more processes available in ready queue,
// the CPU is just ticking along the clock cycle, waiting for a process to be ready.


//if we're gonna be doing Threaded, we can include the tick() in an overrun run() function of the CPUSerf

//to be called by a function in the Scheduler calling all tick() of every CPU under it
//CPUs are always ready after every tick or cycle
void CPUSerf::tick() {
	this->SerfisReady = false;
	if (this->SerfisAvailable()) {
		this->SerfisReady = true;
		return;
	}
	else if (this->process->hasRemainingCommands()) {
		// since FCFS, keep this process state to RUNNING
		this->process->setState(Process::RUNNING);
		//obtain latest command
		if (this->process->getNextCommand().getCommandType() == ICommand::PRINT) {
			String print_output = this->process->getNextCommand().executePrint();
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
	this->SerfisReady = true;
}

bool CPUSerf::isReady() {
	return this->SerfisReady;
}

void CPUSerf::switchProcess(std::shared_ptr<Process> process) {
	if (this->process->getState() == Process::FINISHED) {
		this->process = process;
	}
	else {
		this->process->setState(Process::WAITING);
		this->process = process;
	}
	//this->SerfisAvailable = false;
}