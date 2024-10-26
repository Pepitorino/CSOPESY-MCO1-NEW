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
	std::vector<String> strings;
	//iterate through processTable by sequence of ProcessOrderVector and return a vector of strings
	//to obtain from each process: process name \t time of last command executed (MM/DD/YYYY) \t
	//Last core run on Process (if finished, display Finished), \t current line of code (processProgress)/commandList.size;
	//
	//get details of cores from Scheduler
	std::tuple<float, int, int> coredetails = Scheduler::getInstance()->findCoresUsed();
	//coredetails[0]; //cores used
	//coredetails[1]; //cores available
	// CPU Utilization: std::get<0>(coredetails)%
	// Cores used: std::get<1>(coredetails)
	// Cores available: std::get<2>(coredetails)
	std::ostringstream corestrstream;
	corestrstream << "CPU Utilization: " << std::fixed << std::setprecision(2) << std::get<0>(coredetails) << "%\n";
	corestrstream << "Cores used: " << std::get<1>(coredetails) << "\n";
	corestrstream << "Cores available: " << std::get<2>(coredetails) << "\n\n";
	strings.push_back(corestrstream.str());
	
	//get copy of processes from processVector
	strings.push_back("--------------------------------\n");
	strings.push_back("Running processes:\n");
	std::shared_ptr<std::vector<std::shared_ptr<Process>>> processList = this->giveProcess_InOrderVectorToScheduler();
	//iterate through processList
	std::vector<String> finished_strings; //to be printed after running processes
	for (int i = 0; i < processList->size(); i++) {
		//separate into finished and running
		std::tuple <String, String, String, int, int> ProcessDetails = processList->at(i)->HoldapTo();
		if (std::get<3>(ProcessDetails) != std::get<4>(ProcessDetails)) // for running processes
			strings.push_back(ProcessDetailsFormatter(ProcessDetails));
		else // for finished processes
			finished_strings.push_back(ProcessDetailsFormatter(ProcessDetails));
	}
	strings.push_back("\n");
	strings.push_back("Finished processes:\n");
	for (int i = 0; i < finished_strings.size(); i++) {
		strings.push_back(finished_strings.at(i));
	}
	strings.push_back("--------------------------------\n\n");

	return strings;
}

String ConsoleManager::ProcessDetailsFormatter(std::tuple <String, String, String, int, int> ProcessDetails) {
	std::ostringstream procdetailstrstream;
	procdetailstrstream << std::left << std::setw(15) << StringShortener(std::get<0>(ProcessDetails), 15)
						<< std::left << std::setw(25) << std::get<1>(ProcessDetails);
						//check muna if complete or not (print core)
	if (std::get<3>(ProcessDetails) == std::get<4>(ProcessDetails)) {
		procdetailstrstream << std::left << std::setw(15) << "Finished";
	}
	else {
		procdetailstrstream << std::left << std::setw(15) << std::get<2>(ProcessDetails);
	}

	procdetailstrstream << std::left << std::setw(15) << std::to_string(std::get<3>(ProcessDetails)) << "/" << std::to_string(std::get<4>(ProcessDetails)) << "\n";
	return procdetailstrstream.str();
}

String ConsoleManager::StringShortener(String processName, int width) {
	if (processName.length() > width) {
		return "..." + processName.substr(processName.length() - (width-3));
	}
	return processName;
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