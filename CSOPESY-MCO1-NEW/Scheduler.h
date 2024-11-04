#pragma once
#include "TypeDefRepo.h"
#include "Process.h"
#include "CPUSerf.h"
#include "ConsoleManager.h"

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
	//void finishProcess();
	bool CPUProcessRequest(int CPUid); // to be used by CPUSerf whenever it's ready to take in a new process
	void shutdown();
	void run();
	std::tuple<float, int, int> findCoresUsed();

private:

	Scheduler();
	~Scheduler() = default;
	Scheduler(Scheduler const&) {}; //copy constructor is private
	void hireCPUSerfs(int cores);
	void fireSerfs();
	void ProcessQueuer();
	void ProcessGiver();
	void processListCopyUpdater();
	//adds process to processqueue
	int ProcessWaitingChecker(); //checks if there are processes waiting to be added to processQueue
	void FinishedProcessVectorUpdater();


	Scheduler& operator=(Scheduler const&) {}; //assignment operator is private
	static Scheduler* SCHEDULER_FOR_THE_STREETS; //singleton instance
	//processqueue is a sharedptr now between scheduler and its scheduling type
	bool initialized = false;
	bool running;
	std::mutex processQueueMutex;


	// we don't have a vector of shared_ptr processes in Console Manager, so a solution is to have a processList copy in Sched
	// then after that, compare with our processCounter here in Scheduler and compare with copied processList
	std::shared_ptr<std::vector<std::shared_ptr<Process>>> processListCopy; //when process added to processList @ ConsoleManager, add it to processQueue
	//to include CPUSerf vector
	std::vector<std::shared_ptr<CPUSerf>> cpuList;
	//to be used by ProcessQueuer() whenever this->processCounter < processQueue->size()
	std::queue<std::shared_ptr<Process>> processQueue;

	std::vector<bool> finishedProcesses; //to be skipped in ProcessWaitingChecker
	
	//AScheduler scheduler;

	//config settings
	//include mutex for processQueue

	int cores;
	int processCounter;
	SchedulingAlgo schedulingAlgo;
	uint32_t quantumCycles, batchProcessFreq, minIns, maxIns, delays;
};

