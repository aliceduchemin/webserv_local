#include "../includes/Connexion.hpp"

WS::Connexion::Connexion(int new_socket, WS::Website *website) 
{
	this->m_socket = new_socket;
	this->m_website = website;
	this->m_state = 0;
	this->m_send = 0;
	this->m_hasFileFd = false;
	this->m_requestExists = false;
}

int WS::Connexion::findReadState()
{
	size_t	i;
	size_t	t;
	
	i = m_buff.find("\r\n\r\n");
	if (i != std::string::npos) //if the headers part is read (empty line before the message body)
	{
		if ((t = m_buff.find("Content-Length: ")) != std::string::npos) 
		{
			t = std::atoi(m_buff.substr(t + 16, 10).c_str()) + i + 4;
			if (m_buff.size() > t) // if we received more than expected, we ignore it
			{
				m_buff = m_buff.substr(0, t);
				return (1);
			}
			else if (m_buff.size() == t)
				return (1);
			else
				return (0);
		}
		else if (m_buff.find("Transfer-Encoding: chunked") != std::string::npos)
		{
			if (m_buff.substr(m_buff.size() - 5, 5) == "0\r\n\r\n") // if we found a zero-length chunk
				return (1);
			else
				return (0);
		}
		else
			return (1); //if no Length header is found
	}
	return (0); //if the header part is not read 
}

int WS::Connexion::reader()
{
	//read from the connection
	char tempBuffer[BUFFER_SIZE + 1];
	ssize_t bufferSize;
	
	bufferSize = read(this->m_socket, tempBuffer, BUFFER_SIZE);
	if (bufferSize == -1)
		return (1);
	else if (bufferSize >= 0)
	{
		tempBuffer[bufferSize] = '\0';
		m_buff += std::string(tempBuffer);
		m_state = this->findReadState();
		if (m_state != 0)
		{
			int headerlength = m_buff.find("\r\n\r\n") + 4;
			if (headerlength == 4)
				return (1);
			if ((static_cast<int>(m_buff.size()) - headerlength) > this->m_website->getBodySize())
				this->m_is_below_max_body = false;
			else
				this->m_is_below_max_body = true;
		}
	}	
	return (0);
}

