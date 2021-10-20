# Message queues in disastrOS

> Message Queues in disastrOS ---> [x1] [C98]  
> implement an IPC system based on message queues in disastrOS to allow (blocking) communication between processes;  
> the starting point is the version of disastrOS presented in class - available [here](https://gitlab.com/grisetti/sistemi_operativi_2019_20/-/tree/master/source/08_disastrOS/disastrOS_04_resources).
 
## Fundamental system calls to implement:  
* mq_open: creates or open the queue and returns a file descriptor to it.  
* mq_unlink: destroys the queue.  
* mq_close: closes the descriptor.  
* mq_send: given a file descriptor and a message, this call writes that message on the message queue. If the queue is full, the process sleeps itself.    
* Leggi messaggio: prende il descrittore della coda aperta e legge un messaggio. Se la coda è vuota si blocca. Il processo che tenta di leggere da una coda vuota viene messo a dormire.  


