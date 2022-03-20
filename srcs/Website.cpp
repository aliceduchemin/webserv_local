#include "../includes/Website.hpp"
#include "../includes/Response.hpp"

WS::Website::Website() 
{
	this->hasASocket = false;
}


WS::Website::~Website()
{
	if (this->hasASocket == true)
		delete m_socket;
}

//SETTEURS
void WS::Website::setServerName(std::string str)
{
    std::istringstream iss(str);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	m_server_name = results;
}

void WS::Website::setErrorPage(std::string str)
{
    m_error_page = str;
}

void WS::Website::setClient_max_body_size(std::string str)
{
    m_client_max_body_size = str;
}

void WS::Website::addHostPort(std::string str)
{
    m_host_port = str;
}

void WS::Website::addRoute(Route route)
{
    m_routes.push_back(route);
}

void	WS::Website::pointToWebserver(Webserver *webserver)
{
	m_webserver = webserver;
}

//AUTRES
void WS::Website::init(void)
{
    m_server_name.clear();
    m_host_port.clear();
    m_error_page.clear();
    m_client_max_body_size.clear();
    m_routes.clear();
}

void WS::Website::create_socket()
{
	m_socket = new WS::PassiveSocket(AF_INET, SOCK_STREAM, 0, this->getPort(), 10);
	this->hasASocket = true;
}

void	WS::Website::print(void)
{
	std::vector<std::string>::iterator	it;
    size_t i;

	std::cout << "  Host and Port = " << m_host_port << std::endl;
    std::cout << "  Error page = " << m_error_page << std::endl;
    std::cout << "  Client max body size = " << m_client_max_body_size << std::endl;
    i = 0;
	while (i < m_routes.size())
	{
        std::cout << std::endl << "  Server's route n°" << i << " :"<< std::endl;
		m_routes[i].print();
        i++;
	}
}

std::string	WS::Website::isValid()
{
    if (this->m_host_port.empty()) 
	{
        return "Host and port not found";
	}
	
	if (!(this->checkHostFormat()))
	{
        return "Host invalid format";
	}

	if (!(this->checkPortFormat()))
	{
        return "Host invalid format";
	}

	if (this->m_error_page.empty())
	{
        return "Default error page not found";
	}

    if (this->m_client_max_body_size.empty())
	{
        return "Body size limit not found";
	}

	if (!(this->checkBodySizeFormat()))
	{
        return "Max Body Size invalid format";
	}

	if (this->m_routes.empty())
	{
        return "Location not found";
	}

    return "OK";
}

bool WS::Website::checkHostFormat()
{
	if (this->m_host_port.substr(0,10) != "127.0.0.1:")
		return false;
	return true;
}

bool WS::Website::checkPortFormat()
{
	int port;
	std::istringstream(this->m_host_port.substr(10,14)) >> port;

	if (port == 0 or port > 65535)
		return false;
	return true;
}

bool WS::Website::checkBodySizeFormat()
{
	int size;
	std::istringstream(this->m_client_max_body_size) >> size;

	if (size == 0 || size == 2147483647)
		return false;
	return true;
}

//ACCESSEURS
std::vector<WS::Route> WS::Website::getRoute()
{
	return m_routes;
}

WS::PassiveSocket *WS::Website::getSocket()
{
	return m_socket;
}

int WS::Website::getNewFd()
{
	return new_socket;
}

std::string WS::Website::getErrorPage()
{
	return m_error_page;
}

std::string WS::Website::getHost()
{
	return m_host_port.substr(0,9);
}

int WS::Website::getPort()
{
	int port;
	std::istringstream(m_host_port.substr(10,14)) >> port;

	return port;
}

int	WS::Website::getBodySize()
{
	int bodySize;
	std::istringstream(this->m_client_max_body_size) >> bodySize;
	return bodySize;
}

bool WS::Website::getHasASocket()
{
	return hasASocket;
}

WS::Webserver*	WS::Website::getWebserver()
{
	return m_webserver;
}