#pragma once
#include "TypeDefRepo.h"
#include "ICommand.h"
class Process
{
public:
	enum process_state{
		READY,
		RUNNING,
		FINISHED
	};

	Process();
	void execute_command(); //increases process progress, and changes state to finished if finished
private:
	String processName;
	int pid;
	int cpuCoreId;
	int processProgress;
	std::vector<ICommand> commandList;
	process_state state;
	
};

