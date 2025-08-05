#include "../includes/philosophers.h"

void	print_actions(t_philo_action action, t_philo *philo)
{
	long	elapsed;

	elapsed = get_time(MILISECOND) - philo->table->start_simulation;
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
}

int	write_action(t_philo_action action, t_philo *philo)
{
	int		is_finished;
	int		is_full;

	is_finished = 0;
	is_full = 0;
	if (!safe_mutex_handle(&philo->table->write_lock, LOCK))
		return (0);
	if (!get_int(&philo->table->lock, &philo->table->end_simulation,
			&is_finished)
		|| !get_int(&philo->lock, &philo->is_full, &is_full))
	{
		if (!safe_mutex_handle(&philo->table->write_lock, UNLOCK))
			return (0);
		return (0);
	}
	if (is_finished || is_full)
	{
		if (!safe_mutex_handle(&philo->table->write_lock, UNLOCK))
			return (0);
		return (1);
	}
	print_actions(action, philo);
	if (!safe_mutex_handle(&philo->table->write_lock, UNLOCK))
		return (0);
	return (1);
}
