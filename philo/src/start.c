/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:05:48 by ggalizon          #+#    #+#             */
/*   Updated: 2025/08/06 10:05:49 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	create_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_number)
	{
		if (!safe_thread_handle(&table->philos[i].thread_id, routine,
				&table->philos[i], CREATE))
			return (0);
		i++;
	}
	return (1);
}

static int	join_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_number)
	{
		if (!safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN))
			return (0);
		i++;
	}
	return (1);
}

int	start(t_table *table)
{
	if (!table->nbr_limit_meals)
		return (1);
	if (table->philo_number == 1)
	{
		if (!safe_thread_handle(&table->philos[0].thread_id,
				single_philo_routine, &table->philos[0], CREATE))
			return (0);
	}
	else
	{
		if (!create_philos(table))
			return (0);
	}
	table->start_simulation = get_time(MILISECOND);
	if (!table->start_simulation
		|| !safe_thread_handle(&table->monitor, monitor, table, CREATE)
		|| !set_int(&table->lock, &table->all_ready, 1))
		return (0);
	if (!join_philos(table))
		return (0);
	if (!set_int(&table->lock, &table->end_simulation, 1)
		|| !safe_thread_handle(&table->monitor, NULL, NULL, JOIN))
		return (0);
	return (1);
}
