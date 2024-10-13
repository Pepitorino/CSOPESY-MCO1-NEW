#pragma once
#include "TypeDefRepo.h"
#include <unordered_map>
#include "AConsole.h"
#include "Process.h"
#include "MainConsole.h"
#include "ProcessConsole.h"
//Singleton
class ConsoleManager
{
public:
	enum console {
		PROCESS_CONSOLE,
		MAIN_CONSOLE,
		SCHEDULING_CONSOLE,

	};
	static void initialize();
	static void destroy();
	static ConsoleManager* getInstance();

	void tick();
	void drawConsole();

	void switchMainConsole();
	void switchProcessConsole(String process);
private:
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {}; //copy constructor is private
	ConsoleManager& operator=(ConsoleManager const&) {}; //assignment operator is private
	static ConsoleManager* sharedInstance;
	
	std::shared_ptr<MainConsole> mainConsole;
	std::shared_ptr<ProcessConsole> processConsole;
	std::shared_ptr<AConsole> currentConsole;

	// Considered adding a vector of Processes para matrack yung mga processes created so far (for use in creating Process ID)
	std::unordered_map<String, std::shared_ptr<Process>> processTable;

	//HANDLE consoleHandle;
	bool running = true;
};

