#include "PrintCommandOutput.h"
#include "AProcessCommandOutput.h"
#include "TypeDefRepo.h"

//string output is determined by the core that ran the process.
PrintCommandOutput::PrintCommandOutput(int coreId, time_t timeOfExecution, String output) : AProcessCommandOutput(coreId, timeOfExecution)
{
	this->output = output;
}