#include "../includes/Route.hpp"

WS::Route::Route() 
{
    m_auto_index = "off";
}

void WS::Route::print(void)
{
    std::vector<std::string>::iterator	it;

    std::cout << "    Location = " << this->m_condition << std::endl;
    std::cout << "    Root directory = " << this->m_root << std::endl;
    std::cout << "    Index file = " << this->m_index << std::endl;
    std::cout << "    Auto index is " << this->m_auto_index << std::endl;
    std::cout << "    Upload = " << this->m_upload << std::endl;
    std::cout << "    Cgi_path ? " << (this->m_cgi_path.empty() ? "NA" : this->m_cgi_path) << std::endl;
    std::cout << "    Cgi_extension ? " << (this->m_cgi_extension.empty() ? "NA" : this->m_cgi_extension) << std::endl;
    std::cout << "    Redirection HTTP ? " << (this->m_redirection.empty() ? "No" : this->m_redirection) << std::endl;
    for (it = this->m_methods.begin(); it != this->m_methods.end(); ++it)
	{
		std::cout << "    Method accepted : " << *it << std::endl;
	}
}

std::string	WS::Route::isValid()
{
    std::vector<std::string>::iterator	it;

    if (this->m_condition.empty())
        return "Invalid location";

    if (this->m_condition[0] != '/')
        return "Location must begin with /";

    if (this->m_root.empty())
        return "Root directory not found";
    
    if (this->m_methods.empty())
        return "HTTP accepted method must be specified";

    for (it = this->m_methods.begin(); it != this->m_methods.end(); ++it)
	{
        if (*it != "POST" && *it != "GET" && *it != "DELETE" )
            return (*it + " method not accepted (only POST, GET and DELETE are)");
        if (*it == "POST")
        {
            if (this->m_upload.empty() && this->m_cgi_extension.empty())
                return "Post method need an upload directory or a CGI extension";
            if (!(this->m_cgi_extension.empty())) // CGI Location
            {
                if (this->m_cgi_path.empty())
                    return "CGI path must be specified for this CGI Location";
            }
        }
	}

    

    if (this->m_redirection.empty()) // if CGI or GET Location => index or Autoindex needed
    {
        if (this->m_auto_index == "off")
        {
            if (this->m_index.empty())
                return "Without autoindex, an index page must be specified ";
        }
        if (this->m_auto_index.empty() && this->m_index.empty())
            return "Autoindex or an index page must be specified ";

        if (!(this->m_auto_index.empty()) && this->m_auto_index != "off" && this->m_auto_index != "on")
            return "Autoindex value not accepted (only on and off are)";
    }
    else
    {
        if (this->m_redirection.substr(0,7) != "http://" && this->m_redirection.substr(0,8) != "https://" )
            return  this->m_redirection + " : redirection value must begin with http:// or https:// ";
    }

    return "OK";
}

//SETTEURS
void WS::Route::setCondition(std::string str)
{
    this->m_condition = str.substr(0, str.find_first_of(" \r\t\f\v"));
}

void WS::Route::setCgiPath(std::string str)
{
    this->m_cgi_path = str;
}

void WS::Route::setCgiExtension(std::string str)
{
    this->m_cgi_extension = str;
}

void WS::Route::setMethods(std::string str)
{
    std::istringstream iss(str);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	this->m_methods = results;
}

void WS::Route::setRedirection(std::string str)
{
    this->m_redirection = str;
}

void WS::Route::setUpload(std::string str)
{
    this->m_upload = str;
}

void WS::Route::setRoot(std::string str)
{
    this->m_root = str;
}

void WS::Route::setAutoIndex(std::string str)
{
    this->m_auto_index = str;
}

void WS::Route::setIndex(std::string str)
{
 	this->m_index = str;
}

void WS::Route::init(void)
{
    this->m_condition.clear();
    this->m_cgi_path.clear();
    this->m_cgi_extension.clear();
    this->m_methods.clear();
    this->m_redirection.clear();
    this->m_root.clear();
    this->m_auto_index = "off";
    this->m_index.clear();
    this->m_upload.clear();
}

//ACCESSEURS
std::string WS::Route::getCgiPath()
{
    return this->m_cgi_path;
}

std::string WS::Route::getCorrectedCgiPath()
{
    std::string str;

    str = this->m_cgi_path;
    if (this->m_cgi_path[0] == '.')
		str = this->m_cgi_path.substr(1, this->m_cgi_path.size());
	if (str[0] == '/')
		str = str.substr(1, str.size());
    return (str);
}

std::string WS::Route::getCgiExtension()
{
    return this->m_cgi_extension;
}

std::string WS::Route::getRedirection()
{
    return this->m_redirection;
}

std::string WS::Route::getCondition()
{
    return this->m_condition;
}

std::string WS::Route::getExistingCondition()
{
    if (strncmp(this->m_condition.c_str(), "/", this->m_condition.size()) == 0)
        return "";
    return this->m_condition;
}

std::string WS::Route::getIndex()
{
    return this->m_index;
}

std::string WS::Route::getAutoIndex()
{
    return this->m_auto_index;
}


std::string WS::Route::getUpload()
{
    return this->m_upload;
}


std::vector<std::string>  *WS::Route::getMethods()
{
	return &(this->m_methods);
}

std::string WS::Route::getRootDir()
{
	return ("./" + m_root);
}