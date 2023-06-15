#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>
#include <AccCtrl.h>
#include <AclAPI.h>
#include <sddl.h>
#include<mutex>
#include <iostream>
#include <filesystem>
#include <thread>
#include <functional>
#include <fstream>
#include<vector>
#include<string>                                                             
#ifdef _WIN64
#pragma comment(lib,"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.19041.0\\um\\x64\\ntdll.lib")
#define DEFAULT_SIZE DWORD64 
#define ARCH_PROCESS 0
#elif _WIN32
#pragma comment(lib,"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.19041.0\\um\\x86\\ntdll.lib")
#define DEFAULT_SIZE DWORD32
#define ARCH_PROCESS 1
#endif

#define MAX_USER_NAME_LENGTH 256

#define NtInitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }

#define MAX_SERVICE_NAME_LENTGH 256
                                     
#define SERVICE_ALL_TYPE (SERVICE_DRIVER|SERVICE_FILE_SYSTEM_DRIVER|SERVICE_KERNEL_DRIVER|SERVICE_WIN32)

#define MAX_SCAN_MEMORY_REGION_SIZE  812000

#define DllExport   __declspec( dllexport )
#define DLLImport   __declspec( dllimport )

#ifdef NT_EXPORT
#define NT_IS DllExport
#else
#define NT_IS DLLImport

#endif
#ifndef _WIN64
#define GDI_HANDLE_BUFFER_SIZE 34
#else
#define GDI_HANDLE_BUFFER_SIZE 60
#endif

#define RTL_MAX_DRIVE_LETTERS 32
#define FLS_MAXIMUM_AVAILABLE 128

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

#ifndef NT_INFORMATION
#define NT_INFORMATION(Status) ((((ULONG)(Status)) >> 30) == 1)
#endif

#ifndef NT_WARNING
#define NT_WARNING(Status) ((((ULONG)(Status)) >> 30) == 2)
#endif

#ifndef NT_ERROR
#define NT_ERROR(Status) ((((ULONG)(Status)) >> 30) == 3)
#endif

#define NtCurrentProcess() ( (HANDLE)(LONG_PTR) -1 )  

#define GDI_BATCH_BUFFER_SIZE 310

#define PIO_APC_ROUTINE     PVOID



typedef ULONG_PTR  KAFFINITY;

typedef ULONG GDI_HANDLE_BUFFER[GDI_HANDLE_BUFFER_SIZE];
typedef enum _PROCESSINFOCLASS
{
    ProcessBasicInformation, // q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION
    ProcessQuotaLimits, // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
    ProcessIoCounters, // q: IO_COUNTERS
    ProcessVmCounters, // q: VM_COUNTERS, VM_COUNTERS_EX, VM_COUNTERS_EX2
    ProcessTimes, // q: KERNEL_USER_TIMES
    ProcessBasePriority, // s: KPRIORITY
    ProcessRaisePriority, // s: ULONG
    ProcessDebugPort, // q: HANDLE
    ProcessExceptionPort, // s: HANDLE
    ProcessAccessToken, // s: PROCESS_ACCESS_TOKEN
    ProcessLdtInformation, // qs: PROCESS_LDT_INFORMATION // 10
    ProcessLdtSize, // s: PROCESS_LDT_SIZE
    ProcessDefaultHardErrorMode, // qs: ULONG
    ProcessIoPortHandlers, // (kernel-mode only)
    ProcessPooledUsageAndLimits, // q: POOLED_USAGE_AND_LIMITS
    ProcessWorkingSetWatch, // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup, // s: BOOLEAN
    ProcessPriorityClass, // qs: PROCESS_PRIORITY_CLASS
    ProcessWx86Information,
    ProcessHandleCount, // q: ULONG, PROCESS_HANDLE_INFORMATION // 20
    ProcessAffinityMask, // s: KAFFINITY
    ProcessPriorityBoost, // qs: ULONG
    ProcessDeviceMap, // qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX
    ProcessSessionInformation, // q: PROCESS_SESSION_INFORMATION
    ProcessForegroundInformation, // s: PROCESS_FOREGROUND_BACKGROUND
    ProcessWow64Information, // q: ULONG_PTR
    ProcessImageFileName, // q: CUNICODE_STRING
    ProcessLUIDDeviceMapsEnabled, // q: ULONG
    ProcessBreakOnTermination, // qs: ULONG
    ProcessDebugObjectHandle, // q: HANDLE // 30
    ProcessDebugFlags, // qs: ULONG
    ProcessHandleTracing, // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables, otherwise enables
    ProcessIoPriority, // qs: IO_PRIORITY_HINT
    ProcessExecuteFlags, // qs: ULONG
    ProcessResourceManagement,
    ProcessCookie, // q: ULONG
    ProcessImageInformation, // q: SECTION_IMAGE_INFORMATION
    ProcessCycleTime, // q: PROCESS_CYCLE_TIME_INFORMATION // since VISTA
    ProcessPagePriority, // q: ULONG
    ProcessInstrumentationCallback, // 40
    ProcessThreadStackAllocation, // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX
    ProcessWorkingSetWatchEx, // q: PROCESS_WS_WATCH_INFORMATION_EX[]
    ProcessImageFileNameWin32, // q: CUNICODE_STRING
    ProcessImageFileMapping, // q: HANDLE (input)
    ProcessAffinityUpdateMode, // qs: PROCESS_AFFINITY_UPDATE_MODE
    ProcessMemoryAllocationMode, // qs: PROCESS_MEMORY_ALLOCATION_MODE
    ProcessGroupInformation, // q: USHORT[]
    ProcessTokenVirtualizationEnabled, // s: ULONG
    ProcessConsoleHostProcess, // q: ULONG_PTR
    ProcessWindowInformation, // q: PROCESS_WINDOW_INFORMATION // 50
    ProcessHandleInformation, // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8
    ProcessMitigationPolicy, // s: PROCESS_MITIGATION_POLICY_INFORMATION
    ProcessDynamicFunctionTableInformation,
    ProcessHandleCheckingMode,
    ProcessKeepAliveCount, // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
    ProcessRevokeFileHandles, // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
    ProcessWorkingSetControl, // s: PROCESS_WORKING_SET_CONTROL
    ProcessHandleTable, // since WINBLUE
    ProcessCheckStackExtentsMode,
    ProcessCommandLineInformation, // q: CUNICODE_STRING // 60
    ProcessProtectionInformation, // q: PS_PROTECTION
    ProcessMemoryExhaustion, // PROCESS_MEMORY_EXHAUSTION_INFO // since THRESHOLD
    ProcessFaultInformation, // PROCESS_FAULT_INFORMATION
    ProcessTelemetryIdInformation, // PROCESS_TELEMETRY_ID_INFORMATION
    ProcessCommitReleaseInformation, // PROCESS_COMMIT_RELEASE_INFORMATION
    ProcessDefaultCpuSetsInformation,
    ProcessAllowedCpuSetsInformation,
    ProcessSubsystemProcess,
    ProcessJobMemoryInformation, // PROCESS_JOB_MEMORY_INFO
    ProcessInPrivate, // since THRESHOLD2 // 70
    ProcessRaiseUMExceptionOnInvalidHandleClose,
    ProcessIumChallengeResponse,
    ProcessChildProcessInformation, // PROCESS_CHILD_PROCESS_INFORMATION
    ProcessHighGraphicsPriorityInformation,
    ProcessSubsystemInformation, // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
    ProcessEnergyValues, // PROCESS_ENERGY_VALUES, PROCESS_EXTENDED_ENERGY_VALUES
    ProcessActivityThrottleState, // PROCESS_ACTIVITY_THROTTLE_STATE
    ProcessActivityThrottlePolicy, // PROCESS_ACTIVITY_THROTTLE_POLICY
    ProcessWin32kSyscallFilterInformation,
    ProcessDisableSystemAllowedCpuSets,
    ProcessWakeInformation, // PROCESS_WAKE_INFORMATION
    ProcessEnergyTrackingState, // PROCESS_ENERGY_TRACKING_STATE
    MaxProcessInfoClass
} PROCESSINFOCLASS;

// Directory def 



//Source: http://processhacker.sourceforge.net
typedef enum _FILE_INFORMATION_CLASS
{
    FileDirectoryInformation = 1, // FILE_DIRECTORY_INFORMATION
    FileFullDirectoryInformation, // FILE_FULL_DIR_INFORMATION
    FileBothDirectoryInformation, // FILE_BOTH_DIR_INFORMATION
    FileBasicInformation, // FILE_BASIC_INFORMATION
    FileStandardInformation, // FILE_STANDARD_INFORMATION
    FileInternalInformation, // FILE_INTERNAL_INFORMATION
    FileEaInformation, // FILE_EA_INFORMATION
    FileAccessInformation, // FILE_ACCESS_INFORMATION
    FileNameInformation, // FILE_NAME_INFORMATION
    FileRenameInformation, // FILE_RENAME_INFORMATION // 10
    FileLinkInformation, // FILE_LINK_INFORMATION
    FileNamesInformation, // FILE_NAMES_INFORMATION
    FileDispositionInformation, // FILE_DISPOSITION_INFORMATION
    FilePositionInformation, // FILE_POSITION_INFORMATION
    FileFullEaInformation, // FILE_FULL_EA_INFORMATION
    FileModeInformation, // FILE_MODE_INFORMATION
    FileAlignmentInformation, // FILE_ALIGNMENT_INFORMATION
    FileAllInformation, // FILE_ALL_INFORMATION
    FileAllocationInformation, // FILE_ALLOCATION_INFORMATION
    FileEndOfFileInformation, // FILE_END_OF_FILE_INFORMATION // 20
    FileAlternateNameInformation, // FILE_NAME_INFORMATION
    FileStreamInformation, // FILE_STREAM_INFORMATION
    FilePipeInformation, // FILE_PIPE_INFORMATION
    FilePipeLocalInformation, // FILE_PIPE_LOCAL_INFORMATION
    FilePipeRemoteInformation, // FILE_PIPE_REMOTE_INFORMATION
    FileMailslotQueryInformation, // FILE_MAILSLOT_QUERY_INFORMATION
    FileMailslotSetInformation, // FILE_MAILSLOT_SET_INFORMATION
    FileCompressionInformation, // FILE_COMPRESSION_INFORMATION
    FileObjectIdInformation, // FILE_OBJECTID_INFORMATION
    FileCompletionInformation, // FILE_COMPLETION_INFORMATION // 30
    FileMoveClusterInformation, // FILE_MOVE_CLUSTER_INFORMATION
    FileQuotaInformation, // FILE_QUOTA_INFORMATION
    FileReparsePointInformation, // FILE_REPARSE_POINT_INFORMATION
    FileNetworkOpenInformation, // FILE_NETWORK_OPEN_INFORMATION
    FileAttributeTagInformation, // FILE_ATTRIBUTE_TAG_INFORMATION
    FileTrackingInformation, // FILE_TRACKING_INFORMATION
    FileIdBothDirectoryInformation, // FILE_ID_BOTH_DIR_INFORMATION
    FileIdFullDirectoryInformation, // FILE_ID_FULL_DIR_INFORMATION
    FileValidDataLengthInformation, // FILE_VALID_DATA_LENGTH_INFORMATION
    FileShortNameInformation, // FILE_NAME_INFORMATION // 40
    FileIoCompletionNotificationInformation, // FILE_IO_COMPLETION_NOTIFICATION_INFORMATION // since VISTA
    FileIoStatusBlockRangeInformation, // FILE_IOSTATUSBLOCK_RANGE_INFORMATION
    FileIoPriorityHintInformation, // FILE_IO_PRIORITY_HINT_INFORMATION
    FileSfioReserveInformation, // FILE_SFIO_RESERVE_INFORMATION
    FileSfioVolumeInformation, // FILE_SFIO_VOLUME_INFORMATION
    FileHardLinkInformation, // FILE_LINKS_INFORMATION
    FileProcessIdsUsingFileInformation, // FILE_PROCESS_IDS_USING_FILE_INFORMATION
    FileNormalizedNameInformation, // FILE_NAME_INFORMATION
    FileNetworkPhysicalNameInformation, // FILE_NETWORK_PHYSICAL_NAME_INFORMATION
    FileIdGlobalTxDirectoryInformation, // FILE_ID_GLOBAL_TX_DIR_INFORMATION // since WIN7 // 50
    FileIsRemoteDeviceInformation, // FILE_IS_REMOTE_DEVICE_INFORMATION
    FileUnusedInformation,
    FileNumaNodeInformation, // FILE_NUMA_NODE_INFORMATION
    FileStandardLinkInformation, // FILE_STANDARD_LINK_INFORMATION
    FileRemoteProtocolInformation, // FILE_REMOTE_PROTOCOL_INFORMATION
    FileRenameInformationBypassAccessCheck, // (kernel-mode only); FILE_RENAME_INFORMATION // since WIN8
    FileLinkInformationBypassAccessCheck, // (kernel-mode only); FILE_LINK_INFORMATION
    FileVolumeNameInformation, // FILE_VOLUME_NAME_INFORMATION
    FileIdInformation, // FILE_ID_INFORMATION
    FileIdExtdDirectoryInformation, // FILE_ID_EXTD_DIR_INFORMATION
    FileReplaceCompletionInformation, // FILE_COMPLETION_INFORMATION // since WINBLUE
    FileHardLinkFullIdInformation, // FILE_LINK_ENTRY_FULL_ID_INFORMATION
    FileIdExtdBothDirectoryInformation, // FILE_ID_EXTD_BOTH_DIR_INFORMATION // since THRESHOLD
    FileDispositionInformationEx, // FILE_DISPOSITION_INFO_EX // since REDSTONE
    FileRenameInformationEx,
    FileRenameInformationExBypassAccessCheck,
    FileDesiredStorageClassInformation, // FILE_DESIRED_STORAGE_CLASS_INFORMATION // since REDSTONE2
    FileStatInformation, // FILE_STAT_INFORMATION
    FileMaximumInformation
} FILE_INFORMATION_CLASS, * PFILE_INFORMATION_CLASS;



