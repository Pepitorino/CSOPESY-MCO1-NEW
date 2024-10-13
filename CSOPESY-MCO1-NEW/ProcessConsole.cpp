#include "ProcessConsole.h"

ProcessConsole::ProcessConsole() : AConsole("PROCESS_CONSOLE") {

}

//NOTHING SHOULD HAPPEN
void ProcessConsole::onEnabled() {
	return;
}

void ProcessConsole::onEnabled(std::shared_ptr<Process> process) {
	this->displayedProcess = process;
}

//should be similar to the mainconsole, havent added the keyboard polling yet
void ProcessConsole::process() {

}

void ProcessConsole::drawConsole() {

}
