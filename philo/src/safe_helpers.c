#include "../includes/philosophers.h"

int	set_int(pthread_mutex_t *mutex, int *dest, int value)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (0);
	*dest = value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		return (0);
	return (1);
}

int	get_int(pthread_mutex_t *mutex, int *value, int *dest)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (0);
	*dest = *value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		return (0);
	return (1);
}

int	increase_int(pthread_mutex_t *lock, int *value)
{
	if (!safe_mutex_handle(lock, LOCK))
		return (0);
	(*value)++;
	if (!safe_mutex_handle(lock, UNLOCK))
		return (0);
	return (1);
}

int	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (0);
	*dest = value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		return (0);
	return (1);
}

int	get_long(pthread_mutex_t *mutex, long *value, long *dest)
{
	if (!safe_mutex_handle(mutex, LOCK))
		return (0);
	*dest = *value;
	if (!safe_mutex_handle(mutex, UNLOCK))
		return (0);
	return (1);
}
