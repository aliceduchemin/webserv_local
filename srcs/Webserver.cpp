#include "../includes/Webserver.hpp"

#define TRUE             1
#define FALSE            0

WS::Webserver::Webserver(const char *conf)
{
    std::ifstream   file(conf);

	try
	{
		if (file.is_open() && file.good())
    	{	
			parseConfigFile(&file);
			file.close();
			print();
		}
  		else
   	    	throw std::logic_error("Configuration file not found");
	}
	catch(const std::exception& e)
	{
		m_buildIsSuccessful = false;
		std::cerr << "Config Error :" << std::endl;
		std::cerr << e.what()  << std::endl;
		return ;
	}
	m_buildIsSuccessful = true;
	buildStatusMsg();
}

void		WS::Webserver::buildStatusMsg()
{
	this->m_statusMsg[200] = "OK";
	this->m_statusMsg[202] = "Accepted";
	this->m_statusMsg[204] = "No Content";
	this->m_statusMsg[308] = "Permanent Redirect";
	this->m_statusMsg[400] = "Bad Request";
	this->m_statusMsg[403] = "Forbidden";
	this->m_statusMsg[404] = "Not Found";
	this->m_statusMsg[405] = "Method Not Allowed";
	this->m_statusMsg[413] = "Request Entity Too Large";
	this->m_statusMsg[500] = "Internal Server Error";
	this->m_statusMsg[501] = "Not Implemented";
}

std::map<int, std::string>	WS::Webserver::getStatusMsg()
{
	return this->m_statusMsg;
}

bool		WS::Webserver::getBuildIsSuccessfull()
{
	return this->m_buildIsSuccessful;
}

void		WS::Webserver::parseConfigFile(std::ifstream *file)
{
    std::string     line;
	Website         server_tmp;
    Route           route_tmp;
	bool            server_creation(false);
    bool            location_creation(false);
	int				i = 1;

	while (getline(*file, line))
	{
		if (compareToken(line, "server"))
			server_creation = true;
		if (compareToken(line, "server_name") && server_creation)
			server_tmp.setServerName(getString(line));
		if (compareToken(line, "error_page") && server_creation)
			server_tmp.setErrorPage(getString(line));
		if (compareToken(line, "client_max_body_size") && server_creation)
			server_tmp.setClient_max_body_size(getString(line));
		if (compareToken(line, "listen") && server_creation)
			server_tmp.addHostPort(getString(line));
		if (compareToken(line, "location") && server_creation)
		{
			location_creation = true;
			route_tmp.setCondition(getString(line));
		}
		if (compareToken(line, "cgi_path") && location_creation)
			route_tmp.setCgiPath(getString(line));
		if (compareToken(line, "cgi_extension") && location_creation)
			route_tmp.setCgiExtension(getString(line));
		if (compareToken(line, "method") && location_creation)
			route_tmp.setMethods(getString(line));
		if (compareToken(line, "redirect") && location_creation)
			route_tmp.setRedirection(getString(line));
		if (compareToken(line, "root") && location_creation)
			route_tmp.setRoot(getString(line));
		if (compareToken(line, "auto_index") && location_creation)
			route_tmp.setAutoIndex(getString(line));
		if (compareToken(line, "index") && location_creation)
			route_tmp.setIndex(getString(line));
        if (compareToken(line, "upload") && location_creation)
            route_tmp.setUpload(getString(line));
		if (compareToken(line, "upload") && location_creation)
			route_tmp.setUpload(getString(line));
		if (compareToken(line, "}"))
		{
			if (server_creation && !location_creation)
			{
				if(server_tmp.isValid() != "OK")
				{
					file->close();
					throw std::logic_error("Invalid Server on line " + static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str() + " : " + server_tmp.isValid());
				}
				server_tmp.pointToWebserver(this);
				m_websites.push_back(server_tmp);
				server_creation = false;
				server_tmp.init();
			}
			else if (location_creation)
			{
				if(route_tmp.isValid() != "OK")
				{
					file->close();
					throw std::logic_error("Invalid Location on line " + static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str() + " : " + route_tmp.isValid());
				}
				server_tmp.addRoute(route_tmp);
				location_creation = false;
				route_tmp.init();
			}
		}
		i++;
	}
	if (server_creation || location_creation)
		throw std::logic_error("Configuration not ended correctly"); 
}

