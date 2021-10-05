#include "../includes/PassiveSocket.hpp"

WS::PassiveSocket::PassiveSocket(int domain, int service, int protocol, int port, int bklg) : \
Socket(domain, service, protocol, port)
{
	backlog = bklg;
	bindSocket();
	listenSocket();
}

//AUTRES
void	WS::PassiveSocket::bindSocket()
{
	//affectation d'une adresse à la socket
	struct sockaddr_in address = getAddress();
	int ret;
	int on = 1;

	ret = setsockopt(getSock_fd(), SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	if (ret < 0)
	{
		perror("Failed to setsockopt()");
		close(getSock_fd());
		exit(EXIT_FAILURE);
	}

	ret = fcntl(getSock_fd(), F_SETFL, O_NONBLOCK);
	if (ret < 0)
	{
		perror("Failed to sfcntl()");
		close(getSock_fd());
		exit(EXIT_FAILURE);
	}

	ret = bind(getSock_fd(), (struct sockaddr *)&address, sizeof(address));
	if (ret < 0)
	{
		perror("Failed to bind the socket to port");
		close(getSock_fd());
		exit(EXIT_FAILURE);
	}
}

void	WS::PassiveSocket::listenSocket()
{
	//attente de connexion
	int ret;

	ret = listen(getSock_fd(), backlog);
	if (ret < 0)
	{
		perror("Failed to listen on the socket");
		close(getSock_fd());
		exit(EXIT_FAILURE);
	}
}