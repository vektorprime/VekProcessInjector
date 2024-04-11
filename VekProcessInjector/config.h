#pragma once

#include <string>

//central place to hold config that gets copied upon thread creation. Used to avoid passing a reference and dealing with mutex. Also returns wide strings for WINAPI.

class Config
{
public:
	std::string m_processDirectory = "C:\\Users\\Test\\Desktop\\";
	std::string m_commandArgument = "C:\\Users\\Test\\Desktop\\prog.exe";
	std::string m_dllName = "new.dll";
	std::string m_processName = "Example.exe";
	int m_processID = 0;

	std::wstring getProcessDirectory();
	std::wstring getCommandArgument();
	std::wstring getDllName();
	std::wstring getProcessName();
private:
	//std::string m_wideProcessDirectory;
	//std::string m_wideCommandArgument;
	//std::string m_wideDllName;
};