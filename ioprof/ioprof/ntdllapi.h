#ifndef _NTDLLAPI_H_
#define _NTDLLAPI_H_

#pragma once

#include <windows.h>
#include <subauth.h>
#include <tchar.h>
#include <vector>


#ifndef NTSTATUS
#define LONG NTSTATUS
#endif

// Types of information which can get NtQueryInformationProcess
enum PROCESSINFOCLASS : int
{
	ProcessBasicInformation,
	ProcessQuotaLimits,
	ProcessIoCounters,
	ProcessVmCounters,
	ProcessTimes,
	ProcessBasePriority,
	ProcessRaisePriority,
	ProcessDebugPort,
	ProcessExceptionPort,
	ProcessAccessToken,
	ProcessLdtInformation,
	ProcessLdtSize,
	ProcessDefaultHardErrorMode,
	ProcessIoPortHandlers,
	ProcessPooledUsageAndLimits,
	ProcessWorkingSetWatch,
	ProcessUserModeIOPL,
	ProcessEnableAlignmentFaultFixup,
	ProcessPriorityClass,
	ProcessWx86Information,
	ProcessHandleCount,
	ProcessAffinityMask,
	ProcessPriorityBoost,
	ProcessDeviceMap,
	ProcessSessionInformation,
	ProcessForegroundInformation,
	ProcessWow64Information,
	ProcessImageFileName,
	ProcessLUIDDeviceMapsEnabled,
	ProcessBreakOnTermination,
	ProcessDebugObjectHandle,
	ProcessDebugFlags,
	ProcessHandleTracing,
	ProcessIoPriority,
	ProcessExecuteFlags,
	ProcessResourceManagement,
	ProcessCookie,
	ProcessImageInformation,
	ProcessCycleTime,
	ProcessPagePriority,
	ProcessInstrumentationCallback,
	ProcessThreadStackAllocation,
	ProcessWorkingSetWatchEx,
	ProcessImageFileNameWin32,
	ProcessImageFileMapping,
	ProcessAffinityUpdateMode,
	ProcessMemoryAllocationMode,
	ProcessGroupInformation,
	ProcessTokenVirtualizationEnabled,
	ProcessConsoleHostProcess,
	ProcessWindowInformation,
	ProcessHandleInformation,
	ProcessMitigationPolicy,
	ProcessDynamicFunctionTableInformation,
	ProcessHandleCheckingMode,
	ProcessKeepAliveCount,
	ProcessRevokeFileHandles,
	MaxProcessInfoClass
};

// Types of information which can get NtQuerySystemInformation
typedef enum _SYSTEMINFOCLASS
{
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallInformation,
	SystemConfigurationInformation,
	SystemProcessorCounters,
	SystemGlobalFlag,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLockInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectTypeInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcInformation,
	SystemFullMemoryInformation,
	SystemLoadDriver,
	SystemUnloadDriver,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemNextEventIdInformation,
	SystemEventIdsInformation,
	SystemCrashDumpInformation,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemAddDriver,
	SystemPrioritySeparationInformation,
	SystemPlugPlayBusInformation,
	SystemDockInformation,
	SystemPowerInfo,
	SystemProcessorSpeedInformation,
	SystemTimeZoneInformation,
	SystemLookasideInformation,
	SystemSetTimeSlipEvent,
	SystemCreateSession,
	SystemDeleteSession,
	SystemInvalidInfoClass1,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemAddVerifier,
	SystemSessionProcessesInformation,
	MaxSystemInfoClass
} SYSTEMINFOCLASS, *PSYSTEMINFOCLASS;

// Information about all processes in system
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

// Pointer to NtQueryInformationProcess in NTDLL.DLL
typedef NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(
	_In_ HANDLE ProcessHandle,
	_In_ PROCESSINFOCLASS ProcessInformationClass,
	_Out_ PVOID ProcessInformation,
	_In_ ULONG ProcessInformationLength,
	_Out_ PULONG ReturnLength
	);

// Pointer to NtQuerySystemInformation in NTDLL.DLL
typedef NTSTATUS(NTAPI *pfnNtQuerySystemInformation)(
	_In_ SYSTEMINFOCLASS SystemInformationClass,
	_Out_ PVOID	pSystemInformation,
	_In_ ULONG uSystemInformationLength,
	_Out_ PULONG puReturnLength OPTIONAL
	);

// Struct with information of interest about process
typedef struct PROCESSINFO
{
	DWORD dwPID;
	std::string ImageName;
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

//Codes with various program results
enum ProgCodes {
	success,
	procNotFound,
	unableToGetInfo,
	unableToEnablePrivilege,
	unableToOpenFIle,
	unableToSaveFile,
	wrongArguments
};

#endif
