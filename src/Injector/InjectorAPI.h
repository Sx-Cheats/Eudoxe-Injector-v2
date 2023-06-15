#pragma once

#ifndef EUDOXE_INJECTOR_API
#define EUDOXE_INJECTOR_API


#include "../NTapi/NTapi.h"

#define ASMJIT_EMBED  
#define ASMJIT_STATIC

#include "../asmjit/headers/asmjit.h"
#include <mutex>
																		 
#ifdef _WIN64
#pragma comment(lib,"C:\\Users\\Benois\\Desktop\\HOOK\\asmjit\\libs\\Multi-threadding_debug-dll-lib(MDd)\\x64\\asmjit.lib")
#elif _WIN32
#pragma comment(lib,"C:\\Users\\Benois\\Desktop\\HOOK\\asmjit\\libs\\Multi-threadding_debug-dll-lib(MDd)\\x32\\asmjit.lib")
#endif


using namespace asmjit;

class EudoxeInjectorAPI_t
{
private:

	x86::Assembler assembler = x86::Assembler();
	void JitClear(DEFAULT_SIZE BaseAddress = 0)
	{
		code.detach(&assembler);
		code.reset();
		code.init(rt.environment(), rt.cpuFeatures(), BaseAddress);
		code.attach(&assembler);
	}

public:

	std::mutex injector_mutex;

	JitRuntime rt = JitRuntime();
	CodeHolder code = CodeHolder();

	EudoxeInjectorAPI_t() : assembler(&code) {};

	enum class InjectStatus
	{
		NA, // 0 
		INJECT_SUCESS, // 1
		INJECT_FAILED, // 2 
		FAILED_WRITE_PROXY_ENTRYPOINT,	  // 3
		FAILED_ALLOC,  // 4 
		FAILED_WRITE_DLL_PATH, // 5 
		PROCESS_NOT_FOUND, // 7 
		DLL_NOT_FOUND,	   // 8
		FAILED_OPEN_PROCESS,	// 9 
		TIMEOUT_EXCEEDED // 10
	};

	enum class InjectMethod
	{
		CreateRemoteThread_,
		NtCreateThreadEx_,
		QueueUserAPC_,
		ThreadHiJacking_,
		NtCreateUsreThread_,
	};

	enum class FunctionToLoadDll
	{
		LoadLibraryExA_,
		LoadLibraryExW_,
	};

	typedef struct InjectSettings
	{
		bool              HiJackHandle     = false;
		bool              RandomFileName   = false;
		bool              UseCodeCave      = false;
		bool              RemoveFromPEB    = false;
		InjectMethod      Method           = InjectMethod::CreateRemoteThread_;
		InjectStatus      Status           = InjectStatus::NA;
		FunctionToLoadDll FunctionToUse    = FunctionToLoadDll::LoadLibraryExA_;
		float             Timeout          = 10.0f;
		DWORD             Pid              = 0;
	};

	InjectStatus Inject(LPCSTR ProcessName, std::string DllPath, InjectSettings Settings={false,false,false,false,InjectMethod::CreateRemoteThread_});
	InjectStatus Inject(DWORD ProcessPid, std::string DllPath, InjectSettings Settings = { false,false,false,false,InjectMethod::CreateRemoteThread_ });

private:

	typedef struct __ProxyInject
	{
		struct
		{
			bool IsAllocate = false;
			MemBlock<PVOID> * location = nullptr;

		} BaseAddress;

		struct
		{
			bool IsAllocate = false;
			MemBlock<PVOID> * location = nullptr;

		} DllPath;

		HANDLE HandleToProcess = INVALID_HANDLE_VALUE;

		InjectSettings Settings{};

		DWORD ProxyPageOldProtect = 0;

	}  ProxyInjection, * PProxyInjection;

	MemBlock<PVOID> * __build_caller_dll_loader__(PVOID dllpathex,PVOID proxy_build_base_address, InjectSettings settings);

	void __clear__(PProxyInjection proxy);

	ProxyInjection* __build_proxy__(DWORD pid,std::string & dll_path, InjectSettings settings);

	InjectStatus __inject__(DWORD pid, std::string & dll_path, InjectSettings pettings);
	InjectSettings __check_param__(DWORD pid, std::string &dll_path, InjectSettings settings);

	void __createremotethread__(ProxyInjection * proxy);
	void __ntcreatethreadex__(ProxyInjection* proxy);
	void __queueuserapc__(ProxyInjection* proxy, std::string& dll_path);
	void __rtlcreateusrethread__(ProxyInjection* proxy);
	void __threadhijacking__(ProxyInjection* proxy, std::string& dll_path);
};

#endif
