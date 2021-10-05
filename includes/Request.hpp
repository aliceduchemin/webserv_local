#ifndef Request_hpp
#define Request_hpp

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <climits>
#include <iostream>

#include "Parser.hpp"
#include "Route.hpp"
#include "Website.hpp"

namespace WS
{
	class Connexion;
	
	class Request
	{
		private:
			WS::Website 	*m_website;
			WS::Connexion	*m_connexion;
			WS::Route		m_route;
			std::string		m_method;
			std::string		m_contentType;
			std::string		m_contentLength;
			std::string 	m_url;
			std::string 	m_query;
			std::string 	m_protocolVersion;
			std::string 	m_host;
			std::string 	m_port;
			std::string 	m_accept;
			std::string 	m_encoding;
			std::string 	m_connection;
			std::string 	m_upInsecure;
			std::string 	m_userAgent;
			std::string 	m_body;
			std::string 	m_url_location;
			std::string 	m_target;
			std::string 	m_file;
			std::string 	m_autoindex;
			bool			m_is_autoindex;
			bool			m_isPostNonCgi;
			int				m_status;
			char			m_here[PATH_MAX];

		public:
			//CONSTRUCTEURS
			Request(std::string request, WS::Website *website, Connexion *connexion);

			//PROCESS
			void		findRoute();
			std::string	findUrlLocation();
			void 		findFile();
			int 		checkMethod();
			void		createAutoIndex(std::string dir);
			int			cgiProcess();
			void		deleteReq();
			
			//SETTEURS
			void		setUrlAndQuery(Parser &parse);
			void		setPort(Parser &parse);
			char**		setEnv();
			char**		setArgv();
			void		setStatus(int code);
			void		setErrorPage();
			bool		isPostNonCgi();

			//ACCESSEURS
			std::string 	getUrl();
			std::string 	getQuery();
			std::string 	getContentType();
			std::string 	getContentLength();
			std::string 	getUrlLocation();
			std::string 	getMethod();
			std::string 	getProtocolVersion();
			std::string 	getHost();
			std::string 	getPort();
			std::string 	getAccept();
			std::string 	getEncoding();
			std::string 	getConnection();
			std::string 	getInsecure();
			std::string 	getUserAgent();
			std::string		 getBody();
			std::string 	getFile();
			WS::Route		*getRoute();
			std::string		getTypeReq();
			WS::Connexion	*getConnexion();
			int				getStatus();
			bool			getIsAutoindex();
			bool			getPostNonCgi();
			std::string		getAutoindex();
	};
}

#include "Connexion.hpp"

#endif