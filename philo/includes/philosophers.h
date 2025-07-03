#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <errno.h>

typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH
}	t_opcode;

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
	t_fork	*first_fork;
	t_fork	*second_fork;
	pthread_t	thread_id;
	t_table	*table;
}	t_philo;

struct s_table
{
	int	philo_number;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	int	nbr_limit_meals;
	int	start_simulation;
	int		end_simulation;
	t_fork	*forks;
	t_philo	*philos;
};

int	ft_atoi(const char *str);
int data_init(char **av, t_table *table);
int	check_input(int ac, char **av);
void	*safe_malloc(size_t size);
int	safe_mutex_handle(pthread_mutex_t *mutex, t_opcode opcode);


#endif
