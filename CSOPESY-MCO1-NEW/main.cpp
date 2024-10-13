#include "TypeDefRepo.h"
#include "ConsoleManager.h"

int main() {
	int cpuCycles;
	bool running = true;

	ConsoleManager::initialize();

	ConsoleManager* consoleManagerInstance = ConsoleManager::getInstance();
	std::cout << consoleManagerInstance;

	while (running) {
		consoleManagerInstance->tick();
		//Scheduler->tick();
	}

	consoleManagerInstance->destroy();

	return 0;
}