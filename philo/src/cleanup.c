#include "../includes/philosophers.h"

int	destroy_philos(t_table *table)
{
	int i;

	i = 0;
	while (i < table->philo_number)
	{
		if (!safe_mutex_handle(&table->philos[i].lock, DESTROY))
			return (0);
		i++;
	}
	free(table->philos);
	return (1);
}

int	destroy_forks(t_table *table)
{
	int i;

	i = 0;
		while (i < table->philo_number)
		{
			if (!safe_mutex_handle(&table->forks[i].lock, DESTROY))
				return (0);
			i++;
		}
	free(table->forks);
	return (1);
}

int	cleanup(t_table *table)
{
	if (table->philos)
	{
		if (!destroy_philos(table))
			return (0);
	}
	if (table->forks)
	{
		if (!destroy_forks(table))
			return (0);
	}
	if (!safe_mutex_handle(&table->write_lock, DESTROY))
		return (0);
	if (!safe_mutex_handle(&table->lock, DESTROY))
		return (0);
	return (1);
}
