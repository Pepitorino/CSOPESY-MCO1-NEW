#pragma once
#include <iostream>
#include <thread>

class ThreadClass
{
public:
	ThreadClass() = default;
	~ThreadClass() = default;
	void start();
	static void sleep(int ms);

protected:
	virtual void run() = 0;
};