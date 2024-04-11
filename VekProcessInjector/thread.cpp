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
	if (config.m_processID == 0)
	{
		Injector procInjector(config.getProcessName(), config.getDllName());
		procInjector.injectAfterRunningProc();
	}
	else
	{
		Injector procInjector(config.m_processID, config.getDllName());
		procInjector.injectAfterRunningProc();
	}


}