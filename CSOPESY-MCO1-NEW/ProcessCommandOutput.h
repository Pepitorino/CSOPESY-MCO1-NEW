#pragma once
#include "TypeDefRepo.h"

class ProcessCommandOutput
{
public:
	ProcessCommandOutput(int coreId, time_t timeOfExecution, String input_OutputfromProcess);
	~ProcessCommandOutput() = default;

	//to be added
	enum OutputType
	{
		PRINT,
		IO
	};

	time_t getTimeOfExecution();

private:
	int coreId;
	time_t timeOfExecution;
	String Output;
	OutputType outputType;
};
