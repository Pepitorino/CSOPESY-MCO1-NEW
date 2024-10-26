#pragma once
#include "ThreadClass.h"
#include "Process.h"
#include "TypeDefRepo.h"
#include "PrintCommand.h"
#include "ProcessCommandOutput.h"

//I just realized, what's the point of making them threads if we're gonna evaluate each core's status per CPU cycle?

class CPUSerf : public ThreadClass
{
public:

	CPUSerf(int coreId, int RRLimit); //when constructed, no available process is possible
	~CPUSerf() = default;
	//void tick();

	void switchProcess(std::shared_ptr<Process>);
	void fireSerf(); //to be called by Scheduler when closing the program
	bool hasProcess(); //{ return process != nullptr; }

private:

	//bool GoSignalFromScheduler = false; //if we're gonna be doing Threaded, we can include the tick() in an overrun run() function of the CPUSerf
	void ProcessWaitAndGet(); //to call a function in the Scheduler to get a process from the processQueue, keeps the 
	// run() function of the CPUSerf running until a process is obtained.
	void run() override;
	void conductProcess(); //create output in respect to command, create ProcessCommandOutput object w/ string output from commandoutput 
	// (in this case executePrint() for PRINT), add to ProcessOutputs vector of Process
	
	int coreId;
	std::shared_ptr<Process> process;
	bool SerfisRunning; //initialized true

	uint32_t CPUCycles, CPUWaittime; // needed pa ba this?
	int CPUCyclesCounter; // for RR
	int RRLimit; // for RR, -1 for FCFS
	// bool SerfisAvailable; //initialized true, considered not needed since we can just check if process is nullptr or finished state
};