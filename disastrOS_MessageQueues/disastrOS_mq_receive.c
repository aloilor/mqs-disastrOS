#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_resource.h"
#include "disastrOS_descriptor.h"
#include "disastrOS_message_queue.h"

// to receive a message through the message queue
void internal_mqReceive(){
    int SLEEP = 150;

    // retrieve the fd of the message queue to receive the message from
    int fd=running->syscall_args[0];

    // retrieve the descriptor for the message queue
    Descriptor* des=DescriptorList_byFd(&running->descriptors, fd);    
    // if the fd is not in the the process, we return an error
    if (! des){
        running->syscall_retvalue=DSOS_ERESOURCERECV;
        return;
    }

    // retrieve the message queue
    MessageQueue* mq = (MessageQueue*) des->resource;

    // check if empty: if empty sleep the running process for SLEEP milliseconds, to mimic the blocking aspect of the mailbox
    if (mq->num_msg == 0){
        printf("The queue is empty, let's go to sleep for a while and preempt to another process:)\n");
        disastrOS_sleep(SLEEP);
    }

    Message* msg_read = (Message*) List_detach((ListHead*)&mq->messages.head, (ListItem*) mq->messages.head.first);
    assert(msg_read);
    printf("This is the message I've read and that I just removed from the queue: %s\n", msg_read->msg);
    
    // let's free the memory from the pool allocator for
    Message_free(msg_read);
    mq->num_msg--;

    // let's send back to the process the message we have just read
    running->syscall_args[1] = (char*) msg_read->msg; 

    //on success return 0
    running->syscall_retvalue=0;
}