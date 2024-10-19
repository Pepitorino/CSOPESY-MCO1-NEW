#pragma once
#include "TypeDefRepo.h"
#include "Process.h"
#include "CPUSerf.h"

class Scheduler : public ThreadClass
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
	// void ProcessQueueUpdater
	// void ProcessQueuer
	//void addProcess(); //adds process to processqueue 
	void finishProcess();
private:

	Scheduler();
	~Scheduler() = default;
	Scheduler(Scheduler const&) {}; //copy constructor is private
	void hireCPUSerfs(int cores);
	void GiveWorkToSerf(int coreid, std::shared_ptr<Process> Process_FromQueue);
	void run();
	void ProcessQueuer(); //adds process to processqueue
	//void ProcessQueueUpdater();
	int ProcessWaitingChecker();
	bool ProcessinReadyQueueCheck();
	int CPUisAvailable();
	bool AreAllSerfsReady();
	void MakeSerfDoWork();


	Scheduler& operator=(Scheduler const&) {}; //assignment operator is private
	static Scheduler* SCHEDULER_FOR_THE_STREETS; //singleton instance
	//processqueue is a sharedptr now between scheduler and its scheduling type
	bool initialized = false;
	bool running;



	// we don't have a vector of shared_ptr processes in Console Manager, so a solution is to have a processList copy in Sched
	// then after that, compare with our processCounter here in Scheduler and compare with copied processList
	std::shared_ptr<std::vector<std::shared_ptr<Process>>> processListCopy; //when process added to processList @ ConsoleManager, add it to processQueue
	//to include CPUSerf vector
	std::vector<std::shared_ptr<CPUSerf>> cpuList;
	//to be used by ProcessQueuer() whenever this->processCounter < processQueue->size()
	std::queue<std::shared_ptr<Process>> processQueue;

	
	//AScheduler scheduler;

	//config settings
	int cores;
	int processCounter;
	SchedulingAlgo schedulingAlgo;
	uint32_t quantumCycles, batchProcessFreq, minIns, maxIns, delays;
};

