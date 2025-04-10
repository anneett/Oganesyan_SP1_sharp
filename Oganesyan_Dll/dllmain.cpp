// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

struct MessageHeader
{
    int to;
    int from;
    int type;
    int size;
};

HANDLE hMutex = CreateMutex(NULL, FALSE, L"FileMapMutex");

extern "C" {

    __declspec(dllexport) void __stdcall startThread(int selected_thread, const wchar_t* text)
    {

    }

    __declspec(dllexport) void __stdcall stopThread(int selected_thread, const wchar_t* text)
    {

    }

    __declspec(dllexport) void __stdcall sendData(int selected_thread, const wchar_t* text)
    {

        ReleaseMutex(hMutex);
    }

    __declspec(dllexport) void __stdcall confirmThread(int selected_thread, const wchar_t* text)
    {
        
    }

}