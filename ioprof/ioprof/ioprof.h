#ifndef _IOPROF_H_
#define _IOPROF_H_

#pragma once

#include "ntdllapi.h"
#include <memory>
#include <tlhelp32.h>
#include <cstdlib>

class IoProf
{
private:

	NTDLLAPI ntdllapi;
	HMODULE hNtDll;
	ProgCodes enableTokenPrivilege(_In_ LPCTSTR pszPrivilege);
	HANDLE getProcessList(PROCESSENTRY32 *pe32);

public:

	IoProf();
	~IoProf();
	DWORD findProcessId(char processname[]);
	ProgCodes getNtProcessInfo(_In_ const DWORD dwPID, _Out_ PROCESSINFO *ppi);
	ProgCodes getNtProcessesInfo(std::vector<PROCESSINFO> *ppi);
};

#endif
