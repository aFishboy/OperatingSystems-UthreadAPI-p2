#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "queue.h"

void queue_print(queue_t queue, void *data)
{
    if (queue_length(queue) == 0){
        printf("ERROR queue empty cant print");
    }
    printf("data = %d\n", *((int *) data));//////////////////////////
    queue_delete(queue, data);
}

int main(void){
    queue_t q = queue_create();
    int x = 55;
    int y = 65;
    int z = 75;
    int *ptr;
    queue_enqueue(q, &x);
    queue_enqueue(q, &y);
    queue_enqueue(q, &z);
    queue_iterate(q, queue_print);
    printf("queueDel %i\n",queue_delete(q, &z));
    printf("length %i\n",queue_length(q));
    queue_iterate(q, queue_print);
    queue_dequeue(q, (void**)&ptr);
    queue_dequeue(q, (void**)&ptr);
    queue_destroy(q);
    return 0;
}