typedef struct _FILE_PIPE_PEEK_BUFFER
{
    ULONG NamedPipeState;
    ULONG ReadDataAvailable;
    ULONG NumberOfMessages;
    ULONG MessageLength;
    CHAR Data[1];
} FILE_PIPE_PEEK_BUFFER, * PFILE_PIPE_PEEK_BUFFER;

typedef struct _NAMED_PIPE_CREATE_PARAMETERS
{
    ULONG NamedPipeType;
    ULONG ReadMode;
    ULONG CompletionMode;
    ULONG MaximumInstances;
    ULONG InboundQuota;
    ULONG OutboundQuota;
    LARGE_INTEGER DefaultTimeout;
    BOOLEAN TimeoutSpecified;
} NAMED_PIPE_CREATE_PARAMETERS, * PNAMED_PIPE_CREATE_PARAMETERS;

typedef struct _FILE_NETWORK_OPEN_INFORMATION
{
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, * PFILE_NETWORK_OPEN_INFORMATION;

typedef struct _SYSTEM_TIMEOFDAY_INFORMATION
{
    LARGE_INTEGER BootTime;
    LARGE_INTEGER CurrentTime;
    LARGE_INTEGER TimeZoneBias;
    ULONG TimeZoneId;
    ULONG Reserved;
    ULONGLONG BootTimeBias;
    ULONGLONG SleepTimeBias;
} SYSTEM_TIMEOFDAY_INFORMATION, * PSYSTEM_TIMEOFDAY_INFORMATION;

typedef struct _SYSTEM_CONSOLE_INFORMATION
{
    ULONG DriverLoaded : 1;
    ULONG Spare : 31;
} SYSTEM_CONSOLE_INFORMATION, * PSYSTEM_CONSOLE_INFORMATION;

                     
typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemBasicInformation, // q: SYSTEM_BASIC_INFORMATION
    SystemProcessorInformation, // q: SYSTEM_PROCESSOR_INFORMATION
    SystemPerformanceInformation, // q: SYSTEM_PERFORMANCE_INFORMATION
    SystemTimeOfDayInformation, // q: SYSTEM_TIMEOFDAY_INFORMATION
    SystemPathInformation, // not implemented
    SystemProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
    SystemCallCountInformation, // q: SYSTEM_CALL_COUNT_INFORMATION
    SystemDeviceInformation, // q: SYSTEM_DEVICE_INFORMATION
    SystemProcessorPerformanceInformation, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
    SystemFlagsInformation, // q: SYSTEM_FLAGS_INFORMATION
    SystemCallTimeInformation, // not implemented // SYSTEM_CALL_TIME_INFORMATION // 10
    SystemModuleInformation, // q: RTL_PROCESS_MODULES
    SystemLocksInformation, // q: RTL_PROCESS_LOCKS
    SystemStackTraceInformation, // q: RTL_PROCESS_BACKTRACES
    SystemPagedPoolInformation, // not implemented
    SystemNonPagedPoolInformation, // not implemented
    SystemHandleInformation, // q: SYSTEM_HANDLE_INFORMATION
    SystemObjectInformation, // q: SYSTEM_OBJECTTYPE_INFORMATION mixed with SYSTEM_OBJECT_INFORMATION
    SystemPageFileInformation, // q: SYSTEM_PAGEFILE_INFORMATION
    SystemVdmInstemulInformation, // q
    SystemVdmBopInformation, // not implemented // 20
    SystemFileCacheInformation, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemCache)
    SystemPoolTagInformation, // q: SYSTEM_POOLTAG_INFORMATION
    SystemInterruptInformation, // q: SYSTEM_INTERRUPT_INFORMATION
    SystemDpcBehaviorInformation, // q: SYSTEM_DPC_BEHAVIOR_INFORMATION; s: SYSTEM_DPC_BEHAVIOR_INFORMATION (requires SeLoadDriverPrivilege)
    SystemFullMemoryInformation, // not implemented
    SystemLoadGdiDriverInformation, // s (kernel-mode only)
    SystemUnloadGdiDriverInformation, // s (kernel-mode only)
    SystemTimeAdjustmentInformation, // q: SYSTEM_QUERY_TIME_ADJUST_INFORMATION; s: SYSTEM_SET_TIME_ADJUST_INFORMATION (requires SeSystemtimePrivilege)
    SystemSummaryMemoryInformation, // not implemented
    SystemMirrorMemoryInformation, // s (requires license value "Kernel-MemoryMirroringSupported") (requires SeShutdownPrivilege) // 30
    SystemPerformanceTraceInformation, // q; s: (type depends on EVENT_TRACE_INFORMATION_CLASS)
    SystemObsolete0, // not implemented
    SystemExceptionInformation, // q: SYSTEM_EXCEPTION_INFORMATION
    SystemCrashDumpStateInformation, // s (requires SeDebugPrivilege)
    SystemKernelDebuggerInformation, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION
    SystemContextSwitchInformation, // q: SYSTEM_CONTEXT_SWITCH_INFORMATION
    SystemRegistryQuotaInformation, // q: SYSTEM_REGISTRY_QUOTA_INFORMATION; s (requires SeIncreaseQuotaPrivilege)
    SystemExtendServiceTableInformation, // s (requires SeLoadDriverPrivilege) // loads win32k only
    SystemPrioritySeperation, // s (requires SeTcbPrivilege)
    SystemVerifierAddDriverInformation, // s (requires SeDebugPrivilege) // 40
    SystemVerifierRemoveDriverInformation, // s (requires SeDebugPrivilege)
    SystemProcessorIdleInformation, // q: SYSTEM_PROCESSOR_IDLE_INFORMATION
    SystemLegacyDriverInformation, // q: SYSTEM_LEGACY_DRIVER_INFORMATION
    SystemCurrentTimeZoneInformation, // q
    SystemLookasideInformation, // q: SYSTEM_LOOKASIDE_INFORMATION
    SystemTimeSlipNotification, // s (requires SeSystemtimePrivilege)
    SystemSessionCreate, // not implemented
    SystemSessionDetach, // not implemented
    SystemSessionInformation, // not implemented
    SystemRangeStartInformation, // q: SYSTEM_RANGE_START_INFORMATION // 50
    SystemVerifierInformation, // q: SYSTEM_VERIFIER_INFORMATION; s (requires SeDebugPrivilege)
    SystemVerifierThunkExtend, // s (kernel-mode only)
    SystemSessionProcessInformation, // q: SYSTEM_SESSION_PROCESS_INFORMATION
    SystemLoadGdiDriverInSystemSpace, // s (kernel-mode only) (same as SystemLoadGdiDriverInformation)
    SystemNumaProcessorMap, // q
    SystemPrefetcherInformation, // q: PREFETCHER_INFORMATION; s: PREFETCHER_INFORMATION // PfSnQueryPrefetcherInformation
    SystemExtendedProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
    SystemRecommendedSharedDataAlignment, // q
    SystemComPlusPackage, // q; s
    SystemNumaAvailableMemory, // 60
    SystemProcessorPowerInformation, // q: SYSTEM_PROCESSOR_POWER_INFORMATION
    SystemEmulationBasicInformation, // q
    SystemEmulationProcessorInformation,
    SystemExtendedHandleInformation, // q: SYSTEM_HANDLE_INFORMATION_EX
    SystemLostDelayedWriteInformation, // q: ULONG
    SystemBigPoolInformation, // q: SYSTEM_BIGPOOL_INFORMATION
    SystemSessionPoolTagInformation, // q: SYSTEM_SESSION_POOLTAG_INFORMATION
    SystemSessionMappedViewInformation, // q: SYSTEM_SESSION_MAPPED_VIEW_INFORMATION
    SystemHotpatchInformation, // q; s
    SystemObjectSecurityMode, // q // 70
    SystemWatchdogTimerHandler, // s (kernel-mode only)
    SystemWatchdogTimerInformation, // q (kernel-mode only); s (kernel-mode only)
    SystemLogicalProcessorInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION
    SystemWow64SharedInformationObsolete, // not implemented
    SystemRegisterFirmwareTableInformationHandler, // s (kernel-mode only)
    SystemFirmwareTableInformation, // SYSTEM_FIRMWARE_TABLE_INFORMATION
    SystemModuleInformationEx, // q: RTL_PROCESS_MODULE_INFORMATION_EX
    SystemVerifierTriageInformation, // not implemented
    SystemSuperfetchInformation, // q; s: SUPERFETCH_INFORMATION // PfQuerySuperfetchInformation
    SystemMemoryListInformation, // q: SYSTEM_MEMORY_LIST_INFORMATION; s: SYSTEM_MEMORY_LIST_COMMAND (requires SeProfileSingleProcessPrivilege) // 80
    SystemFileCacheInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (same as SystemFileCacheInformation)
    SystemThreadPriorityClientIdInformation, // s: SYSTEM_THREAD_CID_PRIORITY_INFORMATION (requires SeIncreaseBasePriorityPrivilege)
    SystemProcessorIdleCycleTimeInformation, // q: SYSTEM_PROCESSOR_IDLE_CYCLE_TIME_INFORMATION[]
    SystemVerifierCancellationInformation, // not implemented // name:wow64:whNT32QuerySystemVerifierCancellationInformation
    SystemProcessorPowerInformationEx, // not implemented
    SystemRefTraceInformation, // q; s: SYSTEM_REF_TRACE_INFORMATION // ObQueryRefTraceInformation
    SystemSpecialPoolInformation, // q; s (requires SeDebugPrivilege) // MmSpecialPoolTag, then MmSpecialPoolCatchOverruns != 0
    SystemProcessIdInformation, // q: SYSTEM_PROCESS_ID_INFORMATION
    SystemErrorPortInformation, // s (requires SeTcbPrivilege)
    SystemBootEnvironmentInformation, // q: SYSTEM_BOOT_ENVIRONMENT_INFORMATION // 90
    SystemHypervisorInformation, // q; s (kernel-mode only)
    SystemVerifierInformationEx, // q; s: SYSTEM_VERIFIER_INFORMATION_EX
    SystemTimeZoneInformation, // s (requires SeTimeZonePrivilege)
    SystemImageFileExecutionOptionsInformation, // s: SYSTEM_IMAGE_FILE_EXECUTION_OPTIONS_INFORMATION (requires SeTcbPrivilege)
    SystemCoverageInformation, // q; s // name:wow64:whNT32QuerySystemCoverageInformation; ExpCovQueryInformation
    SystemPrefetchPatchInformation, // not implemented
    SystemVerifierFaultsInformation, // s (requires SeDebugPrivilege)
    SystemSystemPartitionInformation, // q: SYSTEM_SYSTEM_PARTITION_INFORMATION
    SystemSystemDiskInformation, // q: SYSTEM_SYSTEM_DISK_INFORMATION
    SystemProcessorPerformanceDistribution, // q: SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION // 100
    SystemNumaProximityNodeInformation, // q
    SystemDynamicTimeZoneInformation, // q; s (requires SeTimeZonePrivilege)
    SystemCodeIntegrityInformation, // q: SYSTEM_CODEINTEGRITY_INFORMATION // SeCodeIntegrityQueryInformation
    SystemProcessorMicrocodeUpdateInformation, // s
    SystemProcessorBrandString, // q // HaliQuerySystemInformation -> HalpGetProcessorBrandString, info class 23
    SystemVirtualAddressInformation, // q: SYSTEM_VA_LIST_INFORMATION[]; s: SYSTEM_VA_LIST_INFORMATION[] (requires SeIncreaseQuotaPrivilege) // MmQuerySystemVaInformation
    SystemLogicalProcessorAndGroupInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX // since WIN7 // KeQueryLogicalProcessorRelationship
    SystemProcessorCycleTimeInformation, // q: SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION[]
    SystemStoreInformation, // q; s // SmQueryStoreInformation
    SystemRegistryAppendString, // s: SYSTEM_REGISTRY_APPEND_STRING_PARAMETERS // 110
    SystemAitSamplingValue, // s: ULONG (requires SeProfileSingleProcessPrivilege)
    SystemVhdBootInformation, // q: SYSTEM_VHD_BOOT_INFORMATION
    SystemCpuQuotaInformation, // q; s // PsQueryCpuQuotaInformation
    SystemNativeBasicInformation, // not implemented
    SystemSpare1, // not implemented
    SystemLowPriorityIoInformation, // q: SYSTEM_LOW_PRIORITY_IO_INFORMATION
    SystemTpmBootEntropyInformation, // q: TPM_BOOT_ENTROPY_NT_RESULT // ExQueryTpmBootEntropyInformation
    SystemVerifierCountersInformation, // q: SYSTEM_VERIFIER_COUNTERS_INFORMATION
    SystemPagedPoolInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypePagedPool)
    SystemSystemPtesInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemPtes) // 120
    SystemNodeDistanceInformation, // q
    SystemAcpiAuditInformation, // q: SYSTEM_ACPI_AUDIT_INFORMATION // HaliQuerySystemInformation -> HalpAuditQueryResults, info class 26
    SystemBasicPerformanceInformation, // q: SYSTEM_BASIC_PERFORMANCE_INFORMATION // name:wow64:whNtQuerySystemInformation_SystemBasicPerformanceInformation
    SystemQueryPerformanceCounterInformation, // q: SYSTEM_QUERY_PERFORMANCE_COUNTER_INFORMATION // since WIN7 SP1
    SystemSessionBigPoolInformation, // q: SYSTEM_SESSION_POOLTAG_INFORMATION // since WIN8
    SystemBootGraphicsInformation, // q; s: SYSTEM_BOOT_GRAPHICS_INFORMATION (kernel-mode only)
    SystemScrubPhysicalMemoryInformation, // q; s: MEMORY_SCRUB_INFORMATION
    SystemBadPageInformation,
    SystemProcessorProfileControlArea, // q; s: SYSTEM_PROCESSOR_PROFILE_CONTROL_AREA
    SystemCombinePhysicalMemoryInformation, // s: MEMORY_COMBINE_INFORMATION, MEMORY_COMBINE_INFORMATION_EX, MEMORY_COMBINE_INFORMATION_EX2 // 130
    SystemEntropyInterruptTimingCallback,
    SystemConsoleInformation, // q: SYSTEM_CONSOLE_INFORMATION
    SystemPlatformBinaryInformation, // q: SYSTEM_PLATFORM_BINARY_INFORMATION
    SystemThrottleNotificationInformation,
    SystemHypervisorProcessorCountInformation, // q: SYSTEM_HYPERVISOR_PROCESSOR_COUNT_INFORMATION
    SystemDeviceDataInformation, // q: SYSTEM_DEVICE_DATA_INFORMATION
    SystemDeviceDataEnumerationInformation,
    SystemMemoryTopologyInformation, // q: SYSTEM_MEMORY_TOPOLOGY_INFORMATION
    SystemMemoryChannelInformation, // q: SYSTEM_MEMORY_CHANNEL_INFORMATION
    SystemBootLogoInformation, // q: SYSTEM_BOOT_LOGO_INFORMATION // 140
    SystemProcessorPerformanceInformationEx, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION_EX // since WINBLUE
    SystemSpare0,
    SystemSecureBootPolicyInformation, // q: SYSTEM_SECUREBOOT_POLICY_INFORMATION
    SystemPageFileInformationEx, // q: SYSTEM_PAGEFILE_INFORMATION_EX
    SystemSecureBootInformation, // q: SYSTEM_SECUREBOOT_INFORMATION
    SystemEntropyInterruptTimingRawInformation,
    SystemPortableWorkspaceEfiLauncherInformation, // q: SYSTEM_PORTABLE_WORKSPACE_EFI_LAUNCHER_INFORMATION
    SystemFullProcessInformation, // q: SYSTEM_PROCESS_INFORMATION with SYSTEM_PROCESS_INFORMATION_EXTENSION (requires admin)
    SystemKernelDebuggerInformationEx, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION_EX
    SystemBootMetadataInformation, // 150
    SystemSoftRebootInformation,
    SystemElamCertificateInformation, // s: SYSTEM_ELAM_CERTIFICATE_INFORMATION
    SystemOfflineDumpConfigInformation,
    SystemProcessorFeaturesInformation, // q: SYSTEM_PROCESSOR_FEATURES_INFORMATION
    SystemRegistryReconciliationInformation,
    SystemEdidInformation,
    SystemManufacturingInformation, // q: SYSTEM_MANUFACTURING_INFORMATION // since THRESHOLD
    SystemEnergyEstimationConfigInformation, // q: SYSTEM_ENERGY_ESTIMATION_CONFIG_INFORMATION
    SystemHypervisorDetailInformation, // q: SYSTEM_HYPERVISOR_DETAIL_INFORMATION
    SystemProcessorCycleStatsInformation, // q: SYSTEM_PROCESSOR_CYCLE_STATS_INFORMATION // 160
    SystemVmGenerationCountInformation,
    SystemTrustedPlatformModuleInformation, // q: SYSTEM_TPM_INFORMATION
    SystemKernelDebuggerFlags,
    SystemCodeIntegrityPolicyInformation, // q: SYSTEM_CODEINTEGRITYPOLICY_INFORMATION
    SystemIsolatedUserModeInformation, // q: SYSTEM_ISOLATED_USER_MODE_INFORMATION
    SystemHardwareSecurityTestInterfaceResultsInformation,
    SystemSingleModuleInformation, // q: SYSTEM_SINGLE_MODULE_INFORMATION
    SystemAllowedCpuSetsInformation,
    SystemDmaProtectionInformation, // q: SYSTEM_DMA_PROTECTION_INFORMATION
    SystemInterruptCpuSetsInformation, // q: SYSTEM_INTERRUPT_CPU_SET_INFORMATION // 170
    SystemSecureBootPolicyFullInformation, // q: SYSTEM_SECUREBOOT_POLICY_FULL_INFORMATION
    SystemCodeIntegrityPolicyFullInformation,
    SystemAffinitizedInterruptProcessorInformation,
    SystemRootSiloInformation, // q: SYSTEM_ROOT_SILO_INFORMATION
    SystemCpuSetInformation, // q: SYSTEM_CPU_SET_INFORMATION // since THRESHOLD2
    SystemCpuSetTagInformation, // q: SYSTEM_CPU_SET_TAG_INFORMATION
    SystemWin32WerStartCallout,
    SystemSecureKernelProfileInformation, // q: SYSTEM_SECURE_KERNEL_HYPERGUARD_PROFILE_INFORMATION
    SystemCodeIntegrityPlatformManifestInformation, // q: SYSTEM_SECUREBOOT_PLATFORM_MANIFEST_INFORMATION // since REDSTONE
    SystemInterruptSteeringInformation, // 180
    SystemSupportedProcessorArchitectures,
    SystemMemoryUsageInformation, // q: SYSTEM_MEMORY_USAGE_INFORMATION
    SystemCodeIntegrityCertificateInformation, // q: SYSTEM_CODEINTEGRITY_CERTIFICATE_INFORMATION
    SystemPhysicalMemoryInformation, // q: SYSTEM_PHYSICAL_MEMORY_INFORMATION // since REDSTONE2
    SystemControlFlowTransition,
    SystemKernelDebuggingAllowed,
    SystemActivityModerationExeState, // SYSTEM_ACTIVITY_MODERATION_EXE_STATE
    SystemActivityModerationUserSettings, // SYSTEM_ACTIVITY_MODERATION_USER_SETTINGS
    SystemCodeIntegrityPoliciesFullInformation,
    SystemCodeIntegrityUnlockInformation, // SYSTEM_CODEINTEGRITY_UNLOCK_INFORMATION // 190
    SystemIntegrityQuotaInformation,
    SystemFlushInformation, // q: SYSTEM_FLUSH_INFORMATION
    MaxSystemInfoClass
} SYSTEM_INFORMATION_CLASS;

