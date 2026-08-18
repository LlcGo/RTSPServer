#include "SocketsOps.h"

int sockets::createTcpSock()
{

#ifndef WIN32
 
	int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);

#else
	int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	unsigned long ul = 1;
	int ret = ioctlsocket(sockfd, FIONBIO, (unsigned long*)&ul);

	if (ret == SOCKET_ERROR)
	{

	}
#endif
	
	return sockfd;
}

int sockets::createUdpSock()
{
#ifndef WIN32

	int sockfd = ::socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);

#else
	int sockfd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	unsigned long ul = 1;
	int ret = ioctlsocket(sockfd, FIONBIO, (unsigned long*)&ul);

	if (ret == SOCKET_ERROR)
	{

	}
#endif

	return sockfd;
}