#include <stdexcept>
#include <iostream>

#include "snapshot.h"



HANDLE Snapshot::run()
{
	m_snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (m_snapshotHandle == nullptr)
	{
		throw std::runtime_error("Unable to get handle for CreateToolhelp32Snapshot");
	}
}


int Snapshot::getProcessIdFromName(const std::wstring nameToFind)
{
	PROCESSENTRY32 currentProcess;
	currentProcess.dwSize = sizeof(currentProcess);
	if (!Process32First(m_snapshotHandle, &currentProcess))
	{
		//TODO ERROR
		throw std::runtime_error("Unable to get first process in snapshot");
	}
	std::wstring currentProcessName(currentProcess.szExeFile);
	//std::wcout << "Current process name is " << currentProcessName << " and the PID is " << currentProcess.th32ProcessID << std::endl;

	//go  through all processes in case there are multiple instances
	while (Process32Next(m_snapshotHandle, &currentProcess))
	{
		currentProcessName = currentProcess.szExeFile;
		if (currentProcessName == nameToFind)
		{
			std::wcout << "Found the process" << std::endl;
			std::wcout << "Current process name is " << currentProcessName << " and the PID is " << currentProcess.th32ProcessID << std::endl;
			m_proccessID = currentProcess.th32ProcessID;
		}
		

	}

	//for now this will only find 1 process even though there might be more with same name.
	// 
	// //go  through all processes in case there are multiple instances
	//while (Process32Next(m_snapshotHandle, &currentProcess))
	//{
	//	currentProcessName = currentProcess.szExeFile;
	//	if (currentProcessName == name)
	//	{
	//		std::wcout << "Found the process" << std::endl;
	//		m_foundProcesses.push_back(currentProcess);
	//	}
	//	std::wcout << "Current process name is " << currentProcessName << " and the PID is " << currentProcess.th32ProcessID << std::endl;

	//}

	return m_proccessID;
}
