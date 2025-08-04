/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:33:49 by ggalizon          #+#    #+#             */
/*   Updated: 2025/08/04 17:33:50 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	init_table_struct(t_table *table)
{
	table->philo_number = 0;
	table->time_to_die = 0;
	table->time_to_eat = 0;
	table->time_to_sleep = 0;
	table->nbr_limit_meals = 0;
	table->start_simulation = 0;
	table->end_simulation = 0;
	table->all_ready = 0;
	table->running_threads = 0;
	table->philos = NULL;
	table->forks = NULL;
}

int	main(int ac, char **av)
{
	t_table	table;
	int		status;

	init_table_struct(&table);
	status = 0;
	if (!check_input(ac, av))
		status = 1;
	else if (!data_init(av, &table))
		status = 1;
	else if (!start(&table))
		status = 1;
	if (!cleanup(&table))
		status = 1;
	return (status);
}
