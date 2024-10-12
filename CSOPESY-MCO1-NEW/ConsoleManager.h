#pragma once
#include "TypeDefRepo.h"
#include <unordered_map>
#include "AConsole.h"
#include "Process.h"
class ConsoleManager
{
public:
	enum console {
		processConsole,
		mainConsole,
		schedulingConsole,

	};
	static void initialize();
	static void destroy();
	static ConsoleManager* getInstance();

	void tick();
	void drawConsole();

	bool switchMainConsole();
	bool switchProcessConsole(String process);
private:
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {}; //copy constructor is private
	ConsoleManager& operator=(ConsoleManager const&) {}; //assignment operator is private
	static ConsoleManager* sharedInstance;
	
	std::shared_ptr<AConsole> mainConsole;
	std::shared_ptr<AConsole> processConsole;
	std::shared_ptr<AConsole> currentConsole;

	// Considered adding a vector of Processes para matrack yung mga processes created so far (for use in creating Process ID)
	std::unordered_map<String, std::shared_ptr<Process>> processTable;

	//HANDLE consoleHandle;
	bool running = true;
};

