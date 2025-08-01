#include "../includes/philosophers.h"


void	write_action(t_philo_action action, t_philo *philo)
{
	long elapsed;

	safe_mutex_handle(&philo->table->write_lock, LOCK);
	elapsed = get_time(MILISECOND) - philo->table->start_simulation;
	if (get_int(&philo->table->lock, &philo->table->end_simulation)
		|| get_int(&philo->lock, &philo->is_full))
    {
        safe_mutex_handle(&philo->table->write_lock, UNLOCK);
        return ;
    }
	if (action == TAKE_FIRST_FORK || action == TAKE_SECOND_FORK)
		printf("%ld %d has taken a fork\n", elapsed, philo->id);
	else if (action == EATING)
		printf("%ld %d is eating\n", elapsed, philo->id);
	else if (action == SLEEPING)
		printf("%ld %d is sleeping\n", elapsed, philo->id);
	else if (action == THINKING)
		printf("%ld %d is thinking\n", elapsed, philo->id);
	else if (action == DIED)
		printf("%ld %d died\n", elapsed, philo->id);
	safe_mutex_handle(&philo->table->write_lock, UNLOCK);
}
