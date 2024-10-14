#pragma once
#include "TypeDefRepo.h"

class PrintCommandOutput : public AProcessCommandOutput
{
public:
	PrintCommandOutput(int coreId, time_t timeOfExecution, String output);
	~PrintCommandOutput() = default;

private:
	String output;
}