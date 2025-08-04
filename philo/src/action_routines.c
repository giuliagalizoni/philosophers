/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_routines.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalizon <ggalizon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:31:16 by ggalizon          #+#    #+#             */
/*   Updated: 2025/08/04 17:32:24 by ggalizon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	think(t_philo *philo)
{
	long	time_to_think;

	if (!write_action(THINKING, philo))
		return (0);
	if (philo->table->philo_number % 2 == 0)
		return (1);
	time_to_think = philo->table->time_to_eat * 2 - philo->table->time_to_sleep;
	if (time_to_think < 0)
		time_to_think = 0;
	if (!ft_usleep(time_to_think * 0.5, philo->table))
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
		if (philo->id % 2)
		{
			if (!think(philo))
				return (0);
		}
	}
	return (1);
}

int	eat(t_philo *philo)
{
	if (!safe_mutex_handle(&philo->first_fork->lock, LOCK))
		return (0);
	write_action(TAKE_FIRST_FORK, philo);
	if (!safe_mutex_handle(&philo->second_fork->lock, LOCK))
		return (0);
	write_action(TAKE_SECOND_FORK, philo);
	if (!set_long(&philo->lock, &philo->last_meal_time, get_time(MILISECOND)))
		return (0);
	philo->meals_counter++;
	if (!write_action(EATING, philo))
		return (0);
	if (!ft_usleep(philo->table->time_to_eat, philo->table))
		return (0);
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
