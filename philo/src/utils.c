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
