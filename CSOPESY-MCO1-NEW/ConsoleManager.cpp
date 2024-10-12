#include "ConsoleManager.h"

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance()
{
	return sharedInstance;
}

void ConsoleManager::initialize()
{
	sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy()
{
	delete sharedInstance;
}

void ConsoleManager::drawConsole()
{
	this->currentConsole->drawConsole();
}

bool ConsoleManager::switchMainConsole() {
	this->currentConsole = this->mainConsole;
}

bool ConsoleManager::switchProcessConsole(String process) {
	std::shared_ptr<Process> ptrProcess = this->processTable.find(process)->second;
	this->currentConsole = this->processConsole;
	this->currentConsole->onEnabled(ptrProcess);
}

void ConsoleManager::tick() {
	this->drawConsole();
}
