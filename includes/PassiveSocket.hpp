#ifndef PassiveSocket_hpp
#define PassiveSocket_hpp

#include "Socket.hpp"

namespace WS
{
	class PassiveSocket : public Socket
	{
		private:
			int backlog;

		public:
			//CONSTRUCTEURS
			PassiveSocket(int domain, int service, int protocol, \
				int port, int bklg);

			//AUTRES
			void	bindSocket();
			void	listenSocket();
	};
}

#endif