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

	bool switchConsole(console newConsole);
private:
	typedef std::unordered_map<console, std::shared_ptr<AConsole>> ConsoleTable;
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {}; //copy constructor is private
	ConsoleManager& operator=(ConsoleManager const&) {}; //assignment operator is private
	static ConsoleManager* sharedInstance;


	ConsoleTable consoleTable; // should be private, pero public muna for debugging
	std::shared_ptr<AConsole> currentConsole;
	std::shared_ptr<AConsole> previousConsole;

	// Considered adding a vector of Processes para matrack yung mga processes created so far (for use in creating Process ID)
	std::vector<std::shared_ptr<Process>> processTable;
	ConsoleTable consoleTable;

	//HANDLE consoleHandle;
	bool running = true;
};

