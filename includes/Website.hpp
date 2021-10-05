#ifndef Website_hpp
#define Website_hpp

# include <iostream>
# include <vector>
# include "Route.hpp"
# include <sstream>
# include <iterator>
#include "PassiveSocket.hpp"
#include "Parser.hpp"

namespace WS
{
	class Webserver;

	class Website
	{
		private:
			std::string 				m_error_page;
			std::string 				m_client_max_body_size;
			std::vector<std::string>	m_server_name;
			std::string					m_host_port;
			std::vector<Route>			m_routes;
			PassiveSocket 				*m_socket;
			int 						new_socket;
			bool						hasASocket;
			Webserver					*m_webserver;
			
		public:
			//CONSTRUCTEURS
			Website();
			~Website();
			
			//SETTEURS
			void		setServerName(std::string str);
			void 		setErrorPage(std::string str);
			void 		setClient_max_body_size(std::string str);
			void		addHostPort(std::string str);
			void		addRoute(Route route);
			void		pointToWebserver(Webserver *webserver);
			
			//AUTRES
			void		init(void);
			void		print(void);
			void 		create_socket();
			std::string	isValid();
			bool		checkHostFormat();
			bool		checkPortFormat();
			bool		checkBodySizeFormat();
			
			//ACCESSEURS
			std::vector<Route>	getRoute();
			PassiveSocket 		*getSocket();
			int 				getNewFd();
			std::string			getHost();
			int 				getPort();
			std::string 		getErrorPage();
			int					getBodySize();
			bool				getHasASocket();
			Webserver*			getWebserver();
	};
}

# include "Webserver.hpp"

#endif