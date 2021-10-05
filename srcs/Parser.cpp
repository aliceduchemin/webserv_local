#include "../includes/Parser.hpp"

//CONSTRUCTEURS
WS::Parser::Parser(char *buffer, size_t bufferSize)
{
	int numberOfLines = 0;
	std::string line;

	for (size_t i = 0; i < bufferSize; i++)
	{
		if (buffer[i] == '\n')
			numberOfLines++;
		line += buffer[i];
	}
	str = line;
	parseStrIntoVector(line, numberOfLines);
}

WS::Parser::Parser(std::string request)
{
	str = request;
	int numberOfLines = 0;
	int i = 0;
	while (request[i] != 0)
	{
		if (request[i] == '\n')
			numberOfLines++;
		i++;
	}
	parseStrIntoVector(request, numberOfLines);
}

std::string WS::Parser::getprotocolVersion()
{
	return getLine(0).substr(getLine(0).find_last_of("HTTP") - 3, getLine(0).size() - getLine(0).find_last_of("HTTP") + 2);
}

//AUTRES
void WS::Parser::parseStrIntoVector(std::string line, int numberOfLines)
{
	std::string newLine;
	size_t pos = 0;
	
	while (numberOfLines > 0 && (pos = line.find("\n")) != std::string::npos)
	{
		newLine = line.substr(0, pos);
		line.erase(0, pos + 1);
		parseLineIntoVector(newLine);
		numberOfLines--;
	}
	newLine = line.substr(0, pos + 1);
	parseLineIntoVector(newLine);
}

bool	WS::Parser::parseLineIntoVector(std::string line)
{
	if (line[0] == '#' || line.empty())
		return false;

	size_t startPos = line.find_first_not_of(" \n\r\t\f\v");
	size_t delimiterPos = line.find_last_not_of(" \n\r\t\f\v");
	if (delimiterPos == std::string::npos || startPos == std::string::npos)
		return false;
	std::string token = line.substr(startPos, delimiterPos + 1);
	tokens.push_back(token);
	return true;
}

//PRIVATE : ACCESSEURS
std::string WS::Parser::getLine(int nLine)
{
	size_t pos = 0;
	std::string line;
	int i = 0;

	while ((pos = str.find("\n")) != std::string::npos)
	{
		line = str.substr(0, pos);
		if (i == nLine)
			return line;
		str.erase(0, pos + 1);
		i++;
	}
	line = str.substr(0, pos);
	return line;
}

//PUBLIC : ACCESSEURS
std::string WS::Parser::getElem(std::string line, char delimiter, int nToken)
{
	size_t pos = 0;
	std::string token;
	int i = 0;

	while ((pos = line.find(delimiter)) != std::string::npos)
	{
		token = line.substr(0, pos);
		if (i == nToken)
			return token;
		line.erase(0, pos + 1);
		i++;
	}
	token = line.substr(0, std::string::npos);
	return token;
}

std::string WS::Parser::getBody()
{
	std::string body;
	size_t startPos = str.find("\r\n\r\n") + 4;
	size_t endPos = str.size();
	body = str.substr(startPos, endPos);
	return body;
}

std::string WS::Parser::getTokenFromStr(int nLine, int nToken)
{
	return getElem(getLine(nLine), ' ', nToken);
}

std::string WS::Parser::getTokenFromVector(std::string identifier)
{
	std::vector<std::string>::iterator it;
	std::string token = "NA";

	for (it = tokens.begin(); it != tokens.end(); ++it)
	{
		size_t delimiterPos = it->find_first_of(" \r\t\f\v");
		std::string iden = it->substr(0, delimiterPos);
		if (iden.compare(identifier) == 0 && delimiterPos < it->size())
		{
			token = it->substr(delimiterPos, it->size());
			size_t newStart = token.find_first_not_of(" \r\t\f\v");
			token.erase(0, newStart);
			return token;
		}
	}
	return token;
}

std::vector<std::string> WS::Parser::getVectorFromVector(std::string identifier)
{
	std::vector<std::string>::iterator it;
	std::vector<std::string> vec;
	std::string token;
	int i;

	i = 0;
	for (it = tokens.begin(); it != tokens.end(); ++it)
	{
		size_t delimiterPos = it->find_first_of(" \r\t\f\v");
		std::string iden = it->substr(0, delimiterPos);
		if (iden.compare(identifier) == 0 && delimiterPos < it->size())
		{
			token = it->substr(delimiterPos, it->size());
			size_t newStart = token.find_first_not_of(" \r\t\f\v");
			token.erase(0, newStart);
			vec.push_back(token);
		}
		i++;
	}
	return vec;
}

std::string WS::Parser::getContent()
{
	return str;
}

std::vector<std::string> WS::Parser::getVector()
{
	return tokens;
}