#include "pch.h"
#include "IOCollector.h"

#define _SECOND ((UINT64) 10000000)
#define TIMEDIF ((UINT64) 11644473600)


IOCollector::IOCollector() {
	GetProcessList();
}


IOCollector::~IOCollector() {
}

BOOL IOCollector::GetProcessList() {
	HANDLE hProcessSnap, hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	PIO_COUNTERS lpIoCounters = new IO_COUNTERS;
	FILETIME CreationTime, ExitTime, KernelTime, UserTime;
	UINT64 procLiveTime;
	BOOL procIO = FALSE, procTime = FALSE;


	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		printError(L"CreateToolhelp32Snapshot (of processes)");
		return(FALSE);
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		printError(L"Process32First");
		CloseHandle(hProcessSnap);
		return(FALSE);
	}

	do
	{
		_tprintf(L"\n\n=====================================================");
		_tprintf(L"\nPROCESS NAME:  %s", pe32.szExeFile);
		_tprintf(L"\n-------------------------------------------------------");

		dwPriorityClass = 0;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		if (hProcess == NULL)
			printError(L"OpenProcess");
		else {
			procIO = GetProcessIoCounters(hProcess, lpIoCounters);
			procTime = GetProcessTimes(hProcess, &CreationTime, &ExitTime, &KernelTime, &UserTime);

			if (!procIO)
				printError(L"GetProcessIoCounters");

			if (!procTime)
				printError(L"GetProcessTimes");

			CloseHandle(hProcess);
		}

		_tprintf(L"\n  Process ID        = 0x%08X", pe32.th32ProcessID);
		_tprintf(L"\n  Thread count      = %d", pe32.cntThreads);
		_tprintf(L"\n  Parent process ID = 0x%08X", pe32.th32ParentProcessID);

		if (procIO && procTime) {
			procLiveTime = (ULONGLONG) time(0) - procTimeInS(CreationTime);

			if (procLiveTime) {
				_tprintf(L"\n  I/O data:");
				_tprintf(L"\n  read operations  = %I64d", lpIoCounters->ReadOperationCount);
				_tprintf(L"\n  write operations = %I64d", lpIoCounters->WriteOperationCount);
				_tprintf(L"\n  rb/s             = %I64d", lpIoCounters->ReadTransferCount / procLiveTime);
				_tprintf(L"\n  wb/s             = %I64d", lpIoCounters->WriteTransferCount / procLiveTime);
			}
		}

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return(TRUE);
}

ULONGLONG IOCollector::procTimeInS(FILETIME &ft) {
	ULONGLONG qwResult;
	qwResult = (((ULONGLONG)ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
	return qwResult / _SECOND - 11644473600;
}

void IOCollector::printError(CONST TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	_tprintf(L"\n  WARNING: %s failed with error %d (%s)", msg, eNum, sysMsg);
}