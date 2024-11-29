#include "TypeDefRepo.h"
#include "ConsoleManager.h"
#include "Scheduler.h"
#include "MemoryAllocator.h"

int main() {
	srand(time(NULL));
	uint64_t timeslice = 0;
	bool running = true;

	ConsoleManager::initialize();
	Scheduler::initialize();
	MemoryAllocator::initialize();

	ConsoleManager* consoleManagerInstance = ConsoleManager::getInstance();
	Scheduler* schedulerInstance = Scheduler::getInstance();

	//std::thread SchedulerThread(std::bind(&Scheduler::run, schedulerInstance));
	// we have separate class for Scheduler

	while (consoleManagerInstance->getRunning()) {
		MemoryAllocator::getInstance()->printBackingStore();
		consoleManagerInstance->createDummyProcess(timeslice);
		consoleManagerInstance->tick();
		timeslice++;
	}

	Scheduler::destroy();
	ConsoleManager::destroy();

	return 0;
}