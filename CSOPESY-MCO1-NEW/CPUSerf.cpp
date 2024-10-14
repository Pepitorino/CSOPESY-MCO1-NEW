#include "CPUSerf.h"
#include "ProcessCommandOutput.h"

CPUSerf::CPUSerf(int coreId) {
	this->coreId = coreId;
	this->process = nullptr; //when constructed, no available process is possible
	this->SerfisReady = true; //initialized true
	this->SerfisAvailable = true; //initialized true
}

bool CPUSerf::SerfisAvailable() {
	return (process == nullptr || process->getState() == Process::FINISHED);
}

//to be called by a function in the Scheduler calling all tick() of every CPU under it
void CPUSerf::tick() {
	this->SerfisAvailable = false;
	if (this->process->hasRemainingCommands()) {
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
	this->SerfisAvailable = true;
}

bool CPUSerf::isRunning_RIGHTO() {
	return this->Serf_is_WORKWORK;
}

void CPUSerf::switchProcess(std::shared_ptr<Process> process) {
	this->process = process;
}