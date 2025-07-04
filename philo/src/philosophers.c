#include "../includes/philosophers.h"

// if no meals, return 0
// if 1 philo special case
// create all threads with philos
// create a monitor thread
// sync the beggining -> philo starts running
// join


// wait all philos, sync start
// infinite loop
void	*routine(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table->all_ready);
	//set last meal time
	while(!finish_simulation(philo->table))
	{
		// philo is full?
		if (philo->is_full) //todo thread safe
			break ;
		// eat
		eat(philo);
		// sleep -> write status & ft_usleep

		// think
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
	result = value;
	safe_mutex_handle(mutex, UNLOCK);
	return (result);
}

void	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	int result;
	safe_mutex_handle(mutex, LOCK);
	result = value;
	safe_mutex_handle(mutex, UNLOCK);
	return (result);
}

int	finish_simulation(t_table *table)
{
	return (get_int(table->mutex, &table->end_simulation));
}

void	wait_all_threads(t_table *table)
{
	while(!get_int(&table->mutex, &table->all_ready))
		;
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

int	start(t_table *table)
{
	int i;

	if (!table->nbr_limit_meals)
		return ; //maybe turn into int function
	if (table->philo_number == 1)
		; // special case
	i = 0;
	while (i < table->philo_number)
	{
		// still have to code this function
		safe_thread_handle(&table->philos[i].id, routine,
			&table->philos[i], CREATE);
		i++;
	}
	// start of simulation
	table->start_simulation = gettime(MILISECOND);
	if (!table->start_simulation)
		return (0); // keep track of error
	// now all threads are ready
	set_int(&table->mutex, &table->all_ready, 1);
	// wait for everyone
	i = 0;
	while(i < table->philo_number)
	{
		safe_thread_handle(&table->philos[i].id, NULL, NULL, JOIN);
		i++;
	}

}
