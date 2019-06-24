#ifndef _IOPROF_H_
#define _IOPROF_H_

#pragma once

#include "ntdllapi.h"
#include <vector>

class IoProf
{
private:

	NTDLLAPI ntdllapi;
	HMODULE hNtDll;

public:

	IoProf();
	~IoProf();
	BOOL enableTokenPrivilege(_In_ LPCTSTR pszPrivilege);
	BOOL getNtProcessInfo(_In_ const DWORD dwPID, _Out_ PROCESSINFO *ppi);
	DWORD findProcessId(char processname[]);
	HANDLE getProcessList(PROCESSENTRY32 *pe32);
	std::vector<PROCESSINFO> getNtProcessesInfo();
};

#endif
