#pragma once
#include "TypeDefRepo.h"

class ICommand
{
	enum CommandType
	{
		IO,
		PRINT
	};

	ICommand(CommandType commandType);
	CommandType getCommandType();
	virtual void execute() = 0;

protected:
	CommandType commandType;
};

