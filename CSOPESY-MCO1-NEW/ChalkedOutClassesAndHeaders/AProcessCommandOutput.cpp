#include "AProcessCommandOutput.h"
#include "TypeDefRepo.h"

AProcessCommandOutput::AProcessCommandOutput(int coreId, time_t timeOfExecution)
{
	this->coreId = coreId;
	this->timeOfExecution = timeOfExecution;
}