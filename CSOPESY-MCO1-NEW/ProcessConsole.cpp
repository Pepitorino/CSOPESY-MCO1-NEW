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

void ProcessConsole::commands(String input) {

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
    this->keyboardPolling();
    this->drawConsole();
}

void ProcessConsole::drawConsole() {
    system("cls");
    std::cout << "Process Name: " << this->displayedProcess->getName() << std::endl;
    std::cout << "ID: " << this->displayedProcess->getPid() << std::endl;
    std::cout << std::endl;
    std::cout << "Current instruction line: " << this->displayedProcess->getProcessProgress() << std::endl;
    std::cout << "Lines of code: " << this->displayedProcess->getLines() << std::endl;

    std::cout << "Enter command: " << this->command;
    Sleep(5);
}
