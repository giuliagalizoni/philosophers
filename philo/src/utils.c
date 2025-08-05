#include "../includes/philosophers.h"

void	*safe_malloc(size_t size, char *context)
{
	void* memory;

	memory = malloc(size);
	if (!memory)
	{
		ft_perror("malloc failed", context);
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
	int		is_finished;

	start = get_time(MICROSECOND);
	is_finished = 0;
	while ((get_time(MICROSECOND) - start) < usec)
	{
		if (!simulation_is_finished(table, &is_finished))
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

static void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

void	ft_perror(char *error, char *context)
{
	write(2, "Error: ", 7);
	if (context)
	{
		ft_putstr_fd(context, 2);
		write(2, ": ", 2);
	}
	if (error)
		ft_putstr_fd(error, 2);
	else
		ft_putstr_fd("something went wrong", 2);
	write(2, "\n", 1);
}
