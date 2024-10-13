#include "MainConsole.h"
#include "TypeDefRepo.h"
#include <windows.h>
#include "ConsoleManager.h"

void MainConsole::title() {
	std::cout << "============================================================================\n";
	std::cout << " ______     ______     ______     ______   ______     ______     __  __    \n";
	std::cout << "/\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\\n";
	std::cout << "\\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  _-/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  \n";
	std::cout << " \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ \n";
	std::cout << "  \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/\n";
	std::cout << " By (S16):\n Ambrosio, Carlos Felipe \n Del Rosario, Lance Victor\n Magabo, Julianna Margaux\n";
    // leaving ju here as a remembrance
	std::cout << "============================================================================\n";
}

MainConsole::MainConsole() : AConsole("MAIN_CONSOLE") {

}

//uses a marquee console type beat now to render the screen
void MainConsole::process() {
	this->keyboardPolling();
	this->drawConsole();
}

void MainConsole::keyboardPolling() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 13) {
            String input = this->command;
            this->toPrint.push_back("\nEnter command:" + this->command + '\n');
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

    // uses toPrint to print old things
    // honestly if i knew how to use windows.h
    // that would probably be more effective but 
    // it doesnt work on my machine for whatever reason
    for (String string : toPrint) {
        std::cout << string;
    }

    std::cout << "\nEnter command:" << this->command;
    Sleep(5);
}

//TODO:handle commands
//just add the commands + whatever was before to the "toPrint" variable
void MainConsole::commands(String input) {
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

    if (args[0] == "initialize") {
        //check if already initialized
        if (!(consoleManagerInstance->checkInitialized())) {
            ConsoleManager::initProgram();
            this->toPrint.push_back("Initialized!");
        }
    }
    else if (args[0] == "help") {
        String help = "Here are the possible commands:\n\'initialize\' - Initializes the system\n\'screen\' - Displays the screen\n\'scheduler-test\' - Tests the scheduler\n\'scheduler-stop\' - Stops the scheduler\n\'report-util\' - Generates a report\n\'clear\' - Clears the screen\n\'exit\' - Exits the program";
        this->toPrint.push_back(help + '\n');
    }
    else if (args[0] == "scheduler-test") {

    }
    else if (args[0] == "scheduler-stop") {

    }
    else if (args[0] == "report-util") {

    }
    else if (args[0] == "clear") {
        this->toPrint.clear();
    }
    else if (args[0] == "exit") {
        ConsoleManager::end();
    }
    else if (args[0] == "screen") {

    }
    else {

    }

}

void MainConsole::onEnabled(std::shared_ptr<Process> process) {}
