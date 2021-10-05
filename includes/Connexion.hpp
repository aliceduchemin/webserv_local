#ifndef Connexion_hpp
#define Connexion_hpp

# include <iostream>
# include <vector>
# include "Route.hpp"
# include <sstream>
# include <iterator>
# include "PassiveSocket.hpp"
# include "Parser.hpp"
# include "Request.hpp"
# include "Website.hpp"
# include <sys/socket.h>
# include <cstring>
# include <cerrno>

# define BUFFER_SIZE 10000

namespace WS
{
	class Response;

	class Connexion
	{
		private:
			int				m_send;
			int 			m_socket;
			WS::Request 	*m_req;
			WS::Website		*m_website;
			WS::Response 	*m_rep;
			int				m_state;
			std::string		m_buff;
			std::string		m_result;
			int				m_fileFd;
			bool 			m_hasFileFd;
			bool 			m_requestExists;
			std::string		m_fileBuff;
			FILE			*m_fIn;
			int				m_fdIn;
			FILE			*m_fOut;
			int				m_fdOut;
			bool			m_is_below_max_body;
			
		public:
			//CONSTRUCTEURS
			Connexion(int new_socket, WS::Website *website);

			void	accepter(PassiveSocket socket);
			int		reader();
			int		handler();
			int		responder();
			int		findReadState();
			int		fileReader();
			int		cgiWriter();
			int		cgiReader();
			int 	postNonCgiWriter();
			void	destructRequest();

			void 		setState(int state);
			void 		setHasFileFd(bool value);

			int 		getSocket();
			WS::Website *getWebsite();
			WS::Request *getRequest();
			int 		getState();
			int			getFileFd();
			int			getfdIn();
			FILE		*getfIn();
			int			getfdOut();
			FILE		*getfOut();
			bool 		getHasFileFd();
			bool 		getRequestExists();
	};
}

# include "Response.hpp"

#endif