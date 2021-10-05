#ifndef Socket_hpp
#define Socket_hpp

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>

namespace WS
{
	class Socket
	{
		private:
			struct sockaddr_in address;
			int sock_fd;

		public:
			//CONSTRUCTEURS
			Socket(int domain, int service, \
				int protocol, int port);
			~Socket();

			//ACCESSEURS
			struct sockaddr_in getAddress();
			int getSock_fd();
	};
}

#endif