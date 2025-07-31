#include "../includes/philosophers.h"


void	write_action(t_philo_action action, t_philo *philo)
{
	long elapsed;

	if (philo->is_full) //thread safe
		return ;
	elapsed = get_time(MILISECOND) - philo->table->start_simulation;
	safe_mutex_handle(&philo->table->write_lock, LOCK);
	if (!finish_simulation(philo->table))
	{
		if (action == TAKE_FIRST_FORK || action == TAKE_SECOND_FORK)
			printf("%ld %d has taken a fork\n", elapsed, philo->id);
		else if (action == EATING)
			printf("%ld %d is eating\n", elapsed, philo->id);
		else if (action == SLEEPING)
			printf("%ld %d is sleeping\n", elapsed, philo->id);
		else if (action == THINKING)
			printf("%ld %d is thinking\n", elapsed, philo->id);
		else if (action == EATING)
			printf("%ld %d died\n", elapsed, philo->id);
	}
	safe_mutex_handle(&philo->table->write_lock, UNLOCK);
}
