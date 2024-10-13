#include "TypeDefRepo.h"
#include "ConsoleManager.h"
#include "Scheduler.h"

int main() {
	int cpuCycles;
	bool running = true;

	ConsoleManager::initialize();
	Scheduler::initialize();

	ConsoleManager* consoleManagerInstance = ConsoleManager::getInstance();
	Scheduler* schedulerInstance = Scheduler::getInstance();
	std::cout << consoleManagerInstance;

	while (consoleManagerInstance->getRunning()) {
		consoleManagerInstance->tick();
		schedulerInstance->tick();
	}

	ConsoleManager::destroy();
	Scheduler::destroy();

	return 0;
}