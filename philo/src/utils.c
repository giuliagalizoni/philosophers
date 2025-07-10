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

// void	ft_usleep()
// {

// }


// * opcode for mutex and thread functions
// enum e_opcode
// {
// 	LOCK,
// 	UNLOCK,
// 	INIT,
// 	DESTROY,
// 	CREATE,
// 	JOIN,
// 	DETACH
// }	t_opcode;

// Embed controls on return status

static int	handle_mutex_error(int status, t_opcode opcode)
{
	(void)opcode; // check later
	if (0 == status)
		return 1;
	if (EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
	{
		// print error "The value specified by mutex is invalid."
		return 0;
	}
	else if (EINVAL == status && INIT == opcode)
	{
		// print error "The value specified by attr is invalid."
		return 0;
	}
	else if (EDEADLK == status)
	{
		// print error "A deadlock would occur if the thread blocked waiting for mutex."
		return 0;
	}
	else if (EPERM == status)
	{
		// print error "The current thread does not hold a lock on mutex."
		return 0;
	}
	else if (ENOMEM == status)
	{
		// print error "The process cannot allocate enough memory to create another mutex."
		return 0;
	}
	else if (EBUSY == status)
	{
		// print error "Mutex is locked."
		return 0;
	}
	return (1);
}

static int	handle_thread_error(int status, t_opcode opcode)
{
	(void)opcode; // check this later
	if (0 == status)
		return 1;
	if (EAGAIN == status)
	{
		// "No resources to create another thread."
		return (0);
	}
	else if (EPERM == status)
	{
		// "The caller does not have appropriate permission."
		return (0);
	}
	else if (EINVAL == status)
	{
		// "The value specified by attr is invalid."
		return (0);
	}
	else if (ESRCH == status)
	{
		// "No thread could be found corresponding to that specified by the given thread ID, thread."
		return (0);
	}
	else if (EDEADLK == status)
	{
		// "A deadlock was detected or the value of thread specifies the calling thread."
		return (0);
	}
	return (1);
}

// MUTEX SAFE
// init
// destroy
// lock
// unlock

int	safe_mutex_handle(pthread_mutex_t *mutex, t_opcode opcode)
{
	if (LOCK == opcode)
		return handle_mutex_error(pthread_mutex_lock(mutex), opcode);
	else if (UNLOCK == opcode)
		return handle_mutex_error(pthread_mutex_unlock(mutex), opcode);
	else if (INIT == opcode)
		return handle_mutex_error(pthread_mutex_init(mutex, NULL), opcode);
	else if (DESTROY == opcode)
		return handle_mutex_error(pthread_mutex_destroy(mutex), opcode);
	else
	{
		printf("wrong opcode"); // perror f
		return (0);
	}
	return (1);
}

int	safe_thread_handle(pthread_t *thread, void *(*function)(void *), void *data, t_opcode opcode)
{
	if (CREATE == opcode)
		return (handle_thread_error(pthread_create(thread, NULL, function, data), opcode));
	else if (JOIN == opcode)
		return (handle_thread_error(pthread_join(*thread, NULL), opcode));
	else if (DETACH == opcode)
		return (handle_thread_error(pthread_detach(*thread), opcode));
	else
	{
		// "Wrong opcode -- show options"
		return 0;
	}
}

void	ft_usleep(long usec, t_table *table)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = get_time(MICROSECOND);
	while ((get_time(MICROSECOND) - start) < usec)
	{
		if (finish_simulation(table))
			break;
		elapsed = get_time(MICROSECOND) - start;
		remaining = usec - elapsed;
		// to get a spinlock threshold
		if (remaining > 1000)
			usleep(remaining / 2);
		else
		{
			// spinklock
			while(get_time(MICROSECOND) - start < usec)
				;
		}
	}
}
