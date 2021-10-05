#ifndef Webserver_hpp
#define Webserver_hpp


# include <iostream>
# include <fstream>
# include <istream>
# include <ostream>
# include <iterator>
# include "Route.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "Connexion.hpp"
# include "Webserv.hpp"
# include <map>

# include <sys/ioctl.h>
# include <sys/socket.h>
# include <ctime>
# include <netinet/in.h>
# include <cerrno>
# include <stdexcept>
# include <cstring>
# include <cstdlib>

namespace WS
{
	class Website;

	class Webserver
	{
		private:
			std::vector<Website>				m_websites;
			std::vector<Connexion>				m_connexions;
			bool 								m_buildIsSuccessful;
			std::map<int, std::string>			m_statusMsg;
		
		public:
			//CONSTRUCTEURS
			Webserver(const char *conf);
			~Webserver();

			bool		getBuildIsSuccessfull();
			void		parseConfigFile(std::ifstream *file);
			void		print(void);
			bool 		compareToken(std::string line, std::string identifier);
			std::string getString(std::string line);
			int			launch(void);
			int			maxListener();
			void		buildStatusMsg();
			std::map<int, std::string>	getStatusMsg();
	};
}

# include "Website.hpp"

#endif