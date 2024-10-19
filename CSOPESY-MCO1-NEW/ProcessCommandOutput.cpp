#include "ProcessCommandOutput.h"
#include "TypeDefRepo.h"

//when this is called by CPUSerf, it must input the necessary details
ProcessCommandOutput::ProcessCommandOutput(int coreId, time_t timeOfExecution, String input_OutputfromProcess)
{
	this->coreId = coreId;
	this->timeOfExecution = timeOfExecution;

	//as of this time, the outputType is only PRINT
	// SOON: To add in ProcessCommandOutput(+int outputType)
	this->outputType = OutputType::PRINT;

	time_t startTime = timeOfExecution;

	// Convert time_t to tm struct for local time
	struct tm localTime;
	localtime_s(&localTime, &startTime);

	// Create a buffer to hold the formatted time string
	char timeBuffer[80];
	strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y %I:%M:%S%p", &localTime);
	std::cout << "Time Started: " << timeBuffer << std::endl;
	String timeBuffer2 = timeBuffer;
	String string_output = "(" + timeBuffer2 + ")" + "Core:" + std::to_string(coreId) + " " + input_OutputfromProcess;

	this->Output = string_output;
}