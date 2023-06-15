#pragma once

#include "InjectorAPI.h"

#include <Psapi.h>

#pragma comment(lib,"psapi")


EudoxeInjectorAPI_t::InjectStatus EudoxeInjectorAPI_t::Inject(LPCSTR ProcessName, std::string DllPath, InjectSettings Settings)
{
	return Inject((DWORD)NTapi::GetProcessInfo(ProcessName)->UniqueProcessId, DllPath, Settings);
}

EudoxeInjectorAPI_t::InjectStatus EudoxeInjectorAPI_t::Inject(DWORD Pid, std::string DllPath, InjectSettings Settings)
{
   return  __inject__(Pid,DllPath, Settings);
}	

EudoxeInjectorAPI_t::InjectSettings EudoxeInjectorAPI_t::__check_param__(DWORD pid, std::string & dll_path,InjectSettings settings)
{
	if (pid <= 0 || !NTapi::ProcessExist(pid))
	{
		settings.Status = InjectStatus::PROCESS_NOT_FOUND;
		return settings;
	}

	if (dll_path.empty() || !NTapi::FileExistA(dll_path.c_str()))
	{
		settings.Status = InjectStatus::DLL_NOT_FOUND;
		return settings;

		return settings;
	}

	if (strcmp(NTapi::FileExtensionA(dll_path.c_str()).c_str(), ".dll"))
	{
		settings.Status = InjectStatus::DLL_NOT_FOUND;
		return settings;
	}

	MODULE_ENTRY* modules = NTapi::GetModulesEx(pid);

	if (settings.RandomFileName)
	{
		std::string new_file_path = std::filesystem::path(dll_path).parent_path().string()+"\\"+NTapi::RandomWordA(32)+".dll";
		if (NTapi::CopyFileA(new_file_path, dll_path))
			dll_path = new_file_path;
	} 
	
	return 	settings;
}


#ifdef  _WIN64

MemBlock<PVOID> * EudoxeInjectorAPI_t::__build_caller_dll_loader__(PVOID dllpathex, PVOID proxy_build_base_address, InjectSettings settings)
{
	JitClear((DWORD64)proxy_build_base_address);

	assembler.push (x86::rbp);
	assembler.sub(x86::rsp, 0x30);
	assembler.mov(x86::rcx, (DWORD64)dllpathex);

	assembler.xor_(x86::r8d, x86::r8d);
	assembler.xor_(x86::edx, x86::edx);

	if(settings.FunctionToUse == FunctionToLoadDll::LoadLibraryExA_)
     	assembler.mov(x86::rax,NTapi::GetFuncAddress(NTapi::GetModuleAddress("KERNELBASE.dll"), "LoadLibraryExA"));
	else if (settings.FunctionToUse == FunctionToLoadDll::LoadLibraryExW_)
		assembler.mov(x86::rax,NTapi::GetFuncAddress(NTapi::GetModuleAddress("KERNELBASE.dll"), "LoadLibraryExW"));

	assembler.call(x86::rax);
	assembler.add(x86::rsp, 0x30);
	assembler.pop(x86::rbp);
	assembler.ret();

	MemBlock<PVOID> * mb = new MemBlock;
																	   
	mb->BaseAddress = (PBYTE)(PBYTE)code.sectionById(0)->buffer().data();

	mb->size =  code.codeSize();

	return mb;
}

#elif _WIN32

MemBlock<PVOID>* EudoxeInjectorAPI_t::__build_caller_dll_loader__(PVOID dllpathex, PVOID proxy_build_base_address, InjectSettings settings)
{
	JitClear((DWORD64)proxy_build_base_address);

	assembler.push(0x00);
	assembler.push(0x00);
	assembler.push(dllpathex);

	if (settings.FunctionToUse == FunctionToLoadDll::LoadLibraryExA_)
		assembler.call(NTapi::GetFuncAddress(NTapi::GetModuleAddress("KERNELBASE.dll"), "LoadLibraryExA"));
	else if (settings.FunctionToUse == FunctionToLoadDll::LoadLibraryExW_)
		assembler.call( NTapi::GetFuncAddress(NTapi::GetModuleAddress("KERNELBASE.dll"), "LoadLibraryExW"));

	assembler.ret();	  

	MemBlock<PVOID> * mb = new MemBlock;

    PBYTE PathDllEntry = (PBYTE)dllpathex;

	PBYTE d = (PBYTE)NTapi::GetFuncAddress(NTapi::GetModuleAddress("KERNELBASE.dll"), "LoadLibraryExA");

	mb->BaseAddress =   (PBYTE)(PBYTE)code.sectionById(0)->buffer().data();

	mb->size = code.codeSize();

	return mb;
}


