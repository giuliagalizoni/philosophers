/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:06:17 by ggalizon          #+#    #+#             */
/*   Updated: 2025/08/06 10:06:24 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	all_threads_running(t_table *table, int *are_running)
{
	int	running_threads_count;

	if (!get_int(&table->lock, &table->running_threads, &running_threads_count))
		return (0);
	if (running_threads_count == table->philo_number)
		*are_running = 1;
	else
		*are_running = 0;
	return (1);
}

static int	dead_philo(t_philo *philo, int	*has_died)
{
	long	elapsed;
	long	last_meal_time;
	int		is_full;

	*has_died = 0;
	if (!get_int(&philo->lock, &philo->is_full, &is_full))
		return (0);
	if (is_full)
		return (1);
	if (!get_long(&philo->lock, &philo->last_meal_time, &last_meal_time))
		return (0);
	elapsed = get_time(MILISECOND) - last_meal_time;
	if (elapsed > philo->table->time_to_die / 1000)
		*has_died = 1;
	return (1);
}

int	check_deaths(t_table *table)
{
	int	i;
	int	has_died;

	i = 0;
	while (i < table->philo_number)
	{
		if (!dead_philo(table->philos + i, &has_died))
			return (0);
		if (has_died)
		{
			if (!write_action(DIED, table->philos + i))
				return (0);
			if (!set_int(&table->lock, &table->end_simulation, 1))
				return (0);
			return (1);
		}
		i++;
	}
	return (1);
}

void	*monitor(void *data)
{
	t_table	*table;
	int		are_running;
	int		is_finished;

	table = (t_table *)data;
	are_running = 0;
	while (!are_running)
	{
		if (!all_threads_running(table, &are_running))
			return (NULL);
		usleep(100);
	}
	is_finished = 0;
	while (!is_finished)
	{
		if (!simulation_is_finished(table, &is_finished))
			return (NULL);
		if (!check_deaths(table))
			return (NULL);
		usleep(100);
	}
	return (data);
}
