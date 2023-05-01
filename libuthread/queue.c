#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "queue.h"

struct queueNode {
    void *nodeData;
    struct queueNode *next;
};

struct queue {
    struct queueNode *head;
    struct queueNode *tail;
    int length;
};

queue_t queue_create(void)
{
	queue_t queue = (queue_t) malloc(sizeof(queue_t));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL) {
        return -1;
    }
    if (queue->length != 0) {
        return -1;
    }
    queueNode current = queue->head;
    while (current != NULL) {
        queueNode temp = current;
        current = current->next;
		printf("data = %d\n", *((int *) temp->nodeData));/////////////////////////////////////////////
        free(temp);
	}
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	queueNode node = (queueNode) malloc(sizeof(queueNode));
	if (node == NULL) {
        return -1;
    }
	node->nodeData = data;
    node->next = NULL;
    if (queue->head == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }

    return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}

