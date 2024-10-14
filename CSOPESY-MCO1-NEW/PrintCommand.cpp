#include "PrintCommand.h"
#include "TypedefRepo.h"

PrintCommand::PrintCommand(String toPrint, CommandType commType) : ICommand (commType)
{
	this->toPrint = toPrint;
}

String PrintCommand::executePrint()
{
	return this->toPrint;
}

void PrintCommand::execute()
{

	//implementation found to be unnecessary, since CPUSerf would handle the contents to be put into the ProcessCommandOutput

	//should we print this? Nahhh, let's just log it
	//std::cout << "PID" << this->pid << ":" << this->toPrint << std::endl;

	//kaya linagay sa message buffer kasi may times na hindi naman priniprint yung message dapat sa current console

	//std::stringstream msg;
	//msg << String("PID") << this->pid << String(":") << this->toPrint << std::endl;
	//MessageBuffer::getInstance()->log(msg.str(), this->pid);
}