#endif

EudoxeInjectorAPI_t::ProxyInjection * EudoxeInjectorAPI_t::__build_proxy__(DWORD pid,std::string & dll_path, InjectSettings settings)
{
	HANDLE HandleToProcess= INVALID_HANDLE_VALUE;
	PProxyInjection Pi = new ProxyInjection;
	Pi->Settings = settings;

	ACCESS_MASK am = (PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE);

	if (settings.HiJackHandle)
	{
		EnumObj<SYSTEM_HANDLE_TABLE_ENTRY_INFO> * ProcessHandle = NTapi::GetAllHandles();
		do
		{
			if ((DWORD)ProcessHandle->element.ObjectTypeIndex == 0x7 && ProcessHandle->element.GrantedAccess == 0x1fffff)
			{
				HandleToProcess = NTapi::DublicateHandle((DWORD)ProcessHandle->element.UniqueProcessId,(HANDLE)ProcessHandle->element.HandleValue,false);

				if (HandleToProcess != INVALID_HANDLE_VALUE)
				{
					if (GetProcessId(HandleToProcess) == pid)
							break;
					else
					{																																															 
						NtClose(HandleToProcess);
						HandleToProcess = INVALID_HANDLE_VALUE;
					}
				}
			} 

		} while ((ProcessHandle=ProcessHandle->Next)->Next);

		NTapi::FreeEnumObj(ProcessHandle);
	} 

	if (HandleToProcess == INVALID_HANDLE_VALUE)
		HandleToProcess = NTapi::OpenProcess(pid, PROCESS_ALL_ACCESS );

	

	if (HandleToProcess == INVALID_HANDLE_VALUE)
	{
		Pi->Settings.Status = InjectStatus::FAILED_OPEN_PROCESS;
		return Pi;
	}

	Pi->HandleToProcess = HandleToProcess;

	NTapi::SetHANDLEProtect(HandleToProcess, true, false);

	PVOID DllPathEx = nullptr, EntryPointEx = nullptr;
	size_t dlllen = 0;

	if (settings.FunctionToUse == FunctionToLoadDll::LoadLibraryExW_)
		dlllen = (dll_path.size() * 2) + 3;

	else  if (settings.FunctionToUse == FunctionToLoadDll::LoadLibraryExA_)
		dlllen = dll_path. size();


	if (settings.UseCodeCave)
		DllPathEx = NTapi::SearchCodeCave(HandleToProcess, PAGE_READWRITE, dlllen);

	if (!DllPathEx)
	{
		SIZE_T a = dlllen;
		if (NT_SUCCESS(NtAllocateVirtualMemory(HandleToProcess, &DllPathEx, 0, &a, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)))
			Pi->DllPath.IsAllocate = true;
	}

	if (DllPathEx == nullptr)
	{
		Pi->Settings.Status = InjectStatus::FAILED_ALLOC;
		return Pi;
	}

	if (settings.FunctionToUse == FunctionToLoadDll::LoadLibraryExA_)
	{
		if (!NT_SUCCESS(NtWriteVirtualMemory(HandleToProcess, DllPathEx, (PVOID)dll_path.c_str(), dlllen, nullptr)))
		{
			Pi->Settings.Status = InjectStatus::FAILED_WRITE_DLL_PATH;
			return Pi;
		}
	}
	else if (settings.FunctionToUse == FunctionToLoadDll::LoadLibraryExW_)
	{
		std::wstring wdll(dll_path.begin(), dll_path. end());

		if (!NT_SUCCESS(NtWriteVirtualMemory(HandleToProcess, DllPathEx, (PVOID)wdll.c_str(), dlllen, nullptr)))
		{
			Pi->Settings.Status = InjectStatus::FAILED_WRITE_DLL_PATH;
			return Pi;
		}
	}

	if (settings.UseCodeCave)
	{
		EntryPointEx = NTapi::SearchCodeCave(HandleToProcess, PAGE_READWRITE, 68);
		if (EntryPointEx)
			Pi->ProxyPageOldProtect = NTapi::SetPageAccess(HandleToProcess, EntryPointEx, PAGE_EXECUTE_READWRITE);
	}

	if (EntryPointEx == nullptr)
	{
		SIZE_T a = 68;
		if (NT_SUCCESS(NtAllocateVirtualMemory(HandleToProcess, &EntryPointEx, 0, &a, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE)))
			Pi->BaseAddress.IsAllocate = true;
	}

	if (EntryPointEx == nullptr)
	{
		Pi->Settings.Status = InjectStatus::FAILED_ALLOC;
		return Pi;
	}

	MemBlock<PVOID> * memblock = __build_caller_dll_loader__(DllPathEx, EntryPointEx, settings);

	if (!NT_SUCCESS(NtWriteVirtualMemory(HandleToProcess, EntryPointEx, (PVOID)memblock->BaseAddress, memblock->size, nullptr)))
	{
		delete memblock;

		Pi->Settings.Status = InjectStatus::FAILED_WRITE_PROXY_ENTRYPOINT;
		return Pi;
	}

	Pi->BaseAddress.location = new MemBlock();
	Pi->BaseAddress.location->BaseAddress = (PBYTE)EntryPointEx;

	Pi->BaseAddress.location->size = memblock->size;

	Pi->DllPath.location = new MemBlock();
	Pi->DllPath.location->BaseAddress = (PBYTE)DllPathEx;
	Pi->DllPath.location->size = dlllen;

	Pi->Settings.Status = InjectStatus::NA;

	delete memblock;


	return Pi;
}

