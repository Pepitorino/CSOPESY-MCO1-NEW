#pragma once
#include "TypeDefRepo.h"
#include "ICommand.h"
#include "ProcessCommandOutput.h"


class Process
{
public:
	enum process_state {
		WAITING,
		READY,
		RUNNING,
		FINISHED
	};

	Process(String name, int instructions);
	void CommandExecuted(int RanbyCPUID); //increases process progress, and changes state to finished if finished
	std::shared_ptr<ICommand> getNextCommand();
	void setState(process_state state);
	process_state getState();
	bool hasRemainingCommands();
	void addProcessOutput(ProcessCommandOutput output);
	void addCommand(std::shared_ptr<ICommand> command);
	String getName();
	int getPid();
	int getProcessProgress();
	int getLines();
	int getCpuCoreId();
	int getMemorySize();

	//for ConsoleManager
	std::tuple<String, String, String, int, int> HoldapTo();
	process_state state; //to be evaluated by the scheduler
	mutable std::shared_mutex processMutex;

private:
	String processName;
	int pid;
	int cpuCoreId; //siguro this is to determine sino yung last CPU na naghandle sa process

	int MemorySize;

	int processProgress; // to be checked by CPUSerf to determine until when (commandList.size) to run the process
	//sidenote: All ICommands do not have any idea that they have been processed, only the processProgress, when beyond any
	//ICommand's index, tells the process is finished.

	time_t timemade; // in case no command is executed, this will be the time the process will display for screen -ls
	std::vector<std::shared_ptr<ICommand>> commandList; // to be evaluated by the CPUSerf
	std::vector<ProcessCommandOutput> ProcessOutputs;

	
	//SOON: 'print' command is to iterate through the ProcessOutputs vector of the process. 
};