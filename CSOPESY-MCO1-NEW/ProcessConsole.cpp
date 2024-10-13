#include "ProcessConsole.h"

ProcessConsole::ProcessConsole() : AConsole("PROCESS_CONSOLE") {

}

void ProcessConsole::keyboardPolling() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 13) {
            String input = this->command;
            this->toPrint.push_back("Enter command:" + this->command + '\n');
            this->command = "";
            if (input != "") this->commands(input);
        }
        else if (key == 8) {
            if (this->command.length() > 0) {
                this->command.pop_back();
            }
        }
        else {
            this->command.push_back(key);
        }
    }
}

void ProcessConsole::commands() {

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
