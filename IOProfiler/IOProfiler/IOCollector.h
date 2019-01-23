#pragma once
class IOCollector
{
public:
	IOCollector();
	~IOCollector();
	BOOL GetProcessList();
	BOOL ListProcessModules(DWORD dwPID);
	BOOL ListProcessThreads(DWORD dwOwnerPID);
	UINT64 procTimeInS(FILETIME &ft);
	void printError(CONST TCHAR* msg);
};

