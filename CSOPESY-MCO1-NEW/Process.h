#pragma once
#include "TypeDefRepo.h"
#include "ICommand.h"
#include "ProcessCommandOutput.h"

class Process
{
public:
	enum process_state{
		WAITING,
		READY,
		RUNNING,
		FINISHED
	};

	Process(String name);
	void CommandExecuted(int RanbyCPUID); //increases process progress, and changes state to finished if finished
	ICommand getNextCommand();
	void setState(process_state state);
	process_state getState();
	bool hasRemainingCommands();
	void addProcessOutput(ProcessCommandOutput output);

private:
	String processName;
	int pid;
	int cpuCoreId; //siguro this is to determine sino yung last CPU na naghandle sa process

	int processProgress; // to be checked by CPUSerf to determine until when (commandList.size) to run the process
	//sidenote: All ICommands do not have any idea that they have been processed, only the processProgress, when beyond any
	//ICommand's index, tells the process is finished.


	std::vector<ICommand> commandList; // to be evaluated by the CPUSerf
	std::vector<ProcessCommandOutput> ProcessOutputs;
	process_state state; //to be evaluated by the scheduler
	
	//SOON: 'print' command is to iterate through the ProcessOutputs vector of the process. 
};

