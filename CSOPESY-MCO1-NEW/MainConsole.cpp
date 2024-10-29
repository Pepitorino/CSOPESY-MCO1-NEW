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
    // LV [10/13/24]: <3 respectful and understanding decision from her
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
	// should also clear toPrint
	this->toPrint.clear();
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
        if (!(consoleManagerInstance->checkInitialized())) {
            ConsoleManager::initProgram();
            this->toPrint.push_back("Initialized!\n");
            return;
        }
        else {
            this->toPrint.push_back("Already initialized!\n");
            return;
        }
    }
    else if (args[0] == "clear") {
        this->toPrint.clear();
        return;
    }
    else if (args[0] == "exit") {
        ConsoleManager::end();
        return;
    }
    else if (args[0] == "help") {
        String help = "Here are the possible commands:\n\'initialize\' - Initializes the system\n\'screen\' - Displays the screen\n\'scheduler-test\' - Tests the scheduler\n\'scheduler-stop\' - Stops the scheduler\n\'report-util\' - Generates a report\n\'clear\' - Clears the screen\n\'exit\' - Exits the program";
        this->toPrint.push_back(help + '\n');
        return;
    }

    if (!consoleManagerInstance->checkInitialized()) {
        this->toPrint.push_back("Not yet initialized!\n");
        return;
    }

	else if (args[0] == "scheduler-test") {
		//to be continued
        this->toPrint.push_back("Dummy process generation starting...\n");

        //to include yung printing out the list of processes
        consoleManagerInstance->setCreateBatches(true);

    }
    else if (args[0] == "scheduler-stop") {
        //to be continued
        this->toPrint.push_back("Dummy process generation stopping...\n");
        consoleManagerInstance->setCreateBatches(false);
    }
    else if (args[0] == "report-util") {
		//to be continued
        std::vector<String> outputList = consoleManagerInstance->obtainProcessDetails();
        std::fstream ofs;
        ofs.open("csopesy-log.txt", std::ios::out | std::ios::trunc);
        for (String string : outputList) ofs << string;
        ofs.close();
        this->toPrint.push_back("Report generated at csopesy-log.txt\n");
    }
    else if (args[0] == "screen") {
        if (args.size() < 2) {
            this->toPrint.push_back("Error: No process action key entered.\n");
            return;
        }
        else if (args[1] == "-r") {
            if (args.size() < 3) {
                this->toPrint.push_back("Error: No process name entered.\n");
                return;
            }
            else {
                if (!consoleManagerInstance->DoesProcessExist(args[2])) {
                    this->toPrint.push_back("Error: Process does not exist");
                }
                else {
                    consoleManagerInstance->switchProcessConsole(args[2]);
                }
            }
            //std::cout << "screen -r" << " command recognized. Doing something." << std::endl;
			//check if process exists, if no, call ConsoleManager command to create Process.
        }
        else if (args[1] == "-s") {
            if (args.size() < 3) {
                this->toPrint.push_back("Error: No process name entered.\n");
                return;
            }

            if (consoleManagerInstance->DoesProcessExist(args[2])) {
                this->toPrint.push_back("Error: Process already exists\n");
            }
            else {
                consoleManagerInstance->addProcess(args[2]);
                consoleManagerInstance->switchProcessConsole(args[2]);
            }
            // create process

            //call ConsoleManager to create Process
        }
        else if (args[1] == "-ls") {
			//use obtain string vector returned by ConsoleManager, then add to toPrint each element of string vector received
			std::cout << "screen -ls" << " command recognized. Doing something." << std::endl;
			//use consoleManager to get list of String vectors
			std::vector<String> outputList = consoleManagerInstance->obtainProcessDetails();
			for (String line : outputList) {
				this->toPrint.push_back(line);
			}
        }

        else if (args.size() < 3) {
            this->toPrint.push_back("Error: unrecognized action key");
        }

		else {
			this->toPrint.push_back("Error: 'screen' action entered has too many arguments.\n");
		}
    } // for screen commands
    else {
        this->toPrint.push_back("Command not recognized, please try again.\n");
    }

}

void MainConsole::onEnabled(std::shared_ptr<Process> process) {

}
