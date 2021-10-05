#ifndef Response_hpp
#define Response_hpp

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <climits>
#include <dirent.h>

#include "Parser.hpp"
#include "utils.hpp"
#include "Route.hpp"
#include "Website.hpp"

namespace WS
{	
	class Connexion;

	class Request;

	class Response
	{
		private:
			std::string version;
			std::string	file;
			std::string	content;
			size_t 		m_size;
			std::string m_body;
			std::string m_response;
			std::string m_result;

			WS::Request	*m_req;
			WS::Website *m_website;

		public:
			//CONSTRUCTEURS
			Response(Request *req, Website *website, std::string fileBuff);

			//SETTEURS
			void	fillBody(Parser *parse, std::string status, std::string msg);

			//ACCESSEURS
			std::string getResponse();
			std::string getContent();
			int 		getSize();
			std::string	getUrl();
			std::string	getVersion();
			std::string	getBody();
	};
}

#include "Connexion.hpp"
#include "Request.hpp"

#endif