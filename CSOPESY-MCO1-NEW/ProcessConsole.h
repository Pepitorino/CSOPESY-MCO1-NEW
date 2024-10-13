#pragma once
#include "AConsole.h"
#include "TypeDefRepo.h"
#include "Process.h"
class ProcessConsole :
    public AConsole
{
public:
	ProcessConsole();
	void onEnabled() override;
	void onEnabled(std::shared_ptr<Process> process);
	void keyboardPolling();
	void process() override;
	void drawConsole() override;
	void commands(String input);
private:
	std::shared_ptr<Process> displayedProcess;
	String command;
	std::vector<String> toPrint;
	bool refreshed = false;
};

