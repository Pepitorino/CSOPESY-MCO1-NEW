#include "Scheduler.h"
#include "CPUSerf.h"
#include "ConsoleManager.h"
#include "TypeDefRepo.h"

Scheduler* Scheduler::SCHEDULER_FOR_THE_STREETS = nullptr;

Scheduler* Scheduler::getInstance()
{
	return SCHEDULER_FOR_THE_STREETS;
}

void Scheduler::initialize() {
	SCHEDULER_FOR_THE_STREETS = new Scheduler();
	SCHEDULER_FOR_THE_STREETS->initialized = true;
}

Scheduler::Scheduler() : ThreadClass()
{

}

void Scheduler::destroy()
{
	SCHEDULER_FOR_THE_STREETS->shutdown();
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
	SCHEDULER_FOR_THE_STREETS->processCounter = 0; // to be used by ProcessQueuer() whenever this procCounter < processQueue->size()

	//auto updated by ConsoleManager since shared_ptr
	SCHEDULER_FOR_THE_STREETS->processListCopy = ConsoleManager::getInstance()->giveProcess_InOrderVectorToScheduler();
	SCHEDULER_FOR_THE_STREETS->hireCPUSerfs(SCHEDULER_FOR_THE_STREETS->cores);
	SCHEDULER_FOR_THE_STREETS->start();
}

void Scheduler::run() {
	u_int processQueuerCounter = 0;
	while (!initialized) {
		Sleep(5);
	}
	while (running)
	{
		//update processListCopy
		//SCHEDULER_FOR_THE_STREETS->processListCopyUpdater();
		//no need ProcessUpdater() since it's already shared_ptr to vector of shared_ptr of Process in ConsoleManager
		//if ((processQueuerCounter % 3 == 0) && processListCopy != nullptr) {
			SCHEDULER_FOR_THE_STREETS->processListCopyUpdater();
			SCHEDULER_FOR_THE_STREETS->ProcessQueuer();
		//}
		//SCHEDULER_FOR_THE_STREETS->ProcessGiver();
		// no need to check for empty cores, they do their own request to Scheduler if there's no work for them to do
		//Sleep(100);
		processQueuerCounter++;
		if (processQueuerCounter == 4000000000) processQueuerCounter = 0;
	}
}

void Scheduler::shutdown() {
	running = false;
	fireSerfs();
}

void Scheduler::tick() {

}