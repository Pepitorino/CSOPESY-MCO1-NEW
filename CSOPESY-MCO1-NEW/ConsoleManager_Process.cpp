#include "ConsoleManager.h"
#include "Scheduler.h"
#include "TypeDefRepo.h"

/*
	ConsoleManager_Process code division from top to bottom, indicated by "// <--- [(division name)] --->" :
		1. Scheduler
		2. ConsoleManager related Process creation
		3. Main Console related Process creation
		4. Process class object creation
*/

// <--- [For Scheduler] --->
//return a shared_ptr of vector of shared_ptr of Process to be used by Scheduler
std::shared_ptr<std::vector<std::shared_ptr<Process>>> ConsoleManager::giveProcess_InOrderVectorToScheduler() {
	return this->Process_InOrderVector;
}

// <--- [For ConsoleManager while loop under main.cpp] --->
//create dummy processes
void ConsoleManager::createDummyProcess(int timeslice) {
	//exit conditions
	if (!this->initialized) return;
	if (timeslice % this->batchProcessFreq != 0) return;
	if (!this->creatingBatches) return;

	int range = this->maxIns - this->minIns;
	int randomNum = rand() % range + this->minIns;

	String name = "process" + std::to_string(this->countNumberProcesses());

	std::shared_ptr<Process> process = std::make_shared<Process>(name, randomNum);

	this->processTable.insert(std::make_pair(name, process));
	this->ProcessOrderVector.push_back(name);
	this->Process_InOrderVector->push_back(process);
}

void ConsoleManager::setCreateBatches(bool setting) {
	this->creatingBatches = setting;
}

// <--- [For Main Console] --->
// This funciton assumes that the process name provided is unique.
//Process Creation, to be used by MainConsole, number of instructions is automatic
void ConsoleManager::addProcess(String process) {
	int range = this->maxIns - this->minIns;
	int randomNum = rand() % range + this->minIns;

	std::shared_ptr<Process> ptrProcess = std::make_shared<Process>(process, randomNum);
	this->processTable.insert(std::make_pair(process, ptrProcess));

	//add to vector
	this->ProcessOrderVector.push_back(process);

	//add to Process_InOrderVector
	this->Process_InOrderVector->push_back(ptrProcess);
}

//return a string of vector processes for MainConsole to print regarding Process statuses
std::vector<String> ConsoleManager::obtainProcessDetails() {
	//iterate through processTable by sequence of ProcessOrderVector and return a vector of strings
	//to obtain from each process: process name \t time of last command executed (MM/DD/YYYY) \t
	//Last core run on Process (if finished, display Finished), \t current line of code (processProgress)/commandList.size;
	//


	std::vector<String> strings;
	return strings;
}

//Processes table related
bool ConsoleManager::DoesProcessExist(String process) {
	return (this->processTable.find(process) == processTable.end()) ? false : true;
}

// <--- [For Process] --->
//used by Process when creating a new Process
int ConsoleManager::countNumberProcesses() {
	return this->processTable.size();
}