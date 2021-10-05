#include "../includes/Request.hpp"

//CONSTRUCTEURS
WS::Request::Request(std::string request, Website *website, Connexion *connexion)
{
	this->m_website = website;
	this->m_connexion = connexion;
	this->m_status = 200;
	Parser parse(request);

	try
	{
		this->m_method = parse.getTokenFromStr(0, 0);
		setUrlAndQuery(parse);
		this->m_protocolVersion = parse.getprotocolVersion();
		this->m_host = parse.getElem(parse.getTokenFromVector("Host:"), ':', 0);
		setPort(parse);
		this->m_accept = parse.getTokenFromVector("Accept:");
		this->m_contentType = parse.getTokenFromVector("Content-Type:");
		if (m_contentType.compare("NA") == 0)
			m_contentType = "text/html";
		this->m_contentLength = parse.getTokenFromVector("Content-Length:");
		this->m_encoding = parse.getTokenFromVector("Accept-Encoding:");
		this->m_connection = parse.getTokenFromVector("Connection:");
		this->m_upInsecure = parse.getTokenFromVector("Upgrade-Insecure-Requests:");
		this->m_userAgent = parse.getTokenFromVector("User-Agent:");
		this->m_body = parse.getBody();
		this->m_is_autoindex = false;

		getcwd(this->m_here, PATH_MAX);
		m_url_location = findUrlLocation();
		this->findRoute();
		if (this->checkMethod() == 1)
			return ;
		this->m_target = this->m_route.getRootDir() + this->m_url.substr(this->m_route.getExistingCondition().size(), this->m_url.size());
		this->m_isPostNonCgi = isPostNonCgi();
		this->findFile();
	}
	catch(const std::exception& e)
	{
		std::cerr << "HTTP syntax not respected" << std::endl;
		this->m_status = 400;
		return ;
	}
}

std::string WS::Request::findUrlLocation()
{
	std::size_t p;

	p = this->m_url.substr(1, this->m_url.size()).find_first_of("/");
	if (p == std::string::npos)
		return ("/");
	return (this->m_url.substr(0, p + 1));
}

void	WS::Request::findRoute()
{
	size_t i;

	i = 0;
	while( i < this->m_website->getRoute().size())
	{
		if (m_url_location.size() == this->m_website->getRoute()[i].getCondition().size() && strncmp(this->m_website->getRoute()[i].getCondition().c_str(), m_url_location.c_str(), this->m_website->getRoute()[i].getCondition().size()) == 0)
		{
			this->m_route = this->m_website->getRoute()[i];
			break;
		}
		i++;
	}
	if (i == this->m_website->getRoute().size())
	{
		i = 0;
		while( i < this->m_website->getRoute().size())
		{
			if (strncmp(this->m_website->getRoute()[i].getCondition().c_str(), "/", this->m_website->getRoute()[i].getCondition().size()) == 0)
			{
				this->m_route = this->m_website->getRoute()[i];
				break;
			}
			i++;
		}
	}
}

void	WS::Request::setErrorPage()
{
	struct stat buf;

	this->m_file = this->m_website->getErrorPage();
	if (stat(this->m_file.c_str(), &buf) < 0) //if the target doesn't exist
		this->m_website->setErrorPage("");
	this->m_status = 404;
	this->m_method = "GET";
}

void	WS::Request::findFile()
{
	struct stat buf;

	if (stat(this->m_target.c_str(), &buf) < 0 && !(this->m_isPostNonCgi)) //if the target doesn't exist
		setErrorPage();
	else if (this->m_route.getRedirection().size() != 0)//if the target is a redirection
		this->m_status = 308;
	else if (S_ISDIR(buf.st_mode)) //if the target is a directory
	{
		if (this->m_route.getAutoIndex() == "on")
		{
			this->m_is_autoindex = true;
			this->createAutoIndex(this->m_target);
		}
		else
		{
			this->m_is_autoindex = false;
			this->m_target = this->m_route.getRootDir() + "/" + this->m_route.getIndex();
			if (stat(this->m_target.c_str(), &buf) < 0) //if the target doesn't exist
				setErrorPage();
			else
				this->m_file = this->m_target;
		}
	}
	else
	{
		if (this->m_isPostNonCgi)
		{
			std::string uploadDir = this->m_route.getRootDir() + "/" + this->m_route.getUpload();
			this->m_file = uploadDir + this->m_url.substr(this->m_route.getExistingCondition().size(), this->m_url.size());
			if (stat(uploadDir.c_str(), &buf) < 0)
			{
				std::cout << "Upload directory does not exist" << std::endl;
				setErrorPage();
			}
		}
		else
			this->m_file = this->m_target;
	}

	//std::cout << "File requested : " << (this->m_file.empty() ? "Auto index" : this->m_file) << std::endl;
}

