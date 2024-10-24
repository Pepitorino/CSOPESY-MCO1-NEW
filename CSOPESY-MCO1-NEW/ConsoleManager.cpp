#include "ConsoleManager.h"
#include "Scheduler.h"
#include "TypeDefRepo.h"
// mostly the same as our old one
ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance()
{
	return sharedInstance;
}

void ConsoleManager::initialize()
{
	std::cout << "Hello World"; //debugging
	sharedInstance = new ConsoleManager();
}

void ConsoleManager::initProgram() {
	if (!sharedInstance->initialized) {
		std::ifstream settings;
		settings.open("config.txt");
		int cores;
		Scheduler::SchedulingAlgo schedulingAlgo;
		uint32_t quantumCycles, batchProcessFreq, minIns, maxIns, delays;

		// Read specific lines of the config file
		std::string temp;

		// 1. Reading num-cpu
		settings >> temp >> cores;
		if (temp != "num-cpu") throw std::runtime_error("Error: Expected 'num-cpu'");

		// 2. Reading scheduler
		std::string algoStr;
		settings >> temp >> algoStr;
		algoStr = algoStr.substr(1, algoStr.size() - 2);
		if (algoStr == "rr") {
			schedulingAlgo = Scheduler::SchedulingAlgo::RR;
		}
		else if (algoStr == "fcfs") {
			schedulingAlgo = Scheduler::SchedulingAlgo::FCFS;
		}
		else if (algoStr == "sjfp") {
			schedulingAlgo = Scheduler::SchedulingAlgo::SJFP;
		}
		else if (algoStr == "sjfnp") {
			schedulingAlgo = Scheduler::SchedulingAlgo::SJFNP;
		}
		else {
			throw std::runtime_error("Error: Unknown scheduling algorithm");
		}

		// 3. Reading quantum-cycles
		settings >> temp >> quantumCycles;
		if (temp != "quantum-cycles") throw std::runtime_error("Error: Expected 'quantum-cycles'");

		// 4. Reading batch-process-freq
		settings >> temp >> batchProcessFreq;
		if (temp != "batch-process-freq") throw std::runtime_error("Error: Expected 'batch-process-freq'");
		sharedInstance->batchProcessFreq = batchProcessFreq;

		// 5. Reading min-ins
		settings >> temp >> minIns;
		if (temp != "min-ins") throw std::runtime_error("Error: Expected 'min-ins'");
		sharedInstance->minIns = minIns;

		// 6. Reading max-ins
		settings >> temp >> maxIns;
		if (temp != "max-ins") throw std::runtime_error("Error: Expected 'max-ins'");
		sharedInstance->maxIns = maxIns;

		if (maxIns < minIns) throw std::runtime_error("Error: Max instructions should be less than minimum instructions.");

		// 7. Reading delay-per-exec
		settings >> temp >> delays;
		if (temp != "delay-per-exec") throw std::runtime_error("Error: Expected 'delay-per-exec'");



		Scheduler::initScheduler(cores, schedulingAlgo, quantumCycles, batchProcessFreq, minIns, maxIns, delays);
	}
	sharedInstance->initialized = true;
}

bool ConsoleManager::checkInitialized() {
	return this->initialized;
}

ConsoleManager::ConsoleManager()
{
	this->mainConsole = std::make_shared<MainConsole>();
	this->processConsole = std::make_shared<ProcessConsole>();
	this->currentConsole = this->mainConsole;
}

void ConsoleManager::destroy()
{
	delete sharedInstance;
}

bool ConsoleManager::getRunning() 
{
	return this->running;
}

void ConsoleManager::end() {
	sharedInstance->running = false;
}

void ConsoleManager::drawConsole()
{
	this->currentConsole->drawConsole();
}

void ConsoleManager::switchMainConsole() {
	this->currentConsole = this->mainConsole;
}

//return a shared_ptr of vector of shared_ptr of Process to be used by Scheduler
std::shared_ptr<std::vector<std::shared_ptr<Process>>> ConsoleManager::giveProcess_InOrderVectorToScheduler() {
	return this->Process_InOrderVector;
}

//Processes table related
bool ConsoleManager::DoesProcessExist(String process) {
	return (this->processTable.find(process) == processTable.end()) ? false : true; 
}

void ConsoleManager::addProcess(String process) {
	std::shared_ptr<Process> ptrProcess = std::make_shared<Process>(process);
	this->processTable.insert(std::make_pair(process, ptrProcess));

	//add to vector
	this->ProcessOrderVector.push_back(process); 

	//add to Process_InOrderVector
	this->Process_InOrderVector->push_back(ptrProcess);
}

int ConsoleManager::countNumberProcesses() {
	return this->processTable.size();
}

//return a string of vector processes for MainConsole to print regarding Process statuses
std::vector<String> ConsoleManager::obtainProcessDetails() {
	//iterate through processTable by sequence of ProcessOrderVector and return a vector of strings
	//to obtain from each process: process name \t time of last command executed (MM/DD/YYYY) \t
	//Last core run on Process (if finished, display Finished), \t current line of code (processProgress)/commandList.size;
	std::vector<String> strings;
	return strings;
}

//ProcessConsole related
void ConsoleManager::switchProcessConsole(String process) {
	std::shared_ptr<Process> ptrProcess = this->processTable.find(process)->second;
	this->currentConsole = this->processConsole;
	this->currentConsole->onEnabled(ptrProcess);
}

void ConsoleManager::tick() {
	this->currentConsole->process();
}

//create dummy processes
void ConsoleManager::createDummyProcess(int timeslice) {
	//exit conditions
	if (!this->initialized) return;
	if (timeslice % this->batchProcessFreq == 0) return;
	if (!this->creatingBatches) return;

	int range = this->maxIns - this->minIns;
	int randomNum = rand() % range + this->minIns;

	String name = "process" + std::to_string(this->countNumberProcesses());

	this->Process_InOrderVector->push_back(std::make_shared<Process>(name, randomNum));
}