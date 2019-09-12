#include "ntdllapi.h"

// Load NTDLL Library and get entry addresses
HMODULE NTDLLAPI::loadNTDLLFunctions()
{
	HMODULE hNtDll = LoadLibrary(_T("ntdll.dll"));
	if (hNtDll == NULL) return NULL;

	gNtQuerySystemInformation = (pfnNtQuerySystemInformation)GetProcAddress(hNtDll,
		"NtQuerySystemInformation");

	gNtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll,
		"NtQueryInformationProcess");

	if (gNtQuerySystemInformation == NULL || gNtQueryInformationProcess == NULL) {
		FreeLibrary(hNtDll);
		return NULL;
	}

	return hNtDll;
}

// Unloads the NTDLL.DLL and resets the ntdll methods
void NTDLLAPI::freeNTDLLFunctions(HMODULE hNtDll)
{
	if (hNtDll)
		FreeLibrary(hNtDll);
	gNtQueryInformationProcess = NULL;
	gNtQuerySystemInformation = NULL;
}
