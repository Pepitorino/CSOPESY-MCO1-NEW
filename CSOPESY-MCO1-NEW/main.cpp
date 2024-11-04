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

	schedulerInstance->start();

	while (consoleManagerInstance->getRunning()) {
		consoleManagerInstance->createDummyProcess(timeslice);
		consoleManagerInstance->tick();
		timeslice++;
	}

	Scheduler::destroy();
	ConsoleManager::destroy();

	return 0;
}