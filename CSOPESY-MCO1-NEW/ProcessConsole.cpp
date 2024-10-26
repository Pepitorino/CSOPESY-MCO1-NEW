#include "ProcessConsole.h"
#include "ConsoleManager.h"

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
    ConsoleManager* consoleManagerInstance = ConsoleManager::getInstance();
    std::vector<String> args;
    String delimiter = " ";
    size_t pos = 0;

    while ((pos = input.find(delimiter)) != String::npos) {
        String arg = input.substr(0, pos);
        args.push_back(arg);
        input.erase(0, pos + delimiter.length());
    }

    if (!input.empty()) {
        args.push_back(input);
    }

    if (args.empty()) {
        std::cerr << "Error: No command entered." << std::endl;
        return;
    }

    if (args[0] == "process-smi") {

    }
    else if (args[0] == "exit") {
        consoleManagerInstance->switchMainConsole();
    }
    else {
        this->toPrint.push_back("Command not recognized, please try again.\n");
    }
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
