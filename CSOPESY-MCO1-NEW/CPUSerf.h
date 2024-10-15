#pragma once
#include "ThreadClass.h"
#include "Process.h"
#include "TypeDefRepo.h"
#include "PrintCommand.h"
#include "ProcessCommandOutput.h"

//I just realized, what's the point of making them threads if we're gonna evaluate each core's status per CPU cycle?

class CPUSerf
{
public:

	CPUSerf(int coreId); //when constructed, no available process is possible
	~CPUSerf() = default;
	void tick();

	void switchProcess(std::shared_ptr<Process>);
	bool isReady();
	bool SerfisAvailable();

private:

	//bool GoSignalFromScheduler = false; //if we're gonna be doing Threaded, we can include the tick() in an overrun run() function of the CPUSerf

	int coreId;
	std::shared_ptr<Process> process;
	bool SerfisReady; //initialized true
	// bool SerfisAvailable; //initialized true, considered not needed since we can just check if process is nullptr or finished state
};