WS::Webserver::~Webserver()
{
	std::cout << "--- Shutting Webserv off ---" << std::endl;
	size_t j = 0;
	if (m_websites.empty() == false)
	{
		while (j < m_websites.size())
		{
			if (m_websites.size() > 0)
				m_websites.pop_back();
			j++;
		}
	}
	if (m_connexions.empty() == false)
	{
		while (m_connexions.size() > 0)
		{
		//	std::cout << "Closing connection on fd " <<  m_connexions[m_connexions.size() - 1].getSocket() << std::endl;
			if (m_connexions[m_connexions.size() - 1].getRequestExists() == true)
				m_connexions[m_connexions.size() - 1].destructRequest();
			m_connexions.pop_back();
		}
	}
}

bool WS::Webserver::compareToken(std::string line, std::string identifier)
{
    std::size_t debut;
    std::size_t fin;

    debut = line.find_first_not_of(" \r\t\f\v");
    if (debut == std::string::npos)
        return (false);
    fin = line.substr(debut, line.size()).find_first_of(" \r\t\f\v");
    return(line.substr(debut, fin) == identifier);
}

std::string WS::Webserver::getString(std::string line)
{
    std::size_t debut;
    std::string line_tmp;
    std::size_t fin;

    debut = line.find_first_not_of(" \r\t\f\v");
    line_tmp = line.substr(debut, line.size());
    debut = line_tmp.find_first_of(" \r\t\f\v");
    line_tmp = line_tmp.substr(debut, line_tmp.size());
    debut = line_tmp.find_first_not_of(" \r\t\f\v");
    line_tmp = line_tmp.substr(debut , line_tmp.size());
    fin = line_tmp.find_last_of(";");
    return(line_tmp.substr(0, fin));
}

void	WS::Webserver::print(void)
{
	size_t i;

    i = 0;
	while (i < m_websites.size())
	{
        std::cout << std::endl << "--- Creating server n°" << i << " ---" << std::endl;
		m_websites[i].print();
        i++;
	}
}

int	    WS::Webserver::maxListener()
{
    size_t j;
    int ret;

    j = 0;
    ret = m_websites[j].getSocket()->getSock_fd();
    while(j < m_websites.size())
    {
        if (ret < m_websites[j].getSocket()->getSock_fd())
            ret = m_websites[j].getSocket()->getSock_fd();
        j++;
    }
    return (ret);
}

