#pragma once

#include "Message.h"
#include "asio.h"

class Session
{
public:
	int id;
	wstring name;
	queue<Message> messages;

	mutex mx;
	Session(int id, wstring name)
		:id(id), name(name)
	{
	}

	void add(Message& m)
	{
		lock_guard<mutex> lg(mx);
		messages.push(m);
	}

	void send(tcp::socket& s)
	{
		lock_guard<mutex> lg(mx);
		if (messages.empty())
		{
			Message::send(s, id, MR_BROKER, MT_NODATA);
		}
		else
		{
			messages.front().send(s);
			messages.pop();
		}
	}
};
