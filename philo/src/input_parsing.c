#include "../includes/philosophers.h"

// convert args to long (why not int? I think I'll try int)
// check for invalid input and throw error


int	ft_atoi(const char *str)
{
	int	i;
	int	number;
	int	flag;

	i = 0;
	number = 0;
	flag = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		(i)++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			flag = (flag) * -1;
		i++;
	}
	if (str[i] < '0' || str[i] > '9')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		number = number * 10 + (str[i] - '0');
		i++;
	}
	return (number * flag);
}

// shouldn't be negative
// shouldn't be 0
// shouldn't be invalid -- maybe this is the only one that should be thrown inside atoi
