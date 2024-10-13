#pragma once
#include "TypeDefRepo.h"
#include "Process.h"
class AScheduler
{
public:
	enum SchedulingAlgorithm {
		DEBUG,
		FCFS,
		SJF_NP,
		SJF_P,
		RR
	};

	AScheduler();

	virtual void addProcess(std::shared_ptr<Process> process) = 0;
	virtual std::shared_ptr<Process> findProcess(String processName) = 0;
	virtual std::shared_ptr<Process> findProcess(int pid) = 0;

	virtual void execute() = 0; // void run() overide
	virtual std::shared_ptr<Process> getNextProcess() = 0;

	virtual void init() = 0;
	virtual int getQueueSize() = 0;
};

