#include "../includes/philosophers.h"

#define NUM_PHILO 4

// void *routine(void *arg)
// {
// 	int id = *(int *)arg;
// 	printf("Philosopher %d is thinking.\n", id);
// 	sleep(1); // Simulate thinking
// 	printf("Philosopher %d is hungry.\n", id);
// 	sleep(1); // Simulate hunger
// 	printf("Philosopher %d is eating.\n", id);
// 	sleep(1); // Simulate eating
// 	printf("Philosopher %d is done eating.\n", id);
// 	return NULL;
// }
//TODO
// - custom perror
// - cleanup function

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

static void	philo_init(t_table *table)
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
		safe_mutex_handle(&philo->lock, INIT);
		assign_forks(philo, table->forks, i);
		i++;
	}
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
	safe_mutex_handle(&table->lock, INIT);
	safe_mutex_handle(&table->write_lock, INIT); // is this right?
	table->forks = safe_malloc(sizeof(t_fork) * table->philo_number);
	i = 0;
	while(i < table->philo_number)
	{
		if (!safe_mutex_handle(&table->forks[i].lock, INIT))
			return 0; // clean up
		table->forks[i].id = i;
		i++;
	}
	philo_init(table);
	return (1);
}

void	cleanup(t_table *table)
{
	t_philo *philo;
	int	i;

	i = 0;
	while (i < table->philo_number)
	{
		philo = table->philos + i;
		safe_mutex_handle(&philo->lock, DESTROY); // deal with return value
		i++;
	}
	safe_mutex_handle(&table->write_lock, DESTROY); // deal with return value
	safe_mutex_handle(&table->lock, DESTROY); // deal with return value
	free(table->forks);
	free(table->philos);
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
	cleanup(&table);
	return 0;

}

// int	main()
// {
// 	pthread_t philo[NUM_PHILO];
// 	int ids[NUM_PHILO];
// 	int i = 0;

// 	// input check
// 	// ac must be 5 or 6
// 	// throw error and exit if not
// 	// input parsing
// 	// init data
// 	// start simulation


// 	while (i < NUM_PHILO)
// 	{
// 		ids[i] = i + 1;
// 		if (pthread_create(&philo[i],  NULL, routine, &ids[i]))
// 		{
// 			perror("Failed to create thread");
// 			return 1;
// 		}
// 		i++;
// 	}
// 	i = 0;
// 	while (i < NUM_PHILO)
// 	{
// 		if (pthread_join(philo[i], NULL))
// 		{
// 			perror("failed to join thread");
// 			return 1;
// 		}
// 		i++;
// 	}

// 	return 0;

// }
