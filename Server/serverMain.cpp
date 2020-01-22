#include <iostream>
#include <SFML/Network.hpp>

const unsigned short PORT = 20000;
const std::string IPADDRESS("192.168.0.200");

std::string msgSend;

sf::TcpSocket socket;
sf::Mutex globalMutex;
bool quit = false;

void DoStuff(void)
{
	static std::string oldMsg;
	while (!quit)
	{
		sf::Packet packetSend;
		globalMutex.lock();
		packetSend << msgSend;;
		globalMutex.unlock();

		socket.send(packetSend);

		std::string msg;
		sf::Packet packetReceive;

		socket.receive(packetReceive);
		if ((packetReceive >> msg) && oldMsg != msg && !msg.empty())
		{
			std::cout << std::endl << "[Client]: " << msg << std::endl;
			oldMsg = msg;
		}
	}
}

void Server(void)
{
	sf::Socket::Status;

	sf::TcpListener listener;
	listener.listen(PORT);
	listener.accept(socket);
	std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
}

void GetInput(void)
{
	std::string s;
	getline(std::cin, s);
	if (s == "exit")
		quit = true;
	globalMutex.lock();
	msgSend = s;
	globalMutex.unlock();
}


int main()
{
	system("color b");

	sf::Thread* thread = 0;
	
	std::cout << "The server is running..." << std::endl;
	Server();

	thread = new sf::Thread(&DoStuff);
	thread->launch();

	while (!quit)
	{
		GetInput();
	}

	if (thread)
	{
		thread->wait();
		delete thread;
	}

	return 0;
}