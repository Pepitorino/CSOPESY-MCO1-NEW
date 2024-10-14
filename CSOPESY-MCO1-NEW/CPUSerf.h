#pragma once
#include "ThreadClass.h"
#include "Process.h"
#include "TypeDefRepo.h"
#include "PrintCommand.h"
#include "ProcessCommandOutput.h"

class CPUSerf : public ThreadClass
{
public:

	CpuSerf(int coreId); //when constructed, no available process is possible
	~CpuSerf() = default;
	void tick();


	//dont know how to implement yet
	void switchProcess(std::shared_ptr<Process>);
	void setRunning(bool running);
private:
	int coreId;
	std::shared_ptr<Process> process;
	bool SerfisReady; //initialized true
	bool SerfisAvailable; //initialized true
};