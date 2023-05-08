#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

// Function to print a single integer from queue
// designed to be used with iterate function
void queue_print(queue_t queue, void *data)
{
    if (queue_length(queue) == 0){
        printf("ERROR queue empty cant print");
    }
    printf("data = %d\n", *((int *) data));
}

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
	printf("\n");
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3;
	int *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
	printf("\n");
}

/* Callback function that increments items */
static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}

void test_iterator(void)
{
	fprintf(stderr, "*** TEST test_iterator ***\n");
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
    TEST_ASSERT(data[0] == 2);
	TEST_ASSERT(data[1] == 3);
	TEST_ASSERT(data[2] == 4);
	TEST_ASSERT(data[3] == 5);
	TEST_ASSERT(data[4] == 6);
	TEST_ASSERT(data[5] == 42);

    TEST_ASSERT(queue_length(q) == 9);
	printf("\n");
}

void test_dequeue(void)
{
    fprintf(stderr, "*** TEST test_dequeue ***\n");
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 10};
    size_t i;
	int *ptr;
	char myString1[] = "String1";
	char myString2[] = "String2";
	size_t arrayLen = sizeof(data) / sizeof(data[0]);
    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < arrayLen; i++){
        queue_enqueue(q, &data[i]);
	}
	
	for (i = 0; i < arrayLen; i++){
		queue_dequeue(q, (void**)&ptr);
		TEST_ASSERT(ptr == &data[i]);
	}
    
    TEST_ASSERT(queue_length(q) == 0);
	
	queue_enqueue(q, &myString1);
	queue_enqueue(q, &myString2);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == (void*)&myString1);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == (void*)&myString2);
	
	printf("\n");
}

void test_print(void){
	fprintf(stderr, "*** TEST queue_print ***\n");
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 10};
    size_t i;
	int *ptr;
	size_t arrayLen = sizeof(data) / sizeof(data[0]);

	q = queue_create();
	queue_enqueue(q, &data[0]);
	queue_dequeue(q, (void**)&ptr);
    for (i = 1; i < arrayLen; i++){
        queue_enqueue(q, &data[i]);
	}
	queue_delete(q, &data[10]);
	queue_dequeue(q, (void**)&ptr);
	queue_dequeue(q, (void**)&ptr);
	queue_enqueue(q, &data[0]);

	queue_iterate(q, queue_print);
	printf("q length: %i\n",queue_length(q));
	TEST_ASSERT(queue_length(q) == 8);
}

int main(void)
{
	test_create();
	test_queue_simple();
	test_iterator();
	test_dequeue();
	test_print();

	return 0;
}
