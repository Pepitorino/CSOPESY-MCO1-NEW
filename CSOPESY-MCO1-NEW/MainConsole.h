#pragma once
#include "AConsole.h"
class MainConsole : public AConsole
{
public:
	MainConsole();
	void title();
	void onEnabled() override;
	void process() override;
	void drawConsole() override;

private:
	bool refreshed = false;
};

