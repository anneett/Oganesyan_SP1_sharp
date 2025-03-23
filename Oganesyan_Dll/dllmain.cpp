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

struct header
{
    int addr;
    int size;
};

HANDLE hMutex = CreateMutex(NULL, FALSE, L"GlobalFileMapMutex");

wstring GetLastErrorString(DWORD ErrorID = 0)
{
    if (!ErrorID)
        ErrorID = GetLastError();
    if (!ErrorID)
        return wstring();

    LPWSTR pBuff = nullptr;
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, ErrorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&pBuff, 0, NULL);
    wstring s(pBuff, size);
    LocalFree(pBuff);

    return s;
}

HANDLE mapsend(int addr, const wchar_t* str)
{
    header h = { addr, int(wcslen(str) * 2) };
    HANDLE hFile = CreateFile(L"filemap.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
    //	hFile = INVALID_HANDLE_VALUE;

    HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, h.size + sizeof(header), L"MyMap");
    BYTE* buff = (BYTE*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.size + sizeof(header));

    memcpy(buff, &h, sizeof(header));
    memcpy(buff + sizeof(header), str, h.size);

    UnmapViewOfFile(buff);
    //	return hFileMap;

    CloseHandle(hFileMap);
    CloseHandle(hFile);
    return hFileMap;
}

wstring mapreceive(header& h)
{
    HANDLE hFile = CreateFile(L"filemap.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
    //	hFile = INVALID_HANDLE_VALUE;

    HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, sizeof(header), L"MyMap");

    LPVOID buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(header));
    h = *((header*)buff);
    UnmapViewOfFile(buff);
    CloseHandle(hFileMap);

    int n = h.size + sizeof(header);
    hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, n, L"MyMap");
    buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, n);
    //  wcout << GetLastErrorString();
    wstring s((wchar_t*)((BYTE*)buff + sizeof(header)), h.size / 2);

    UnmapViewOfFile(buff);
    CloseHandle(hFileMap);
    CloseHandle(hFile);
    return s;
}

extern "C" {

    __declspec(dllexport) void __stdcall SendData(int selected_thread, const wchar_t* text)
    {
        WaitForSingleObject(hMutex, INFINITE);
        mapsend(selected_thread, text);
        ReleaseMutex(hMutex);

        //  if (hSendEvent != NULL)
        //  {
        //      SetEvent(hSendEvent);
        //      CloseHandle(hSendEvent);
        //  }
    };

    //  __declspec(dllimport) string getMessage(header& h) {
    // 
    //  };
}