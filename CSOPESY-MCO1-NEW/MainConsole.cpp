#include "MainConsole.h"
#include "TypeDefRepo.h"

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
            //handle command
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
