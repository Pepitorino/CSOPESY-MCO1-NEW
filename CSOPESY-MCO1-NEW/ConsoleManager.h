#pragma once
#include "TypeDefRepo.h"
#include <unordered_map>
#include <vector>
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
	static void end();
	static ConsoleManager* getInstance();
	static void initProgram(); //read config.txt
	
	bool checkInitialized();
	void tick();
	void drawConsole();
	bool getRunning();

	void switchMainConsole();
	void switchProcessConsole(String process);

	//process table related
	bool DoesProcessExist(String process);
	void addProcess(String process);
	int countNumberProcesses();

private:
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {}; //copy constructor is private
	ConsoleManager& operator=(ConsoleManager const&) {}; //assignment operator is private
	static ConsoleManager* sharedInstance;
	
	std::shared_ptr<MainConsole> mainConsole;
	std::shared_ptr<ProcessConsole> processConsole;
	std::shared_ptr<AConsole> currentConsole;

	//RATIONALE to why unordered_map and vector instead of vector of Processes:
	// To fast access the process if it exists, we use unordered_map.
	// If we were to use vector alone, then we would have to iterate through the vector to check if the process exists given its name.
	// Considered adding a vector of Processes para matrack yung mga processes created so far (for use in creating Process ID)
	std::unordered_map<String, std::shared_ptr<Process>> processTable;

	//since unordered map, we need a matchingVector to determine which order nilagay yung mga processes
	//to be used in MainConsole whenever using 'process -ls'
	std::vector<String> ProcessOrderVector;

	//HANDLE consoleHandle;
	bool initialized = false;
	bool running = true;
};

