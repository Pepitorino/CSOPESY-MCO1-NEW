#pragma once
#include "AConsole.h"
class MainConsole : public AConsole
{
public:
	MainConsole();
	void title();
	void keyboardPolling();
	void commands();
	void onEnabled() override;
	void process() override;
	void drawConsole() override;

private:
	//function pointers
	std::string command;
	bool refreshed = false;
};

