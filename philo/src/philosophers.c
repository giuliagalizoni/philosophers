#include "../includes/philosophers.h"

int	wait_all_threads(t_table *table)
{
	int	all_threads_ready;

	all_threads_ready = 0;
	while(!all_threads_ready)
	{
		if (!get_int(&table->lock, &table->all_ready, &all_threads_ready))
			return 0;
		usleep(100);
	}
	return (1);
}

int	finish_simulation(t_table *table, int *is_finished)
{
	if (!get_int(&table->lock, &table->end_simulation, is_finished))
		return (0);
	return (1);
}

void	*routine(void *data)
{
	t_philo *philo;
	int	is_full;
	int	is_finished;

	philo = (t_philo *)data;
	if (!wait_all_threads(philo->table))
		return (NULL);

	// increase running threads
	if (!increase_int(&philo->table->lock, &philo->table->running_threads))
		return (NULL);

	//set last meal time
	if (!set_long(&philo->lock, &philo->last_meal_time, get_time(MILISECOND)))
		return (NULL);

	// to make the game fair
	if (!force_think(philo))
		return (NULL);

	is_finished = 0;
	while(!is_finished)
	{
		if (!finish_simulation(philo->table, &is_finished))
			return (NULL);
		// 1 philo is full?
		if (!get_int(&philo->lock, &philo->is_full, &is_full))
			return (NULL);
		if (is_full)
			break;
		// 2 eat
		if (!eat(philo))
			return (NULL);
		// 3 sleep -> write status & ft_usleep
		if(!write_action(SLEEPING, philo))
			return (NULL);
		if (!ft_usleep(philo->table->time_to_sleep, philo->table))
			return (NULL);
		// 4 think
		if (!think(philo))
			return (NULL);
	}
	return (NULL);
}

void	*single_philo_routine(void *arg)
{
	t_philo *philo;
	int	is_finished;

	philo = (t_philo *)arg;
	wait_all_threads(philo->table); // return value??
	if (!set_long(&philo->lock, &philo->last_meal_time, get_time(MILISECOND)))
		return (NULL);
	if (!increase_int(&philo->table->lock, &philo->table->running_threads))
		return (NULL);
	if (!write_action(TAKE_FIRST_FORK, philo))
		return NULL;
	is_finished = 0;
	while(!is_finished)
	{
		if (!finish_simulation(philo->table, &is_finished))
			return (NULL);
		usleep(200);
	}
	return (NULL);
}

int	start(t_table *table)
{
	int	i;

	i = 0;
	if (!table->nbr_limit_meals)
		return (1); //maybe turn into int function -- idk what I meant here
	if (table->philo_number == 1)
	{
		if (!safe_thread_handle(&table->philos[0].thread_id, single_philo_routine, &table->philos[0], CREATE))
			return (0);
	}
	else
	{
		while (i < table->philo_number)
		{
			if (!safe_thread_handle(&table->philos[i].thread_id, routine,
				&table->philos[i], CREATE))
				return (0);
			i++;
		}
	}
	// monitor
	if (!safe_thread_handle(&table->monitor, monitor, table, CREATE))
		return (0);
	// start of simulation
	table->start_simulation = get_time(MILISECOND);
	if (!table->start_simulation)
		return (0); // keep track of error
	// now all threads are ready
	if (!set_int(&table->lock, &table->all_ready, 1))
		return (0);
	// wait for everyone
	i = 0;
	while(i < table->philo_number)
	{
		if (!safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN))
			return (0);
		i++;
	}
	// if we manage to reach this line, all philos are full
	if (!set_int(&table->lock, &table->end_simulation, 1))
		return (0);
	if (!safe_thread_handle(&table->monitor, NULL, NULL, JOIN))
		return (0);
	return 1;
}
