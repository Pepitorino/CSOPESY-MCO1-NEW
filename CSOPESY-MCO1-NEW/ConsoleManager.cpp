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

bool ConsoleManager::switchConsole(console newConsole) {
	this->previousConsole = this->currentConsole;
	for (auto x : this->consoleTable) {
		if (x.first == newConsole) this->currentConsole = x.second;
	}
}

void ConsoleManager::tick() {
	this->drawConsole();
}
