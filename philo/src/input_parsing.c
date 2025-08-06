/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:06:38 by ggalizon          #+#    #+#             */
/*   Updated: 2025/08/06 18:11:49 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	ft_atoi(const char *str, int *over_intmax)
{
	int	i;
	long	number;
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
		if ((number > (__INT_MAX__ - (str[i] - '0')) / 10))
		{
			*over_intmax = 1;
			return (0);
		}
		number = number * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (0);
	return ((int)number * flag);
}

int	check_input_chars(char *inputs)
{
	int	i;

	i = 1;
	while (inputs[i])
	{
		if (inputs[i] == 32 || (inputs[i] >= 9 && inputs[i] <= 13))
		{
			i++;
			continue ;
		}
		if (inputs[i] < '0' || inputs[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_int_max(char *str)
{
	int	i;
	int	count;

	i = 0;
	while (str[i] == '0')
		i++;
	count = 0;
	while (str[i])
	{
		count++;
		i++;
	}
	if (count > 10)
		return (0);
	return (1);
}

int	valid_input(char *input, char *context)
{
	int	n;
	int	over_intmax;

	if (!input)
		return (0);
	if (!check_input_chars(input))
	{
		ft_perror("invalid character", context);
		printf(
			"Usage: ./philo <num_philos> <time_die> <time_eat>"
			"<time_sleep> [num_meals]\n");
		return (0);
	}
	over_intmax = 0;
	n = ft_atoi(input, &over_intmax);
	if (!check_int_max(input) || over_intmax)
		return (ft_perror("value should not exceed INT_MAX", context), 0);
	if (n <= 0)
		return (ft_perror("value shoud be greater than 0", context), 0);
	return (1);
}

int	check_input(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		ft_perror(
			"Usage: ./philo <num_philos> <time_die> <time_eat>"
			"<time_sleep> [num_meals]",
			"wrong number of arguemnts");
		return (0);
	}
	if (!valid_input(av[1], "philo_number"))
		return (0);
	if (!valid_input(av[2], "time_to_die"))
		return (0);
	if (!valid_input(av[3], "time_to_eat"))
		return (0);
	if (!valid_input(av[4], "time_to_sleep"))
		return (0);
	if (av[5] && !valid_input(av[5], "limit_of_meals"))
		return (0);
	return (1);
}
