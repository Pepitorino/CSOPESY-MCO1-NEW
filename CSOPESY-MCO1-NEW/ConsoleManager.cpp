#include "ConsoleManager.h"
// mostly the same as our old one
ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance()
{
	return sharedInstance;
}

void ConsoleManager::initialize()
{
	std::cout << "Hello World"; //debugging
	sharedInstance = new ConsoleManager();
}

void ConsoleManager::initProgram() {

}

ConsoleManager::ConsoleManager()
{
	this->mainConsole = std::make_shared<MainConsole>();
	this->processConsole = std::make_shared<ProcessConsole>();
	this->currentConsole = this->mainConsole;
}

void ConsoleManager::destroy()
{
	delete sharedInstance;
}

void ConsoleManager::drawConsole()
{
	this->currentConsole->drawConsole();
}

void ConsoleManager::switchMainConsole() {
	this->currentConsole = this->mainConsole;
}

void ConsoleManager::switchProcessConsole(String process) {
	std::shared_ptr<Process> ptrProcess = this->processTable.find(process)->second;
	this->currentConsole = this->processConsole;
	this->currentConsole->onEnabled(ptrProcess);
}

void ConsoleManager::tick() {
	this->currentConsole->process();
}