typedef enum _THREADINFOCLASS
{
    ThreadBasicInformation, // q: THREAD_BASIC_INFORMATION
    ThreadTimes, // q: KERNEL_USER_TIMES
    ThreadPriority, // s: KPRIORITY
    ThreadBasePriority, // s: LONG
    ThreadAffinityMask, // s: KAFFINITY
    ThreadImpersonationToken, // s: HANDLE
    ThreadDescriptorTableEntry, // q: DESCRIPTOR_TABLE_ENTRY (or WOW64_DESCRIPTOR_TABLE_ENTRY)
    ThreadEnableAlignmentFaultFixup, // s: BOOLEAN
    ThreadEventPair,
    ThreadQuerySetWin32StartAddress, // q: PVOID <- Thread Start Address | Start Address Thread
    ThreadZeroTlsCell, // 10
    ThreadPerformanceCount, // q: LARGE_INTEGER
    ThreadAmILastThread, // q: ULONG
    ThreadIdealProcessor, // s: ULONG
    ThreadPriorityBoost, // qs: ULONG
    ThreadSetTlsArrayAddress,
    ThreadIsIoPending, // q: ULONG
    ThreadHideFromDebugger, // s: void
    ThreadBreakOnTermination, // qs: ULONG
    ThreadSwitchLegacyState,
    ThreadIsTerminated, // q: ULONG // 20
    ThreadLastSystemCall, // q: THREAD_LAST_SYSCALL_INFORMATION
    ThreadIoPriority, // qs: IO_PRIORITY_HINT
    ThreadCycleTime, // q: THREAD_CYCLE_TIME_INFORMATION
    ThreadPagePriority, // q: ULONG
    ThreadActualBasePriority,
    ThreadTebInformation, // q: THREAD_TEB_INFORMATION (requires THREAD_GET_CONTEXT + THREAD_SET_CONTEXT)
    ThreadCSwitchMon,
    ThreadCSwitchPmu,
    ThreadWow64Context, // q: WOW64_CONTEXT
    ThreadGroupInformation, // q: GROUP_AFFINITY // 30
    ThreadUmsInformation, // q: THREAD_UMS_INFORMATION
    ThreadCounterProfiling,
    ThreadIdealProcessorEx, // q: PROCESSOR_NUMBER
    ThreadCpuAccountingInformation, // since WIN8
    ThreadSuspendCount, // since WINBLUE
    ThreadHeterogeneousCpuPolicy, // q: KHETERO_CPU_POLICY // since THRESHOLD
    ThreadContainerId, // q: GUID
    ThreadNameInformation, // qs: THREAD_NAME_INFORMATION
    ThreadSelectedCpuSets,
    ThreadSystemThreadInformation, // q: SYSTEM_THREAD_INFORMATION // 40
    ThreadActualGroupAffinity, // since THRESHOLD2
    ThreadDynamicCodePolicyInfo,
    ThreadExplicitCaseSensitivity,
    ThreadWorkOnBehalfTicket,
    ThreadSubsystemInformation, // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
    ThreadDbgkWerReportActive,
    ThreadAttachContainer,
    MaxThreadInfoClass

} THREADINFOCLASS;

typedef enum _LDR_DDAG_STATE
{
    LdrModulesMerged = -5,
    LdrModulesInitError = -4,
    LdrModulesSnapError = -3,
    LdrModulesUnloaded = -2,
    LdrModulesUnloading = -1,
    LdrModulesPlaceHolder = 0,
    LdrModulesMapping = 1,
    LdrModulesMapped = 2,
    LdrModulesWaitingForDependencies = 3,
    LdrModulesSnapping = 4,
    LdrModulesSnapped = 5,
    LdrModulesCondensed = 6,
    LdrModulesReadyToInit = 7,
    LdrModulesInitializing = 8,
    LdrModulesReadyToRun = 9

} LDR_DDAG_STATE;  



typedef enum _OBJECT_INFORMATION_CLASS
{
    ObjectBasicInformation, // OBJECT_BASIC_INFORMATION
    ObjectNameInformation, // OBJECT_NAME_INFORMATION
    ObjectTypeInformation, // OBJECT_TYPE_INFORMATION
    ObjectTypesInformation, // OBJECT_TYPES_INFORMATION
    ObjectHandleFlagInformation, // OBJECT_HANDLE_FLAG_INFORMATION
    ObjectSessionInformation,
    ObjectSessionObjectInformation,
    MaxObjectInfoClass

} OBJECT_INFORMATION_CLASS;

typedef enum _LDR_DLL_LOAD_REASON
{
    LoadReasonStaticDependency,
    LoadReasonStaticForwarderDependency,
    LoadReasonDynamicForwarderDependency,
    LoadReasonDelayloadDependency,
    LoadReasonDynamicLoad,
    LoadReasonAsImageLoad,
    LoadReasonAsDataLoad,
    LoadReasonUnknown = -1
} LDR_DLL_LOAD_REASON, * PLDR_DLL_LOAD_REASON;

typedef NTSTATUS (NTAPI*PRTL_HEAP_COMMIT_ROUTINE)(PVOID Base,PVOID* CommitAddress,PSIZE_T CommitSize);


