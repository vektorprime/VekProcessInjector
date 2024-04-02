#include "injector.h"

#include <iostream>
#include <stdexcept>

void Injector::setProcessDirectory(std::wstring directory)
{
	m_processDirectory = directory;

}

std::wstring Injector::getProcessDirectory()
{
	return m_processDirectory;
}



void Injector::setCommandDirectory(std::wstring argument)
{
	m_commandArgument = argument;
}

std::wstring Injector::getCommandArgument()
{
	return m_commandArgument;
}


void Injector::setDllName(std::wstring name)
{
	m_dllName = name;
}

std::wstring Injector::getDllName()
{
	return m_dllName;
}

void Injector::virtualAllocBeforeInject()
{
	//allocate memory in target proc
	m_remoteBuffer = VirtualAllocEx(m_processHandle, NULL, m_dllSize, MEM_COMMIT, PAGE_READWRITE);

	if (m_remoteBuffer == NULL)
	{
		m_injectorState = InjectState::VirtualAllocFailed;
		throw std::runtime_error("Failed to allocate memory in remote process buffer");
	}
}

void Injector::writeProcessMemoryBeforeInject()
{
	BOOL remoteWrite = WriteProcessMemory(m_processHandle, m_remoteBuffer, m_dllName.c_str(), m_dllSize, NULL);
	if (remoteWrite == false)
	{
		m_injectorState = InjectState::WriteProcessMemoryFailed;
		throw std::runtime_error("Failed to write the dllPath in the remote process buffer");
	}
}

void Injector::getModuleHandleBeforeInject()
{
	m_kernel32ModuleHandle = GetModuleHandle(L"Kernel32");
	if (m_kernel32ModuleHandle == NULL)
	{
		m_injectorState = InjectState::GetModuleHandleFailed;
		throw std::runtime_error("Failed to get a valid Kernel32 module handler");
	}
}

void Injector::injectBeforeRunningProc()
{
	m_injectorType = InjectType::BeforeRunningProcess;
	if (!CreateProcess(NULL, m_commandArgument.data(), NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, m_processDirectory.data(), &m_procStartupInfo, &m_procInfo))
	{
		m_injectorState = InjectState::CreateProcessFailed;
		throw std::runtime_error("Failed to create proc");
	}

	m_dllSize = m_dllName.size() * sizeof(wchar_t);

	m_processHandle = m_procInfo.hProcess;

	//allocate memory in target proc
	virtualAllocBeforeInject();

	//write path of DLL to newly allocated memory in target proc
	writeProcessMemoryBeforeInject();

	//Get a handle to the kernel32.dll module in this process's memory space.
	getModuleHandleBeforeInject();

	//Search the module for a function called LoadLibraryW via the handle,
	//and return the address relative to the base address of the module within this process's memory space.
	//Store this address in an object of type PTHREAD_START_ROUTINE so we can use it later.
	PTHREAD_START_ROUTINE functionToExecuteAddress = reinterpret_cast<PTHREAD_START_ROUTINE>(
		GetProcAddress(
			m_kernel32ModuleHandle,
			"LoadLibraryW"
		));

	//Create a remote thread in the process that we have a handle to.
	//Pass the offset from the module's base address for the function we want to execute in the remote process (LoadLibraryW).
	//Pass the DLL name via the remoteBuffer we filled in previously in the target process's memory space.
	m_remoteThread = CreateRemoteThread(m_processHandle, NULL, 0, functionToExecuteAddress, m_remoteBuffer, 0, NULL);

	if (m_remoteThread == NULL)
	{
		m_injectorState = InjectState::CreateRemoteThreadFailed;
		throw std::runtime_error("Failed to create a thread in the remote process");

	}

	ResumeThread(m_procInfo.hThread);


}


void Injector::injectAfterRunningProc()
{

	m_injectorType = InjectType::AfterRunningProcess;

	m_processHandle = OpenProcess(
		PROCESS_QUERY_INFORMATION |                 // For Alloc
		PROCESS_CREATE_THREAD |                     // For CreateRemoteThread
		PROCESS_VM_OPERATION |                      // For VirtualAllocEx
		PROCESS_VM_WRITE,                           // For WriteProcessMemory
		FALSE,
		m_processID
	);

	if (m_processHandle == NULL)
	{
		m_injectorState = InjectState::OpenProcessFailed;
		throw std::runtime_error("Failed to get handle for remote proc with OpenProcess");
	}

	m_dllSize = m_dllName.size() * sizeof(wchar_t);


	//allocate memory in target proc
	virtualAllocBeforeInject();

	//write path of DLL to newly allocated memory in target proc
	writeProcessMemoryBeforeInject();

	//Get a handle to the kernel32.dll module in this process's memory space.
	getModuleHandleBeforeInject();

	//Search the module for a function called LoadLibraryW via the handle,
	//and return the address relative to the base address of the module within this process's memory space.
	//Store this address in an object of type PTHREAD_START_ROUTINE so we can use it later.
	PTHREAD_START_ROUTINE functionToExecuteAddress = reinterpret_cast<PTHREAD_START_ROUTINE>(
		GetProcAddress(
			m_kernel32ModuleHandle,
			"LoadLibraryW"
		));

	//Create a remote thread in the process that we have a handle to.
	//Pass the offset from the module's base address for the function we want to execute in the remote process (LoadLibraryW).
	//Pass the DLL name via the remoteBuffer we filled in previously in the target process's memory space.
	m_remoteThread = CreateRemoteThread(m_processHandle, NULL, 0, functionToExecuteAddress, m_remoteBuffer, 0, NULL);

	if (m_remoteThread == NULL)
	{
		m_injectorState = InjectState::CreateRemoteThreadFailed;
		throw std::runtime_error("Failed to create a thread in the remote process");

	}

}
