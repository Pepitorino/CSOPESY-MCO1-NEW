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

private:
	int coreId;
	time_t timeOfExecution;
	String Output;
	OutputType outputType;
};
