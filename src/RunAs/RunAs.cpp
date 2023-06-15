#include "RunAs.h"




std::string  __stdcall RunAs::WhoamI()
{
	char  user[MAX_PATH];
	char  domain[MAX_PATH];
	SID_NAME_USE  snu;
	LookupAccountSidA(NULL, NTapi::GetCurrentPSID(), user, (new DWORD{ MAX_PATH }), domain, (new DWORD{ MAX_PATH }), &snu);

	return std::string(domain) + "\\" + std::string(user);
}


RunAs::Acc __stdcall  RunAs::IsRunningLikeWho()
{
	HANDLE tk = NULL;

	Acc a = Acc::Null;

	if (NtOpenProcessToken(NtCurrentProcess(), TOKEN_READ, &tk))
		return Acc::Null;

	DWORD dwSize;


	PSID pSID = NULL;
	ConvertStringSidToSidA("S-1-5-18", &pSID);

	if (EqualSid(pSID, NTapi::GetCurrentPSID()))
	{
		a = Acc::System;
		goto _ret;

	}

	TOKEN_ELEVATION Elevation;

	NtQueryInformationToken(tk, TokenElevation, &Elevation, sizeof(Elevation), (new DWORD{ sizeof(TOKEN_ELEVATION) }));

	if (Elevation.TokenIsElevated)
		a = Acc::Admin;
	else
		a = Acc::User;

_ret:

	FreeSid(pSID);
	NtClose(tk);

	return a;

}

bool RunAs::StartProgramAs(std::string ProgramPath, RunAs::Acc LikeWho)
{
	if (LikeWho == Acc::User || LikeWho == Acc::Admin)
	{
		SHELLEXECUTEINFOA sei = { sizeof(sei) };

		if (LikeWho == Acc::User)
			sei.lpVerb = "open";

		else if (LikeWho == Acc::Admin)
			sei.lpVerb = "runas";

		
		sei.lpFile = ProgramPath.c_str();
		sei.hwnd = NULL;
		sei.nShow = SW_NORMAL;

		if (ShellExecuteExA(&sei))
			return true;
		else
			return false;

	} else if (LikeWho == Acc::Null)
	{
		STARTUPINFOA startupInfo;
		PROCESS_INFORMATION processInformation;
		ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
		ZeroMemory(&processInformation, sizeof(PROCESS_INFORMATION));
		startupInfo.cb = sizeof(STARTUPINFO);

		return (bool)(CreateProcessA(
			(char*)ProgramPath.c_str(),
			NULL, 
			nullptr,                   
			nullptr,                  
			TRUE,                       
			CREATE_DEFAULT_ERROR_MODE,
			nullptr,                    
			nullptr,                  
			&startupInfo,               
			&processInformation                
		) != 0);

	} else if (LikeWho == Acc::System)
	{
		if ((BYTE)RunAs::IsRunningLikeWho() <= 1)
			return false;

		HANDLE Pr = NTapi::OpenProcess((DWORD)NTapi::GetProcessInfo("winlogon.exe")->UniqueProcessId, PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION);
		HANDLE Tk = NULL;

		if (NT_SUCCESS(NtOpenProcessToken(Pr, TOKEN_QUERY | TOKEN_DUPLICATE, &Tk)))
		{
			HANDLE CloneHSys;
			if (NT_SUCCESS(DuplicateTokenEx(Tk, TOKEN_ADJUST_DEFAULT | (0x0100) | TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, NULL, SecurityImpersonation, TokenPrimary, &CloneHSys)))
			{

				STARTUPINFOW startupInfo;
				PROCESS_INFORMATION processInformation;
				ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
				ZeroMemory(&processInformation, sizeof(PROCESS_INFORMATION));
				startupInfo.cb = sizeof(STARTUPINFO);

				std::wstring path(ProgramPath.begin(), ProgramPath.end());

				if (CreateProcessWithTokenW(CloneHSys, LOGON_WITH_PROFILE, path.c_str(), NULL, 0, NULL, NULL, &startupInfo, &processInformation) != 0)
				{
					NtClose(Pr);
					NtClose(Tk);
					NtClose(CloneHSys);
					return true;
				}
			}

			NtClose(CloneHSys);
		}

		NtClose(Pr);
		NtClose(Tk);

		return false;

	}

	return false;
}


bool __stdcall RunAs::StartAs(RunAs::Acc LikeWho)
{
	SHELLEXECUTEINFOW sei = { sizeof(sei) };

	if (LikeWho == Acc::User || LikeWho == Acc::Admin)
	{
		if (LikeWho == Acc::User)
			sei.lpVerb = L"open";
		if (LikeWho == Acc::Admin)
			sei.lpVerb = L"runas";

		sei.lpFile = NTapi::Peb.ProcessParameters->ImagePathName.Buffer;
		sei.hwnd = NULL;
		sei.nShow = SW_NORMAL;

		if (ShellExecuteExW(&sei))
			return true;
		else
			return false;

	}
	else if (LikeWho == Acc::System)
	{

		if ((BYTE)RunAs::IsRunningLikeWho() <= 1)
			return false;

		HANDLE Pr = NTapi::OpenProcess((DWORD)NTapi::GetProcessInfo("winlogon.exe")->UniqueProcessId, PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION);
		HANDLE Tk;

		if (NT_SUCCESS(NtOpenProcessToken(Pr, TOKEN_QUERY | TOKEN_DUPLICATE, &Tk)))
		{
			HANDLE CloneHSys;
			if (NT_SUCCESS(DuplicateTokenEx(Tk, TOKEN_ADJUST_DEFAULT | (0x0100) | TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, NULL, SecurityImpersonation, TokenPrimary, &CloneHSys)))
			{

				STARTUPINFOW startupInfo;
				PROCESS_INFORMATION processInformation;
				ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
				ZeroMemory(&processInformation, sizeof(PROCESS_INFORMATION));
				startupInfo.cb = sizeof(STARTUPINFO);

				if (CreateProcessWithTokenW(CloneHSys, LOGON_WITH_PROFILE, NTapi::Peb.ProcessParameters->ImagePathName.Buffer, NULL, 0, NULL, NULL, &startupInfo, &processInformation) != 0)
				{
					NtClose(Pr);
					NtClose(Tk);
					NtClose(CloneHSys);
					return true;
				}

			}

			NtClose(CloneHSys);

		}

		NtClose(Pr);
		NtClose(Tk);

		return false;

	}

	return false;
}