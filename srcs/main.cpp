#include "../includes/Webserv.hpp"

WS::Webserver *g_webserver;

void	ft_sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		delete g_webserver;
		exit(1);
	}
}

int main(int ac, char** av)
{
	const char *conf;

	if (ac < 3)
	{
		conf = av[1] ? av[1] : _CONF_;
		g_webserver = new WS::Webserver(conf);
		if (g_webserver->getBuildIsSuccessfull() == false)
		{
			delete g_webserver;
			exit(1);
		}	
		signal(SIGINT, ft_sig_handler);
		if (g_webserver->launch() == -1)
			delete g_webserver;
	}
	else
		std::cerr << "Wrong Argument number" << std::endl;

	return 1;
}