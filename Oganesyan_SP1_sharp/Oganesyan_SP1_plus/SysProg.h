#pragma once

#include <windows.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <queue>
#include <string>

#include <map>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>

#include <locale>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include <corecrt_io.h>

using namespace std;

inline void DoWrite()
{
	std::wcout << std::endl;
}

template <class T, typename... Args> inline void DoWrite(T& value, Args... args)
{
	std::wcout << value << L" ";
	DoWrite(args...);
}

static CRITICAL_SECTION cs;
static bool initCS = true;
template <typename... Args> inline void SafeWrite(Args... args)
{
	if (initCS)
	{
		InitializeCriticalSection(&cs);
		initCS = false;
	}
	EnterCriticalSection(&cs);
	DoWrite(args...);
	LeaveCriticalSection(&cs);
}

//mutex m;
//template <typename... Args> inline void SafeWrite(Args... args)
//{
//	lock_guard<mutex> lg(m);
//	DoWrite(args...);
//}
