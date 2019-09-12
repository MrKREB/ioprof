#ifndef _NTDLLAPI_H_
#define _NTDLLAPI_H_

#pragma once

#include <winternl.h>
#include <windows.h>
#include <psapi.h>
#include <subauth.h>
#include <tlhelp32.h>
#include <cstdlib>
#include <tchar.h>
#include <vector>

#define STRSAFE_LIB
#include <strsafe.h>

#pragma comment(lib, "strsafe.lib")
#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment (lib, "dbghelp.lib")
#pragma comment (lib, "setupapi.lib")

#ifndef NTSTATUS
#define LONG NTSTATUS
#endif

#define MAX_UNICODE_PATH 32767L
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define MAXIMUM_FILENAME_LENGTH	256

enum PROCESSINFOCLASS : int
{
	ProcessBasicInformation = 0, // 0, q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION
	ProcessQuotaLimits, // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
	ProcessIoCounters, // q: IO_COUNTERS
	ProcessVmCounters, // q: VM_COUNTERS, VM_COUNTERS_EX
	ProcessTimes, // q: KERNEL_USER_TIMES
	ProcessBasePriority, // s: KPRIORITY
	ProcessRaisePriority, // s: ULONG
	ProcessDebugPort, // q: HANDLE
	ProcessExceptionPort, // s: HANDLE
	ProcessAccessToken, // s: PROCESS_ACCESS_TOKEN
	ProcessLdtInformation, // 10
	ProcessLdtSize,
	ProcessDefaultHardErrorMode, // qs: ULONG
	ProcessIoPortHandlers, // (kernel-mode only)
	ProcessPooledUsageAndLimits, // q: POOLED_USAGE_AND_LIMITS
	ProcessWorkingSetWatch, // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
	ProcessUserModeIOPL,
	ProcessEnableAlignmentFaultFixup, // s: BOOLEAN
	ProcessPriorityClass, // qs: PROCESS_PRIORITY_CLASS
	ProcessWx86Information,
	ProcessHandleCount, // 20, q: ULONG, PROCESS_HANDLE_INFORMATION
	ProcessAffinityMask, // s: KAFFINITY
	ProcessPriorityBoost, // qs: ULONG
	ProcessDeviceMap, // qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX
	ProcessSessionInformation, // q: PROCESS_SESSION_INFORMATION
	ProcessForegroundInformation, // s: PROCESS_FOREGROUND_BACKGROUND
	ProcessWow64Information, // q: ULONG_PTR
	ProcessImageFileName, // q: UNICODE_STRING
	ProcessLUIDDeviceMapsEnabled, // q: ULONG
	ProcessBreakOnTermination, // qs: ULONG
	ProcessDebugObjectHandle, // 30, q: HANDLE
	ProcessDebugFlags, // qs: ULONG
	ProcessHandleTracing, // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables, otherwise enables
	ProcessIoPriority, // qs: ULONG
	ProcessExecuteFlags, // qs: ULONG
	ProcessResourceManagement,
	ProcessCookie, // q: ULONG
	ProcessImageInformation, // q: SECTION_IMAGE_INFORMATION
	ProcessCycleTime, // q: PROCESS_CYCLE_TIME_INFORMATION
	ProcessPagePriority, // q: ULONG
	ProcessInstrumentationCallback, // 40
	ProcessThreadStackAllocation, // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX
	ProcessWorkingSetWatchEx, // q: PROCESS_WS_WATCH_INFORMATION_EX[]
	ProcessImageFileNameWin32, // q: UNICODE_STRING
	ProcessImageFileMapping, // q: HANDLE (input)
	ProcessAffinityUpdateMode, // qs: PROCESS_AFFINITY_UPDATE_MODE
	ProcessMemoryAllocationMode, // qs: PROCESS_MEMORY_ALLOCATION_MODE
	ProcessGroupInformation, // q: USHORT[]
	ProcessTokenVirtualizationEnabled, // s: ULONG
	ProcessConsoleHostProcess, // q: ULONG_PTR
	ProcessWindowInformation, // 50, q: PROCESS_WINDOW_INFORMATION
	ProcessHandleInformation, // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8
	ProcessMitigationPolicy, // s: PROCESS_MITIGATION_POLICY_INFORMATION
	ProcessDynamicFunctionTableInformation,
	ProcessHandleCheckingMode,
	ProcessKeepAliveCount, // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
	ProcessRevokeFileHandles, // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
	MaxProcessInfoClass
};

typedef ULONG PPS_POST_PROCESS_INIT_ROUTINE;


