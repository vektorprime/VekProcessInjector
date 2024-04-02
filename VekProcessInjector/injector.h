#pragma once

#include <string>
#include <Windows.h>
#include <iostream>


//RAII encapsulated WINAPI for injection before and after process creation
//beforce process creation starts process as suspended, this ensures the first thing done is DLL injection

enum class InjectType
{
	None,
	BeforeRunningProcess,
	AfterRunningProcess
};

enum class InjectState
{
	None,
	OpenProcessFailed,
	CreateProcessFailed,
	VirtualAllocFailed,
	WriteProcessMemoryFailed,
	GetModuleHandleFailed,
	CreateRemoteThreadFailed,
	Finished
};



class Injector
{

public:
	Injector(std::wstring processDirectory, std::wstring commandArgument, std::wstring dllName) :
		m_processDirectory(processDirectory),
		m_commandArgument(commandArgument),
		m_dllName(dllName)
	{
		ZeroMemory(&m_procStartupInfo, sizeof(m_procStartupInfo));
		m_procStartupInfo.cb = sizeof(m_procInfo);
	}
	Injector(DWORD processID, std::wstring dllName) :
		m_processID(processID),
		m_dllName(dllName)
	{
		ZeroMemory(&m_procStartupInfo, sizeof(m_procStartupInfo));
		m_procStartupInfo.cb = sizeof(m_procInfo);
	}
	~Injector()
	{
		if (m_injectorType == InjectType::BeforeRunningProcess)
		{
			switch (m_injectorState)
			{
			case InjectState::None:
				break;

			case InjectState::CreateProcessFailed:
				break;

			case InjectState::VirtualAllocFailed:
				WaitForSingleObject(m_procInfo.hProcess, INFINITE);
				//close handle in remote proc and thread
				CloseHandle(m_processHandle);
				CloseHandle(m_procInfo.hThread);
				break;

			case InjectState::WriteProcessMemoryFailed:
				//free memory in remote proc
				VirtualFreeEx(m_processHandle, m_remoteBuffer, 0, MEM_RELEASE);
				WaitForSingleObject(m_procInfo.hProcess, INFINITE);
				//close handle in remote proc and thread
				CloseHandle(m_processHandle);
				CloseHandle(m_procInfo.hThread);
				break;
			case InjectState::GetModuleHandleFailed:
				//free memory in remote proc
				VirtualFreeEx(m_processHandle, m_remoteBuffer, 0, MEM_RELEASE);
				//close handle in remote proc and thread
				CloseHandle(m_processHandle);
				CloseHandle(m_kernel32ModuleHandle);
				break;
			case InjectState::CreateRemoteThreadFailed:
				//free memory in remote proc
				VirtualFreeEx(m_processHandle, m_remoteBuffer, 0, MEM_RELEASE);
				WaitForSingleObject(m_procInfo.hProcess, INFINITE);
				//close handle in remote proc and thread
				CloseHandle(m_processHandle);
				CloseHandle(m_procInfo.hThread);
				CloseHandle(m_kernel32ModuleHandle);
				break;
			default:

				//cleanup
				//wait for the thread to return
				WaitForSingleObject(m_remoteThread, INFINITE);
				DWORD threadExitCode = 0;
				GetExitCodeThread(m_remoteThread, &threadExitCode);
				if (threadExitCode == NULL)
				{
					std::cout << "The remote thread was created but returned an error. Error : " << threadExitCode << std::endl;
				}
				//free memory in remote proc
				VirtualFreeEx(m_processHandle, m_remoteBuffer, 0, MEM_RELEASE);
				WaitForSingleObject(m_procInfo.hProcess, INFINITE);
				//close handle in remote proc
				CloseHandle(m_processHandle);
				CloseHandle(m_procInfo.hThread);
				CloseHandle(m_kernel32ModuleHandle);
				break;
			}
		}
		else if (m_injectorType == InjectType::AfterRunningProcess)
		{
			switch (m_injectorState)
			{
			case InjectState::None:
				break;

			case InjectState::OpenProcessFailed:
				break;

			case InjectState::VirtualAllocFailed:
				//close handle in remote proc and thread
				CloseHandle(m_processHandle);
				break;

			case InjectState::WriteProcessMemoryFailed:
				//free memory in remote proc
				VirtualFreeEx(m_processHandle, m_remoteBuffer, 0, MEM_RELEASE);
				//close handle in remote proc and thread
				CloseHandle(m_processHandle);
				break;
			case InjectState::GetModuleHandleFailed:
				//free memory in remote proc
				VirtualFreeEx(m_processHandle, m_remoteBuffer, 0, MEM_RELEASE);
				//close handle in remote proc and thread
				CloseHandle(m_processHandle);
				break;
			case InjectState::CreateRemoteThreadFailed:
				//free memory in remote proc
				VirtualFreeEx(m_processHandle, m_remoteBuffer, 0, MEM_RELEASE);
				//close handle in remote proc and thread
				CloseHandle(m_processHandle);
				CloseHandle(m_kernel32ModuleHandle);
				break;


			default:

				//cleanup
				//wait for the thread to return
				WaitForSingleObject(m_remoteThread, INFINITE);
				DWORD threadExitCode = 0;
				GetExitCodeThread(m_remoteThread, &threadExitCode);
				if (threadExitCode == NULL)
				{
					std::cout << "The remote thread was created but returned an error. Error : " << threadExitCode << std::endl;
				}
				//free memory in remote proc
				VirtualFreeEx(m_processHandle, m_remoteBuffer, 0, MEM_RELEASE);
				//close handle in remote proc
				CloseHandle(m_processHandle);
				CloseHandle(m_kernel32ModuleHandle);
				break;
			}
		}
		else
		{
			//TODO
		}
	
				

	}
	void injectAfterRunningProc();
	void injectBeforeRunningProc();
	void setProcessDirectory(std::wstring directory);
	std::wstring getProcessDirectory();
	void setCommandDirectory(std::wstring argument);
	std::wstring getCommandArgument();
	void setDllName(std::wstring name);
	std::wstring getDllName();

private:
	void virtualAllocBeforeInject();
	void writeProcessMemoryBeforeInject();
	void getModuleHandleBeforeInject();

	InjectState m_injectorState = InjectState::None;
	InjectType m_injectorType = InjectType::None;
	std::wstring m_processDirectory;
	std::wstring m_commandArgument;
	std::wstring m_dllName;
	STARTUPINFO m_procStartupInfo;
	PROCESS_INFORMATION m_procInfo;
	size_t m_dllSize = 0;
	HANDLE m_processHandle = nullptr;
	PVOID m_remoteBuffer = nullptr;
	HMODULE m_kernel32ModuleHandle = nullptr;
	HANDLE m_remoteThread = nullptr;
	DWORD m_processID = 0;
};