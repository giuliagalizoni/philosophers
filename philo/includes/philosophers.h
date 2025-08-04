#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <errno.h>

typedef enum e_action
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}	t_philo_action;

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

typedef enum e_time_code
{
	SECOND,
	MILISECOND,
	MICROSECOND,
}	t_time_code;

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
	pthread_mutex_t lock;
	t_table	*table;
}	t_philo;

struct s_table
{
	int	philo_number;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	int	nbr_limit_meals;
	long	start_simulation;
	int		end_simulation;
	int		all_ready;
	int	running_threads;
	pthread_t	monitor;
	pthread_mutex_t	lock;
	pthread_mutex_t write_lock;
	t_fork	*forks;
	t_philo	*philos;
};

int	ft_atoi(const char *str);
int data_init(char **av, t_table *table);
int	check_input(int ac, char **av);
int	start(t_table *table);
void	*safe_malloc(size_t size);
int	safe_mutex_handle(pthread_mutex_t *mutex, t_opcode opcode);
int	safe_thread_handle(pthread_t *thread, void *(*function)(void *), void *data, t_opcode opcode);

int	write_action(t_philo_action action, t_philo *philo);

// safety helpers
int	set_int(pthread_mutex_t *mutex, int *dest, int value);
int	get_int(pthread_mutex_t *mutex, int *value, int *dest);
int	increase_int(pthread_mutex_t *mutex, int *value);
int	set_long(pthread_mutex_t *mutex, long *dest, long value);
int	get_long(pthread_mutex_t *mutex, long *value, long *dest);


int	finish_simulation(t_table *table, int *is_finished);
long	get_time(t_time_code timecode);
int	ft_usleep(long usec, t_table *table);

void	*monitor(void *data);

int	cleanup(t_table *table);

// action routines
int	force_think(t_philo *philo);
int	think(t_philo *philo);
int eat(t_philo *philo);


#endif
