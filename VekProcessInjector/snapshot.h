#pragma once



#pragma once

#include <vector>

#include "Windows.h"
#include "TlHelp32.h"




class Snapshot
{
public:
	Snapshot()
	{
		m_snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (m_snapshotHandle == nullptr)
		{
			throw std::runtime_error("Unable to get handle for CreateToolhelp32Snapshot");
		}
	}


	~Snapshot()
	{
		switch (m_currentSnapshotState)
		{
		case (SnapshotState::SnapshotSuccessful):
			CloseHandle(m_snapshotHandle);
			break;

		default:
			break;
		}
	}

	HANDLE run();

	int getProcessIdFromName(const std::wstring name);


private:
	enum class SnapshotState
	{
		None,
		SnapshotFailed,
		SnapshotSuccessful,
	};

	int m_proccessID = 0;
	//PROCESSENTRY32 m_currentProcess;
	std::vector<PROCESSENTRY32> m_foundProcesses;
	SnapshotState m_currentSnapshotState = SnapshotState::None;
	HANDLE m_snapshotHandle = nullptr;
};