#include "Process.h"
#include "ConsoleManager.h"
#include "PrintCommand.h"

//only called in MainConsole whenever there is a successful checking of non-existing process inquired.
Process::Process(String name, int instructions) {
	std::unique_lock<std::shared_mutex> lock(processMutex);
	this->processName = name;
	this->pid = ConsoleManager::getInstance()->countNumberProcesses();
	this->cpuCoreId = -1;
	this->processProgress = 0;
	this->state = process_state::WAITING;
	this->ProcessOutputs = {};
	this->timemade = time(0);

	//initialize commandList
	for (int i = 0; i < instructions; i++) {
		this->commandList.push_back(std::make_shared<PrintCommand>("Hello World from Process " + name));
	}
}

//return state
Process::process_state Process::getState() {
	//std::lock_guard<std::mutex> lockglobal(ConsoleManager::processListMutex);
	//std::lock_guard<std::mutex> lock(processMutex);
	return this->state;
}

//set state
void Process::setState(process_state state) {
	//std::lock_guard<std::mutex> lockglobal(ConsoleManager::processListMutex);
	//std::lock_guard<std::mutex> lock(processMutex);
	this->state = state;
}

//add command to commandList, command is to be provided in scheduler-test from MainConsole->ConsoleManager
void Process::addCommand(std::shared_ptr<ICommand> command) {
	this->commandList.push_back(command);
}

std::shared_ptr<ICommand> Process::getNextCommand() {
	//std::lock_guard<std::mutex> lock(processMutex);
	return this->commandList[this->processProgress];
}

//instead of returning the command, we do this to avoid accessing empty indices
bool Process::hasRemainingCommands() {
	//std::lock_guard<std::mutex> lock(processMutex);
	return this->processProgress < this->commandList.size();
}

//add ProcessCommandOutput to ProcessOutputs
void Process::addProcessOutput(ProcessCommandOutput output) {
	//std::lock_guard<std::mutex> lock(processMutex);
	this->ProcessOutputs.push_back(output);
}

void Process::CommandExecuted(int RanbyCPUID) {
	//std::lock_guard<std::mutex> lock(processMutex);
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

// for ConsoleManager
std::tuple<String, String, String, int, int> Process::HoldapTo() {
	// name, time of last command exc or time made (if no command executed), last core run on, current line code, size of commandList
	// this->processName
	String time = "";
	time_t startTime;
	if (this->ProcessOutputs.empty()) { // if there are no outputs
		startTime = this->timemade;
	}
	else { // if there are outputs
		startTime = this->ProcessOutputs.back().getTimeOfExecution();
	}
	
	//time conversion

	// Convert time_t to tm struct for local time
	struct tm localTime;
	localtime_s(&localTime, &startTime);

	// Create a buffer to hold the formatted time string
	char timeBuffer[80];
	strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y %I:%M:%S%p", &localTime);
	//std::cout << "Time Started: " << timeBuffer << std::endl;
	String timeBuffer2 = timeBuffer;
	time = "(" + timeBuffer2 + ")";

	//this->cpuCoreId
	String core;
	if (this->cpuCoreId == -1) {
		core = "Core: None";
	}
	else {
		core = "Core: " + std::to_string(this->cpuCoreId);
	}

	//this->processProgress
	int NumberOfCommands = this->commandList.size();

	return std::make_tuple(this->processName, time, core, this->processProgress, NumberOfCommands);
}

String Process::getName() {
	return this->processName;
}

int Process::getPid() {
	return this->pid;
}

int Process::getProcessProgress() {
	return this->processProgress;
}

int Process::getLines() {
	return this->commandList.size();
}

int Process::getCpuCoreId()
{
	return this->cpuCoreId;
}