typedef enum _SYSTEMINFOCLASS
{
	SystemBasicInformation,             // 0x002C
	SystemProcessorInformation,         // 0x000C
	SystemPerformanceInformation,       // 0x0138
	SystemTimeInformation,              // 0x0020
	SystemPathInformation,              // not implemented
	SystemProcessInformation,           // 0x00C8+ per process
	SystemCallInformation,              // 0x0018 + (n * 0x0004)
	SystemConfigurationInformation,     // 0x0018
	SystemProcessorCounters,            // 0x0030 per cpu
	SystemGlobalFlag,                   // 0x0004 (fails if size != 4)
	SystemCallTimeInformation,          // not implemented
	SystemModuleInformation,            // 0x0004 + (n * 0x011C)
	SystemLockInformation,              // 0x0004 + (n * 0x0024)
	SystemStackTraceInformation,        // not implemented
	SystemPagedPoolInformation,         // checked build only
	SystemNonPagedPoolInformation,      // checked build only
	SystemHandleInformation,            // 0x0004  + (n * 0x0010)
	SystemObjectTypeInformation,        // 0x0038+ + (n * 0x0030+)
	SystemPageFileInformation,          // 0x0018+ per page file
	SystemVdmInstemulInformation,       // 0x0088
	SystemVdmBopInformation,            // invalid info class
	SystemCacheInformation,             // 0x0024
	SystemPoolTagInformation,           // 0x0004 + (n * 0x001C)
	SystemInterruptInformation,         // 0x0000, or 0x0018 per cpu
	SystemDpcInformation,               // 0x0014
	SystemFullMemoryInformation,        // checked build only
	SystemLoadDriver,                   // 0x0018, set mode only
	SystemUnloadDriver,                 // 0x0004, set mode only
	SystemTimeAdjustmentInformation,    // 0x000C, 0x0008 writeable
	SystemSummaryMemoryInformation,     // checked build only
	SystemNextEventIdInformation,       // checked build only
	SystemEventIdsInformation,          // checked build only
	SystemCrashDumpInformation,         // 0x0004
	SystemExceptionInformation,         // 0x0010
	SystemCrashDumpStateInformation,    // 0x0004
	SystemDebuggerInformation,          // 0x0002
	SystemContextSwitchInformation,     // 0x0030
	SystemRegistryQuotaInformation,     // 0x000C
	SystemAddDriver,                    // 0x0008, set mode only
	SystemPrioritySeparationInformation,// 0x0004, set mode only
	SystemPlugPlayBusInformation,       // not implemented
	SystemDockInformation,              // not implemented
	SystemPowerInfo,             // 0x0060 (XP only!)
	SystemProcessorSpeedInformation,    // 0x000C (XP only!)
	SystemTimeZoneInformation,          // 0x00AC
	SystemLookasideInformation,         // n * 0x0020
	SystemSetTimeSlipEvent,
	SystemCreateSession,				// set mode only
	SystemDeleteSession,				// set mode only
	SystemInvalidInfoClass1,			// invalid info class
	SystemRangeStartInformation,		// 0x0004 (fails if size != 4)
	SystemVerifierInformation,
	SystemAddVerifier,
	SystemSessionProcessesInformation,	// checked build only
	MaxSystemInfoClass
} SYSTEMINFOCLASS, *PSYSTEMINFOCLASS;

typedef struct _SYSTEM_PROCESS_INFORMATION {
	ULONG			NextEntryOffset;
	ULONG			NumberOfThreads;
	LARGE_INTEGER	WorkingSetPrivateSize;
	ULONG			HardFaultCount;
	ULONG			NumberOfThreadsHighWatermark;
	ULONGLONG		CycleTime;
	LARGE_INTEGER	CreateTime;
	LARGE_INTEGER	UserTime;
	LARGE_INTEGER	KernelTime;
	UNICODE_STRING	ImageName;
	LONG			BasePriority;
	PVOID			UniqueProcessId;
	PVOID			InheritedFromUniqueProcessId;
	ULONG			HandleCount;
	ULONG			SessionId;
	ULONG_PTR		UniqueProcessKey;
	ULONG_PTR		PeakVirtualSize;
	ULONG_PTR		VirtualSize;
	ULONG			PageFaultCount;
	ULONG_PTR		PeakWorkingSetSize;
	ULONG_PTR		WorkingSetSize;
	ULONG_PTR		QuotaPeakPagedPoolUsage;
	ULONG_PTR		QuotaPagedPoolUsage;
	ULONG_PTR		QuotaPeakNonPagedPoolUsage;
	ULONG_PTR		QuotaNonPagedPoolUsage;
	ULONG_PTR		PagefileUsage;
	ULONG_PTR		PeakPagefileUsage;
	ULONG_PTR		PrivatePageCount;
	LARGE_INTEGER	ReadOperationCount;
	LARGE_INTEGER	WriteOperationCount;
	LARGE_INTEGER	OtherOperationCount;
	LARGE_INTEGER	ReadTransferCount;
	LARGE_INTEGER	WriteTransferCount;
	LARGE_INTEGER	OtherTransferCount;
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

// NtQueryInformationProcess in NTDLL.DLL
typedef NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(
	_In_ HANDLE ProcessHandle,
	_In_ PROCESSINFOCLASS ProcessInformationClass,
	_Out_ PVOID ProcessInformation,
	_In_ ULONG ProcessInformationLength,
	_Out_ PULONG ReturnLength
	);


// NtQuerySystemInformation in NTDLL.DLL
typedef NTSTATUS(NTAPI *pfnNtQuerySystemInformation)(
	_In_ SYSTEMINFOCLASS SystemInformationClass,
	_Out_ PVOID	pSystemInformation,
	_In_ ULONG uSystemInformationLength,
	_Out_ PULONG puReturnLength OPTIONAL
	);

typedef struct PROCESSINFO
{
	DWORD	dwPID;
	std::string	ImageName;
	IO_COUNTERS ioCounters;
	ULONGLONG AllOpsCnt;
	ULONGLONG AllBytes;
} PROCESSINFO;

class NTDLLAPI {

public:

	pfnNtQuerySystemInformation gNtQuerySystemInformation;
	pfnNtQueryInformationProcess gNtQueryInformationProcess;

	HMODULE loadNTDLLFunctions(void);
	void freeNTDLLFunctions(_In_ HMODULE hNtDll);
};

enum ProgCodes {
	success,
	procNotFound,
	unableToGetInfo,
	unableToEnablePrivilege,
	unableToOpenFIle,
	wrongArguments
};

#endif
