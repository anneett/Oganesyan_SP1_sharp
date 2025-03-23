#include "SysProg.h"
#include "../../Oganesyan_Dll/dllmain.cpp"

using namespace std;

HANDLE hEvent;
//	HANDLE hMutex;

enum MessageTypes
{
	MT_CLOSE,
	MT_DATA,
};

struct MessageHeader
{
	int messageType;
	int size;
};

struct Message
{
	MessageHeader header = { 0 };
	wstring data;
	Message() = default;
	Message(MessageTypes messageType, const wstring& data = L"")
		:data(data)
	{
		header = { messageType,  int(data.length()) };
	}
};

class Session
{
	queue<Message> messages;
	CRITICAL_SECTION cs;
	HANDLE hEvent;

public:
	int sessionID;

	Session(int sessionID)
		:sessionID(sessionID)
	{
		InitializeCriticalSection(&cs);
		hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	~Session()
	{
		DeleteCriticalSection(&cs);
		CloseHandle(hEvent);
	}

	void addMessage(Message& m)
	{
		EnterCriticalSection(&cs);
		messages.push(m);
		SetEvent(hEvent);
		LeaveCriticalSection(&cs);
	}

	bool getMessage(Message& m)
	{
		bool res = false;
		WaitForSingleObject(hEvent, INFINITE);
		EnterCriticalSection(&cs);
		if (!messages.empty())
		{
			res = true;
			m = messages.front();
			messages.pop();
		}
		if (messages.empty())
		{
			ResetEvent(hEvent);
		}
		LeaveCriticalSection(&cs);
		return res;
	}

	void addMessage(MessageTypes messageType, const wstring& data = L"")
	{
		Message m(messageType, data);
		addMessage(m);
	}
};

void MyThread(Session* session)
{
	SafeWrite(L"session", session->sessionID, L"created");
	while (true)
	{
		Message m;
		if (session->getMessage(m))
		{
			switch (m.header.messageType)
			{
			case MT_CLOSE:
			{
				SafeWrite(L"session", session->sessionID, L"closed");
				delete session;
				return;
			}
			case MT_DATA:
			{
				//	SafeWrite("session", session->sessionID, "data", m.data);
				if (session->sessionID >= 0)
				{
					wstring filename = to_wstring(session->sessionID) + L".txt";
					wofstream fout(filename, ios::app);
					fout.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
					if (fout.is_open())
					{
						fout << m.data << "\n";
						fout.close();
					}
				}
				Sleep(500 * session->sessionID);
				break;
			}
			}
		}
	}
	return;
}

int main(int argc, char* argv[])
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	InitializeCriticalSection(&cs);
	//	hMutex = CreateMutex(NULL, FALSE, NULL);

	vector<Session*> sessions;
	vector<thread> threads;

	HANDLE hStartEvent = CreateEvent(NULL, FALSE, FALSE, L"StartEvent");
	HANDLE hStopEvent = CreateEvent(NULL, FALSE, FALSE, L"StopEvent");
	HANDLE hCloseEvent = CreateEvent(NULL, FALSE, FALSE, L"CloseEvent");
	HANDLE hSendEvent = CreateEvent(NULL, FALSE, FALSE, L"SendEvent");
	HANDLE hConfirmEvent = CreateEvent(NULL, FALSE, FALSE, L"ConfirmEvent");
	HANDLE hControlEvents[4] = { hStartEvent, hStopEvent, hSendEvent, hCloseEvent };

	int i = 0;

	while (true)
	{
		int n = WaitForMultipleObjects(4, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0;

		switch (n)
		{
		case 0:
		{
			sessions.push_back(new Session(i));
			threads.emplace_back(MyThread, sessions.back());
			i++;
			SetEvent(hConfirmEvent);
			break;
		}
		case 1:
		{
			i--;
			sessions[i]->addMessage(MT_CLOSE);
			threads.back().join();
			threads.pop_back();
			sessions.pop_back();
			
			ResetEvent(hStopEvent);
			SetEvent(hConfirmEvent);
			break;
		}
		case 2:
		{
			header h;
			wstring data = mapreceive(h);
			//	string message = getMessage(h);
			//	sessions[i]->addMessage(MT_DATA, message);
			//	HANDLE hFile = CreateFile(L"file.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, 0);
			////	SetFilePointer

			//	DWORD dwDone;
			//	WriteFile(hFile, &i, sizeof(i), &dwDone, NULL);
			//	wcout << dwDone << endl;
			//	CloseHandle(hFile);

			wstring message(data.begin(), data.end());
			if (h.addr == -2)
			{
				SafeWrite(L"Сообщение отправлено всем потокам.");
				for (auto& sess : sessions)
					sess->addMessage(MT_DATA, message);
				SafeWrite(L"Главный поток:", message);
			}
			else if (h.addr == -1)
			{
				SafeWrite(L"Сообщение отправлено главному потоку.");
				SafeWrite(L"Главный поток:", message);
			}
			else
			{
				SafeWrite(L"Сообщение записано в .txt файл потока", h.addr);
				int index = h.addr;
				if (index >= 0 && index < sessions.size())
				{
					sessions[index]->addMessage(MT_DATA, message);
				}
			}

			ResetEvent(hSendEvent);
			SetEvent(hConfirmEvent);
			break;
		}
		case 3:
		{
			SetEvent(hConfirmEvent);
			return 0;
		}
		}
	}

	//	WaitForMultipleObjects((DWORD)threads.size(), threads.data(), TRUE, INFINITE);

	SetEvent(hConfirmEvent);
	DeleteCriticalSection(&cs);
	//	CloseHandle(hMutex);

    return 0;
}