void EudoxeInjectorAPI_t::__clear__(PProxyInjection proxy)
{
	JitClear();

		if (proxy->DllPath.location != nullptr)
		{
			PBYTE zero = (PBYTE)std::calloc(1, proxy->DllPath.location->size);
			NtWriteVirtualMemory(proxy->HandleToProcess, proxy->DllPath.location->BaseAddress, (PVOID)zero, proxy->DllPath.location->size, nullptr);
			free(zero);

			if (proxy->DllPath.IsAllocate)
			{
				SIZE_T a = proxy->DllPath.location->size;
				NtFreeVirtualMemory(proxy->HandleToProcess, (PVOID*)&proxy->DllPath.location->BaseAddress, &a, MEM_DECOMMIT | MEM_RELEASE);
			}
		}

		if (proxy->BaseAddress.location != nullptr)
		{
			PBYTE zero = (PBYTE)std::calloc(1, proxy->BaseAddress.location->size);
			NtWriteVirtualMemory(proxy->HandleToProcess, proxy->BaseAddress.location->BaseAddress, (PVOID)zero, proxy->BaseAddress.location->size, nullptr);
			free(zero);

			if (proxy->BaseAddress.IsAllocate)
			{
				SIZE_T a = proxy->BaseAddress.location->size;
				NtFreeVirtualMemory(proxy->HandleToProcess, (PVOID*)&proxy->BaseAddress.location->BaseAddress, &a, MEM_DECOMMIT | MEM_RELEASE);

			}
			else

				NTapi::SetPageAccess(proxy->HandleToProcess, proxy->BaseAddress.location->BaseAddress, proxy->ProxyPageOldProtect);
		}

	if (proxy->HandleToProcess && proxy->HandleToProcess != INVALID_HANDLE_VALUE)
	{
		NTapi::SetHANDLEProtect(proxy->HandleToProcess, false);
		NtClose(proxy->HandleToProcess);
	}
}

EudoxeInjectorAPI_t::InjectStatus EudoxeInjectorAPI_t::__inject__(DWORD pid, std::string & dll_path, InjectSettings settings)
{
	settings = __check_param__(pid,dll_path,settings);

	if (settings.Status != InjectStatus::NA)
		return settings.Status;

	settings.Pid = pid;

	PProxyInjection proxy = __build_proxy__(pid, dll_path, settings);

	if (proxy->Settings.Status != InjectStatus::NA)
	{
		__clear__(proxy);

		return proxy->Settings.Status;
	}

	switch (settings.Method)
	{
	case EudoxeInjectorAPI_t::InjectMethod::CreateRemoteThread_:
		__createremotethread__(proxy);
		break;
	case EudoxeInjectorAPI_t::InjectMethod::NtCreateThreadEx_:
		__ntcreatethreadex__(proxy);
		break;
	case EudoxeInjectorAPI_t::InjectMethod::QueueUserAPC_:
		__queueuserapc__(proxy, dll_path);
		break;
	case EudoxeInjectorAPI_t::InjectMethod::ThreadHiJacking_:
		__threadhijacking__(proxy,dll_path);
		break;
	case EudoxeInjectorAPI_t::InjectMethod::NtCreateUsreThread_:
		__rtlcreateusrethread__(proxy);
		break;
	default:
		break;
	}

	if (proxy->Settings.Status == InjectStatus::INJECT_SUCESS)
	{
		if (proxy->Settings.RemoveFromPEB)
			NTapi::RemoveFromPEBEx(proxy->Settings.Pid, NTapi::GetFileNamefromPathA(dll_path));

		__clear__(proxy);
	}

	

	return proxy->Settings.Status;
}							 



