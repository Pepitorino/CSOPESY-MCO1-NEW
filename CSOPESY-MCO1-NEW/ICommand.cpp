#include "ICommand.h"
#include "TypeDefRepo.h"

ICommand::ICommand(CommandType commandType) {
	this->commandType = commandType;
}

ICommand::CommandType ICommand::getCommandType() {
	return this->commandType;
}