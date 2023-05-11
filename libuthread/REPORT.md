# Introduction
In this project there were many different tasks, but they all worked together to  
create a thread library that allowed the use of running threads, syncronization  
and preemptive scheduling. This implementation was broken up into four phases  
and will be explained in more depth in this report.  

# Workload
Sam was responsible for the following parts:
* Phase 1: Queue API
* Phase 2: Uthread API

While Yutian was responsible for:
* Phase 3: Semaphore API
* Phase 4: Preemption

# Implementation  
## Phase 1 Decisions
We were tasked with implementing a FIFO (first in first out) queue with the 
constraint that all operations (apart from the iterate and delete operation) 
must be O(1). From these constaints we knew a linked list would be the best 
option, but we still needed to decide the implementation of the linked list. We
ultimately decided that a doubly linked list was not needed and a singly linked
list with a head and tail node would allow us to (fulfill?) the constraints.

### Building the Queue API 
The queue implementation was standard for a FIFO queue. The queue was made up of
nodes that each held a void pointer to data which allowed for any type of data 
to be stored in the queue. The queue is operated with 7 functions: create, 
destroy, enqueue, dequeue, delete, length and iterate. Each function does its 
(respective?) task, while iterate is more notable. The iterate function allows 
for a generic way to call a custom function on each node of the queue. This may
be a function that prints out each node or something more complex.

### Testing the Queue
Once the queue had been implemented we moved onto testing. An example queue 
tester had been provided so we checked that the queue worked with the simple 
before moving onto more complex ones. The testing mainly consisted of adding 
data to the queue and then modifying the queue (dequeueing, deleting, attempting
to destroy) and then checking that the queue responded accordingly. To ensure 
the queue was working we used assertions to verify the pointers being stored and
their respective data matched with our expected outcome.





https://www.geeksforgeeks.org/time-delay-c/#