void EudoxeInjectorAPI_t::__createremotethread__(ProxyInjection* proxy)
{

	HANDLE th = CreateRemoteThread(proxy->HandleToProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)proxy->BaseAddress.location->BaseAddress, NULL, NULL, NULL);
	
	if (!th || th == INVALID_HANDLE_VALUE)
	{
		proxy->Settings.Status = InjectStatus::INJECT_FAILED;

		return;
	}

	NTapi::SetWindowFocus(proxy->Settings.Pid);

	DWORD wait_state = WaitForSingleObject(th, proxy->Settings.Timeout * 1e3);

	NtClose(th);

	if (wait_state == WAIT_TIMEOUT)
		proxy->Settings.Status = InjectStatus::TIMEOUT_EXCEEDED;
	else if(wait_state == WAIT_FAILED)
		proxy->Settings.Status = InjectStatus::INJECT_FAILED;
	else
		proxy->Settings.Status = InjectStatus::INJECT_SUCESS;

	return;
}

void EudoxeInjectorAPI_t::__ntcreatethreadex__(ProxyInjection* proxy)
{
	HANDLE th;

	NTSTATUS level = NtCreateThreadEx(&th, THREAD_ALL_ACCESS, NULL, proxy->HandleToProcess, (LPTHREAD_START_ROUTINE)proxy->BaseAddress.location->BaseAddress, NULL, FALSE, FALSE, FALSE, FALSE, FALSE);
	
	if (!NT_SUCCESS(level) || !th)
	{
		proxy->Settings.Status = InjectStatus::INJECT_FAILED;

		return;
	}

	NTapi::SetWindowFocus(proxy->Settings.Pid);

	DWORD wait_state = WaitForSingleObject(th, proxy->Settings.Timeout * 1e3);

	NtClose(th);

	if (wait_state == WAIT_TIMEOUT)
		proxy->Settings.Status = InjectStatus::TIMEOUT_EXCEEDED;
	else if (wait_state == WAIT_FAILED)
		proxy->Settings.Status = InjectStatus::INJECT_FAILED;
	else
		proxy->Settings.Status = InjectStatus::INJECT_SUCESS;

	return;
}

void EudoxeInjectorAPI_t::__queueuserapc__(ProxyInjection* proxy,std::string & dll_path)
{
	auto process_info = NTapi::GetProcessInfo(proxy->Settings.Pid);

	CLIENT_ID ClientId = { 0 };
	HANDLE thh = INVALID_HANDLE_VALUE;
	OBJECT_ATTRIBUTES            obja{};

	for (ULONG it = 0; it < process_info->NumberOfThreads; it++)
	{
		NtInitializeObjectAttributes(&obja, NULL, 0, NULL, NULL);

		NtOpenThread(&thh, THREAD_ALL_ACCESS, &obja, &process_info->Threads[it].ClientId);

		if (!thh)
			continue;

		if (NTapi::ModuleExistEx(NTapi::GetModulesEx(proxy->Settings.Pid), dll_path))
		{
			proxy->Settings.Status = InjectStatus::INJECT_SUCESS;
			NtClose(thh);

			return;
		}

		if (process_info->Threads[it].WaitReason != WrQueue)
		{
			NtClose(thh);
			continue;
		}

		if (!NT_SUCCESS(NtSuspendThread(thh, 0))) {
			NtClose(thh);
			continue;
		}

		if (!NT_SUCCESS(NtQueueApcThread(thh, (PIO_APC_ROUTINE)proxy->BaseAddress.location->BaseAddress, NULL, NULL, NULL)))
		{
			NtAlertResumeThread(thh, 0);
			NtClose(thh);
			continue;
		}
		
		NtAlertResumeThread(thh, 0);

		NTapi::SetWindowFocus(proxy->Settings.Pid);

		DWORD wait_state = WaitForSingleObject(thh, proxy->Settings.Timeout * 1e3);

		if (wait_state == WAIT_TIMEOUT)
			proxy->Settings.Status = InjectStatus::TIMEOUT_EXCEEDED;
		else if (wait_state == WAIT_FAILED)
			proxy->Settings.Status = InjectStatus::INJECT_FAILED;
		else
		{
			proxy->Settings.Status = InjectStatus::INJECT_SUCESS;
			NtClose(thh);

			return;
		}

		NtClose(thh);
	}

	proxy->Settings.Status = InjectStatus::INJECT_FAILED;

	return;
}

