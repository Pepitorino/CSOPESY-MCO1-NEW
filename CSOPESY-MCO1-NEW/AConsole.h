#pragma once
#include "TypeDefRepo.h"
#include "Process.h"

class AConsole
{
public:
	AConsole(String name);
	~AConsole() = default;

protected:
	String getName();
	//2 different on enableds, kind of inefficient but its whats needed
	//having a separate function to switch to the processconsole still seems better than
	//having a buncha extra consoles, seems like a waste of bytes
	virtual void onEnabled() = 0; //called when the console is run the first time
	virtual void onEnabled(std::shared_ptr<Process> process) = 0;
	virtual void process() = 0; // any algo, computation
	virtual void drawConsole() = 0; // virtual void display() = 0, called per frame (aka in main menu, called per call of CM instance_;

private:
	String name;
	friend class ConsoleManager;
};

