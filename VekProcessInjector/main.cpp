#include <iostream>
#include <Windows.h>
#include <array>
#include <string>
#include <thread>
#include <mutex>
#include "gui.h"
#include "injector.h"
#include "config.h"
#include "thread.h"

using std::cout;
using std::endl;
using std::cin;


std::mutex guiLock;

Config injectorConfig{};
Gui guiInstance{};

int main()
{
	//initiate GUI in new thread, which can initiate and detach threads for injections
	std::thread guiWorker(&Gui::run, &guiInstance, std::ref(injectorConfig));
	guiWorker.join();

	return 0;
}

