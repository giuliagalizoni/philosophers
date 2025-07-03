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

int check_input_chars(char *inputs)
{
	int i;

	i = 1;
		while(inputs[i])
		{
			if (inputs[i] == 32 || (inputs[i] >=9 && inputs[i] <= 13))
			{
				i++;
				continue;
			}
			if (inputs[i] < '0' || inputs[i] > '9') //gotta check if alpha after number is ok
			{
				printf("invalid character\n"); // review error printing
				return (0);
			}
		i++;
	}
	return (1);
}

int valid_input(char *input, char *err)
{
	int n;

	if (!input)
		return (0);
	if (!check_input_chars(input))
	{
		printf("%s\n", err); // change this after
		return (0);
	}
	n = ft_atoi(input);
	if (n <= 0)
	{
		printf("%s\n", err); // change this after
		return (0);
	}
	return (1);
}
int	check_input(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		printf("input needed\n");
		return (0);
	}
	if (!valid_input(av[1], "philo number error")) // change error messages
		return (0);
	if (!valid_input(av[2], "time to die error"))
		return (0);
	if (!valid_input(av[3], "time to eat error"))
		return (0);
	if (!valid_input(av[4], "time to sleep error"))
		return (0);
	if (av[5] && !valid_input(av[5], "limit of meals error"))
		return (0);
	return (1);
}


