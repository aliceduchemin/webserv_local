#ifndef Parser_hpp
#define Parser_hpp

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <sys/stat.h>
#include <vector>

namespace WS
{
	class Parser
	{
		private:
			std::string 				str;
			std::vector<std::string>	tokens;

			//ACCESSEURS : découpage getTokenInStr()
			std::string getLine(int nLine);
			
		public:
			//CONSTRUCTEURS
			Parser(char *buffer, size_t bufferSize);
			Parser(std::string request);

			//AUTRES
			void parseStrIntoVector(std::string line, int numberOfLines);
			bool parseLineIntoVector(std::string line);
		
			//ACCESSEURS
			std::string 				getprotocolVersion();
			std::string 				getTokenFromVector(std::string identifier);
			std::string 				getElem(std::string line, char delimiter, int nToken);
			std::string 				getTokenFromStr(int line, int elem);
			std::vector<std::string> 	getVectorFromVector(std::string identifier);
			std::string					getBody();
			std::string 				getContent();
			std::vector<std::string>	getVector();
	};
}

#endif