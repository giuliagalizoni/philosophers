/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:06:38 by ggalizon          #+#    #+#             */
/*   Updated: 2025/08/07 09:21:24 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	check_input_chars(char *str, int *is_negative)
{
	int	i;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			*is_negative = 1;
		i++;
	}
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

static int	check_int_range(char *str)
{
	int		len;
	char	*start;

	start = str;
	while (*start == ' ' || *start == '\t' || *start == '+')
		start++;
	while (*start == '0')
		start++;
	len = 0;
	while (start[len] >= '0' && start[len] <= '9')
		len++;
	if (len > 10)
		return (0);
	if (len < 10)
		return (1);
	if (ft_strncmp(start, "2147483647", 10) > 0)
		return (0);
	return (1);
}

int	valid_input(char *input, char *context)
{
	int	is_negative;

	is_negative = 0;
	if (!input)
		return (0);
	if (!check_input_chars(input, &is_negative))
	{
		ft_perror("invalid character", context);
		printf(
			"Usage: ./philo <num_philos> <time_die> <time_eat> "
			"<time_sleep> [num_meals]\n");
		return (0);
	}
	if (is_negative)
	{
		ft_perror("value must be greater than 0", context);
		return (0);
	}
	if (!check_int_range(input))
	{
		ft_perror("value is outside the integer range", context);
		return (0);
	}
	return (1);
}

int	check_input(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		ft_perror(
			"Usage: ./philo <num_philos> <time_die> <time_eat> "
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