typedef struct _RTL_HEAP_PARAMETERS
{
    ULONG Length;
    SIZE_T SegmentReserve;
    SIZE_T SegmentCommit;
    SIZE_T DeCommitFreeBlockThreshold;
    SIZE_T DeCommitTotalFreeThreshold;
    SIZE_T MaximumAllocationSize;
    SIZE_T VirtualMemoryThreshold;
    SIZE_T InitialCommit;
    SIZE_T InitialReserve;
    PRTL_HEAP_COMMIT_ROUTINE CommitRoutine;
    SIZE_T Reserved[2];
} RTL_HEAP_PARAMETERS, * PRTL_HEAP_PARAMETERS;

typedef enum _KWAIT_REASON
{
    Executive,
    FreePage,
    PageIn,
    PoolAllocation,
    DelayExecution,
    Suspended,
    UserRequest,
    WrExecutive,
    WrFreePage,
    WrPageIn,
    WrPoolAllocation,
    WrDelayExecution,
    WrSuspended,
    WrUserRequest,
    WrEventPair,
    WrQueue,
    WrLpcReceive,
    WrLpcReply,
    WrVirtualMemory,
    WrPageOut,
    WrRendezvous,
    WrKeyedEvent,
    WrTerminated,
    WrProcessInSwap,
    WrCpuRateControl,
    WrCalloutStack,
    WrKernel,
    WrResource,
    WrPushLock,
    WrMutex,
    WrQuantumEnd,
    WrDispatchInt,
    WrPreempted,
    WrYieldExecution,
    WrFastMutex,
    WrGuardedMutex,
    WrRundown,
    WrAlertByThreadId,
    WrDeferredPreempt,
    MaximumWaitReason

} KWAIT_REASON;

typedef enum _SECTION_INFORMATION_CLASS
{
    SectionBasicInformation,
    SectionImageInformation,
    SectionRelocationInformation, // name:wow64:whNtQuerySection_SectionRelocationInformation
    SectionOriginalBaseInformation, // PVOID BaseAddress
    SectionInternalImageInformation, // SECTION_INTERNAL_IMAGE_INFORMATION // since REDSTONE2
    MaxSectionInfoClass
} SECTION_INFORMATION_CLASS;


typedef VOID(NTAPI* PPS_POST_PROCESS_INIT_ROUTINE) (VOID);
typedef LONG  NTSTATUS;
typedef LONG  KPRIORITY;



typedef struct __UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} CUNICODE_STRING, * PCUNICODE_STRING;



typedef struct __OBJECT_TYPE_INFORMATION
{
    CUNICODE_STRING TypeName;
    ULONG TotalNumberOfObjects;
    ULONG TotalNumberOfHandles;
    ULONG TotalPagedPoolUsage;
    ULONG TotalNonPagedPoolUsage;
    ULONG TotalNamePoolUsage;
    ULONG TotalHandleTableUsage;
    ULONG HighWaterNumberOfObjects;
    ULONG HighWaterNumberOfHandles;
    ULONG HighWaterPagedPoolUsage;
    ULONG HighWaterNonPagedPoolUsage;
    ULONG HighWaterNamePoolUsage;
    ULONG HighWaterHandleTableUsage;
    ULONG InvalidAttributes;
    GENERIC_MAPPING GenericMapping;
    ULONG ValidAccessMask;
    BOOLEAN SecurityRequired;
    BOOLEAN MaintainHandleCount;
    UCHAR TypeIndex; // Since Windows 8.1
    CHAR ReservedByte;
    ULONG PoolType;
    ULONG DefaultPagedPoolCharge;
    ULONG DefaultNonPagedPoolCharge;

} OBJECT_TYPE_INFORMATION_, * POBJECT_TYPE_INFORMATION_;

typedef struct _OBJECT_BASIC_INFORMATION
{
    ULONG Attributes;
    ACCESS_MASK GrantedAccess;
    ULONG HandleCount;
    ULONG PointerCount;
    ULONG PagedPoolCharge;
    ULONG NonPagedPoolCharge;
    ULONG Reserved[3];
    ULONG NameInfoSize;
    ULONG TypeInfoSize;
    ULONG SecurityDescriptorSize;
    LARGE_INTEGER CreationTime;
} OBJECT_BASIC_INFORMATION, * POBJECT_BASIC_INFORMATION;

typedef struct __OBJECT_NAME_INFORMATION
{
    CUNICODE_STRING Name;

} OBJECT_NAME_INFORMATION_, * POBJECT_NAME_INFORMATION_;

typedef struct _OBJECT_TYPES_INFORMATION
{
    ULONG NumberOfTypes;
    OBJECT_TYPE_INFORMATION_ TypeInformation[1];
} OBJECT_TYPES_INFORMATION, * POBJECT_TYPES_INFORMATION;

typedef struct _OBJECT_HANDLE_FLAG_INFORMATION
{
    BOOLEAN Inherit;
    BOOLEAN ProtectFromClose;
} OBJECT_HANDLE_FLAG_INFORMATION, * POBJECT_HANDLE_FLAG_INFORMATION;


