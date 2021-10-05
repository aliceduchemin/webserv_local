#include "../includes/utils.hpp"

char* joinChar(std::string first, std::string second)
{
	char *res = new char[first.size() + second.size() + 1];

	size_t i = 0;
	while (i < first.size())
	{
		res[i] = first[i];
		i++;
	}
	size_t j = 0;
	while (j < second.size())
	{
		res[i] = second[j];
		i++;
		j++;
	}
	res[i] = 0;
	return res;
}

void	deleteTab(char **tab)
{
	int i = 0;

	while (i < 21)
	{
		delete tab[i];
		i++;
	}
	delete[] tab;
}