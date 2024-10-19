#pragma once
#include "TypeDefRepo.h"

class ICommand
{
public:
	enum CommandType
	{
		IO,
		PRINT
	};

	ICommand(CommandType commandType);
	CommandType getCommandType();
	virtual String execute() = 0;

protected:
	CommandType commandType;
};

