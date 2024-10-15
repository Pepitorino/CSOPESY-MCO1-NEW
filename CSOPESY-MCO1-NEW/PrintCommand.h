#pragma once
#include "ICommand.h"
#include "TypeDefRepo.h"

class PrintCommand :
    public ICommand
{
public:
	//has CommandType getCommandType(); from ICommand
    PrintCommand(String toPrint, CommandType commType = PRINT);
    void execute() override;
	String executePrint();

private:
    //int pid; //unnecessary, already in CPUSerf when creating
    String toPrint;
};