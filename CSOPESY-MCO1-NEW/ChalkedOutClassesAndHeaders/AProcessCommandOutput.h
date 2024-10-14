#pragma once
#include "TypeDefRepo.h"
#include "Process.h"

//should include, run by which core, time of execution, from which process (no need), String output

class AProcessCommandOutput
{
public: 
	AProcessCommandOutput(int coreId, time_t timeOfExecution);
	~AProcessCommandOutput() = default;

protected:
	int coreId;
	time_t timeOfExecution;
}