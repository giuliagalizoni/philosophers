#include "../includes/philosophers.h"

static void assign_forks(t_philo *philo, t_fork *forks, int pos)
{
	philo->second_fork = &forks[pos];
	philo->first_fork = &forks[((pos + 1) % philo->table->philo_number)];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[pos];
		philo->second_fork = &forks[(pos + 1) % philo->table->philo_number];
	}
}

static int	philo_init(t_table *table)
{
	int	i;
	t_philo *philo;

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

int data_init(char **av, t_table *table)
{
	int i;

	table->philo_number = ft_atoi(av[1]);
	table->time_to_die = (long) (ft_atoi(av[2]) * 1000);
	table->time_to_eat = (long) (ft_atoi(av[3]) * 1000);
	table->time_to_sleep = (long) (ft_atoi(av[4]) * 1000);
	if (av[5])
		table->nbr_limit_meals = ft_atoi(av[5]);
	else
		table->nbr_limit_meals = -1;
	table->end_simulation = 0;
	table->all_ready = 0;
	table->running_threads = 0;
	table->philos = safe_malloc(sizeof(t_philo) * table->philo_number);
	if (!safe_mutex_handle(&table->lock, INIT))
		return (0);
	if (!safe_mutex_handle(&table->write_lock, INIT))
		return (0); // is this right?
	table->forks = safe_malloc(sizeof(t_fork) * table->philo_number);
	i = 0;
	while(i < table->philo_number)
	{
		if (!safe_mutex_handle(&table->forks[i].lock, INIT))
			return 0; // clean up
		table->forks[i].id = i;
		i++;
	}
	if (!philo_init(table))
		return (0);
	return (1);
}

int	cleanup(t_table *table)
{
	t_philo *philo;
	int	i;

	i = 0;
	while (i < table->philo_number)
	{
		philo = table->philos + i;
		if (!safe_mutex_handle(&philo->lock, DESTROY))
			return (0);
		i++;
	}
	if (!safe_mutex_handle(&table->write_lock, DESTROY))
		return (0);
	if (!safe_mutex_handle(&table->lock, DESTROY))
		return (0);
	free(table->forks);
	free(table->philos);
	return (1);
}

int	main(int ac, char **av)
{
	t_table table;

	if (!check_input(ac, av))
		return (1);
	if (!data_init(av, &table))
		return (1);
	if (!start(&table))
		return (1);
	if (!cleanup(&table))
		return (1);
	return 0;
}
