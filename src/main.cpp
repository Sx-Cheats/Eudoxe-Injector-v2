
#include "./Injector/InjectorAPI.h"
#include "EudoxeInjector.h"
#include <QtWidgets/QApplication>   


bool CheckParma(EudoxeInjector::eudoxe_injector_options injector_option)
{
    if ((injector_option.DllPath.empty()))
    {
        QMessageBox(QMessageBox::Information, "Info", "Invalid dll path !", QMessageBox::Ok, NULL, Qt::Drawer).exec();
        return false;
    }

    if (strcmp(NTapi::FileExtensionA(injector_option.DllPath.c_str()).c_str(), ".dll"))
    {
        QMessageBox(QMessageBox::Information, "Info", "Invalid dll path !", QMessageBox::Ok, NULL, Qt::Drawer).exec();
        return false;
    }

    if (!QFile(injector_option.DllPath.c_str()).exists())
    {
        QMessageBox(QMessageBox::Information, "Info", "Invalid dll path !", QMessageBox::Ok,NULL, Qt::Drawer).exec();
        return false;
    }

    if (injector_option.InjectorOptions.Pid <= 0)
    {
        QMessageBox(QMessageBox::Information, "Info", "Invalid Process !", QMessageBox::Ok, NULL, Qt::Drawer).exec();
        return false;
    }

    if (NTapi::IsWow64(injector_option.InjectorOptions.Pid) != ARCH_PROCESS)
    {
        QMessageBox(QMessageBox::Information, "Info", "Invalid Process !", QMessageBox::Ok, NULL, Qt::Drawer).exec();
        return false;
    }

    HMODULE h = LoadLibraryExA(injector_option.DllPath.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES);

    if (h == 0)
    {
        QMessageBox(QMessageBox::Information, "Info", "dll must be x64", QMessageBox::Ok, NULL, Qt::Drawer).exec();
        FreeLibrary(h);
        return false;
    }


    FreeLibrary(h);

    return true;
}


void read_inject_status(EudoxeInjectorAPI_t::InjectStatus buffer_data)
{
    switch (buffer_data)
    {
        case EudoxeInjectorAPI_t::InjectStatus::INJECT_SUCESS:
        {
            MessageBoxA(NULL, "Success Inject !", "Server", MB_OK);
            break;

        } case EudoxeInjectorAPI_t::InjectStatus::INJECT_FAILED:
        {
            MessageBoxA(NULL, "Inject Failed !", "Client", MB_OK);
            break;

        } case EudoxeInjectorAPI_t::InjectStatus::FAILED_WRITE_PROXY_ENTRYPOINT:
        {
            MessageBoxA(NULL, "Failed Write Proxy Entrypoint !", "Client", MB_OK);
            break;

        } case EudoxeInjectorAPI_t::InjectStatus::FAILED_ALLOC:
        {
            MessageBoxA(NULL, "Failed Alloc !", "Client", MB_OK);
            break;

        } case EudoxeInjectorAPI_t::InjectStatus::FAILED_WRITE_DLL_PATH:
        {
            MessageBoxA(NULL, "Failed Write Dll Path !", "Client", MB_OK);
            break;

        } case EudoxeInjectorAPI_t::InjectStatus::PROCESS_NOT_FOUND:
        {
            MessageBoxA(NULL, "Process Not Found !", "Client", MB_OK);
            break;

        } case EudoxeInjectorAPI_t::InjectStatus::DLL_NOT_FOUND:
        {
            MessageBoxA(NULL, "Dll Not Found !", "Client", MB_OK);
            break;

        } case EudoxeInjectorAPI_t::InjectStatus::FAILED_OPEN_PROCESS:
        {
            MessageBoxA(NULL, "Failed Open Process !", "Client", MB_OK);
            break;

        }case EudoxeInjectorAPI_t::InjectStatus::TIMEOUT_EXCEEDED:
        {
            MessageBoxA(NULL, "Timeout exceeded !", "Client", MB_OK);
            break;
        }

    }
}

int main(int argc, char *argv[])
{
    NTapi::Init();

    if (RunAs::IsRunningLikeWho() == RunAs::Acc::Admin)
    {
        HANDLE HToken = NULL;

        NtOpenProcessToken(NtCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &HToken);
                       
        NTapi::SetPrivilege(HToken, Privileges::SeDebugPrivilege, true);

        NtClose(HToken);

    }

    QApplication a(argc, argv);

    EudoxeInjector w;

    w.BPInject->click([&]
        {
            if (!CheckParma(w.EudoxeInjectorOption))
                return;

            EudoxeInjectorAPI_t::InjectSettings eia_i;

            eia_i.FunctionToUse = (EudoxeInjectorAPI_t::FunctionToLoadDll)w.EudoxeInjectorOption.InjectorOptions.FunctionToUse;
            eia_i.HiJackHandle = w.EudoxeInjectorOption.InjectorOptions.HiJackHandle;
            eia_i.Method = (EudoxeInjectorAPI_t::InjectMethod)w.EudoxeInjectorOption.InjectorOptions.Method;
            eia_i.Pid = w.EudoxeInjectorOption.InjectorOptions.Pid;
            eia_i.RandomFileName = w.EudoxeInjectorOption.InjectorOptions.RandomFileName;
            eia_i.RemoveFromPEB = w.EudoxeInjectorOption.InjectorOptions.RemoveFromPEB;
            eia_i.Timeout = w.EudoxeInjectorOption.InjectorOptions.Timeout;
            eia_i.UseCodeCave = w.EudoxeInjectorOption.InjectorOptions.UseCodeCave;

            EudoxeInjectorAPI_t eia{};

            read_inject_status(eia.Inject(eia_i.Pid, w.EudoxeInjectorOption.DllPath, eia_i));

            Sleep(1500);

        });

    w.show();

    return a.exec();
}
