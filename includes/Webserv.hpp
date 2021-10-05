#ifndef Webserv_hpp
#define Webserv_hpp

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <list>

#include "Socket.hpp"
#include "PassiveSocket.hpp"
#include "Parser.hpp"
#include "utils.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Website.hpp"
#include "Webserver.hpp"
#include "Route.hpp"

# ifdef __linux__
#  define _CONF_ "./default_ubuntu.conf"
# elif __APPLE__
#  define _CONF_ "./default.conf"
# endif

#endif