#pragma once
#include "TypeDefRepo.h"
#include "Process.h"
class Scheduler
{
public:
	static void initialize();
	static void destroy();
	static Scheduler* getInstance();

	void tick();
	void addProcess();
	void finishProcess();
private:
	Scheduler(int cores);
	~Scheduler() = default;
	Scheduler(Scheduler const&) {}; //copy constructor is private

	Scheduler& operator=(Scheduler const&) {}; //assignment operator is private
	static Scheduler* SCHEDULER_FOR_THE_STREETS; //singleton instance
	std::shared_ptr<std::vector<std::shared_ptr<Process>>> processQueue; //when process added to processList @ ConsoleManager, add it to processQueue
	//AScheduler scheduler;
};

