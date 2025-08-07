/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:06:54 by ggalizon          #+#    #+#             */
/*   Updated: 2025/08/07 08:18:53 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int pos)
{
	philo->second_fork = &forks[pos];
	philo->first_fork = &forks[((pos + 1) % philo->table->philo_number)];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[pos];
		philo->second_fork = &forks[(pos + 1) % philo->table->philo_number];
	}
}

static int	init_philos(t_table *table)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < table->philo_number)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->is_full = 0;
		philo->meals_counter = 0;
		philo->table = table;
		if (!safe_mutex_handle(&philo->lock, INIT))
			return (0);
		assign_forks(philo, table->forks, i);
		i++;
	}
	return (1);
}

static int	init_forks(t_table *table)
{
	int	i;

	i = 0;
	table->forks = safe_malloc(sizeof(t_fork) * table->philo_number, "Forks");
	while (i < table->philo_number)
	{
		if (!safe_mutex_handle(&table->forks[i].lock, INIT))
			return (0);
		table->forks[i].id = i;
		i++;
	}
	return (1);
}

int	data_init(char **av, t_table *table)
{
	table->philo_number = ft_atoi(av[1]);
	table->time_to_die = (long)(ft_atoi(av[2]) * 1000);
	table->time_to_eat = (long)(ft_atoi(av[3]) * 1000);
	table->time_to_sleep = (long)(ft_atoi(av[4]) * 1000);
	if (av[5])
		table->nbr_limit_meals = ft_atoi(av[5]);
	else
		table->nbr_limit_meals = -1;
	table->end_simulation = 0;
	table->all_ready = 0;
	table->running_threads = 0;
	table->philos = safe_malloc(sizeof(t_philo) * table->philo_number,
			"Philos");
	if (!safe_mutex_handle(&table->lock, INIT))
		return (0);
	if (!safe_mutex_handle(&table->write_lock, INIT))
		return (0);
	if (!init_forks(table))
		return (0);
	if (!init_philos(table))
		return (0);
	return (1);
}

int	ft_atoi(const char *str)
{
	int		i;
	long	number;
	int		flag;

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
	return ((int)number * flag);
}
