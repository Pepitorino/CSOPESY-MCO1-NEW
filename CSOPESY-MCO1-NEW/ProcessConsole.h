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
	void process() override;
	void drawConsole() override;
private:
	std::shared_ptr<Process> displayedProcess;
	bool refreshed = false;
};

