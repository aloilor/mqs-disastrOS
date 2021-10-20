#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_resource.h"
#include "disastrOS_descriptor.h"
#include "disastrOS_message_queue.h"

// to send a message through the message queue
void internal_mqSend(){
    // retrieve the fd of the resource to send the message to
    int fd=running->syscall_args[0];
    char* msg = (char*) running->syscall_args[1];
    
    // retrieve the descriptor for the message queue
    Descriptor* des=DescriptorList_byFd(&running->descriptors, fd);    
    // if the fd is not in the the process, we return an error
    if (! des){
        running->syscall_retvalue=DSOSO_ERESOURCESEND;
        return;
    }
    // retrieve the message queue
    MessageQueue* mq = (MessageQueue*) des->resource;

    // let's create our message
    Message* to_insert = Message_alloc(msg);

    // check if full: if full, put the running process to sleep for six seconds(to mimic the blocking aspect of the queue) 
    while (mq->num_msg == MQ_MAX_MSG_SIZE) {
        printf("Too many messages in the queue, let's sleep for a bit :) \n");
        disastrOS_sleep(150);
    } 

    // otherwise, just continue and add the message to the queue
    List_insert( (ListHead*)&mq->messages, mq->messages.head.last, (ListItem*) to_insert);
    mq->num_msg++;

    //on success return 0
    running->syscall_retvalue=0;

}