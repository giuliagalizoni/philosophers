#include "../includes/philosophers.h"

// if no meals, return 0
// if 1 philo special case
// create all threads with philos
// create a monitor thread
// sync the beggining -> philo starts running
// join


// wait all philos, sync start
// infinite loop
void	wait_all_threads(t_table *table)
{
	while(!get_int(&table->lock, &table->all_ready))
		; // just wait
}

int	finish_simulation(t_table *table)
{
	return (get_int(&table->lock, &table->end_simulation));
}

void	think(t_philo *philo)
{
	long	time_to_think;

	write_action(THINKING, philo);
	if (philo->table->philo_number % 2 == 0)
		return ;
	time_to_think = philo->table->time_to_eat * 2 - philo->table->time_to_sleep;
	if (time_to_think < 0)
		time_to_think = 0;
	ft_usleep(time_to_think * 0.5, philo->table);
}

static void eat(t_philo *philo)
{
	safe_mutex_handle(&philo->first_fork->lock, LOCK);
	write_action(TAKE_FIRST_FORK, philo);
	safe_mutex_handle(&philo->second_fork->lock, LOCK);
	write_action(TAKE_SECOND_FORK, philo);

	set_long(&philo->lock, &philo->last_meal_time, get_time(MILISECOND));
	philo->meals_counter++; // maybe thread safe?
	write_action(EATING, philo);
	ft_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->nbr_limit_meals > 0 && philo->meals_counter == philo->table->nbr_limit_meals)
		set_int(&philo->lock, &philo->is_full, 1);
	safe_mutex_handle(&philo->first_fork->lock, UNLOCK);
	safe_mutex_handle(&philo->second_fork->lock, UNLOCK);
}

void	force_think(t_philo *philo)
{
	if (philo->table->philo_number % 2 == 0)
	{
		if (philo->id % 2 == 0)
			ft_usleep(30000, philo->table);
	}
	else
	{
		if (philo->id % 2 )
			think(philo);
	}
}

void	*routine(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table);

	// increase running threads
	increase_int(&philo->table->lock, &philo->table->running_threads);

	//set last meal time
	set_long(&philo->lock, &philo->last_meal_time, get_time(MILISECOND));

	// to make the game fair

	while(!finish_simulation(philo->table))
	{
		// 1 philo is full?
		if (get_int(&philo->lock, &philo->is_full))
    		break;
		// 2 eat
		eat(philo);
		// 3 sleep -> write status & ft_usleep
		write_action(SLEEPING, philo);
		ft_usleep(philo->table->time_to_sleep, philo->table);

		// 4 think
		think(philo);
	}
	return (NULL);
}

void	set_int(pthread_mutex_t *mutex, int *dest, int value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

int	get_int(pthread_mutex_t *mutex, int *value)
{
	int result;
	safe_mutex_handle(mutex, LOCK);
	result = *value;
	safe_mutex_handle(mutex, UNLOCK);
	return (result);
}

void	increase_int(pthread_mutex_t *lock, int *value)
{
	safe_mutex_handle(lock, LOCK);
	(*value)++;
	safe_mutex_handle(lock, UNLOCK);
}

void	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long result;
	safe_mutex_handle(mutex, LOCK);
	result = *value;
	safe_mutex_handle(mutex, UNLOCK);
	return (result);
}



//choronometer
// * time code: seconds miliseconds microsecond
long	get_time(t_time_code timecode)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		return (0); // keep the track of the error
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

	philo = (t_philo *)arg;
	wait_all_threads(philo->table);
	set_long(&philo->lock, &philo->last_meal_time, get_time(MILISECOND));
	increase_int(&philo->table->lock, &philo->table->running_threads);
	write_action(TAKE_FIRST_FORK, philo);
	while(!finish_simulation(philo->table))
		usleep(200);
	return (NULL);
}

int	start(t_table *table)
{
	int i;

	i = 0;
	if (!table->nbr_limit_meals)
		return 0; //maybe turn into int function
	if (table->philo_number == 1)
	{
		// deal with return value
		safe_thread_handle(&table->philos[0].thread_id, single_philo_routine, &table->philos[0], CREATE);
	}
	else
	{
		while (i < table->philo_number)
		{
			// deal with return value of safe thread
			safe_thread_handle(&table->philos[i].thread_id, routine,
				&table->philos[i], CREATE);
			i++;
		}
	}
	// monitor
	safe_thread_handle(&table->monitor, monitor, table, CREATE);
	// start of simulation
	table->start_simulation = get_time(MILISECOND);
	if (!table->start_simulation)
		return (0); // keep track of error
	// now all threads are ready
	set_int(&table->lock, &table->all_ready, 1);
	// wait for everyone
	i = 0;
	while(i < table->philo_number)
	{
		safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN);
		i++;
	}
	// if we manage to reach this line, all philos are full
	set_int(&table->lock, &table->end_simulation, 1);
	safe_thread_handle(&table->monitor, NULL, NULL, JOIN);

	return 1;
}
