#include "TypeDefRepo.h"
#include "ConsoleManager.h"
#include "Scheduler.h"

int main() {
	srand(time(NULL));
	uint64_t timeslice = 1;
	bool running = true;

	ConsoleManager::initialize();
	Scheduler::initialize();

	ConsoleManager* consoleManagerInstance = ConsoleManager::getInstance();
	Scheduler* schedulerInstance = Scheduler::getInstance();

	std::thread SchedulerThread(std::bind(&Scheduler::run, schedulerInstance));

	while (consoleManagerInstance->getRunning()) {
		consoleManagerInstance->createDummyProcess(timeslice);
		consoleManagerInstance->tick();
		timeslice++;
	}

	ConsoleManager::destroy();
	Scheduler::destroy();

	return 0;
}