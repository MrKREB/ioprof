#ifndef _IOPROF_H_
#define _IOPROF_H_

#pragma once

#include "ntdllapi.h"
#include <memory>

class IoProf
{
private:

	NTDLLAPI ntdllapi;
	HMODULE hNtDll;
	ProgCodes enableTokenPrivilege(_In_ LPCTSTR pszPrivilege);

public:

	IoProf();
	~IoProf();
	DWORD findProcessId(char processname[]);
	HANDLE getProcessList(PROCESSENTRY32 *pe32);
	ProgCodes getNtProcessInfo(_In_ const DWORD dwPID, _Out_ PROCESSINFO *ppi);
	ProgCodes getNtProcessesInfo(std::vector<PROCESSINFO> *ppi);
};

#endif
