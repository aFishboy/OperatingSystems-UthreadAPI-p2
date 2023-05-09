#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

typedef struct semaphore *sem_t;
struct semaphore
{
	/* TODO Phase 3 */
	int count;
};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	sem_t sem = (sem_t)malloc(sizeof(sem_t));
	if (sem != NULL)
	{
		sem->count = count;
	}
	return sem;
}

int sem_destroy(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem == NULL)
	{
		return -1;
	}
	if (sem->count < 0)
	{
		return -1;
	}
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem == NULL)
		return -1;
	while (sem->count <= 0)
	{
		;
		// should be blocked
	}

	sem->count -= 1;
	return 0;
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
	if (sem == NULL)
		return -1;
	sem->count += 1;
	return 0;
	/* Wake up first in line */
}