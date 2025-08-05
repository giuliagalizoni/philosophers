#include "../includes/philosophers.h"

static int	wait_all_threads(t_table *table)
{
	int	all_threads_ready;

	all_threads_ready = 0;
	while (!all_threads_ready)
	{
		if (!get_int(&table->lock, &table->all_ready, &all_threads_ready))
			return (0);
		usleep(100);
	}
	return (1);
}

int	simulation_is_finished(t_table *table, int *is_finished)
{
	if (!get_int(&table->lock, &table->end_simulation, is_finished))
		return (0);
	return (1);
}

int	run_actions(t_philo *philo)
{
	int	is_finished;
	int	is_full;

	is_finished = 0;
	while (!is_finished)
	{
		if (!simulation_is_finished(philo->table, &is_finished))
			return (0);
		if (!get_int(&philo->lock, &philo->is_full, &is_full))
			return (0);
		if (is_full)
			break ;
		if (!eat(philo))
			return (0);
		if (!write_action(SLEEPING, philo))
			return (0);
		if (!ft_usleep(philo->table->time_to_sleep, philo->table))
			return (0);
		if (!think(philo))
			return (0);
	}
	return (1);
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (!wait_all_threads(philo->table))
		return (NULL);
	if (!increase_int(&philo->table->lock, &philo->table->running_threads))
		return (NULL);
	if (!set_long(&philo->lock, &philo->last_meal_time, get_time(MILISECOND)))
		return (NULL);
	if (!force_think(philo))
		return (NULL);
	if (!run_actions(philo))
		return (NULL);
	return (data);
}

void	*single_philo_routine(void *data)
{
	t_philo	*philo;
	int		is_finished;

	philo = (t_philo *)data;
	if (!wait_all_threads(philo->table))
		return (NULL);
	if (!set_long(&philo->lock, &philo->last_meal_time, get_time(MILISECOND)))
		return (NULL);
	if (!increase_int(&philo->table->lock, &philo->table->running_threads))
		return (NULL);
	if (!write_action(TAKE_FIRST_FORK, philo))
		return (NULL);
	is_finished = 0;
	while (!is_finished)
	{
		if (!simulation_is_finished(philo->table, &is_finished))
			return (NULL);
		usleep(200);
	}
	return (data);
}
