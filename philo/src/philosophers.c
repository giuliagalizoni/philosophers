#include "../includes/philosophers.h"

// if no meals, return 0
// if 1 philo special case
// create all threads with philos
// create a monitor thread
// sync the beggining -> philo starts running
// join


// wait all philos, sync start
// infinite loop
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

int	think(t_philo *philo)
{
	long	time_to_think;

	if (!write_action(THINKING, philo))
		return (0);
	if (philo->table->philo_number % 2 == 0)
		return ;
	time_to_think = philo->table->time_to_eat * 2 - philo->table->time_to_sleep;
	if (time_to_think < 0)
		time_to_think = 0;
	ft_usleep(time_to_think * 0.5, philo->table);
	return (1);
}

static int eat(t_philo *philo)
{
	if (!safe_mutex_handle(&philo->first_fork->lock, LOCK))
		return (0);
	write_action(TAKE_FIRST_FORK, philo);
	if (!safe_mutex_handle(&philo->second_fork->lock, LOCK))
		return (0);
	write_action(TAKE_SECOND_FORK, philo);
	if (!set_long(&philo->lock, &philo->last_meal_time, get_time(MILISECOND)))
		return (0);
	philo->meals_counter++; // maybe thread safe?
	if (!write_action(EATING, philo))
		return (0);
	ft_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->nbr_limit_meals > 0 && philo->meals_counter == philo->table->nbr_limit_meals)
	{
		if (!set_int(&philo->lock, &philo->is_full, 1))
			return (0);
	}
	if (!safe_mutex_handle(&philo->first_fork->lock, UNLOCK))
		return (0);
	if (!safe_mutex_handle(&philo->second_fork->lock, UNLOCK))
		return (0);
	return (1);
}

int	force_think(t_philo *philo)
{
	if (philo->table->philo_number % 2 == 0)
	{
		if (philo->id % 2 == 0)
			ft_usleep(30000, philo->table);
	}
	else
	{
		if (philo->id % 2 )
		{
			if (!think(philo))
				return (0);
		}
	}
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
	if (increase_int(&philo->table->lock, &philo->table->running_threads))
		return (NULL);

	//set last meal time
	if (set_long(&philo->lock, &philo->last_meal_time, get_time(MILISECOND)))
		return (NULL);

	// to make the game fair
	force_think(philo);

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
		ft_usleep(philo->table->time_to_sleep, philo->table);

		// 4 think
		think(philo);
	}
	return (NULL);
}

int	set_int(pthread_mutex_t *mutex, int *dest, int value)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (0);
	*dest = value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		return (0);
	return (1);
}

int	get_int(pthread_mutex_t *mutex, int *value, int *dest)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (0);
	*dest = *value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		return (0);
	return (1); //problem
}

int	increase_int(pthread_mutex_t *lock, int *value)
{
	if (!safe_mutex_handle(lock, LOCK))
		return (0);
	(*value)++;
	if (!safe_mutex_handle(lock, UNLOCK))
		return (0);
	return (1);
}

int	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (0);
	*dest = value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		return (0);
	return (1);
}

int	get_long(pthread_mutex_t *mutex, long *value, long *dest)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (0);
	*dest = *value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		return (0);
	return (1);
}

//choronometer
// * time code: seconds miliseconds microsecond
long	get_time(t_time_code timecode)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		return (0); // keep the track of the error // will this work?
	if (SECOND == timecode)
		return (tv.tv_sec + (tv.tv_usec / 1000000));
	else if (MILISECOND == timecode)
		return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
	else if (MICROSECOND == timecode)
		return ((tv.tv_sec * 1000000) + tv.tv_usec);
	else
		return (0); //keep track of the error
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
