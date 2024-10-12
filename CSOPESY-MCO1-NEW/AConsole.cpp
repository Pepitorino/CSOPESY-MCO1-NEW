#include "AConsole.h"

String AConsole::getName()
{
	// return name
	return this->name;
}

//Aconsole constructor
AConsole::AConsole(String name)
{
	this->name = name;
}