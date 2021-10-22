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
    int SLEEP = 30;

    // retrieve the fd of the message queue to send the message to
    int fd=running->syscall_args[0];
    char* msg = (char*) running->syscall_args[1];

    // retrieve the descriptor for the message queue
    Descriptor* des=DescriptorList_byFd(&running->descriptors, fd);    
    // if the fd is not in the the process, we return an error
    if (! des){
        running->syscall_retvalue=DSOS_ERESOURCESEND;
        return;
    }

    // retrieve the message queue
    MessageQueue* mq = (MessageQueue*) des->resource;
    
    // Segmentation fault (core dumped) or *** stack smashing detected ***: <unknown> terminated
    /* 
    while (mq->num_msg == MQ_MAX_MSG_SIZE) {
        printf("The queue is full, let's go to sleep for a while and preempt to another process :) \n");
        disastrOS_sleep(SLEEP);
        disastrOS_printStatus();
    }
    */

    // check if full: if full, put the running process to sleep for SLEEP milliseconds (to mimic the blocking aspect of the queue)
    if (mq->num_msg == MQ_MAX_MSG_SIZE){
        printf("[PID%d]: The queue is full, let's go to sleep and let another process do its job :)\n", disastrOS_getpid());

        if (running->timer) {
            printf("process has already a timer!!!\n");
            running->syscall_retvalue=DSOS_ESLEEP;
            return;
        }
        int cycles_to_sleep=SLEEP;
        int wake_time=disastrOS_time+cycles_to_sleep;
  
        TimerItem* new_timer=TimerList_add(&timer_list, wake_time, running);
        if (! new_timer) {
            printf("no new timer!!!\n");
            running->syscall_retvalue=DSOS_ESLEEP;
            return;
        } 
        
        // let's preempt manually
        running->status = Waiting;
        List_insert(&waiting_list, waiting_list.last, (ListItem*) running);
        if (ready_list.first)
            running=(PCB*) List_detach(&ready_list, ready_list.first);
        else {
            running=0;
            printf ("they are all sleeping\n");
            disastrOS_printStatus();
        }
        
        

        running->syscall_retvalue=DSOS_MQ_FULL;
        return;
    }


    // let's create our message
    Message* to_insert = Message_alloc(msg);

    // otherwise, just continue and add the message to the queue
    List_insert((ListHead*)&mq->messages, mq->messages.head.last, (ListItem*) to_insert);
    mq->num_msg++;    
    printf("[PID%d]: I just sent this message through the message queue: %s\n", disastrOS_getpid(), to_insert->msg);
    
    //on success return 0
    running->syscall_retvalue=0;
}