int 	WS::Connexion::handler()
{
	//handles depending on the client's request (GET, POST or DELETE)
	if (this->m_state == 1 && this->m_requestExists == false)
	{
		this->m_req = new WS::Request(m_buff, this->m_website, this);
		this->m_requestExists = true;
		if (this->m_req->getStatus() == 405)
			this->m_state = 5;
		else if (this->m_is_below_max_body == false)
		{
			this->m_state = 5;
			this->m_req->setStatus(413);
		}
		else if (this->m_req->getIsAutoindex())
			this->m_state = 5;
		else
		{
			this->m_state = 2;
			if (this->m_req->getStatus() == 308)
				return 0;
			else if (this->m_req->getPostNonCgi())
			{
				this->m_fileFd = open(this->m_req->getFile().c_str(), O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
				if (this->m_fileFd == -1)
					return -1;	
			//	std::cout << "File created on " << this->m_fileFd << std::endl;
				this->m_hasFileFd = true;
			}
			else if (this->m_req->getMethod() == "GET" || this->m_req->getMethod() == "POST")
			{
				this->m_fileFd = open(this->m_req->getFile().c_str(), O_RDONLY);
				if (this->m_fileFd == -1)
				{
					if (errno == EACCES)
						this->getRequest()->setStatus(403);
					else if (errno == ENOENT)
						this->getRequest()->setStatus(404);
					this->m_state = 5;
				}
				else
					this->m_hasFileFd = true;
			}
			else if (this->m_req->getMethod() == "DELETE")
			{
				this->m_req->deleteReq();
				if (this->m_req->getStatus() == 404)
				{
					this->m_fileFd = open(this->m_req->getFile().c_str(), O_RDONLY);
					if (this->m_fileFd == -1)
						return -1;
					this->m_hasFileFd = true;
				}
			}
			else
				this->m_req->setStatus(501);
		}
	}
	return 0;
}

int WS::Connexion::fileReader()
{
	//handles depending on the client's request (GET, POST or DELETE)
	char tempBuffer[BUFFER_SIZE + 1];
	ssize_t bufferSize;
	
	bufferSize = read(this->m_fileFd, tempBuffer, BUFFER_SIZE);	
	if (bufferSize == -1)
		return (1);
	else if(bufferSize == 0)
	{
		if (this->m_req->getTypeReq() == "CGI")
		{
			this->m_fIn = tmpfile();
			this->m_fdIn = fileno(this->m_fIn);
			this->m_state = 3;
		}
		else
			this->m_state = 5;
	}
	tempBuffer[bufferSize] = '\0';
	this->m_hasFileFd = true;
	this->m_fileBuff += std::string(tempBuffer);
	return (0);
}

int	 WS::Connexion::cgiWriter()
{
	int ret;

	if (this->getRequest()->getRoute()->getCgiPath().find("cgi_tester") != std::string::npos)
		ret = write(this->m_fdIn, this->m_fileBuff.c_str(), this->m_fileBuff.size());
	else
		ret = write(this->m_fdIn, this->getRequest()->getBody().c_str(), this->getRequest()->getBody().size());
	if (ret < 0)
		return (1);
	lseek(this->m_fdIn, 0, SEEK_SET);
	this->m_fOut = tmpfile();
	this->m_fdOut = fileno(this->m_fOut);
	if (this->m_req->cgiProcess() != 0)
		return (1);
	this->m_state = 4;
	return (0);
}

int WS::Connexion::postNonCgiWriter()
{
	int ret;

	if (this->m_req->getStatus() != 404)
	{
		ret = write(this->m_fileFd, this->m_req->getBody().append("/0").c_str(), this->m_req->getBody().size());
		if (ret < 0)
			return (1);
	}
	this->m_state = 5;
	return (0);
}

int WS::Connexion::cgiReader()
{
	char	buffer[BUFFER_SIZE + 1];
	int		ret;

	if (this->m_result.empty())
		lseek(this->m_req->getConnexion()->getfdOut(), 0, SEEK_SET);

	if((ret = read(m_fdOut, buffer, BUFFER_SIZE)) != 0)
	{
		if (ret < 0)
		{
			fclose(this->m_fIn);
			fclose(this->m_fOut);
			close(this->m_fdIn);
			close(this->m_fdOut);
			return (1);
		}
        buffer[ret] = 0;
		this->m_result += buffer;
	}
	else
	{
		fclose(this->m_fIn);
		fclose(this->m_fOut);
		close(this->m_fdIn);
		close(this->m_fdOut);
		this->m_state = 5;
	}
	return (0);
}

void	WS::Connexion::destructRequest()
{
	delete this->m_req;
	this->m_requestExists = false;
}

int WS::Connexion::responder()
{
	int ret;

	if (this->m_send == 0 && this->m_req->getTypeReq() != "CGI")
		this->m_rep = new WS::Response(m_req, m_website, this->m_fileBuff);
	if (this->m_send == 0 && this->m_req->getTypeReq() == "CGI")
		this->m_rep = new WS::Response(m_req, m_website, this->m_result);
    ret = write(this->m_socket, this->m_rep->getResponse().c_str(), this->m_rep->getSize());
	close(this->m_socket);
	m_buff.clear();
	delete this->m_rep;
	if (ret < 0)
		return (1);
	return (1);
}

void	 WS::Connexion::setState(int state)
{
	m_state = state;
}

void	 WS::Connexion::setHasFileFd(bool value)
{
	m_hasFileFd = value;
}

int WS::Connexion::getSocket()
{	
	return m_socket;
}

int WS::Connexion::getFileFd()
{	
	return m_fileFd;
}

WS::Website *WS::Connexion::getWebsite()
{	
	return m_website;
}

WS::Request *WS::Connexion::getRequest()
{	
	return m_req;
}

int WS::Connexion::getState()
{	
	return m_state;
}

int WS::Connexion::getfdIn()
{	
	return m_fdIn;
}

FILE *WS::Connexion::getfIn()
{	
	return m_fIn;
}

int WS::Connexion::getfdOut()
{	
	return m_fdOut;
}

FILE *WS::Connexion::getfOut()
{	
	return m_fOut;
}

bool 	WS::Connexion::getHasFileFd()
{
	return m_hasFileFd;
}

bool 	WS::Connexion::getRequestExists()
{
	return m_requestExists;
}