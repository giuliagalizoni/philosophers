#include "../includes/philosophers.h"

static int	handle_mutex_error(int status, t_opcode opcode)
{
	if (EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		ft_perror("The value specified by mutex is invalid.", NULL);
	else if (EINVAL == status && INIT == opcode)
		ft_perror("The value specified by attr is invalid.", NULL);
	else if (EDEADLK == status)
		ft_perror("A deadlock would occur if the thread "
			"blocked waiting for mutex.", NULL);
	else if (EPERM == status)
		ft_perror("The current thread does not hold a lock on mutex.", NULL);
	else if (ENOMEM == status)
		ft_perror("The process cannot allocate enough memory "
			"to create another mutex.", NULL);
	else if (EBUSY == status)
		ft_perror("Mutex is locked.", NULL);
	else
		return (1);
	return (0);
}

static int	handle_thread_error(int status)
{
	if (EAGAIN == status)
		ft_perror("No resources to create another thread.", NULL);
	else if (EPERM == status)
		ft_perror("The caller does not have appropriate permission.", NULL);
	else if (EINVAL == status)
		ft_perror("The value specified by attr is invalid.", NULL);
	else if (ESRCH == status)
		ft_perror("No thread could be found corresponding to "
			"that specified by the given thread ID, thread.", NULL);
	else if (EDEADLK == status)
		ft_perror("A deadlock was detected or the value of thread "
			"specifies the calling thread.", NULL);
	else
		return (1);
	return (0);
}

int	safe_mutex_handle(pthread_mutex_t *mutex, t_opcode opcode)
{
	if (LOCK == opcode)
		return (handle_mutex_error(pthread_mutex_lock(mutex), opcode));
	else if (UNLOCK == opcode)
		return (handle_mutex_error(pthread_mutex_unlock(mutex), opcode));
	else if (INIT == opcode)
		return (handle_mutex_error(pthread_mutex_init(mutex, NULL), opcode));
	else if (DESTROY == opcode)
		return (handle_mutex_error(pthread_mutex_destroy(mutex), opcode));
	return (1);
}

int	safe_thread_handle(pthread_t *thread, void *(*function)(void *),
	void *data, t_opcode opcode)
{
	if (CREATE == opcode)
		return (handle_thread_error(pthread_create(thread, NULL,
					function, data)));
	else if (JOIN == opcode)
		return (handle_thread_error(pthread_join(*thread, NULL)));
	else if (DETACH == opcode)
		return (handle_thread_error(pthread_detach(*thread)));
	return (1);
}
