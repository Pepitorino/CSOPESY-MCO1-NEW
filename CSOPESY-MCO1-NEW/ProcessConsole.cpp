#include "ProcessConsole.h"

ProcessConsole::ProcessConsole() : AConsole("PROCESS_CONSOLE") {

}

void ProcessConsole::onEnabled() {
	return;
}

void ProcessConsole::onEnabled(std::shared_ptr<Process> process) {
	this->process = process;
}
