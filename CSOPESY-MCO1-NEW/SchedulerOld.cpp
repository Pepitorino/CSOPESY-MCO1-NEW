#include "Scheduler.h"
#include "CPUSerf.h"
#include "ConsoleManager.h"

Scheduler* Scheduler::SCHEDULER_FOR_THE_STREETS = nullptr;

Scheduler* Scheduler::getInstance()
{
	return SCHEDULER_FOR_THE_STREETS;
}

void Scheduler::initialize() {
	SCHEDULER_FOR_THE_STREETS = new Scheduler();
}

Scheduler::Scheduler()
{
	initialized = true;
	this->processCounter = 0; // to be used by ProcessQueuer() whenever this procCounter < processQueue->size()
}

void Scheduler::destroy()
{
	delete SCHEDULER_FOR_THE_STREETS;
}

void Scheduler::initScheduler(int cores, SchedulingAlgo scheduler, uint32_t quantumCycles, uint32_t batchProcessFreq, uint32_t minIns, uint32_t maxIns, uint32_t delays)
{
	SCHEDULER_FOR_THE_STREETS->cores = cores;
	SCHEDULER_FOR_THE_STREETS->schedulingAlgo = scheduler;
	SCHEDULER_FOR_THE_STREETS->quantumCycles = quantumCycles;
	SCHEDULER_FOR_THE_STREETS->batchProcessFreq = batchProcessFreq;
	SCHEDULER_FOR_THE_STREETS->minIns = minIns; 
	SCHEDULER_FOR_THE_STREETS->maxIns = maxIns;
	SCHEDULER_FOR_THE_STREETS->delays = delays;
	SCHEDULER_FOR_THE_STREETS->running = true;

	//auto updated by ConsoleManager since shared_ptr
	SCHEDULER_FOR_THE_STREETS->processListCopy = ConsoleManager::getInstance()->giveProcess_InOrderVectorToScheduler();
	SCHEDULER_FOR_THE_STREETS->hireCPUSerfs(SCHEDULER_FOR_THE_STREETS->cores);
}

void Scheduler::hireCPUSerfs(int cores) {
	for (int i = 0; i < cores; i++)
	{
		SCHEDULER_FOR_THE_STREETS->cpuList.push_back(std::make_shared<CPUSerf>(i));
	}
}

//no need, ProcessQueueUpdater is already in ProcessQueue
//void ProcessQueueUpdater() {
//	//call this function to updateprocessQueue
//}

int Scheduler::ProcessWaitingChecker() {
	//call this function to check if there are processes waiting to be added to processQueue
	for (int i = 0; i < processListCopy->size(); i++)
	{
		if (processListCopy->at(i)->getState() == Process::WAITING)
			return i;
	}
	return -1; //no process waiting
}

//soon will consider process arrival time and CPU cycles of scheduler
//already considers inserting WAITING processes to processQueue
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

//function to check if there is an available CPU
int Scheduler::CPUisAvailable() {
	for (int i=0; i < this->cpuList.size(); i++)
		if (this->cpuList[i]->SerfisAvailable())
			return i;
	return -1;
}

//function to check if there is an available process
bool Scheduler::ProcessinReadyQueueCheck() {
	if (!processQueue.empty())
		return true;
}

//only one CPUSerf per call
void Scheduler::GiveWorkToSerf(int coreid, std::shared_ptr<Process> Process_FromQueue) {
	this->cpuList[coreid]->switchProcess(Process_FromQueue);
}

bool Scheduler::AreAllSerfsReady() {
	for (int i = 0; i < cpuList.size(); i++)
		if (!cpuList[i]->isReady())
			return false;
	return true;
}

void Scheduler::MakeSerfDoWork() {
	for (int i = 0; i < cpuList.size(); i++)
		cpuList[i]->tick();
}

//running should be updated by ConsoleManager
void Scheduler::run() {
	int availableCPU;
	while (running)
	{
		//no need ProcessUpdater() since it's already shared_ptr to vector of shared_ptr of Process in ConsoleManager
		ProcessQueuer();
		availableCPU = CPUisAvailable(); //returns index of available CPU, -1 if none
		if ((availableCPU != -1) && ProcessinReadyQueueCheck()) {
			GiveWorkToSerf(availableCPU, processQueue.front());
			processQueue.pop();
		}
		else if (AreAllSerfsReady()) MakeSerfDoWork();
	}
}

void Scheduler::tick() {

}
