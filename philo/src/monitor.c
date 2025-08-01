#include "../includes/philosophers.h"

int all_threads_running(pthread_mutex_t *lock, int *threads, int philo_number)
{
	int	value;

	value = 0;
	safe_mutex_handle(lock, LOCK);
	if (*threads == philo_number)
		value = 1;
	safe_mutex_handle(lock, UNLOCK);
	return (value);
}

static int	dead_philo(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (get_int(&philo->lock, &philo->is_full))
		return (0);

	elapsed = get_time(MILISECOND) - get_long(&philo->lock, &philo->last_meal_time);
	time_to_die = philo->table->time_to_die / 1000;

	if (elapsed > time_to_die) // convert to miliseconds
		return (1);
	return (0);

}

void	*monitor(void *data)
{
	int	i;
	t_table *table;

	table = (t_table *)data;
	//spinlcok till al threads run
	while (!all_threads_running(&table->lock, &table->running_threads, table->philo_number))
	{
		;
	}

	while(!finish_simulation(table))
	{
		i = 0;
		while(i < table->philo_number && !finish_simulation(table))
		{
			if (dead_philo(table->philos + i))
			{
				set_int(&table->lock, &table->end_simulation, 1);
				write_action(DIED, table->philos + i);
			}
			i++;
		}
	}
	return (NULL);
}
