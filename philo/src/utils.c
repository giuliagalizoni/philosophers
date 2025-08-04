#include "../includes/philosophers.h"

void	*safe_malloc(size_t size)
{
	void* memory;

	memory = malloc(size);
	if (!memory)
	{
		printf("malloc error\n"); // review error messages;
		return NULL;
	}
	return memory;
}

//choronometer
// * time code: seconds miliseconds microsecond
long	get_time(t_time_code timecode)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		return (0); // keep the track of the error // will this work?
	if (SECOND == timecode)
		return (tv.tv_sec + (tv.tv_usec / 1000000));
	else if (MILISECOND == timecode)
		return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
	else if (MICROSECOND == timecode)
		return ((tv.tv_sec * 1000000) + tv.tv_usec);
	else
		return (0);
}

int	ft_usleep(long usec, t_table *table)
{
	long	start;
	long	elapsed;
	long	remaining;
	int	is_finished;

	start = get_time(MICROSECOND);
	is_finished = 0;
	while ((get_time(MICROSECOND) - start) < usec)
	{
		if (!finish_simulation(table, &is_finished))
			return (0);
		if (is_finished)
			break;
		elapsed = get_time(MICROSECOND) - start;
		remaining = usec - elapsed;
		if (remaining > 1000)
			usleep(remaining / 2);
		else
		{
			while(get_time(MICROSECOND) - start < usec)
				;
		}
	}
	return (1);
}