int WS::Request::checkMethod()
{
    std::vector<std::string>::iterator	it;
	
	for (it = this->m_route.getMethods()->begin(); it != this->m_route.getMethods()->end(); ++it)
	{
		if (*it == this->m_method)
			return (0);
	}
//	std::cerr << "Method " << this->m_method << " unaccepted" << std::endl;
	this->m_status = 405;
	return (1);
}

void WS::Request::createAutoIndex(std::string dir)
{	
	struct dirent *entry;
	std::string dir_path;
	DIR* dir_pointer;
	if ((dir_pointer = opendir(dir.c_str())) == NULL)
	{
		perror("Could not open directory path.");
		exit(EXIT_FAILURE);
	}

	if (dir[dir.size()- 1] == '/')
		dir_path = this->m_route.getExistingCondition() + dir.substr(this->m_route.getRootDir().size(), dir.size());
	else
		dir_path = this->m_route.getExistingCondition() + dir.substr(this->m_route.getRootDir().size(), dir.size()) + "/";
	
	this->m_autoindex = "<html>";
	this->m_autoindex +=  "<head><title>Auto-Index</title></head>";
	this->m_autoindex += "<body bgcolor=\"white\">";
	this->m_autoindex += "	<h1>Index of " + dir_path + "</h1>";

	while ((entry = readdir(dir_pointer)) != NULL) 
   		this->m_autoindex += "<a href=\"" + dir_path + entry->d_name + "\">" + entry->d_name +" </a><br>";

	this->m_autoindex += "</pre><hr></body>";
	this->m_autoindex += "</html>";

	closedir(dir_pointer);
}

int		WS::Request::cgiProcess()
{
	pid_t		pid;
	int			status;
	int			stdIn = dup(STDIN_FILENO);
	int			stdOut = dup(STDOUT_FILENO);

	if ((pid = fork()) == -1)
		return -2;
	else if (pid == 0)
	{
		dup2(this->getConnexion()->getfdIn(), STDIN_FILENO);
		dup2(this->getConnexion()->getfdOut(), STDOUT_FILENO);

		char **argv = setArgv();
		char **env = setEnv();
		if (execve(argv[0], argv, env) == -1)
		{		
			deleteTab(env);
			delete argv[0];
			delete argv[1];
			delete[] argv;
			dup2(stdIn, STDIN_FILENO);
			dup2(stdOut, STDOUT_FILENO);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&status);
		if (status != 0)
			return -1;
	}

	dup2(stdIn, STDIN_FILENO);
	dup2(stdOut, STDOUT_FILENO);
	return 0;
}

void	WS::Request::deleteReq()
{
	char	here[PATH_MAX];
	getcwd(here, PATH_MAX);
	std::string absoluteUrlFile = here + this->getFile().substr(1, this->getFile().size());
	std::ifstream	fileToDelete(absoluteUrlFile.c_str());

	if (fileToDelete.good()) {
		if (std::remove(absoluteUrlFile.c_str()) == 0)
			this->m_status = 204;
		else
			this->m_status = 202;
	}
	else if (fileToDelete.rdstate() & std::ifstream::failbit)
		this->m_status = 403;
	else
		setErrorPage();
}

//SETTEURS
void	WS::Request::setUrlAndQuery(Parser &parse)
{
	size_t		del;
	std::string	tmp;

	del = parse.getTokenFromStr(0, 1).find('?');
	if (del == std::string::npos)
	{
		this->m_url = parse.getTokenFromStr(0, 1);
		this->m_query = "";
	}
	else
	{
		tmp = parse.getTokenFromStr(0, 1);
		this->m_url = tmp.substr(0, del);
		this->m_query = tmp.substr(del + 1, tmp.size());
	}
}

void	WS::Request::setPort(Parser &parse)
{
	size_t		del;
	std::string	tmp;

	del = tmp.find(':');
	if (del == std::string::npos)
		this->m_port = "80";
	else
	{
		tmp = parse.getTokenFromVector("Host:");
		this->m_port = tmp.substr(del + 1, tmp.size());
	}
}

