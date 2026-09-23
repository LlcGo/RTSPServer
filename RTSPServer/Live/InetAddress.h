#ifndef LC_RTSPSERVER_INETADDRESS_H
#define LC_RTSPSERVER_INETADDRESS_H
#include <string>
#include <stdint.h>

#ifndef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif // !WIN32

class Ipv4Address
{
public:
	Ipv4Address();
	Ipv4Address(std::string ip,uint16_t port);
	void setAddr(std::string ip, uint16_t port);
	std::string getIp();
	uint16_t getPort();
	struct sockaddr* getAddr();

private:
	std::string mIp;
	uint16_t mPort;
	struct sockaddr_in mAddr;
};


#endif // !LC_RTSPSERVER_INETADDRESS_H

