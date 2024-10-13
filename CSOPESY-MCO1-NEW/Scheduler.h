#pragma once
#include "TypeDefRepo.h"
#include "Process.h"
class Scheduler
{
public:
	enum SchedulingAlgo {
		FCFS,
		RR,
		SJFP,
		SJFNP
	};
	static void initialize();
	static void initScheduler(int cores, SchedulingAlgo scheduler, uint32_t quantumCycles, uint32_t batchProcessFreq, uint32_t minIns, uint32_t maxIns, uint32_t delays);
	static void destroy();
	static Scheduler* getInstance();

	void tick(); //per tick, just checks for empty cores, or new processes
	void addProcess(); //adds process to processqueue 
	void finishProcess();
private:
	Scheduler(int cores);
	~Scheduler() = default;
	Scheduler(Scheduler const&) {}; //copy constructor is private

	Scheduler& operator=(Scheduler const&) {}; //assignment operator is private
	static Scheduler* SCHEDULER_FOR_THE_STREETS; //singleton instance
	//processqueue is a sharedptr now between scheduler and its scheduling type
	bool initialized = false;
	std::shared_ptr<std::vector<std::shared_ptr<Process>>> processQueue; //when process added to processList @ ConsoleManager, add it to processQueue
	//AScheduler scheduler;

	//config settings
	int cores;
	SchedulingAlgo schedulingAlgo;
	uint32_t quantumCycles, batchProcessFreq, minIns, maxIns, delays;
};

