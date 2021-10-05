#include "../includes/Response.hpp"

WS::Response::Response(Request *req, Website *website, std::string fileBuff)
{	
	Parser parse((char*)fileBuff.c_str(), fileBuff.size());
	std::string status;
	std::string msg;
	std::string header;
	
	this->m_req = req;
	this->m_website = website;
	
	status = static_cast<std::ostringstream*>( &(std::ostringstream() << this->m_req->getStatus()))->str();
	msg = this->m_website->getWebserver()->getStatusMsg()[this->m_req->getStatus()];
	
	fillBody(&parse, status, msg);
	
	header = this->m_req->getProtocolVersion() + " " + status + " " + msg + " \nContent-Type: text/html";
	this->m_response = header + "\n\n" + this->m_body;
	this->m_size = this->m_response.size();
}

//SETTEURS
void	WS::Response::fillBody(Parser *parse, std::string status, std::string msg)
{
	if (this->m_req->getIsAutoindex())
		this->m_body = this->m_req->getAutoindex();
	else if (this->m_req->getConnexion()->getHasFileFd() == true)
	{
		if (this->m_req->getTypeReq() ==  "CGI")
			this->m_body = parse->getBody();
		else
			this->m_body = parse->getContent();	
	}
	else if (this->m_req->getConnexion()->getHasFileFd() == false)
	{
		if (this->m_req->getStatus()  == 308)
		{
			std::string urlRedirect = this->m_req->getRoute()->getRedirection();
			this->m_body = "<head><meta http-equiv=\"Refresh\" content=\"0; URL=" + urlRedirect + "\"></head>";
		}
		else if (this->m_req->getStatus() == 204)
			this->m_body = "";	
		else
			this->m_body = status + " - " + msg;
	}
}

//ACCESSEURS
int	WS::Response::getSize()
{
	return m_size;
}

std::string	WS::Response::getResponse()
{
	return m_response;
}

std::string	WS::Response::getBody()
{
	return m_body;
}
