#pragma once

#include <string>

#ifndef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif // !WIN32

namespace sockets
{
	int createTcpSock();
	int createUdpSock();
	bool bind(int sockfd, std::string ip, uint16_t port);
}