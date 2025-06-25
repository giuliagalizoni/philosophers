#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_table	t_table;
//Fork
typedef struct s_fork
{
	pthread_mutex_t	lock;
	int				id;
}	t_fork;

//Philo
typedef struct s_philo
{
	int		id;
	long	meals_counter;
	int		is_full;
	long	last_meal_time;
	t_fork	*left_fork;
	t_fork	*right_fork;
	pthread_t	thread_id;
	t_table	*table;
}	t_philo;

struct s_table
{
	long	philo_number;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	nbr_limit_meals;
	long	start_simulation;
	int		end_simulation;
	t_fork	*forks;
	t_philo	*philos;
};

#endif