char**	WS::Request::setEnv()
{
	char **env;
	env = new char*[23];
	char	here[PATH_MAX];
	getcwd(here, PATH_MAX);
	std::string absoluteUrl = here + this->m_file.substr(1, this->m_file.size());
	
	env[0] = joinChar("CONTENT_TYPE=", m_contentType);
	env[1] = joinChar("GATEWAY_INTERFACE=", "CGI/1.1");
	env[2] = joinChar("HTTP_ACCEPT=", m_accept);
	env[3] = joinChar("HTTP_ACCEPT_ENCODING=", m_encoding);
	env[4] = joinChar("HTTP_CONNECTION=", m_connection);
	env[5] = joinChar("HTTP_DNT=", "1");
	std::string to_join = this->m_website->getHost() + ":" + static_cast<std::ostringstream*>( &(std::ostringstream() << this->m_website->getPort()))->str();
	env[6] = joinChar("HTTP_HOST=", to_join);
	env[7] = joinChar("HTTP_UPGRADE_INSECURE_REQUESTS=", m_upInsecure);
	env[8] = joinChar("PATH_INFO=", absoluteUrl);
	env[9] = joinChar("PATH_TRANSLATED=", absoluteUrl);
	env[10] = joinChar("HTTP_USER_AGENT=", m_userAgent);
	env[11] = joinChar("QUERY_STRING=", m_query);
	env[12] = joinChar("REDIRECT_STATUS=", "200");
	env[13] = joinChar("REMOTE_ADDR=", this->m_website->getHost());
	env[14] = joinChar("REQUEST_METHOD=", m_method);
	if (m_query.empty())
		env[15] = joinChar("REQUEST_URI=", absoluteUrl);
	else
		env[15] = joinChar("REQUEST_URI=", absoluteUrl + "?" + m_query);
	env[16] = joinChar("SCRIPT_NAME=", absoluteUrl);
	env[17] = joinChar("SERVER_NAME=", this->m_website->getHost());
	env[18] = joinChar("SERVER_PORT=", static_cast<std::ostringstream*>( &(std::ostringstream() << m_website->getPort()))->str());
	env[19] = joinChar("SERVER_PROTOCOL=", "HTTP/1.1");
	env[20] = joinChar("SERVER_SOFTWARE=", "webserv");
	env[21] = 0;
	if (m_method == "POST")
		env[21] = joinChar("CONTENT_LENGTH=", m_contentLength);
	env[22] = 0;
	return env;
}

char**	WS::Request::setArgv()
{
	char **argv;
	argv = new char*[3];

	argv[0] = new char[this->getRoute()->getCorrectedCgiPath().size() + 1];
	argv[0] = strcpy(argv[0], this->getRoute()->getCorrectedCgiPath().c_str());

	argv[1] = new char[this->getUrl().size() + 1];
	argv[1] = strcpy(argv[1], this->getUrl().c_str());

	argv[2] = 0;

	return argv;
}

void	WS::Request::setStatus(int code)
{
	this->m_status = code;
}

//ACCESSEURS
std::string WS::Request::getUrl()
{
	return m_url;
}

std::string WS::Request::getQuery()
{
	return m_query;
}

std::string WS::Request::getContentType()
{
	return m_contentType;
}

std::string WS::Request::getContentLength()
{
	return m_contentLength;
}

std::string WS::Request::getUrlLocation()
{
	return m_url_location;
}

std::string WS::Request::getMethod()
{
	return m_method;
}

std::string WS::Request::getBody()
{
	return m_body;
}

std::string WS::Request::getProtocolVersion()
{
	return m_protocolVersion;
}

std::string WS::Request::getHost()
{
	return m_host;
}

std::string WS::Request::getPort()
{
	return m_port;
}

std::string WS::Request::getAccept()
{
	return m_accept;
}

std::string WS::Request::getEncoding()
{
	return m_encoding;
}

std::string WS::Request::getConnection()
{
	return m_connection;
}

std::string WS::Request::getInsecure()
{
	return m_upInsecure;
}

std::string WS::Request::getUserAgent()
{
	return m_userAgent;
}

std::string WS::Request::getFile()
{
	return m_file;
}

WS::Route	*WS::Request::getRoute()
{
	return &m_route;
}

bool	WS::Request::isPostNonCgi()
{
	if (this->m_method == "POST" && this->m_route.getCgiExtension().empty())
		return (true);
	return (false);
}

std::string	WS::Request::getTypeReq()
{
	std::vector<std::string>::iterator	it;

	if (this->m_status == 404)
		return (std::string("GET"));
	if(this->m_url.size() >= this->m_route.getCgiExtension().size() && this->m_route.getCgiExtension().size() > 0 && this->m_route.getCgiExtension() == this->m_url.substr(this->m_url.size() - this->m_route.getCgiExtension().size(), this->m_url.size()))
		return (std::string("CGI"));
	return (std::string("GET"));
}

WS::Connexion	*WS::Request::getConnexion()
{
	return m_connexion;
}

int			WS::Request::getStatus()
{
	return m_status;
}

bool		WS::Request::getIsAutoindex()
{
	return m_is_autoindex;
}

std::string	WS::Request::getAutoindex()
{
	return m_autoindex;
}

bool		WS::Request::getPostNonCgi()
{
	return m_isPostNonCgi;
}