typedef struct _RTL_DRIVE_LETTER_CURDIR
{
    USHORT Flags;
    USHORT Length;
    ULONG TimeStamp;
    CUNICODE_STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, * PRTL_DRIVE_LETTER_CURDIR;

typedef struct _LDR_SERVICE_TAG_RECORD
{
    struct _LDR_SERVICE_TAG_RECORD* Next;
    ULONG ServiceTag;
} LDR_SERVICE_TAG_RECORD, * PLDR_SERVICE_TAG_RECORD;

typedef struct _LDRP_CSLIST
{
    PSINGLE_LIST_ENTRY Tail;
} LDRP_CSLIST, * PLDRP_CSLIST;

typedef struct _LDR_DDAG_NODE
{
    LIST_ENTRY Modules;
    PLDR_SERVICE_TAG_RECORD ServiceTagList;
    ULONG LoadCount;
    ULONG LoadWhileUnloadingCount;
    ULONG LowestLink;
    union
    {
        LDRP_CSLIST Dependencies;
        SINGLE_LIST_ENTRY RemovalLink;
    };
    LDRP_CSLIST IncomingDependencies;
    LDR_DDAG_STATE State;
    SINGLE_LIST_ENTRY CondenseLink;
    ULONG PreorderNumber;
} LDR_DDAG_NODE, * PLDR_DDAG_NODE;

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
{
    USHORT UniqueProcessId;
    USHORT CreatorBackTraceIndex;
    UCHAR ObjectTypeIndex;
    UCHAR HandleAttributes;
    USHORT HandleValue;
    PVOID Object;
    ULONG GrantedAccess;
    
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX
{
    PVOID Object;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR HandleValue;
    ULONG GrantedAccess;
    USHORT CreatorBackTraceIndex;
    USHORT ObjectTypeIndex;
    ULONG HandleAttributes;
    ULONG Reserved;

} SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO_EX;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
    ULONG_PTR NumberOfHandles;
    ULONG_PTR Reserved;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX Handles[1];

} SYSTEM_HANDLE_INFORMATION_EX, * PSYSTEM_HANDLE_INFORMATION_EX;

typedef struct _RTL_BALANCED_NODE
{
    union
    {
        struct _RTL_BALANCED_NODE* Children[2];
        struct
        {
            struct _RTL_BALANCED_NODE* Left;
            struct _RTL_BALANCED_NODE* Right;
        } s;
    };
    union
    {
        UCHAR Red : 1;
        UCHAR Balance : 2;
        ULONG_PTR ParentValue;
    } u;
} RTL_BALANCED_NODE, * PRTL_BALANCED_NODE;



typedef struct _PEB_LDR_DATA
{
    ULONG Length;
    BOOLEAN Initialized;
    HANDLE SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID EntryInProgress;
    BOOLEAN ShutdownInProgress;
    HANDLE ShutdownThreadId;

} PEB_LDR_DATA, * PPEB_LDR_DATA;



typedef struct _CLIENT_ID
{
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, * PCLIENT_ID;


typedef struct _CURDIR
{
    CUNICODE_STRING DosPath;
    HANDLE Handle;
} CURDIR, * PCURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS
{
    ULONG MaximumLength;
    ULONG Length;

    ULONG Flags;
    ULONG DebugFlags;

    HANDLE ConsoleHandle;
    ULONG ConsoleFlags;
    HANDLE StandardInput;
    HANDLE StandardOutput;
    HANDLE StandardError;

    CURDIR CurrentDirectory;
    CUNICODE_STRING DllPath;
    CUNICODE_STRING ImagePathName;
    CUNICODE_STRING CommandLine;
    PWCHAR Environment;

    ULONG StartingX;
    ULONG StartingY;
    ULONG CountX;
    ULONG CountY;
    ULONG CountCharsX;
    ULONG CountCharsY;
    ULONG FillAttribute;

    ULONG WindowFlags;
    ULONG ShowWindowFlags;
    CUNICODE_STRING WindowTitle;
    CUNICODE_STRING DesktopInfo;
    CUNICODE_STRING ShellInfo;
    CUNICODE_STRING RuntimeData;
    RTL_DRIVE_LETTER_CURDIR CurrentDirectories[RTL_MAX_DRIVE_LETTERS];

    ULONG_PTR EnvironmentSize;
    ULONG_PTR EnvironmentVersion;
    PVOID PackageDependencyData;
    ULONG ProcessGroupId;
    ULONG LoaderThreads;

} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;


typedef struct _PEB
{
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    union
    {
        BOOLEAN BitField;
        struct
        {
            BOOLEAN ImageUsesLargePages : 1;
            BOOLEAN IsProtectedProcess : 1;
            BOOLEAN IsImageDynamicallyRelocated : 1;
            BOOLEAN SkipPatchingUser32Forwarders : 1;
            BOOLEAN IsPackagedProcess : 1;
            BOOLEAN IsAppContainer : 1;
            BOOLEAN IsProtectedProcessLight : 1;
            BOOLEAN IsLongPathAwareProcess : 1;
        } s1;
    } u1;

    HANDLE Mutant;
    PVOID ImageBaseAddress;
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID SubSystemData;
    PVOID ProcessHeap;
    PRTL_CRITICAL_SECTION FastPebLock;
    PVOID AtlThunkSListPtr;
    PVOID IFEOKey;
    union
    {
        ULONG CrossProcessFlags;
        struct
        {
            ULONG ProcessInJob : 1;
            ULONG ProcessInitializing : 1;
            ULONG ProcessUsingVEH : 1;
            ULONG ProcessUsingVCH : 1;
            ULONG ProcessUsingFTH : 1;
            ULONG ProcessPreviouslyThrottled : 1;
            ULONG ProcessCurrentlyThrottled : 1;
            ULONG ReservedBits0 : 25;
        } s2;
    } u2;
    union
    {
        PVOID KernelCallbackTable;
        PVOID UserSharedInfoPtr;
    } u3;
    ULONG SystemReserved[1];
    ULONG AtlThunkSListPtr32;
    PVOID ApiSetMap;
    ULONG TlsExpansionCounter;
    PVOID TlsBitmap;
    ULONG TlsBitmapBits[2];

    PVOID ReadOnlySharedMemoryBase;
    PVOID SharedData; // HotpatchInformation
    PVOID* ReadOnlyStaticServerData;

    PVOID AnsiCodePageData; // PCPTABLEINFO
    PVOID OemCodePageData; // PCPTABLEINFO
    PVOID UnicodeCaseTableData; // PNLSTABLEINFO

    ULONG NumberOfProcessors;
    ULONG NtGlobalFlag;

    LARGE_INTEGER CriticalSectionTimeout;
    SIZE_T HeapSegmentReserve;
    SIZE_T HeapSegmentCommit;
    SIZE_T HeapDeCommitTotalFreeThreshold;
    SIZE_T HeapDeCommitFreeBlockThreshold;

    ULONG NumberOfHeaps;
    ULONG MaximumNumberOfHeaps;
    PVOID* ProcessHeaps; // PHEAP

    PVOID GdiSharedHandleTable;
    PVOID ProcessStarterHelper;
    ULONG GdiDCAttributeList;

    PRTL_CRITICAL_SECTION LoaderLock;

    ULONG OSMajorVersion;
    ULONG OSMinorVersion;
    USHORT OSBuildNumber;
    USHORT OSCSDVersion;
    ULONG OSPlatformId;
    ULONG ImageSubsystem;
    ULONG ImageSubsystemMajorVersion;
    ULONG ImageSubsystemMinorVersion;
    ULONG_PTR ActiveProcessAffinityMask;
    GDI_HANDLE_BUFFER GdiHandleBuffer;
    PVOID PostProcessInitRoutine;

    PVOID TlsExpansionBitmap;
    ULONG TlsExpansionBitmapBits[32];

    ULONG SessionId;

    ULARGE_INTEGER AppCompatFlags;
    ULARGE_INTEGER AppCompatFlagsUser;
    PVOID pShimData;
    PVOID AppCompatInfo; // APPCOMPAT_EXE_DATA

    CUNICODE_STRING CSDVersion;

    PVOID ActivationContextData; // ACTIVATION_CONTEXT_DATA
    PVOID ProcessAssemblyStorageMap; // ASSEMBLY_STORAGE_MAP
    PVOID SystemDefaultActivationContextData; // ACTIVATION_CONTEXT_DATA
    PVOID SystemAssemblyStorageMap; // ASSEMBLY_STORAGE_MAP

    SIZE_T MinimumStackCommit;

    PVOID* FlsCallback;
    LIST_ENTRY FlsListHead;
    PVOID FlsBitmap;
    ULONG FlsBitmapBits[FLS_MAXIMUM_AVAILABLE / (sizeof(ULONG) * 8)];
    ULONG FlsHighIndex;

    PVOID WerRegistrationData;
    PVOID WerShipAssertPtr;
    PVOID pUnused; // pContextData
    PVOID pImageHeaderHash;
    union
    {
        ULONG TracingFlags;
        struct
        {
            ULONG HeapTracingEnabled : 1;
            ULONG CritSecTracingEnabled : 1;
            ULONG LibLoaderTracingEnabled : 1;
            ULONG SpareTracingBits : 29;
        } s3;
    } u4;
    ULONGLONG CsrServerReadOnlySharedMemoryBase;
    PVOID TppWorkerpListLock;
    LIST_ENTRY TppWorkerpList;
    PVOID WaitOnAddressHashTable[128];
    PVOID TelemetryCoverageHeader; // REDSTONE3
    ULONG CloudFileFlags;
} PEB, * PPEB;



typedef struct _FILE_DIRECTORY_INFORMATION {
    ULONG                   NextEntryOffset;
    ULONG                   FileIndex;
    LARGE_INTEGER           CreationTime;
    LARGE_INTEGER           LastAccessTime;
    LARGE_INTEGER           LastWriteTime;
    LARGE_INTEGER           ChangeTime;
    LARGE_INTEGER           EndOfFile;
    LARGE_INTEGER           AllocationSize;
    ULONG                   FileAttributes;
    ULONG                   FileNameLength;
    WCHAR                   FileName[1];

} FILE_DIRECTORY_INFORMATION, * PFILE_DIRECTORY_INFORMATION;

typedef struct _FILE_FULL_DIR_INFORMATION {
    ULONG                   NextEntryOffset;
    ULONG                   FileIndex;
    LARGE_INTEGER           CreationTime;
    LARGE_INTEGER           LastAccessTime;
    LARGE_INTEGER           LastWriteTime;
    LARGE_INTEGER           ChangeTime;
    LARGE_INTEGER           EndOfFile;
    LARGE_INTEGER           AllocationSize;
    ULONG                   FileAttributes;
    ULONG                   FileNameLength;
    ULONG                   EaSize;
    WCHAR                   FileName[1];

} FILE_FULL_DIR_INFORMATION, * PFILE_FULL_DIR_INFORMATION;

typedef struct _FILE_BASIC_INFORMATION {

    LARGE_INTEGER           CreationTime;
    LARGE_INTEGER           LastAccessTime;
    LARGE_INTEGER           LastWriteTime;
    LARGE_INTEGER           ChangeTime;
    ULONG                   FileAttributes;

} FILE_BASIC_INFORMATION, * PFILE_BASIC_INFORMATION;

typedef struct _FILE_NAMES_INFORMATION {

    ULONG                   NextEntryOffset;
    ULONG                   FileIndex;
    ULONG                   FileNameLength;
    WCHAR                   FileName[1];

} FILE_NAMES_INFORMATION, * PFILE_NAMES_INFORMATION;

typedef struct _FILE_STANDARD_INFORMATION {
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG         NumberOfLinks;
    BOOLEAN       DeletePending;
    BOOLEAN       Directory;
} FILE_STANDARD_INFORMATION, * PFILE_STANDARD_INFORMATION;

typedef struct _FILE_INTERNAL_INFORMATION {
    LARGE_INTEGER IndexNumber;
} FILE_INTERNAL_INFORMATION, * PFILE_INTERNAL_INFORMATION;

typedef struct _FILE_EA_INFORMATION {
    ULONG EaSize;
} FILE_EA_INFORMATION, * PFILE_EA_INFORMATION;

typedef struct _FILE_ACCESS_INFORMATION {
    ACCESS_MASK AccessFlags;
} FILE_ACCESS_INFORMATION, * PFILE_ACCESS_INFORMATION;

typedef struct _FILE_POSITION_INFORMATION {
    LARGE_INTEGER CurrentByteOffset;
} FILE_POSITION_INFORMATION, * PFILE_POSITION_INFORMATION;

typedef struct _FILE_MODE_INFORMATION {
    ULONG Mode;
} FILE_MODE_INFORMATION, * PFILE_MODE_INFORMATION;

typedef struct _FILE_ALIGNMENT_INFORMATION {
    ULONG AlignmentRequirement;
} FILE_ALIGNMENT_INFORMATION, * PFILE_ALIGNMENT_INFORMATION;

typedef struct _FILE_NAME_INFORMATION {
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAME_INFORMATION, * PFILE_NAME_INFORMATION;


typedef struct _FILE_ALL_INFORMATION {
    FILE_BASIC_INFORMATION     BasicInformation;
    FILE_STANDARD_INFORMATION  StandardInformation;
    FILE_INTERNAL_INFORMATION  InternalInformation;
    FILE_EA_INFORMATION        EaInformation;
    FILE_ACCESS_INFORMATION    AccessInformation;
    FILE_POSITION_INFORMATION  PositionInformation;
    FILE_MODE_INFORMATION      ModeInformation;
    FILE_ALIGNMENT_INFORMATION AlignmentInformation;
    FILE_NAME_INFORMATION      NameInformation;
} FILE_ALL_INFORMATION, * PFILE_ALL_INFORMATION;


typedef enum _SECTION_INHERIT
{
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;

typedef struct _PROCESS_BASIC_INFORMATION
{
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR AffinityMask;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, * PPROCESS_BASIC_INFORMATION;

typedef enum _KTHREAD_STATE
{
    Initialized,
    Ready,
    Running,
    Standby,
    Terminated,
    Waiting,
    Transition,
    DeferredReady,
    GateWaitObsolete,
    WaitingForProcessInSwap,
    MaximumThreadState

} KTHREAD_STATE, * PKTHREAD_STATE;


typedef struct _SYSTEM_THREAD_INFORMATION
{
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER CreateTime;
    ULONG WaitTime;
    PVOID StartAddress;
    CLIENT_ID ClientId;
    KPRIORITY Priority;
    LONG BasePriority;
    ULONG ContextSwitches;
    ULONG ThreadState;
    KWAIT_REASON WaitReason;

} SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;




typedef struct _TEB_ACTIVE_FRAME_CONTEXT
{
    ULONG Flags;
    PSTR FrameName;
} TEB_ACTIVE_FRAME_CONTEXT, * PTEB_ACTIVE_FRAME_CONTEXT;

typedef struct _TEB_ACTIVE_FRAME
{
    ULONG Flags;
    struct _TEB_ACTIVE_FRAME* Previous;
    PTEB_ACTIVE_FRAME_CONTEXT Context;
} TEB_ACTIVE_FRAME, * PTEB_ACTIVE_FRAME;

typedef struct _ACTIVATION_CONTEXT_STACK
{
    struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME* ActiveFrame;
    LIST_ENTRY FrameListCache;
    ULONG Flags;
    ULONG NextCookieSequenceNumber;
    ULONG StackId;

} ACTIVATION_CONTEXT_STACK, * PACTIVATION_CONTEXT_STACK;

typedef struct _GDI_TEB_BATCHc
{
    ULONG Offset;
    ULONG_PTR HDC;
    ULONG Buffer[GDI_BATCH_BUFFER_SIZE];
} GDI_TEB_BATCHc, * PGDI_TEB_BATCHc;

typedef struct _SYSTEM_PROCESS_ID_INFORMATION {
    HANDLE ProcessId;
    CUNICODE_STRING ImageName;
} SYSTEM_PROCESS_ID_INFORMATION, * PSYSTEM_PROCESS_ID_INFORMATION;

typedef struct NT_PROCESSOR_NUMBER
{
    WORD    Group;
    BYTE    Number;
    BYTE    Reserved;

} NT_PROCESSOR_NUMBER;


typedef struct _TEB
{
    NT_TIB NtTib;

    PVOID EnvironmentPointer;
    CLIENT_ID ClientId;
    PVOID ActiveRpcHandle;
    PVOID ThreadLocalStoragePointer;
    PPEB ProcessEnvironmentBlock;

    ULONG LastErrorValue;
    ULONG CountOfOwnedCriticalSections;
    PVOID CsrClientThread;
    PVOID Win32ThreadInfo;
    ULONG User32Reserved[26];
    ULONG UserReserved[5];
    PVOID WOW32Reserved;
    LCID CurrentLocale;
    ULONG FpSoftwareStatusRegister;
    PVOID ReservedForDebuggerInstrumentation[16];
#ifdef _WIN64
    PVOID SystemReserved1[30];
#else
    PVOID SystemReserved1[26];
#endif
    CHAR PlaceholderCompatibilityMode;
    CHAR PlaceholderReserved[11];
    ULONG ProxiedProcessId;
    ACTIVATION_CONTEXT_STACK ActivationStack;

    UCHAR WorkingOnBehalfTicket[8];
    NTSTATUS ExceptionCode;

    PACTIVATION_CONTEXT_STACK ActivationContextStackPointer;
    ULONG_PTR InstrumentationCallbackSp;
    ULONG_PTR InstrumentationCallbackPreviousPc;
    ULONG_PTR InstrumentationCallbackPreviousSp;
#ifdef _WIN64
    ULONG TxFsContext;
#endif
    BOOLEAN InstrumentationCallbackDisabled;
#ifndef _WIN64
    UCHAR SpareBytes[23];
    ULONG TxFsContext;
#endif
    GDI_TEB_BATCHc GdiTebBatch;
    CLIENT_ID RealClientId;
    HANDLE GdiCachedProcessHandle;
    ULONG GdiClientPID;
    ULONG GdiClientTID;
    PVOID GdiThreadLocalInfo;
    ULONG_PTR Win32ClientInfo[62];
    PVOID glDispatchTable[233];
    ULONG_PTR glReserved1[29];
    PVOID glReserved2;
    PVOID glSectionInfo;
    PVOID glSection;
    PVOID glTable;
    PVOID glCurrentRC;
    PVOID glContext;

    NTSTATUS LastStatusValue;
    CUNICODE_STRING StaticUnicodeString;
    WCHAR StaticUnicodeBuffer[261];

    PVOID DeallocationStack;
    PVOID TlsSlots[64];
    LIST_ENTRY TlsLinks;

    PVOID Vdm;
    PVOID ReservedForNtRpc;
    PVOID DbgSsReserved[2];

    ULONG HardErrorMode;
#ifdef _WIN64
    PVOID Instrumentation[11];
#else
    PVOID Instrumentation[9];
#endif
    GUID ActivityId;

    PVOID SubProcessTag;
    PVOID PerflibData;
    PVOID EtwTraceData;
    PVOID WinSockData;
    ULONG GdiBatchCount;

    union
    {
        NT_PROCESSOR_NUMBER CurrentIdealProcessor;
        ULONG IdealProcessorValue;
        struct
        {
            UCHAR ReservedPad0;
            UCHAR ReservedPad1;
            UCHAR ReservedPad2;
            UCHAR IdealProcessor;
        } s1;
    } u1;

    ULONG GuaranteedStackBytes;
    PVOID ReservedForPerf;
    PVOID ReservedForOle;
    ULONG WaitingOnLoaderLock;
    PVOID SavedPriorityState;
    ULONG_PTR ReservedForCodeCoverage;
    PVOID ThreadPoolData;
    PVOID* TlsExpansionSlots;
#ifdef _WIN64
    PVOID DeallocationBStore;
    PVOID BStoreLimit;
#endif
    ULONG MuiGeneration;
    ULONG IsImpersonating;
    PVOID NlsCache;
    PVOID pShimData;
    USHORT HeapVirtualAffinity;
    USHORT LowFragHeapDataSlot;
    HANDLE CurrentTransactionHandle;
    PTEB_ACTIVE_FRAME ActiveFrame;
    PVOID FlsData;

    PVOID PreferredLanguages;
    PVOID UserPrefLanguages;
    PVOID MergedPrefLanguages;
    ULONG MuiImpersonation;

    union
    {
        USHORT CrossTebFlags;
        USHORT SpareCrossTebBits : 16;
    } u2;
    union
    {
        USHORT SameTebFlags;
        struct
        {
            USHORT SafeThunkCall : 1;
            USHORT InDebugPrint : 1;
            USHORT HasFiberData : 1;
            USHORT SkipThreadAttach : 1;
            USHORT WerInShipAssertCode : 1;
            USHORT RanProcessInit : 1;
            USHORT ClonedThread : 1;
            USHORT SuppressDebugMsg : 1;
            USHORT DisableUserStackWalk : 1;
            USHORT RtlExceptionAttached : 1;
            USHORT InitialThread : 1;
            USHORT SessionAware : 1;
            USHORT LoadOwner : 1;
            USHORT LoaderWorker : 1;
            USHORT SkipLoaderInit : 1;
            USHORT SpareSameTebBits : 1;
        } s2;
    } u3;

    PVOID TxnScopeEnterCallback;
    PVOID TxnScopeExitCallback;
    PVOID TxnScopeContext;
    ULONG LockCount;
    LONG WowTebOffset;
    PVOID ResourceRetValue;
    PVOID ReservedForWdf;
    ULONGLONG ReservedForCrt;
    GUID EffectiveContainerId;

} TEB, * PTEB;

typedef struct _SYSTEM_PROCESS_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER WorkingSetPrivateSize; // Since Vista
    ULONG HardFaultCount; // Since Windows 7
    ULONG NumberOfThreadsHighWatermark; // Since Windows 7
    ULONGLONG CycleTime; // Since Windows 7
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    CUNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SessionId;
    ULONG_PTR UniqueProcessKey; // Since Vista (requires SystemExtendedProcessInformation)
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
    SYSTEM_THREAD_INFORMATION Threads[1];

} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    union
    {
        LIST_ENTRY InInitializationOrderLinks;
        LIST_ENTRY InProgressLinks;
    };
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    CUNICODE_STRING FullDllName;
    CUNICODE_STRING BaseDllName;
    union
    {
        UCHAR FlagGroup[4];
        ULONG Flags;
        struct
        {
            ULONG PackagedBinary : 1;
            ULONG MarkedForRemoval : 1;
            ULONG ImageDll : 1;
            ULONG LoadNotificationsSent : 1;
            ULONG TelemetryEntryProcessed : 1;
            ULONG ProcessStaticImport : 1;
            ULONG InLegacyLists : 1;
            ULONG InIndexes : 1;
            ULONG ShimDll : 1;
            ULONG InExceptionTable : 1;
            ULONG ReservedFlags1 : 2;
            ULONG LoadInProgress : 1;
            ULONG LoadConfigProcessed : 1;
            ULONG EntryProcessed : 1;
            ULONG ProtectDelayLoad : 1;
            ULONG ReservedFlags3 : 2;
            ULONG DontCallForThreads : 1;
            ULONG ProcessAttachCalled : 1;
            ULONG ProcessAttachFailed : 1;
            ULONG CorDeferredValidate : 1;
            ULONG CorImage : 1;
            ULONG DontRelocate : 1;
            ULONG CorILOnly : 1;
            ULONG ReservedFlags5 : 3;
            ULONG Redirected : 1;
            ULONG ReservedFlags6 : 2;
            ULONG CompatDatabaseProcessed : 1;
        } s;
    } u;
    USHORT ObsoleteLoadCount;
    USHORT TlsIndex;
    LIST_ENTRY HashLinks;
    ULONG TimeDateStamp;
    struct _ACTIVATION_CONTEXT* EntryPointActivationContext;
    PVOID Lock;
    PLDR_DDAG_NODE DdagNode;
    LIST_ENTRY NodeModuleLink;
    struct _LDRP_LOAD_CONTEXT* LoadContext;
    PVOID ParentDllBase;
    PVOID SwitchBackContext;
    RTL_BALANCED_NODE BaseAddressIndexNode;
    RTL_BALANCED_NODE MappingInfoIndexNode;
    ULONG_PTR OriginalBase;
    LARGE_INTEGER LoadTime;
    ULONG BaseNameHashValue;
    LDR_DLL_LOAD_REASON LoadReason;
    ULONG ImplicitPathOptions;
    ULONG ReferenceCount;
    ULONG DependentLoadFlags;
    UCHAR SigningLevel; // Since Windows 10 RS2
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _MEMORY_REGION_INFORMATION {
    PVOID AllocationBase;
    ULONG AllocationProtect;
    SIZE_T RegionSize;
    ULONG State;
    ULONG Protect;
    ULONG Type;
} MEMORY_REGION_INFORMATION, * PMEMORY_REGION_INFORMATION;

typedef struct _MEMORY_IMAGE_INFORMATION {
    PVOID BaseAddress;
    ULONG_PTR Size;
    CUNICODE_STRING ImageName;
    USHORT LoadCount;
    USHORT ModuleNameOffset;
    WCHAR ModuleName[MAX_PATH];
} MEMORY_IMAGE_INFORMATION, * PMEMORY_IMAGE_INFORMATION;


typedef enum _MEMORY_INFORMATION_CLASS
{
    MemoryBasicInformation, // MEMORY_BASIC_INFORMATION
    MemoryWorkingSetInformation, // MEMORY_WORKING_SET_INFORMATION
    MemoryMappedFilenameInformation, // CUNICODE_STRING
    MemoryRegionInformation, // MEMORY_REGION_INFORMATION
    MemoryWorkingSetExInformation, // MEMORY_WORKING_SET_EX_INFORMATION
    MemorySharedCommitInformation, // MEMORY_SHARED_COMMIT_INFORMATION
    MemoryImageInformation, // MEMORY_IMAGE_INFORMATION
    MemoryRegionInformationEx,
    MemoryPrivilegedBasicInformation 

} MEMORY_INFORMATION_CLASS;



typedef struct __OBJECT_HANDLE_TYPE
{
    static constexpr wchar_t* KernelAdapter = (wchar_t*)L"KernelAdapter";
    static constexpr wchar_t* Type = (wchar_t*)L"Type";
    static constexpr wchar_t* Adapter = (wchar_t*)L"Adapter";
    static constexpr wchar_t* ControllerConfig = (wchar_t*)L"ControllerConfig";
    static constexpr wchar_t* ControllerState = (wchar_t*)L"ControllerState";
    static constexpr wchar_t* Device = (wchar_t*)L"Device";
    static constexpr wchar_t* DeviceInterface = (wchar_t*)L"DeviceInterface";
    static constexpr wchar_t* Driver = (wchar_t*)L"Driver";
    static constexpr wchar_t* Event = (wchar_t*)L"Event";
    static constexpr wchar_t* IoCompletion = (wchar_t*)L"IoCompletion";
    static constexpr wchar_t* Key = (wchar_t*)L"Key";
    static constexpr wchar_t* Mutant = (wchar_t*)L"Mutant";
    static constexpr wchar_t* NamedPipe = (wchar_t*)L"NamedPipe";
    static constexpr wchar_t* Process = (wchar_t*)L"Process";
    static constexpr wchar_t* Profile = (wchar_t*)L"Profile";
    static constexpr wchar_t* Section = (wchar_t*)L"Section";
    static constexpr wchar_t* Semaphore = (wchar_t*)L"Semaphore";
    static constexpr wchar_t* SymbolicLink = (wchar_t*)L"SymbolicLink";
    static constexpr wchar_t* Thread = (wchar_t*)L"Thread";
    static constexpr wchar_t* Timer = (wchar_t*)L"Timer";
    static constexpr wchar_t* IRTimer = (wchar_t*)L"IRTimer";
    static constexpr wchar_t* Token = (wchar_t*)L"Token";
    static constexpr wchar_t* WindowStation = (wchar_t*)L"WindowStation";
    static constexpr wchar_t* Desktop = (wchar_t*)L"Desktop";
    static constexpr wchar_t* WmiGuid = (wchar_t*)L"WmiGuid";
    static constexpr wchar_t* ALPC_Port = (wchar_t*)L"ALPC Port";
    static constexpr wchar_t* WaitCompletionPacket = (wchar_t*)L"WaitCompletionPacket";
    static constexpr wchar_t* DebugObject = (wchar_t*)L"DebugObject";
    static constexpr wchar_t* Directory = (wchar_t*)L"Directory";
    static constexpr wchar_t* EtwRegistration = (wchar_t*)L"EtwRegistration";
    static constexpr wchar_t* FilterConnectionPort = (wchar_t*)L"FilterConnectionPort";
    static constexpr wchar_t* FltCommunicationPort = (wchar_t*)L"FltCommunicationPort";
    static constexpr wchar_t* FltConnectionPort = (wchar_t*)L"FltConnectionPort";
    static constexpr wchar_t* FltObject = (wchar_t*)L"FltObject";
    static constexpr wchar_t* Job = (wchar_t*)L"Job";
    static constexpr wchar_t* MemoryPartition = (wchar_t*)L"MemoryPartition";
    static constexpr wchar_t* NdisCmState = (wchar_t*)L"NdisCmState";
    static constexpr wchar_t* PowerRequest = (wchar_t*)L"PowerRequest";
    static constexpr wchar_t* PrivateNamespace = (wchar_t*)L"PrivateNamespace";
    static constexpr wchar_t* PcwObject = (wchar_t*)L"PcwObject";
    static constexpr wchar_t* ResourceManager = (wchar_t*)L"ResourceManager";
    static constexpr wchar_t* TmEn = (wchar_t*)L"TmEn";
    static constexpr wchar_t* TmRm = (wchar_t*)L"TmRm";
    static constexpr wchar_t* TmTm = (wchar_t*)L"TmTm";
    static constexpr wchar_t* Transaction = (wchar_t*)L"Transaction";
    static constexpr wchar_t* TpWorkerFactory = (wchar_t*)L"TpWorkerFactory";
    static constexpr wchar_t* Wnf = (wchar_t*)L"Wnf";
    static constexpr wchar_t* WmiDataSource = (wchar_t*)L"WmiDataSource";
    static constexpr wchar_t* UmsCompletionList = (wchar_t*)L"UmsCompletionList";
    static constexpr wchar_t* UmsThreadQueue = (wchar_t*)L"UmsThreadQueue";
    static constexpr wchar_t* TimerQueue = (wchar_t*)L"TimerQueue";
    static constexpr wchar_t* TpCallback = (wchar_t*)L"TpCallback";
    static constexpr wchar_t* KeyedEvent = (wchar_t*)L"KeyedEvent";
    static constexpr wchar_t* BcdObject = (wchar_t*)L"BcdObject";
    static constexpr wchar_t* BcdContainer = (wchar_t*)L"BcdContainer";
    static constexpr wchar_t* Session = (wchar_t*)L"Session";
    static constexpr wchar_t* Resource = (wchar_t*)L"Resource";

} ObjectHandleType;


typedef struct _PRIVILEGES
{
   static constexpr char* SeAssignPrimaryTokenPrivilege = (char*)"SeAssignPrimaryTokenPrivilege";
    static constexpr char* SeAuditPrivilege = (char*)"SeAuditPrivilege";
    static constexpr char* SeBackupPrivilege = (char*)"SeBackupPrivilege";
    static constexpr char* SeChangeNotifyPrivilege = (char*)"SeChangeNotifyPrivilege";
    static constexpr char* SeCreateGlobalPrivilege = (char*)"SeCreateGlobalPrivilege";
    static constexpr char* SeCreatePagefilePrivilege = (char*)"SeCreatePagefilePrivilege";
    static constexpr char* SeCreatePermanentPrivilege = (char*)"SeCreatePermanentPrivilege";
    static constexpr char* SeCreateSymbolicLinkPrivilege = (char*)"SeCreateSymbolicLinkPrivilege";
    static constexpr char* SeCreateTokenPrivilege = (char*)"SeCreateTokenPrivilege";
    static constexpr char* SeDebugPrivilege = (char*)"SeDebugPrivilege";
    static constexpr char* SeEnableDelegationPrivilege = (char*)"SeEnableDelegationPrivilege";
    static constexpr char* SeImpersonatePrivilege = (char*)"SeImpersonatePrivilege";
    static constexpr char* SeIncreaseBasePriorityPrivilege = (char*)"SeIncreaseBasePriorityPrivilege";
    static constexpr char* SeIncreaseQuotaPrivilege = (char*)"SeIncreaseQuotaPrivilege";
    static constexpr char* SeIncreaseWorkingSetPrivilege = (char*)"SeIncreaseWorkingSetPrivilege";
    static constexpr char* SeLoadDriverPrivilege = (char*)"SeLoadDriverPrivilege";
    static constexpr char* SeLockMemoryPrivilege = (char*)"SeLockMemoryPrivilege";
    static constexpr char* SeMachineAccountPrivilege = (char*)"SeMachineAccountPrivilege";
    static constexpr char* SeManageVolumePrivilege = (char*)"SeManageVolumePrivilege";
    static constexpr char* SeProfileSingleProcessPrivilege = (char*)"SeProfileSingleProcessPrivilege";
    static constexpr char* SeRemoteShutdownPrivilege = (char*)"SeRemoteShutdownPrivilege";
    static constexpr char* SeRestorePrivilege = (char*)"SeRestorePrivilege";
    static constexpr char* SeSecurityPrivilege = (char*)"SeSecurityPrivilege";
    static constexpr char* SeShutdownPrivilege = (char*)"SeShutdownPrivilege";
    static constexpr char* SeSyncAgentPrivilege = (char*)"SeSyncAgentPrivilege";
    static constexpr char* SeSystemEnvironmentPrivilege = (char*)"SeSystemEnvironmentPrivilege";
    static constexpr char* SeSystemProfilePrivilege = (char*)"SeSystemProfilePrivilege";
    static constexpr char* SeSystemtimePrivilege = (char*)"SeSystemtimePrivilege";
    static constexpr char* SeTakeOwnershipPrivilege = (char*)"SeTakeOwnershipPrivilege";
    static constexpr char* SeTcbPrivilege = (char*)"SeTcbPrivilege";
    static constexpr char* SeTimeZonePrivilege = (char*)"SeTimeZonePrivilege";
    static constexpr char* SeTrustedCredManAccessPrivilege = (char*)"SeTrustedCredManAccessPrivilege";
    static constexpr char* SeUndockPrivilege = (char*)"SeUndockPrivilege";
    static constexpr char* SeUnsolicitedInputPrivilege = (char*)"SeUnsolicitedInputPrivilege";
    static constexpr char* SeDelegateSessionUserImpersonatePrivilege = (char*)"SeDelegateSessionUserImpersonatePrivilege";

} Privileges;

typedef struct _INITIAL_TEB {

    PVOID                StackBase;
    PVOID                StackLimit;
    PVOID                StackCommit;
    PVOID                StackCommitMax;
    PVOID                StackReserved;


} INITIAL_TEB, * PINITIAL_TEB;

typedef struct _THREAD_NAME_INFORMATION {
    CUNICODE_STRING ThreadName;
} THREAD_NAME_INFORMATION, * PTHREAD_NAME_INFORMATION;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG           Length;
    HANDLE          RootDirectory;
    PCUNICODE_STRING ObjectName;
    ULONG           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

typedef struct _THREAD_BASIC_INFORMATION {

    NTSTATUS                ExitStatus;
    PVOID                   TebBaseAddress;
    CLIENT_ID               ClientId;
    KAFFINITY               AffinityMask;
    KPRIORITY               Priority;
    KPRIORITY               BasePriority;

} THREAD_BASIC_INFORMATION, * PTHREAD_BASIC_INFORMATION;


typedef struct _PROCESS_HANDLE_TABLE_ENTRY_INFO
{
    HANDLE 	    HandleValue;
    ULONG_PTR 	HandleCount;
    ULONG_PTR 	PointerCount;
    ULONG 	    GrantedAccess;
    ULONG 	    ObjectTypeIndex;
    ULONG 	    HandleAttributes;                     
    ULONG 	    Reserved;

} PROCESS_HANDLE_TABLE_ENTRY_INFO, * PPROCESS_HANDLE_TABLE_ENTRY_INFO;

typedef struct _PROCESS_HANDLE_SNAPSHOT_INFORMATION
{
    ULONG_PTR 	NumberOfHandles;
    ULONG_PTR 	Reserved;
    PROCESS_HANDLE_TABLE_ENTRY_INFO 	Handles[1];

}  PROCESS_HANDLE_SNAPSHOT_INFORMATION, * PPROCESS_HANDLE_SNAPSHOT_INFORMATION;


typedef struct _IO_STATUS_BLOCK
{
    union
    {
        NTSTATUS Status;
        PVOID Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;


typedef struct __PUBLIC_OBJECT_TYPE_INFORMATION {
    CUNICODE_STRING TypeName;
    ULONG          Reserved[22];
} PUBLIC_OBJECT_TYPE_INFORMATION, * PPUBLIC_OBJECT_TYPE_INFORMATION;

typedef struct _PUBLIC_OBJECT_BASIC_INFORMATION {
    ULONG       Attributes;
    ACCESS_MASK GrantedAccess;
    ULONG       HandleCount;
    ULONG       PointerCount;
    ULONG       Reserved[10];
} PUBLIC_OBJECT_BASIC_INFORMATION, * PPUBLIC_OBJECT_BASIC_INFORMATION;


typedef enum _EVENT_TYPE {

    NotificationEvent,
    SynchronizationEvent

} EVENT_TYPE, * PEVENT_TYPE;


extern "C" NTSTATUS __stdcall NtWriteVirtualMemory(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID buffer,
    SIZE_T RegionSize,
    PULONG FreeType
);

extern "C" NTSTATUS __stdcall  NtReadVirtualMemory(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID buffer,
    SIZE_T NumberOfBytesToRead,
    PULONG NumberOfBytesReaded
);

extern "C" NTSTATUS __stdcall  NtAllocateVirtualMemory(
    HANDLE ProcessHandle,
    PVOID * BaseAddress,
    ULONG_PTR ZeroBits,
    PSIZE_T RegionSize,
    ULONG AllocationType,
    ULONG Protect
);

extern "C" NTSTATUS __stdcall  NtFreeVirtualMemory(
    HANDLE ProcessHandle,
    PVOID * BaseAddress,
    PSIZE_T RegionSize,
    ULONG FreeType
);

extern "C" NTSTATUS  __stdcall  NtQueryInformationProcess(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength
);



extern "C" NTSTATUS  __stdcall NtCreateFile(
     PHANDLE FileHandle,
     ACCESS_MASK DesiredAccess,
     POBJECT_ATTRIBUTES ObjectAttributes,
     PIO_STATUS_BLOCK IoStatusBlock,
     PLARGE_INTEGER AllocationSize,
     ULONG FileAttributes,
     ULONG ShareAccess,
     ULONG CreateDisposition,
     ULONG CreateOptions,
     PVOID EaBuffer,
     ULONG EaLength
);

extern "C" NTSTATUS  __stdcall  NtCreateThreadEx(

     PHANDLE ThreadHandle,
     ACCESS_MASK DesiredAccess,
     POBJECT_ATTRIBUTES ObjectAttributes,
     HANDLE ProcessHandle,
     PVOID StartRoutine,
     PVOID Argument,
     ULONG CreateFlags,
     ULONG_PTR ZeroBits,
     SIZE_T StackSize,
     SIZE_T MaximumStackSize,
     PVOID AttributeList
);

extern "C" NTSTATUS  __stdcall NtQuerySection(
     HANDLE SectionHandle,
     SECTION_INFORMATION_CLASS SectionInformationClass,
     PVOID SectionInformation,
     SIZE_T SectionInformationLength,
     PSIZE_T ReturnLength
);

extern "C" NTSTATUS  __stdcall NtAreMappedFilesTheSame(
     PVOID File1MappedAsAnImage,
     PVOID File2MappedAsFile
);


extern "C" NTSTATUS  __stdcall NtCreateSection(
     PHANDLE             SectionHandle,
     ACCESS_MASK         DesiredAccess,
     POBJECT_ATTRIBUTES  ObjectAttributes,
     PLARGE_INTEGER      MaximumSize,
     ULONG               SectionPageProtection,
     ULONG               AllocationAttributes,
     HANDLE              FileHandle
);

extern "C" NTSTATUS  __stdcall NtUnmapViewOfSection(
    HANDLE ProcessHandle,
    PVOID  BaseAddress
);

extern "C" NTSTATUS  __stdcall NtCreateSectionEx(

     PHANDLE                  SectionHandle,
     ACCESS_MASK              DesiredAccess,
     POBJECT_ATTRIBUTES       ObjectAttributes,
     PLARGE_INTEGER           MaximumSize,
     ULONG                    SectionPageProtection,
     ULONG                    AllocationAttributes,
     HANDLE                   FileHandle,
     PMEM_EXTENDED_PARAMETER  ExtendedParameters,
     ULONG                    ExtendedParameterCount
);



extern "C" NTSTATUS  __stdcall  NtMapViewOfSection(

     HANDLE SectionHandle,
     HANDLE ProcessHandle,
     PVOID* BaseAddress,
     ULONG_PTR ZeroBits,
     SIZE_T CommitSize,
     PLARGE_INTEGER SectionOffset,
     PSIZE_T ViewSize,
     SECTION_INHERIT InheritDisposition,
     ULONG AllocationType,
     ULONG Win32Protect
);

#if NTDDI_VERSION >= NTDDI_VISTA

extern "C" NTSTATUS  __stdcall  NtOpenSession(
     PHANDLE SessionHandle,
     ACCESS_MASK DesiredAccess,
     POBJECT_ATTRIBUTES ObjectAttributes
);


#endif

extern "C" NTSTATUS  __stdcall NtOpenFile(
     PHANDLE FileHandle,
     ACCESS_MASK DesiredAccess,
     POBJECT_ATTRIBUTES ObjectAttributes,
     PIO_STATUS_BLOCK IoStatusBlock,
     ULONG ShareAccess,
     ULONG OpenOptions
);


extern "C" NTSTATUS   __stdcall  NtClose(HANDLE Handle);

extern "C" NTSTATUS   __stdcall  NtQueryInformationThread(
    HANDLE                   ThreadHandle,
    THREADINFOCLASS ThreadInformationClass,
    PVOID                    ThreadInformation,
    ULONG                    ThreadInformationLength,
    PULONG                    ReturnLength
);

extern "C" NTSTATUS   __stdcall  NtQuerySystemInformation(

    SYSTEM_INFORMATION_CLASS  SystemInformationClass,
    PVOID                    SystemInformation,
    ULONG                    SystemInformationLength,
    PULONG                   ReturnLength
);


extern "C" NTSTATUS  __stdcall  RtlGetNativeSystemInformation(
    SYSTEM_INFORMATION_CLASS  SystemInformationClass,
    PVOID                    SystemInformation,
    ULONG                    SystemInformationLength,
    PULONG                   ReturnLength
);

extern "C" NTSTATUS   __stdcall  NtQuerySystemInformationEx(

     SYSTEM_INFORMATION_CLASS SystemInformationClass,
     PVOID InputBuffer,
     ULONG InputBufferLength,
     PVOID SystemInformation,
     ULONG SystemInformationLength,
     PULONG ReturnLength
);


extern "C" NTSTATUS   __stdcall  NtQueueApcThread(

    HANDLE               ThreadHandle,
    PIO_APC_ROUTINE      ApcRoutine,
    PVOID                ApcRoutineContext,
    PIO_STATUS_BLOCK     ApcStatusBlock,
    ULONG                ApcReserved
);

extern "C" NTSTATUS   __stdcall  NtOpenThread(
    PHANDLE            ThreadHandle,
    ACCESS_MASK        DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PCLIENT_ID         ClientId
);

extern "C" NTSTATUS   __stdcall  NtAlertResumeThread(
    HANDLE            ThreadHandle,
    PULONG                      SuspendCount
);

extern "C" NTSTATUS   __stdcall  NtResumeThread(
    HANDLE           ThreadHandle,
    PULONG           SuspendCount
);

extern "C" NTSTATUS   __stdcall  NtGetContextThread(
    HANDLE           ThreadHandle,
    LPCONTEXT           SuspendCount
);

extern "C" NTSTATUS   __stdcall  NtSetContextThread(
    HANDLE           ThreadHandle,
    LPCONTEXT           SuspendCount
);

extern "C" NTSTATUS   __stdcall  NtImpersonateThread(
     HANDLE ServerThreadHandle,
     HANDLE ClientThreadHandle,
     PSECURITY_QUALITY_OF_SERVICE SecurityQos
);


extern "C" NTSTATUS   __stdcall  LdrLoadDll(
    PWCHAR               PathToFile,
    ULONG                Flags,
    PCUNICODE_STRING      ModuleFileName,
    PHANDLE             ModuleHandle
);


extern "C" NTSTATUS   __stdcall  RtlAllocateHeap(
     PVOID HeapHandle,
     ULONG Flags,
     SIZE_T Size
);

extern "C" NTSTATUS   __stdcall RtlFreeHeap(
     PVOID HeapHandle,
     ULONG Flags,
     PVOID BaseAddress
);

extern "C" PVOID   __stdcall RtlCreateHeap(
     ULONG Flags,
     PVOID HeapBase,
     SIZE_T ReserveSize,
     SIZE_T CommitSize,
     PVOID Lock,
     PRTL_HEAP_PARAMETERS Parameters
);



extern "C" NTSTATUS   __stdcall  RtlCreateUserThread(

    HANDLE               ProcessHandle,
    PSECURITY_DESCRIPTOR SecurityDescriptor,
    BOOLEAN              CreateSuspended,
    ULONG                StackZeroBits,
    PULONG           StackReserved,
    PULONG           StackCommit,
    PVOID                StartAddress,
    PVOID                StartParameter,
    PHANDLE             ThreadHandle,
    PCLIENT_ID          ClientID

);

extern "C" NTSTATUS   __stdcall  NtProtectVirtualMemory(
    HANDLE ProcessHandle,
    PVOID * BaseAddress,
    PSIZE_T RegionSize,
    ULONG NewProtect,
    PULONG OldProtect
);

extern "C" NTSTATUS   __stdcall  NtQueryVirtualMemory(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    MEMORY_INFORMATION_CLASS MemoryInformationClass,
    PVOID MemoryInformation,
    SIZE_T MemoryInformationLength,
    PSIZE_T ReturnLength
);

extern "C" NTSTATUS   __stdcall  NtSuspendThread(
    HANDLE ThreadHandle,
    PULONG PreviousSuspendCount
);

extern "C" NTSTATUS   __stdcall  NtTerminateThread(
    HANDLE ThreadHandle,
    NTSTATUS             ExitStatus
);

extern "C" NTSTATUS   __stdcall  NtOpenProcessToken(
    HANDLE        ProcessHandle, 
    ACCESS_MASK   DesiredAccess, 
    PHANDLE       TokenHandle);


extern "C" NTSTATUS    __stdcall  NtAdjustPrivilegesToken(
     HANDLE               TokenHandle,
     BOOLEAN              DisableAllPrivileges,
     PTOKEN_PRIVILEGES    TokenPrivileges,
     ULONG                PreviousPrivilegesLength,
     PTOKEN_PRIVILEGES   PreviousPrivileges,
     PULONG              RequiredLength);


extern "C" NTSTATUS    __stdcall  NtQueryInformationToken(
     HANDLE               TokenHandle,
     TOKEN_INFORMATION_CLASS TokenInformationClass,
     PVOID               TokenInformation,
     ULONG                TokenInformationLength,
     PULONG              ReturnLength);




extern "C" NTSTATUS    __stdcall  NtDuplicateObject(
    HANDLE      SourceProcessHandle,
    HANDLE      SourceHandle,
    HANDLE      TargetProcessHandle,
    PHANDLE     TargetHandle,
    ACCESS_MASK DesiredAccess,
    ULONG       HandleAttributes,
    ULONG       Options);

extern "C" NTSTATUS    __stdcall NtQueryObject(
     HANDLE Handle,
     OBJECT_INFORMATION_CLASS ObjectInformationClass,
     PVOID ObjectInformation,
     ULONG ObjectInformationLength,
     PULONG ReturnLength
);



extern "C" NTSTATUS    __stdcall  NtOpenProcess(
     PHANDLE             ProcessHandle,
     ACCESS_MASK          AccessMask,
     POBJECT_ATTRIBUTES   ObjectAttributes,
     PCLIENT_ID           ClientId);


extern "C" NTSTATUS    __stdcall NtSetInformationObject(

     HANDLE               ObjectHandle,
     OBJECT_INFORMATION_CLASS ObjectInformationClass,
     PVOID                ObjectInformation,
     ULONG                Length);


extern "C" NTSTATUS    __stdcall NtCreateProcess(

     PHANDLE ProcessHandle,
     ACCESS_MASK DesiredAccess,
     POBJECT_ATTRIBUTES ObjectAttributes,
     HANDLE ParentProcess,
     BOOLEAN InheritObjectTable,
     HANDLE SectionHandle,
     HANDLE DebugPort,
     HANDLE ExceptionPort);

extern "C" NTSTATUS    __stdcall NtCreateProcessEx(

     PHANDLE ProcessHandle,
     ACCESS_MASK DesiredAccess,
     POBJECT_ATTRIBUTES ObjectAttributes,
     HANDLE ParentProcess,
     ULONG Flags,
     HANDLE SectionHandle,
     HANDLE DebugPort,
     HANDLE ExceptionPort,
     BOOLEAN InJob);



extern "C" NTSTATUS    __stdcall NtSetSecurityObject(
    HANDLE               Handle,
    SECURITY_INFORMATION SecurityInformation,
    PSECURITY_DESCRIPTOR SecurityDescriptor);

extern "C" NTSTATUS    __stdcall NtSetInformationThread(
     HANDLE ThreadHandle,
     THREADINFOCLASS ThreadInformationClass,
     PVOID ThreadInformation,
     ULONG ThreadInformationLength
);

extern "C" NTSTATUS   __stdcall NtPulseEvent(
     HANDLE               EventHandle,
     PLONG               PreviousState);

extern "C" NTSTATUS   __stdcall NtClearEvent(
    HANDLE               EventHandle);

extern "C" NTSTATUS   __stdcall NtCreateEvent(
     PHANDLE              EventHandle,
     ACCESS_MASK          DesiredAccess,
     POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
     EVENT_TYPE           EventType,
     BOOLEAN              InitialState);

extern "C" NTSTATUS   __stdcall NtSetEvent(
     HANDLE              EventHandle,
     PLONG               PreviousState OPTIONAL);

extern "C" NTSTATUS   __stdcall NtWaitForSingleObject(
     HANDLE Handle,
     BOOLEAN Alertable,
     PLARGE_INTEGER Timeout
);

extern "C" NTSTATUS   __stdcall NtOpenEvent(
     PHANDLE             EventHandle,
     ACCESS_MASK          DesiredAccess,
     POBJECT_ATTRIBUTES   ObjectAttributes);

extern "C" NTSTATUS   __stdcall NtResetEvent(
     HANDLE               EventHandle,
     PLONG               PreviousState OPTIONAL);


extern "C" NTSTATUS   __stdcall NtQueryInformationFile(
     HANDLE FileHandle,
     PIO_STATUS_BLOCK IoStatusBlock,
     PVOID FileInformation,
     ULONG Length,
     FILE_INFORMATION_CLASS FileInformationClass);

extern "C" NTSTATUS   __stdcall NtSetInformationFile(
     HANDLE FileHandle,
     PIO_STATUS_BLOCK IoStatusBlock,
     PVOID FileInformation,
     ULONG Length,
     FILE_INFORMATION_CLASS FileInformationClass);



FORCEINLINE BOOLEAN RemoveEntryList(PLIST_ENTRY Entry)
{
    PLIST_ENTRY Flink = Entry->Flink;
    PLIST_ENTRY Blink = Entry->Blink;
    Blink->Flink = Flink;
    Flink->Blink = Blink;

    return Flink == Blink;
}



typedef struct _USERA
{
    char  UserName[MAX_USER_NAME_LENGTH];
    char  DomaineName[MAX_USER_NAME_LENGTH];
    SID_NAME_USE SidNameUser;

    _USERA(char * u, char * d, SID_NAME_USE s) : SidNameUser(s) {
        strcpy(UserName, u); 
        strcpy(DomaineName, d);
    };

} USERA, * PUSERA;    

typedef struct _USERW
{
    wchar_t  UserName[MAX_USER_NAME_LENGTH];
    wchar_t  DomaineName[MAX_USER_NAME_LENGTH];
    SID_NAME_USE SidNameUser;

    _USERW(wchar_t * u, wchar_t * d, SID_NAME_USE s) : SidNameUser(s) {
        wcscpy(UserName, u);
        wcscpy(DomaineName, d);
    };

} USERW, * PUSERW;

typedef struct MODULE_ENTRY
{
    std::wstring Name = L"";
    std::wstring path = L"";
    PVOID Address = 0;
    struct MODULE_ENTRY * Next = nullptr;
};

template<typename T = PVOID>
struct EnumObj
{
    

public:
   
    EnumObj<T> * Next = nullptr;
    T element = nullptr;
    EnumObj<T>* Base;
    EnumObj(T e=NULL, EnumObj<T>* n=nullptr, EnumObj<T>* b = nullptr) : element(e), Next(n), Base(b) {};
}; 


template<typename T = PVOID>
struct MemBlock
{
    T BaseAddress;
    size_t size;
};


class NTapi
{
public:
    
    static void Init();

    [[nodiscard]] static EnumObj<PSYSTEM_PROCESS_INFORMATION> * WINAPI GetAllProcess();
    [[nodiscard]] static PSYSTEM_PROCESS_INFORMATION WINAPI GetProcessInfo(std::string appname);
    [[nodiscard]] static PSYSTEM_PROCESS_INFORMATION WINAPI GetProcessInfo(DWORD ProcessPid);
    [[nodiscard]] static EnumObj<PSYSTEM_PROCESS_INFORMATION>* WINAPI GetAllProcessInfo(std::string ProcessName);
    static DEFAULT_SIZE WINAPI  GetModuleAddress(std::string ModuleName);
    static DEFAULT_SIZE WINAPI  GetFuncAddress(DEFAULT_SIZE module, const char* function_name);
    [[nodiscard]] static MODULE_ENTRY * WINAPI GetModules();
    [[nodiscard]] static MODULE_ENTRY * WINAPI GetImportModules(DEFAULT_SIZE ModuleAddress);
    static PVOID WINAPI ModuleExist(DEFAULT_SIZE base);
    static PVOID WINAPI ModuleExistEx(MODULE_ENTRY* Modules, std::string ModulePath);

    static std::wstring WINAPI ReadUTF16EX(HANDLE hauth, PWSTR  address);
    static std::string  WINAPI ReadUTF8EX(HANDLE hauth, PSTR  address);
    static [[nodiscard]] MODULE_ENTRY * WINAPI GetModulesEx(const char* AppName);
    static [[nodiscard]] MODULE_ENTRY * WINAPI GetModulesEx(DWORD pid);
    static DWORD WINAPI SetPageAccess(void* base, ACCESS_MASK DesiredAccess);
    static DWORD WINAPI  SetPageAccess(HANDLE HandleProcess, void* base, ACCESS_MASK DesiredAccess);
    static DWORD WINAPI GetPageAccess(void* base);
    static HWND         GetWindowProcess(DWORD pid);
    static HWND         GetWindowProcess(const char * ProcessName); 

    template<class T>
    static void FreeEnumObj(EnumObj<T> * Enum_Obj)
    {
        EnumObj<T>* _entry = Enum_Obj->Base;

        EnumObj<T>* _prev = nullptr;

        while (true)
        {
            if (_prev)
                delete _prev;

            if (_entry == nullptr)
                break;

            _prev = _entry;

            _entry = _entry->Next;
        }
    };

    static void SetWindowFocus(DWORD pid);
    static void SetWindowFocus(const char* ProcessName);

    static EnumObj<SYSTEM_HANDLE_TABLE_ENTRY_INFO> * GetHandlesProcess(const char* ProcessName);
    static EnumObj<SYSTEM_HANDLE_TABLE_ENTRY_INFO> * GetHandlesProcess(DWORD Pid);
    static EnumObj<SYSTEM_HANDLE_TABLE_ENTRY_INFO> * GetAllHandles();

    static TEB GetCurrentTeb();

    static bool  ProcessExist(std::string ProcessName);
    static bool  ProcessExist(DWORD Pid);

    static HANDLE DublicateHandle(DWORD ProcessId, HANDLE HandleToDuplcate, bool IsInirt=false, ACCESS_MASK Mask = DUPLICATE_SAME_ACCESS);
    static HANDLE DublicateHandle(const char * ProcessName, HANDLE HandleToDuplcate, bool IsInirt=false, ACCESS_MASK Mask = DUPLICATE_SAME_ACCESS);

    static HANDLE OpenProcess(DWORD Pid, ACCESS_MASK Mask);
    static HANDLE OpenProcess(const char * ProcessName, ACCESS_MASK Mask);

    static std::string  GetProcessPath(std::string ProcessName);
    static std::string  GetProcessPath(DWORD pid);

    static BYTE IsWow64(std::string ProcessName);
    static BYTE IsWow64(DWORD ProcessId);

    static BOOL SetPrivilege(HANDLE hToken, LPCSTR lpszPrivilege, BOOL bEnablePrivilege);

    static PSID  GetCurrentPSID();
    static LPSTR PSIDToStringA(PSID psid);
    static LPWSTR PSIDToStringW(PSID psid);     

    static std::string UTF16toUTF8(std::wstring ws);

    static PUSERA GetCurrentUserA();
    static PUSERW GetCurrentUserW();

    static PSYSTEM_PROCESS_INFORMATION GetCurrentProcess();

    static void NtMemCpy(PVOID dst, PVOID src, size_t size);
    static void NtZeroMemory(BYTE* src, size_t size,BYTE c=0x00);
    static bool NtMemCcmp(BYTE* src, BYTE c,size_t size);

    static NTSTATUS SetHANDLEProtect(HANDLE Handle, bool LockedClose, bool IsInirt = false);

    static std::string RandomWordA(WORD Length);
    static std::wstring RandomWordW(WORD Length);

    static bool FileExistA(const char    * PathFile);
    static bool FileExistW(const wchar_t * PathFile);
    static bool CopyFileA(std::string dst, std::string src);
    static bool CopyFileW(std::wstring dst, std::wstring src);

    static std::string FileExtensionA(std::string path);
    static std::wstring FileExtensionW(std::wstring path);

    static std::string  GetFileNamefromPathA(std::string path);
    static std::wstring GetFileNamefromPathW(std::wstring path);

    static std::string  GetDirPathFromPathA(std::string   path);
    static std::wstring GetDirPathFromPathW(std::wstring path);

    static void RemoveFromPEBEx(DWORD pid,std::string DllName);
    static void RemoveFromPEBEx(const char * ProcessName, std::string DllName);

    static PVOID SearchCodeCave(HANDLE ProcessHandle, ACCESS_MASK DesiredAccess = PAGE_READWRITE,size_t NeedSize = MAX_PATH);

    static CUNICODE_STRING * GetHandleType(HANDLE handle);

    static PEB Peb;
    static TEB Teb;

private:

    static [[nodiscard]] MODULE_ENTRY * WINAPI __get_modules_ex__(DWORD pid);
    static HANDLE nt_mutex;
    static HANDLE nt_event_call;

    static void WINAPI __enum_process__(std::function<bool(PSYSTEM_PROCESS_INFORMATION)> callback);

    static PSYSTEM_PROCESS_INFORMATION selected_process_info;

    struct __window_callback_arg_
    {
        DWORD  pid = 0;
        HWND   hwnd = 0;
    };                                                                                    

    static struct __temp_process__
    {
        PSYSTEM_PROCESS_INFORMATION Entry = nullptr;
        PVOID Base = nullptr;
        ULONG size = 0;

    } temp_process;

    static BOOL WINAPI __window_callback__(HWND hwnd, LPARAM lParam);

};