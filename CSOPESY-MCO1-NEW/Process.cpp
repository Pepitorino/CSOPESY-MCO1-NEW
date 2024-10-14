#include "Process.h"
#include "ConsoleManager.h"
#include "TypeDefRepo.h"

//only called in MainConsole whenever there is a successful checking of non-existing process inquired.
Process::Process(String name) {
	this->processName = name;
	this->pid = ConsoleManager::getInstance()->countNumberProcesses();
	this->cpuCoreId = 0;
	this->processProgress = 0;
	this->state = process_state::READY;

	//initialize commandList
	//as for now, 100 printcommands are added to the commandList
}

//return state
Process::process_state Process::getState() {
	return this->state;
}

//set state
void Process::setState(process_state state) {
	this->state = state;
}

//add command to commandList, command is to be provided in scheduler-test from MainConsole->ConsoleManager
void Process::addCommand(ICommand command) {
	this->commandList.push_back(command);
}

ICommand Process::getNextCommand() {
	return this->commandList[this->processProgress];
}

//instead of returning the command, we do this to avoid accessing empty indices
bool Process::hasRemainingCommands() {
	return this->processProgress < this->commandList.size();
}

//add ProcessCommandOutput to ProcessOutputs
void Process::addProcessOutput(ProcessCommandOutput output) {
	this->ProcessOutputs.push_back(output);
}

void Process::CommandExecuted(int RanbyCPUID) {
	//increase process progress
	this->cpuCoreId = RanbyCPUID;
	this->processProgress++;
	//change state to finished if finished
	if (this->processProgress == this->commandList.size()) {
		this->state = process_state::FINISHED;
	}
}

//add process-smi?

//will deal with file management, utilizes ProcessOutputs vector
//void Process::print() {}