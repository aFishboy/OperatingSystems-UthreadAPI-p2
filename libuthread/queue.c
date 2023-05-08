#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"

// Define a structure to organize data of a single node in the queue
typedef struct queueNode {
    void *nodeData;             // Pointer to the data stored in the node
    struct queueNode *next;     // Pointer to the next node in the queue
} *queueNode;

// Define a structure to hold general queue information
struct queue {
    struct queueNode *head;     // Pointer to the head (front) of the queue
    struct queueNode *tail;     // Pointer to the tail (end) of the queue
    int length;                 // Number of elements in the queue
};

// Create a new queue and return a pointer to it
queue_t queue_create(void) 
{
    queue_t queue = (queue_t) malloc(sizeof(queue_t));   // Allocate memory for the queue
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;   // Return pointer to queue or null if malloc fails
}

// Destroy an existing queue and deallocate its memory
int queue_destroy(queue_t queue)
{
    // Check to see if the queue exists or if it is not empty
    if (queue == NULL || queue_length(queue) != 0) { 
        printf("Failed to destroy\n queuelength: %i\n", queue_length(queue));
        printf(" or queue is null = %p\n",  queue);
        return -1;
    }
    free(queue);    // Deallocate memory for the queue
    return 0;
}

// Add a new element to the end of the queue
int queue_enqueue(queue_t queue, void *data)
{
    if (data == NULL || queue == NULL){
        return -1;
    }
    queueNode node = (queueNode) malloc(sizeof(queueNode));  // Allocate memory for the new node
    if (node == NULL) {
        return -1;
    }
    node->nodeData = data; 
    node->next = NULL;
    queue->length++;   // Increment the queue length
    if (queue->head == NULL) {   // If the queue is empty, set the head and tail pointers to the new node
        queue->head = node;
        queue->tail = node;
    } else {    // Otherwise, add the new node to the end of the queue and update the tail pointer
        queue->tail->next = node;
        queue->tail = node;
    }
    return 0;
}

// Remove the element at the front of the queue and return its data by updating **data passed in
int queue_dequeue(queue_t queue, void **data)
{
    if (data == NULL || queue == NULL || queue_length(queue) == 0){
        return -1;
    }
    queueNode tempNode = queue->head;   // Create a temporary node to store the head
    *data = tempNode->nodeData;
    if (queue->head == queue->tail){    // If the queue only has one element, update the tail pointer to null
        queue->tail = NULL;
    }
    queue->head = queue->head->next;    // Update the head pointer to the next node
    free(tempNode);     // Deallocate memory for the old head node
    queue->length--;    // Decrement the queue length
    return 0;
}


// Function to delete a node from the queue
int queue_delete(queue_t queue, void *data_to_find)
{
    // Check for invalid input parameters or an empty queue
    if (data_to_find == NULL || queue == NULL || queue->head == NULL){
        return -1;
    }
    
    // Start at the head of the queue
    queueNode currentNode = queue->head;
    // Keep track of the previous node
    queueNode prevNode = NULL;
    while (currentNode != NULL){
        // If the data at the current node matches the data to be deleted
        if (currentNode->nodeData == data_to_find){
            // If the current node is the head, update the head pointer
            if (prevNode == NULL){
                queue->head = currentNode->next;
            } else {
                // Otherwise, update the next pointer of the previous node to skip the current node
                prevNode->next = currentNode->next;
            }
            // If the current node is also the tail, update the tail pointer
            if (queue->tail == currentNode) {
                queue->tail = prevNode;
            }
            // Deallocate memory for the current node
            free(currentNode);
            // Decrement the queue length
            queue->length--;
            return 0;
        }
        // Move to the next node
        prevNode = currentNode;
        currentNode = currentNode->next;
    }
    // If the data was not found in the queue, return an error
    return -1;
}

// Function to iterate over the nodes in the queue and apply a function to each node
int queue_iterate(queue_t queue, queue_func_t func)
{
    // Check for invalid input parameters or an empty queue
    if (queue == NULL || func == NULL) {
        return -1;
    }
    if (queue->head == NULL){
        return 0;
    }
    
    // Initialize the current node to the head of the queue and the next node to the next node after the head
    queueNode currentNode = queue->head;
    queueNode nextNode = currentNode->next;
    while (currentNode != NULL){
        // Apply the function to the current node
        func(queue, currentNode->nodeData);
        // Move to the next node
        currentNode = nextNode;
        // If the current node is not NULL, update the next node to be the next node after the current node
        if (currentNode != NULL){
            nextNode = currentNode->next;
        }
    }
    
	return 0;
}

// Function to get the length of the queue
int queue_length(queue_t queue)
{
    // Check for invalid input parameter
	if (queue == NULL) {
        return -1;
    }

    // Return the length of the queue
    return queue->length;
}

