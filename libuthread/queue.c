#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "queue.h"

typedef struct queueNode {
    void *nodeData;
    struct queueNode *next;
} *queueNode;

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
    queue->length = 0;
    printf("queue: %p\n", queue);////////////////////////delete later
	return queue;   // returns pointer to queue or null if malloc fails
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue_length(queue) != 0) {
        printf("queuelength: %i\n", queue_length(queue));
		printf("queueD = %p\n",  queue);/////////////////////////////////////////////
        return -1;
    }
    free(queue);
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
    if (data == NULL || queue == NULL){
        return -1;
    }

	queueNode node = (queueNode) malloc(sizeof(queueNode));
	if (node == NULL) {
        return -1;
    }

	node->nodeData = data;
    node->next = NULL;
    queue->length++;
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
	if (*data == NULL || queue == NULL || queue_length(queue) == 0){
        return -1;
    }

    queueNode tempNode = queue->head;
    *data = tempNode->nodeData;

    if (queue->head == queue->tail){
        queue->tail = NULL;
    }
    queue->head = queue->head->next;
	printf("data = %d\n", *((int *) tempNode->nodeData));/////////////////////////////////////////////
    free(tempNode);
    queue->length--;

    return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if (data == NULL || queue == NULL){
        return -1;
    }
    return 0;
    
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	return 0;
}

int queue_length(queue_t queue)
{
	if (queue == NULL) {
        return -1;
    }

    return queue->length;
}

