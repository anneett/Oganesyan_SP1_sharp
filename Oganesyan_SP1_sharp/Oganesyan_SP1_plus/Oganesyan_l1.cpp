#include "SysProg.h"
#include "Session.h"
#include "../../Oganesyan_Dll/dllmain.cpp"

using namespace std;

//void launchClient(wstring path)
//{
//	STARTUPINFO si = { sizeof(si) };
//	PROCESS_INFORMATION pi;
//	CreateProcess(NULL, path.data(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
//	CloseHandle(pi.hThread);
//	CloseHandle(pi.hProcess);
//}

map<int, shared_ptr<Session>> sessions;
mutex sessionsMutex;
int maxID = 0;

//enum MessageTypes
//{
//	MT_CLOSE,
//	MT_DATA,
//};

//struct MessageHeader
//{
//	int messageType;
//	int size;
//};

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
			//MT_EXIT
			{
				SafeWrite(L"session", session->sessionID, L"closed");
				delete session;
				return;
			}
			case MT_DATA:
			{
				wstring filename = to_wstring(session->sessionID) + L".txt";
				wofstream fout(filename, ios::app);
				fout.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
				if (fout.is_open())
				{
					fout << m.data << "\n";
					fout.close();
				}

				Sleep(500 * session->sessionID);
				break;
			}
			}
		}
	}
	return;
}

void processClient(tcp::socket s)
{

	try
	{
		Message m;
		//vector<Session*> sessions;
		//vector<thread> threads;
		int code = m.receive(s);
		cout << m.header.to << ": " << m.header.from << ": " << m.header.type << ": " << code << endl;
		switch (code)
		{
		case MT_INIT:
		{
			unique_lock<mutex> lock(sessionsMutex);
			auto session = make_shared<Session>(++maxID, m.data);
			sessions[session->id] = session;
			threads.emplace_back(MyThread, sessions.back());
			//Message::send(s, session->id, MR_BROKER, MT_INIT);
			break;
		}
		case MT_EXIT:
		{
			unique_lock<mutex> lock(sessionsMutex);
			sessions.erase(m.header.from);
			//Message::send(s, m.header.from, MR_BROKER, MT_CONFIRM);
			break;
		}
		case MT_GETDATA:
		{
			unique_lock<mutex> lock(sessionsMutex);
			auto iSession = sessions.find(m.header.from);
			if (iSession != sessions.end())
			{
				iSession->second->send(s);
			}
			break;
		}
		default:
		{
			unique_lock<mutex> lock(sessionsMutex);
			auto iSessionFrom = sessions.find(m.header.from);
			if (iSessionFrom != sessions.end())
			{
				auto iSessionTo = sessions.find(m.header.to);
				if (iSessionTo != sessions.end())
				{
					iSessionTo->second->add(m);
				}
				else if (m.header.to == MR_ALL)
				{
					for (auto& pair : sessions)
					{
						if (pair.first != m.header.from)
							pair.second->add(m);
					}
				}
				//Message::send(s, m.header.from, MR_BROKER, MT_CONFIRM);
			}
			break;
		}
		}
	}
	catch (std::exception& e)
	{
		std::wcerr << "Exception: " << e.what() << endl;
	}
}


int main(int argc, char* argv[])
{
	std::locale::global(std::locale("rus_rus.866"));
	wcin.imbue(std::locale());
	wcout.imbue(std::locale());

	//SetConsoleOutputCP(65001);
	//SetConsoleCP(65001);
	//wcout.imbue(locale("ru_RU.UTF-8"));

	try
	{
		int port = 12345;
		boost::asio::io_context io;
		tcp::acceptor a(io, tcp::endpoint(tcp::v4(), port));

		//launchClient(L"AsioClient.exe");
		//launchClient(L"SharpClient.exe");

		while (true)
		{
			tcp::socket socket(io);
			a.accept(socket);
			std::thread(processClient, a.accept()).detach();
		}
	}
	catch (std::exception& e)
	{
		std::wcerr << "Exception: " << e.what() << endl;
	}

    return 0;
}