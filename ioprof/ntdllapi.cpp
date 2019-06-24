#include "ntdllapi.h"

// Load NTDLL Library and get entry addresses
HMODULE NTDLLAPI::sm_LoadNTDLLFunctions()
{
	HMODULE hNtDll = LoadLibrary(_T("ntdll.dll"));
	if (hNtDll == NULL) return NULL;

	gNtQuerySystemInformation = (pfnNtQuerySystemInformation)GetProcAddress(hNtDll,
		"NtQuerySystemInformation");

	gNtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll,
		"NtQueryInformationProcess");

	gNtAdjustPrivilegesToken = (pfnNtAdjustPrivilegesToken)GetProcAddress(hNtDll,
		"NtAdjustPrivilegesToken");

	if (gNtQuerySystemInformation == NULL || gNtQueryInformationProcess == NULL
		|| gNtAdjustPrivilegesToken == NULL) {
		FreeLibrary(hNtDll);
		return NULL;
	}

	return hNtDll;
}

// Unloads the NTDLL.DLL and resets the
// global gNtQueryInformationProcess variable
void NTDLLAPI::sm_FreeNTDLLFunctions(HMODULE hNtDll)
{
	if (hNtDll)
		FreeLibrary(hNtDll);
	gNtQueryInformationProcess = NULL;
}