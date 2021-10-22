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
    int SLEEP = 30;

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

    // Segmentation fault (core dumped) or *** stack smashing detected ***: <unknown> terminated
    /*
    while(mq->num_msg == 0){
        printf("The queue is empty, let's go to sleep for a while and preempt to another process:)\n");
        disastrOS_sleep(SLEEP);
    }
    */

    // check if empty: if empty, put the running process to sleep for SLEEP milliseconds, to mimic the blocking aspect of the mailbox
    if (mq->num_msg == 0){
        printf("[PID%d]: The queue is empty, let's go to sleep and let another process do its job :)\n", disastrOS_getpid());
        
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
        running->syscall_retvalue=DSOS_MQ_EMPTY;
        return;
    }

    Message* msg_read = (Message*) List_detach((ListHead*)&mq->messages.head, (ListItem*) mq->messages.head.first);
    assert(msg_read);

    printf("[PID%d]: This is the message I've received and that I just removed from the queue: %s\n",disastrOS_getpid(), msg_read->msg);
    
    mq->num_msg--;

    // let's send back to the process the message we have just read
    running->syscall_args[1] = (char*) msg_read->msg; 

    // let's free the memory from the pool allocator for
    Message_free(msg_read);

    //on success return 0
    running->syscall_retvalue=0;
}