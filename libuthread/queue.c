#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"

typedef struct queueNode { // struct to organize data of single node
    void *nodeData;
    struct queueNode *next;
} *queueNode;

struct queue { // struct to hold general queue info
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
	return queue;   // returns pointer to queue or null if malloc fails
}

int queue_destroy(queue_t queue)
{
    // check to see if queue exists or if it is not empty
	if (queue == NULL || queue_length(queue) != 0) { 
        printf("Failed to destroy\n queuelength: %i\n", queue_length(queue));
		printf(" or queue is null = %p\n",  queue);/////////////////////////////////////////////
        return -1;
    }
    free(queue); // deallocate queue
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
    // create node to store head
    queueNode tempNode = queue->head;
    *data = tempNode->nodeData;
    if (queue->head == queue->tail){
        queue->tail = NULL;
    }
    // move head
    queue->head = queue->head->next;
    free(tempNode);
    queue->length--;
    return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if (data == NULL || queue == NULL){
        return -1;
    }

    if ()
    return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
    if (queue == NULL || func == NULL) {
        return -1;
    }

    queueNode currentNode = queue->head;
    while (currentNode != NULL){
        func(queue, currentNode->nodeData);
        currentNode = currentNode->next;
    }
    
	return 0;
}

int queue_length(queue_t queue)
{
	if (queue == NULL) {
        return -1;
    }

    return queue->length;
}

