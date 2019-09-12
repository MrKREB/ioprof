#define _CRT_SECURE_NO_WARNINGS
#include "ioprof.h"


IoProf::IoProf()
{
	enableTokenPrivilege(SE_DEBUG_NAME);
	hNtDll = ntdllapi.loadNTDLLFunctions();
}


IoProf::~IoProf()
{
	ntdllapi.freeNTDLLFunctions(hNtDll);
}

// Get list of processes
HANDLE IoProf::getProcessList(PROCESSENTRY32 *pe32) {
	HANDLE hProcessSnap;


	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap) return INVALID_HANDLE_VALUE;

	pe32->dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, pe32))
	{
		CloseHandle(hProcessSnap);
		printf("!!! Failed to gather information on system processes! \n");
		return(INVALID_HANDLE_VALUE);
	}

	return hProcessSnap;
}

// Get process id by image name
DWORD IoProf::findProcessId(char processname[])
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	DWORD result = -1;

	hProcessSnap = getProcessList(&pe32);

	do
	{
		if (0 == strcmp(processname, pe32.szExeFile))
		{
			result = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return result;
}

// Enable a privilege for a process token
ProgCodes IoProf::enableTokenPrivilege(LPCTSTR pszPrivilege)
{
	HANDLE hToken = 0;
	TOKEN_PRIVILEGES tkp = { 0 };
	size_t sz = sizeof(TOKEN_PRIVILEGES);

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
		&hToken))
	{
		return unableToEnablePrivilege;
	}

	if (LookupPrivilegeValue(NULL, pszPrivilege,
		&tkp.Privileges[0].Luid))
	{
		tkp.PrivilegeCount = 1;   
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		AdjustTokenPrivileges(hToken, FALSE, &tkp, sz,
			(PTOKEN_PRIVILEGES)NULL, 0);

		if (GetLastError() != ERROR_SUCCESS)
			return unableToEnablePrivilege;

		return success;
	}

	return unableToEnablePrivilege;
}


// Get information about all processes in system
ProgCodes IoProf::getNtProcessesInfo(std::vector<PROCESSINFO> *ppi)
{
	ULONG dwSize = 0;
	PSYSTEM_PROCESS_INFORMATION sysData = NULL;

	PROCESSINFO spi = { 0 };

	NTSTATUS dwStatus = ntdllapi.gNtQuerySystemInformation(
		SystemProcessInformation,
		0,
		0,
		&dwSize);

	if (dwSize == 0)
		return unableToGetInfo;

	std::unique_ptr<byte[]> pMemory(new byte[dwSize]);

	sysData = (PSYSTEM_PROCESS_INFORMATION)pMemory.get();

	dwStatus = ntdllapi.gNtQuerySystemInformation(
		SystemProcessInformation,
		sysData,
		dwSize,
		&dwSize);

	if (dwStatus != STATUS_SUCCESS )
		return unableToGetInfo;

	int sz;
	char* buf;

	while (sysData->NextEntryOffset) {
		sysData = (PSYSTEM_PROCESS_INFORMATION)((LPBYTE)sysData + sysData->NextEntryOffset);
		sz = (int)(sysData->ImageName.Length / sizeof(WCHAR))+1;
		buf = new char[sz];
		sprintf(buf, "%ws", sysData->ImageName.Buffer);

		spi.dwPID = (DWORD)sysData->UniqueProcessId;
		spi.ImageName = buf;
		delete[] buf;
		spi.ioCounters.OtherOperationCount = sysData->OtherOperationCount.QuadPart;
		spi.ioCounters.OtherTransferCount = sysData->OtherTransferCount.QuadPart;
		spi.ioCounters.ReadOperationCount = sysData->ReadOperationCount.QuadPart;
		spi.ioCounters.ReadTransferCount = sysData->ReadTransferCount.QuadPart;
		spi.ioCounters.WriteOperationCount = sysData->WriteOperationCount.QuadPart;
		spi.ioCounters.WriteTransferCount = sysData->WriteTransferCount.QuadPart;

		spi.AllBytes = spi.ioCounters.OtherTransferCount + spi.ioCounters.ReadTransferCount + spi.ioCounters.WriteTransferCount;
		spi.AllOpsCnt = spi.ioCounters.OtherOperationCount + spi.ioCounters.ReadOperationCount + spi.ioCounters.WriteOperationCount;

		ppi->push_back(spi);
	}

	return success;
}


// Get information about one process
ProgCodes IoProf::getNtProcessInfo(const DWORD dwPID, PROCESSINFO *ppi)
{
	DWORD dwSize = 0;
	PROCESSINFO spi = { 0 };

	ZeroMemory(&spi, sizeof(spi));

	// Attempt to access process
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ, FALSE, dwPID);
	if (hProcess == NULL) {
		return procNotFound;
	}

	spi.dwPID = dwPID;

	NTSTATUS dwStatus = ntdllapi.gNtQueryInformationProcess(hProcess,
		ProcessIoCounters,
		0,
		0,
		&dwSize);

	if (dwSize == 0) {
		dwSize = sizeof(IO_COUNTERS);
	}

	dwStatus = ntdllapi.gNtQueryInformationProcess(hProcess,
		ProcessIoCounters,
		&spi.ioCounters,
		dwSize,
		&dwSize);

	if (dwStatus != STATUS_SUCCESS) {
		CloseHandle(hProcess);
		return unableToGetInfo;
	}
		
	CloseHandle(hProcess);

	spi.AllBytes = spi.ioCounters.OtherTransferCount + spi.ioCounters.ReadTransferCount + spi.ioCounters.WriteTransferCount;
	spi.AllOpsCnt = spi.ioCounters.OtherOperationCount + spi.ioCounters.ReadOperationCount + spi.ioCounters.WriteOperationCount;

	*ppi = spi;

	return success;
}