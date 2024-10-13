#include "TypeDefRepo.h"
#include "ConsoleManager.h"

int main() {
	int cpuCycles;
	bool running = true;

	ConsoleManager::initialize();

	ConsoleManager* consoleManagerInstance = ConsoleManager::getInstance();
	std::cout << consoleManagerInstance;

	while (consoleManagerInstance->getRunning()) {
		consoleManagerInstance->tick();
		//schedulerInstance->tick();
	}

	consoleManagerInstance->destroy();

	return 0;
}