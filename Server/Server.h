#pragma once

class Server
{
public:
	static Server& getInstance();

private:
	Server();
	~Server();

	Server(Server& other)			= delete;
	void operator=(Server& other)	= delete;

};

