#pragma once
#include "AConsole.h"
#include "TypeDefRepo.h"
class MainConsole : public AConsole
{
public:
	MainConsole();
	void title();
	void keyboardPolling();
	void commands(String command);
	void onEnabled() override;
	void process() override;
	void drawConsole() override;

private:
	void onEnabled(std::shared_ptr<Process> process);
	String command;
	std::vector<String> toPrint;
	bool refreshed = false;
};

