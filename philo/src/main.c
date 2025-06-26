#include "../includes/philosophers.h"

#define NUM_PHILO 4

// void *routine(void *arg)
// {
// 	int id = *(int *)arg;
// 	printf("Philosopher %d is thinking.\n", id);
// 	sleep(1); // Simulate thinking
// 	printf("Philosopher %d is hungry.\n", id);
// 	sleep(1); // Simulate hunger
// 	printf("Philosopher %d is eating.\n", id);
// 	sleep(1); // Simulate eating
// 	printf("Philosopher %d is done eating.\n", id);
// 	return NULL;
// }
//TODO
// - custom perror
// - cleanup function

int	main(int ac, char **av)
{
	t_table table;

	if (ac < 5 || ac > 6) // tlavez isso possa ir pra main
	{
		printf("input needed\n");
		return (1);
	}
	// check invalid input
	// init data
		// return 1 in case of error
	if (!init(av, &table))
		return (1);

	printf("number of philos: %d\n", table.philo_number);
	printf("number of philos: %d\n", table.time_to_die);
	printf("number of philos: %d\n", table.time_to_eat);
	printf("number of philos: %d\n", table.time_to_sleep);
	if (table.nbr_limit_meals)
		printf("number of philos: %d\n", table.nbr_limit_meals);
	// special case for 1 philo
	// initiate therad
	// cleanup
	return 0;

}

// int	main()
// {
// 	pthread_t philo[NUM_PHILO];
// 	int ids[NUM_PHILO];
// 	int i = 0;

// 	// input check
// 	// ac must be 5 or 6
// 	// throw error and exit if not
// 	// input parsing
// 	// init data
// 	// start simulation


// 	while (i < NUM_PHILO)
// 	{
// 		ids[i] = i + 1;
// 		if (pthread_create(&philo[i],  NULL, routine, &ids[i]))
// 		{
// 			perror("Failed to create thread");
// 			return 1;
// 		}
// 		i++;
// 	}
// 	i = 0;
// 	while (i < NUM_PHILO)
// 	{
// 		if (pthread_join(philo[i], NULL))
// 		{
// 			perror("failed to join thread");
// 			return 1;
// 		}
// 		i++;
// 	}

// 	return 0;

// }
