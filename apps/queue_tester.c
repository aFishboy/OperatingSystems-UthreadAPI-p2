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

    // Declare a queue variable and an array of integers.
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 10};
    size_t i;
    int *ptr;

    // Declare two strings and calculate the size of the integer array.
    char myString1[] = "String1";
    char myString2[] = "String2";
    size_t arrayLen = sizeof(data) / sizeof(data[0]);

    // Create a new queue and enqueue each integer from the array.
    q = queue_create();
    for (i = 0; i < arrayLen; i++) {
        queue_enqueue(q, &data[i]);
    }

    // Dequeue each integer and assert that it is equal to the corresponding
    // integer from the array.
    for (i = 0; i < arrayLen; i++) {
        queue_dequeue(q, (void**)&ptr);
        TEST_ASSERT(ptr == &data[i]);
    }

    // Assert that the length of the queue is now zero.
    TEST_ASSERT(queue_length(q) == 0);

    // Enqueue two strings and dequeue them, asserting that they are equal to
    // the corresponding strings.
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

    // Declare a queue variable and an array of integers.
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 10};
    size_t i;
    int *ptr;
    size_t arrayLen = sizeof(data) / sizeof(data[0]);

    // Create a new queue and enqueue the first integer from the array, then
    // dequeue it immediately.
    q = queue_create();
    queue_enqueue(q, &data[0]);
    queue_dequeue(q, (void**)&ptr);

    // Enqueue the rest of the integers from the array onto the queue.
    for (i = 1; i < arrayLen; i++){
        queue_enqueue(q, &data[i]);
    }

    // Delete the last integer from the queue.
    queue_delete(q, &data[10]);

    // Dequeue two integers from the queue and then enqueue the first integer
    // from the array again.
    queue_dequeue(q, (void**)&ptr);
    queue_dequeue(q, (void**)&ptr);
    queue_enqueue(q, &data[0]);

    // Iterate over the queue and print each integer.
    queue_iterate(q, queue_print);

    // Assert that the length of the queue is now eight.
    printf("q length: %i\n",queue_length(q));
    TEST_ASSERT(queue_length(q) == 8);

    printf("\n");
}

void test_queue_destroy(void) 
{
    fprintf(stderr, "*** TEST queue_destroy with empty queue ***\n");

    // Initialize a queue and enqueue some items
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
	
	// Dequeue all the items to make the queue empty
	for (i = 0; i < arrayLen; i++){
		queue_dequeue(q, (void**)&ptr);
	}
	
	// Test that the queue is empty and that it can be destroyed successfully
	TEST_ASSERT(queue_length(q) == 0);
	TEST_ASSERT(queue_destroy(q) == 0);

    fprintf(stderr, "*** TEST queue_destroy with non empty queue ***\n");
	
	// Initialize a new queue and enqueue an item
	queue_t q2;
	q2 = queue_create();
	queue_enqueue(q2, &data[0]);
	
	// Test that the queue is not empty and that it cannot be destroyed
	TEST_ASSERT(queue_length(q2) == 1);
	TEST_ASSERT(queue_destroy(q2) == -1);

	printf("\n");
}


void test_queue_delete(void)
{
    fprintf(stderr, "*** TEST queue_delete ***\n");
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 10};
    size_t i;
    int *ptr;
    size_t arrayLen = sizeof(data) / sizeof(data[0]);

    // Create a new queue and enqueue all elements in data[] into the queue
    q = queue_create();
    for (i = 0; i < arrayLen; i++){
        queue_enqueue(q, &data[i]);
    }

    // Delete the element at index 5 in data[] from the queue
    queue_delete(q, &data[5]);

    // Check that the queue length is one less than the length of data[]
    TEST_ASSERT((size_t)queue_length(q) == (arrayLen - 1));

    // Dequeue all elements from the queue and check that the order is correct
    for (i = 0; i < (arrayLen - 1); i++){
        queue_dequeue(q, (void**)&ptr);
        if (i < 5){
            TEST_ASSERT(ptr == &data[i]);
        } else {
            TEST_ASSERT(ptr == &data[i+1]);
        }
    }

    // Check that the queue is empty
    TEST_ASSERT(queue_length(q) == 0);
    printf("\n");
}

void test_queue_delete_all(void)
{
    fprintf(stderr, "*** TEST queue_delete_all ***\n");

    // Declare a queue and an array of integers to be used as test data.
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 10};

    // Declare a loop counter and a pointer to an integer.
    size_t i;
    int *ptr;

    size_t arrayLen = sizeof(data) / sizeof(data[0]);

    q = queue_create();

    // Enqueue all the elements in the test data array into the queue.
    for (i = 0; i < arrayLen; i++) {
        queue_enqueue(q, &data[i]);
    }

    // Delete every other element from the queue.
    for (i = 0; i < 11; i += 2) {
        queue_delete(q, &data[i]);
    }

    TEST_ASSERT(queue_length(q) == (5));

    // Print the contents of the queue.
    queue_iterate(q, queue_print);

    printf("\n");

    // Dequeue all the remaining elements in the queue and assert that they are correct.
    for (i = 1; i < 11; i += 2) {
        queue_dequeue(q, (void**)&ptr);
        TEST_ASSERT(ptr == &data[i]);
    }

    TEST_ASSERT(queue_length(q) == 0);

    // Enqueue all the elements in the test data array into the queue again.
    for (i = 0; i < arrayLen; i++) {
        queue_enqueue(q, &data[i]);
    }

    // Delete every other element from the queue.
    for (i = 1; i < 12; i += 2) {
        queue_delete(q, &data[i]);
    }

    // Dequeue all the remaining elements in the queue and assert that they are correct.
    for (i = 0; i < 12; i += 2) {
        queue_dequeue(q, (void**)&ptr);
        TEST_ASSERT(ptr == &data[i]);
    }

    TEST_ASSERT(queue_length(q) == 0);

    printf("\n");
}

void test_delete_error(void)
{
    fprintf(stderr, "*** TEST delete_error ***\n");

    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9, 10};
    size_t i;
    size_t arrayLen = sizeof(data) / sizeof(data[0]);

    int fakeData = 5;

    q = queue_create();

    // Enqueue all the elements in the test data array into the queue.
    for (i = 0; i < arrayLen; i++) {
        queue_enqueue(q, &data[i]);
    }

    // Attempt to delete an element that does not exist in the queue and assert that it fails.
    TEST_ASSERT(queue_delete(q, &fakeData) == -1);

    // Assert that the length of the queue remains the same.
    TEST_ASSERT(queue_length(q) == 11);

    // Print a newline character for formatting.
    printf("\n");
}

void test_enqueue_null(void)
{
    fprintf(stderr, "*** TEST enqueue_null ***\n");

    int num = 5;

    queue_t q = queue_create();

    // Declare a null queue.
    queue_t nullQ = NULL;

    // Attempt to enqueue a null value into the queue and assert that it fails.
    TEST_ASSERT(queue_enqueue(q, NULL) == -1);

    // Attempt to enqueue a value into a null queue and assert that it fails.
    TEST_ASSERT(queue_enqueue(nullQ, &num) == -1);

    printf("\n");
}

int main(void)
{
	test_create();
	test_queue_simple();
	test_iterator();
	test_dequeue();
	test_print();
	test_queue_destroy();
	test_queue_delete();
	test_queue_delete_all();
	test_delete_error();
	test_enqueue_null();

	return 0;
}
