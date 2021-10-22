# Message queues in disastrOS

> Message Queues in disastrOS ---> [x1] [C98]  
> implement an IPC system based on message queues in disastrOS to allow (blocking) communication between processes;  
> the starting point is the version of disastrOS presented in class - available [here](https://gitlab.com/grisetti/sistemi_operativi_2019_20/-/tree/master/source/08_disastrOS/disastrOS_04_resources).
 
## Fundamental system calls to implement:  
* mq_open: creates or opens the queue and returns a file descriptor to it if successful, <0 if not.  
* mq_unlink: destroys the queue. returns 0 if successful, <0 if not.  
* mq_close: closes the descriptor. returns 0 if successful, <0 if not.  
* mq_send: given a file descriptor and a message, writes the message on the message queue. if the queue is full, the process puts itself to sleep. returns 0 if successful, <0 if not.  
* mq_receive: given a file descriptor and a buffer, writes the message on the buffer. if the queue is empty, the running process puts itself to sleep. returns 0 if successful, <0 if not.

## How to run:
- cd disastrOS_MessageQueues/
- make
- ./disastrOS_test (for a stress test of the queue syscalls)
- ./disastrOS_test_blocking (for a test of the blocking aspect of the queue)
