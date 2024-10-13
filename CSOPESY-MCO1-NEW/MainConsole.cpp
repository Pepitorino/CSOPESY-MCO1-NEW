#include "MainConsole.h"
#include "TypeDefRepo.h"
#include <windows.h>

void MainConsole::title() {
	std::cout << "============================================================================\n";
	std::cout << " ______     ______     ______     ______   ______     ______     __  __    \n";
	std::cout << "/\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\\n";
	std::cout << "\\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  _-/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  \n";
	std::cout << " \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ \n";
	std::cout << "  \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/\n";
	std::cout << " By (S16):\n Ambrosio, Carlos Felipe \n Del Rosario, Lance Victor\n Magabo, Julianna Margaux\n";
	std::cout << "============================================================================\n";
}

MainConsole::MainConsole() : AConsole("MAIN_CONSOLE") {

}

void MainConsole::process() {
	this->keyboardPolling();
	this->drawConsole();
}

void MainConsole::keyboardPolling() {
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

void MainConsole::onEnabled() {
    system("cls");
    title();
}

void MainConsole::drawConsole() {
    system("cls");
    title();
    std::cout << "Input \'exit\' to leave the Main Console. Input \'clear\' to clear the Main Console" << std::endl;

    std::cout << "\nEnter command: " << this->command;
}

void MainConsole::commands(String input) {
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

    if (args[0] == "initialize") {

    }
    else if (args[0] == "help") {

    }
    else if (args[0] == "scheduler-test") {

    }
    else if (args[0] == "scheduler-stop") {

    }
    else if (args[0] == "report-util") {

    }
    else if (args[0] == "clear") {

    }
    else if (args[0] == "exit") {

    }
    else {

    }

}

void MainConsole::onEnabled(std::shared_ptr<Process> process) {}
