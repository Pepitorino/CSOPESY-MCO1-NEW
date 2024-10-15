#include "ThreadClass.h"

void ThreadClass::start() {
	std::thread(&ThreadClass::run, this).detach();
}

void ThreadClass::sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}