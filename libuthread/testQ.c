#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "queue.h"

int main(void){
    queue_t q = queue_create();
    int x = 55;
    int y = 65;
    int z = 75;
    int *ptr;

    queue_enqueue(q, &x);
    queue_enqueue(q, &y);
    queue_enqueue(q, &z);
    queue_dequeue(q, (void**)&ptr);
    queue_dequeue(q, (void**)&ptr);
    queue_dequeue(q, (void**)&ptr);
    queue_destroy(q);
    return 0;
}