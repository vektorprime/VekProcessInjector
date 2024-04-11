#include "config.h"
#include "util.h"

//central place to hold config that gets copied upon thread creation. Used to avoid passing a reference and dealing with mutex. Also returns wide strings for WINAPI.


std::wstring Config::getProcessDirectory()
{
	return convertToWstring(m_processDirectory);
}

std::wstring Config::getCommandArgument()
{
	return convertToWstring(m_commandArgument);
}

std::wstring Config::getDllName()
{
	return convertToWstring(m_dllName);
}


std::wstring Config::getProcessName()
{
	return convertToWstring(m_processName);
}