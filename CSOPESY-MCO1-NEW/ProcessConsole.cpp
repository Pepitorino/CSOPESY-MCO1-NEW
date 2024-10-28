#include "ProcessConsole.h"
#include "ConsoleManager.h"

ProcessConsole::ProcessConsole() : AConsole("PROCESS_CONSOLE") {

}

void ProcessConsole::keyboardPolling() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 13) {
            String input = this->command;
            this->toPrint.push_back("Enter command: " + this->command + '\n');
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
        String processName = "Process Name: " + this->displayedProcess->getName() + "\n";
        String pid = "ID: " + std::to_string(this->displayedProcess->getPid()) + "\n\n";
        String currentLine = "Current instruction line: " + std::to_string(this->displayedProcess->getProcessProgress()) + "\n";
        String linesCode = "Lines of code: " + std::to_string(this->displayedProcess->getLines()) + "\n";
        this->toPrint.push_back(processName);
        this->toPrint.push_back(pid);
        this->toPrint.push_back(currentLine);
        this->toPrint.push_back(linesCode);
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
    for (String string : toPrint) {
        std::cout << string;
    }
    std::cout << "Enter command: " << this->command;
    Sleep(5);
}
