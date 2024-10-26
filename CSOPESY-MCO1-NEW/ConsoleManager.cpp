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
	sharedInstance->Process_InOrderVector = std::make_shared<std::vector<std::shared_ptr<Process>>>();
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
	//call Scheduler to end
	Scheduler::getInstance()->shutdown();
	sharedInstance->running = false;
	sharedInstance->Process_InOrderVector = nullptr;
}

void ConsoleManager::drawConsole()
{
	this->currentConsole->drawConsole();
}

void ConsoleManager::switchMainConsole() {
	this->currentConsole = this->mainConsole;
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

bool ConsoleManager::isRunning() {
	return sharedInstance->running;
 }