int		WS::Webserver::launch(void)
{
    int                 new_fd;
    int                 end_server;
    int                 nb_ready;
    int                 max;
	size_t				j;
    fd_set		        fds;
    fd_set		        fds_to_read;
    fd_set		        fds_to_write;

    std::cout << "\n\n--- Launching the server ---" << std::endl;
   
    FD_ZERO(&fds);
    j = 0;
    while(j < m_websites.size())
    {
        m_websites[j].create_socket();
        FD_SET(m_websites[j].getSocket()->getSock_fd(), &fds);
        j++;
    }
    
    max = maxListener();
    end_server = FALSE;
    while (end_server == FALSE)
    {
        fds_to_read = fds;
        fds_to_write = fds;
        nb_ready = select(max + 1, &fds_to_read, &fds_to_write, NULL, NULL); //fill fds_to_read et fds_to_write with the fd ready to be read or written on
        if (nb_ready < 0)
        {
            perror("Select() failed");
            break;
        }

        std::vector<WS::Connexion>::iterator	itw;
        for(itw = m_connexions.begin(); itw != m_connexions.end(); itw++) // see if there is anything is to write on any connection
        {
			if (itw->getState() == 5 && FD_ISSET(itw->getSocket(), &fds_to_write))
            {
                if (!(itw->getRequest()->getIsAutoindex()) && itw->getHasFileFd() == true)
                {
				//	std::cout << "Closing connection to file on fd " << itw->getFileFd() << std::endl;
                    close(itw->getFileFd());
                    FD_CLR(itw->getFileFd(), &fds);
                }
                if (itw->responder()) 
                {   
				//	std::cout << "Closing connection on fd " << itw->getSocket() << std::endl << std::endl;
                    FD_CLR(itw->getSocket(), &fds);
					itw->destructRequest();
					m_connexions.erase(itw);
                    break;
                }
            }
        } 

        std::vector<WS::Connexion>::iterator	itcgi;
        for(itcgi = m_connexions.begin(); itcgi != m_connexions.end(); itcgi++) // see if there is anything to read on tmp file for cgi
        {
            if (itcgi->getState() == 4 && FD_ISSET(itcgi->getfdOut(), &fds_to_read))
            {
                if  (itcgi->cgiReader())
                {
                    FD_CLR(itcgi->getfdOut(), &fds);
                    close(itcgi->getfdOut());
                    m_connexions.erase(itcgi);
                    break;
                }
				//std::cout << "CGI : closing connection on fd " <<itcgi->getfdOut() << " (fd out)" << std::endl;
                if (itcgi->getState() == 5 && FD_ISSET(itcgi->getfdOut(), &fds_to_read))
                    FD_CLR(itcgi->getfdOut(), &fds);
            }
        }

        std::vector<WS::Connexion>::iterator	itcg;
        for(itcg = m_connexions.begin(); itcg != m_connexions.end(); itcg++) // see if there is anything to write on tmp file for cgi
        {
            if (itcg->getState() == 3 && FD_ISSET(itcg->getfdIn(), &fds_to_write))
            {
                if (itcg->cgiWriter() == 1)
				{
                    //std::cout << "CGI : execution failed, closing connection on fd " <<itcg->getfdIn() << " (fd in)" << std::endl;
                    FD_CLR(itcg->getfdIn(), &fds);
                    itcg->getRequest()->setStatus(500);
					itcg->setHasFileFd(false);
                    itcg->setState(5);
				}
                else
                {
                    //std::cout << "CGI : execution complete, closing connection on fd " <<itcg->getfdIn() << " (fd in)";
                    FD_CLR(itcg->getfdIn(), &fds);
                    //std::cout << ", and creating connection on fd " <<itcg->getfdOut() << " (fd out)" << std::endl;
                    FD_SET(itcg->getfdOut(), &fds);
                    if (itcg->getfdOut() > max)
                        max = itcg->getfdOut();
                }
            }
        }

        std::vector<WS::Connexion>::iterator	itr;
        for(itr = m_connexions.begin(); itr != m_connexions.end(); itr++) // see if there is any file to read or write in any connection
        {
            if (itr->getState() == 2 && !(itr->getRequest()->getPostNonCgi()) && FD_ISSET(itr->getFileFd(), &fds_to_read))
            {
				//std::cout << "Reading file on fd " << itr->getFileFd() << "..." << std::endl;
                if  (itr->fileReader())
                {
                    FD_CLR(itr->getFileFd(), &fds);
                    close(itr->getFileFd());
                    m_connexions.erase(itr);
                    break;
                }
                if (itr->getRequest()->getTypeReq() == "CGI" && itr->getState() == 3)
                {
					//std::cout << "CGI : create connection on fd " << itr->getfdIn() << " for execution (fd in)" << std::endl;
                    FD_SET(itr->getfdIn(), &fds);
                    if (itr->getfdIn() > max)
                        max = itr->getfdIn();
                }
            }
            if (itr->getState() == 2 && itr->getRequest()->getPostNonCgi() && FD_ISSET(itr->getFileFd(), &fds_to_write))
            {
                if  (itr->postNonCgiWriter())
                {   
                    FD_CLR(itr->getFileFd(), &fds);
                    close(itr->getFileFd());
                    m_connexions.erase(itr);
                    break;
                }
            }
        }

        std::vector<WS::Connexion>::iterator	itc;
        for(itc = m_connexions.begin(); itc != m_connexions.end(); itc++) // see if there is anything to read in any connection
        {
            if (FD_ISSET(itc->getSocket(), &fds_to_read))
            {
                if  (itc->reader())
                {
                    FD_CLR(itc->getSocket(), &fds);
                    close(itc->getSocket());
                    m_connexions.erase(itc);
                    break;
                }
                else if (itc->handler() == -1)
				{
					itc->destructRequest();
					return -1;
				}	
                else if (itc->getState() == 2 && (itc->getRequest()->getStatus() == 200 || itc->getRequest()->getStatus() == 404))
                {
                    FD_SET(itc->getFileFd(), &fds);
					//std::cout << "Create connection to file to read or upload on fd " << itc->getFileFd() << std::endl;
                    if (itc->getFileFd() > max)
                        max = itc->getFileFd();
                }
				else if (itc->getState() == 2)
					itc->setState(5);
            }
        }
        
        std::vector<WS::Website>::iterator	it;
        for(it = m_websites.begin(); it != m_websites.end(); it++) // see if there is anything is to read in any passive socket (listener)
        {
            if (FD_ISSET(it->getSocket()->getSock_fd(), &fds_to_read))
            {
                new_fd = 0;
                while (new_fd != -1)
                {
                    new_fd = accept(it->getSocket()->getSock_fd(), NULL, NULL); // accept the connection : create an active socket
                    if (new_fd < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            perror("accept() failed");
                            end_server = TRUE;
                        }
                        break;
                    }
					else
						std::cout << "New connection created on fd " << new_fd << std::endl;

                    FD_SET(new_fd, &fds);
                    m_connexions.push_back(Connexion(new_fd, &(*it)));
                    if (new_fd > max)
                        max = new_fd;
                }
            }
        }
    }
	return 0;
}