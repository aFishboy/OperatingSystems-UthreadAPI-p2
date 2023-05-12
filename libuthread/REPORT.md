# Introduction
In this project there were many different tasks, but they all worked together to  
create a thread library that allowed the use of running threads, synchronization  
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
## Phase 1
### Queue Decisions
We were tasked with implementing a FIFO (first in first out) queue with the  
constraint that all operations (apart from the iterate and delete operation)  
must be O(1). From these constraints, we knew a linked list would be the best  
option, but we still needed to decide the implementation of the linked list. We  
ultimately decided that a doubly linked list was not needed and a singly linked  
list with a head and tail node would allow us to satisfy the constraints.  

### Building the Queue API
The queue implementation was standard for a FIFO queue. The queue was made up of  
nodes that each held a void pointer to data which allowed for any type of data  
to be stored in the queue. The queue is operated with seven functions: create,  
destroy, enqueue, dequeue, delete, length and iterate. Each function does its  
respective task, while iterate is more notable. The iterate function allows  
for a generic way to call a custom function on each node of the queue. This may  
be a function that prints out each node or something more complex.  

### Testing the Queue
Once the queue had been implemented, we moved onto testing. An example queue  
tester had been provided so we checked that the queue worked with the simple   
tester before moving onto more complex ones. The testing mainly consisted of   
adding data to the queue and then modifying the queue (dequeuing, deleting,   
attempting to destroy) and then checking that the queue responded accordingly.   
To ensurethe queue was working we used assertions to verify the pointers being   
stored and their respective data matched with our expected outcome.

## Phase 2
### Implementing the Uthread API
In phase 2, our goal was to implement thread management for the uthread library.  
This library would allow applications to use the provided functions, create  
start new threads, terminate or manipulate threads in different ways. We began  
by deciding what information is needed to store a thread in a TCB (Thread  
Control Block). For our implementation we needed to have access to a thread’s  
context, stack pointer and state. We created a struct in order to store a  
thread’s information. We also needed a way to hold multiple threads. The queue  
API from phase 1 allowed us to enqueue TCB's. Once a thread is enqueued it will  
be able to run once it gets dequeued. This implementation allows for all threads  
to take turns in a round-robin fashion (as long as threads yield). We also  
decided to have another queue that could collect exited threads to be freed  
later. Once a thread has exited it would no longer be in the ready queue, but  
instead be added to the zombie queue as it waited to be collected and freed.  

Now that we could store threads for use later we needed to implement the  
initialization of the thread library. To do this, an application calls the  
uthread_run function. Once the thread library is ran, it registers the current  
execution as the idle thread. Next we created the initial thread that could then  
interact with other functions of the library. Once the thread library had been  
initialized with the idle thread and initial thread any of the of the thread  
functions could be used by an application.  

A notable limitation of the thread library in phase 2 is the lack of preemption.  
In order for threads to run they would have to be context switched to. In our  
implementation in order for a context switch to occur a thread must first yield.  
If a thread never yields or exits then other threads will starve.  

### Testing the Uthread API
During phase two, our testing of the uthread API was minimal. We decided once we  
had the API working with the two given test files we would move on and fix any  
bugs we ran into when testing with more complicated inputs in further phases.  

## Phase 3
### Semaphore Goals
For phase 3 we were tasked with implementing a secure way to access a common  
resource between multiple threads. We first needed to decide how our semaphore  
would store important information regarding the resource it was protecting.  
Our first idea was to make a struct to hold the amount of the resource there is  
and also hold a lock integer. Later we realized that this was inefficient to   
have a spinlock and changed to having a semaphore also have a blocked queue  
associated that would hold threads that were waiting on the specified resource.  

### Semaphore Implementation
In order to implement semaphores, our main job was to create a way for a thread  
to grab a resource and then later release the resource for other threads to use.  
This was reflected in the sem up and down functions. In our first iteration, sem  
down would call to lock a resource or if there were none wait in a spinlock   
until it could grab a resource. We later changed it so when there were no   
resources for a thread to grab, it would then be blocked and added to the   
blocked queue. Once it had reached the front of the blocked queue it would be   
woken up and given the resource.  

### Semaphore Testing and Corner Case
One of the major issues was trying to solve the corner case of a thread stealing  
a resource from a previously awoken thread. In order to deal with this, when a  
thread is awoken after being blocked, it is in a while loops that checks the  
count of the semaphore. In most cases the count of the semaphore will not be  
zero, but for the corner case the resource will have been stolen so count will  
be zero. Since the count is back to zero, the thread will be put back in the  
blocked queue and blocked. In order to test the semaphore we used the built in  
testers. We compared what our implementation gave compared to the expected  
result. When we encountered issues we went through the code of the testers to  
see where our code was failing. The semaphore testers also gave us more  
extensive testing for the uthread API.  


## Phase 4
### Preemption Goals
For phase 4 we needed to implement a way for a thread to be forcibly yielded in  
order to stop a thread from never giving away their processing resource. To do  
this we needed to add preemption in the form of signals.  


### Preemption Implementation
The main focus for implementing preemption was setting up signal handling. The  
signal handling can be broken up into four parts. Starting, stopping, enabling  
and disabling preemption. In order to start preemption a timer is initialized to  
fire every 100 Hz. In order to handle this timer the use of sigaddset and   
sigaction to set SIGVTALRM and the handling of the signal. Sometimes preemption  
is not wanted so we needed to add a way to enable or disable preemption. In   
order to accomplish this we use a signal mask and set it through the system call   
sigprocmask in preempt_disable and preempt_enable. Thus the implementation of   
the block and unblock, and the implementation the effect of temporarily   
stopping the preemption.

### Testing Preemption
When trying to decide how to test preemption, it was difficult to find a  
simple way to prove that preemption was working. What we came up with was to  
prove that a thread is being preempted. Since preemption is firing an alarm 100  
times a second, we know that if any thread takes longer than one hundredth of a  
second, it will be forcibly yielded. In our tester we create three threads with  
preemption enabled. The first thread creates two more threads. Thread two and    
three print their respective thread number, but before thread two can print, it   
is forced to busy wait for one second. Since thread two takes longer than one  
hundredth of a second, it should be preempted. This means that the order of  
prints should be thread three and then thread two. We then do the same, but do  
not enable preemption. Now thread two will busy wait for a second, but not be  
forced to yield so it will print thread two and then thread three. This proves  
that the preemption is working in this case.  


## Sources
Operating Systems：Three Easy Pieces（book）  


Deciding on Queue Implementation:  
[stackoverflow.com/any-advantage-of-doubly-linked-queue-vs-singly-linked-queue](https://stackoverflow.com/questions/47553585/is-there-any-advantage-of-doubly-linked-queue-over-singly-linked-queue)  


Void Pointer Help:  
https://www.geeksforgeeks.org/void-pointer-c-cpp/#  


Double Pointer Help:  
https://www.educba.com/double-pointer-in-c/  


Waiting for 1 Sec in order to test Preempt:  
https://www.geeksforgeeks.org/time-delay-c/#  


Semaphore / Locks:  
https://zhuanlan.zhihu.com/p/68750396  


Signal Help:  
https://blog.csdn.net/weibo1230123/article/details/81411827  
https://blog.csdn.net/Alkaid2000/article/details/128052806  
https://blog.csdn.net/qq_43743762/article/details/101381313  


Makefile:  
https://dev.to/iamkhalil42/all-you-need-to-know-about-c-static-libraries-1o0b  
https://tldp.org/HOWTO/Program-Library-HOWTO/static-libraries.html  
