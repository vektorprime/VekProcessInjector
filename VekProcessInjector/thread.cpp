#include "thread.h"
#include "injector.h"

//used as a bridge to launch different kinds of injection threads

void startInjectionThreadBeforeProc(Config config)
{
	Injector procInjector(config.getProcessDirectory(), config.getCommandArgument(), config.getDllName());
	procInjector.injectBeforeRunningProc();
}

void startInjectionThreadAfterProc(Config config)
{
	Injector procInjector(config.m_processID, config.getDllName());
	procInjector.injectAfterRunningProc();
}