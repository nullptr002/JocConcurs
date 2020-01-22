#pragma once

class Client
{
public:
	static Client& getInstance();

private:
	Client();
	~Client();

	Client(Client& other) = delete;
	void operator=(Client& other) = delete;

};

