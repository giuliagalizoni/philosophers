#include "../includes/philosophers.h"

// convert args to long (why not int? I think I'll try int)
// check for invalid input and throw error


static int	ft_atoi(const char *str)
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
	{
		printf("input must contain only digits");
		// exit the program here?
		return (0);
	}
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

static int validate_input(int number)
{
	if (number < 0)
	{
		printf("number must be positive");
		return (0);
	}
	if (number == 0)
	{
		printf("number must be bigger than 0");
		return (0);
	}
	return (1);
}
int	parse_input(char *input)
{
	int n;

	n = ft_atoi(input);
	if (!validate_input(n))
		return (0); // gotta quit the program here somehow?
		// custum exit function ?
}

int init_table(int ac, char **av, t_table *table)
{
	int n;

	if (ac < 5 || ac > 6) // tlavez isso possa ir pra main
		return (0);
	// maybe check first and atoi after?
	table->philo_number = parse_input(av[1]);
	table->time_to_die = parse_input(av[2]);
	return (1);
}
