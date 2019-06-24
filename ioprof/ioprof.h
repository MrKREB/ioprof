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
	BOOL sm_EnableTokenPrivilege(_In_ LPCTSTR pszPrivilege);
	BOOL sm_GetNtProcessInfo(_In_ const DWORD dwPID, _Out_ smPROCESSINFO *ppi);
	DWORD findProcessId(const char *processname);
	HANDLE getProcessList(PROCESSENTRY32 *pe32);
	std::vector<smPROCESSINFO> sm_GetNtProcessesInfo();
};

#endif	// _IOPROF_NTPROCESSINFO_H_