void EudoxeInjectorAPI_t::__threadhijacking__(ProxyInjection* proxy, std::string& dll_path)
{
	CONTEXT ThreadContext{};

	ThreadContext.ContextFlags = CONTEXT_CONTROL;

	auto process_info = NTapi::GetProcessInfo(proxy->Settings.Pid);

	CLIENT_ID ClientId = { 0 };
	HANDLE thh = INVALID_HANDLE_VALUE;
	OBJECT_ATTRIBUTES            obja;
	for (ULONG it = 0; it < process_info->NumberOfThreads; it++)
	{
		NtInitializeObjectAttributes(&obja, NULL, 0, NULL, NULL);

	    NtOpenThread(&thh,THREAD_ALL_ACCESS,&obja,&process_info->Threads[it].ClientId);

		if (NTapi::ModuleExistEx(NTapi::GetModulesEx(proxy->Settings.Pid), dll_path))
		{
			proxy->Settings.Status = InjectStatus::INJECT_SUCESS;
			NtClose(thh);

			return;
		}

		if (!thh)
			continue;

		if (!NT_SUCCESS(NtSuspendThread(thh, 0))) {
			NtClose(thh);
			continue;
		}

		if (NT_SUCCESS(NtGetContextThread(thh, &ThreadContext)))
		{
	
			#if   _WIN64		
				ThreadContext.Rip = (DEFAULT_SIZE)proxy->BaseAddress.location->BaseAddress;
			#elif _WIN32
				ThreadContext.Eip = (DEFAULT_SIZE)proxy->BaseAddress.location->BaseAddress;
			#endif

				if (!NT_SUCCESS(NtSetContextThread(thh, &ThreadContext)))
				{
					NtAlertResumeThread(thh, 0);
					NtClose(thh);
					continue;
			    }

			NtAlertResumeThread(thh, 0);

			NTapi::SetWindowFocus(proxy->Settings.Pid);

			DWORD wait_state = WaitForSingleObject(thh, proxy->Settings.Timeout * 1e3);
			if (wait_state == WAIT_TIMEOUT)
				proxy->Settings.Status = InjectStatus::TIMEOUT_EXCEEDED;
			else if (wait_state == WAIT_FAILED)
				proxy->Settings.Status = InjectStatus::INJECT_FAILED;
			else

			{
				proxy->Settings.Status = InjectStatus::INJECT_SUCESS;
				NtAlertResumeThread(thh, 0);
				NtClose(thh);

				return;

			}
		}

		NtAlertResumeThread(thh, 0);
		NtClose(thh);

	}

	proxy->Settings.Status = InjectStatus::INJECT_FAILED;

	return;
}

void EudoxeInjectorAPI_t::__rtlcreateusrethread__(ProxyInjection* proxy)
{

	HANDLE th = INVALID_HANDLE_VALUE;
	

	 NTSTATUS level = RtlCreateUserThread(proxy->HandleToProcess, NULL, false, 0, 0, 0, (LPTHREAD_START_ROUTINE)proxy->BaseAddress.location->BaseAddress, NULL, &th, NULL);
		if (!NT_SUCCESS(level) || th == NULL)
	{
		proxy->Settings.Status = InjectStatus::INJECT_FAILED;

		return;
	}

	NTapi::SetWindowFocus(proxy->Settings.Pid);

	DWORD wait_state = WaitForSingleObject(th, proxy->Settings.Timeout * 1e3);

	if (wait_state == WAIT_TIMEOUT)
		proxy->Settings.Status = InjectStatus::TIMEOUT_EXCEEDED;
	else if (wait_state == WAIT_FAILED)
		proxy->Settings.Status = InjectStatus::INJECT_FAILED;
	else
		proxy->Settings.Status = InjectStatus::INJECT_SUCESS;


	NtClose(th);

	return;
}