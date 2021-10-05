#include "../includes/Socket.hpp"

WS::Socket::Socket(int domain, int service, int protocol, int port)
{
	//define address structure
	memset(&address, 0, sizeof(address));
	address.sin_family = domain;
	memcpy(&address.sin_addr, &in6addr_any, sizeof(in6addr_any));
	address.sin_port = htons(port);

	//establish socket
	sock_fd = socket(domain, service, protocol);
	if (sock_fd < 0)
	{
		perror("Failed to create the socket");
		exit(EXIT_FAILURE);
	}
}

WS::Socket::~Socket()
{
	close(sock_fd);
}

//ACCESSEURS
struct sockaddr_in	WS::Socket::getAddress()
{
	return address;
}

int		WS::Socket::getSock_fd()
{
	return sock_fd;
}