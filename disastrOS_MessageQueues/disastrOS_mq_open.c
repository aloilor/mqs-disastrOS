#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_resource.h"
#include "disastrOS_descriptor.h"
#include "disastrOS_message_queue.h"


// to open or create a message queue
void internal_mqOpen(){
    // get from the PCB the resource id of the resource to open
    int id=running->syscall_args[0];
    int type=running->syscall_args[1];
    int open_mode=running->syscall_args[2];
    //printf("Message queue DEBUG informations:\nid: %d; type: %d, open_mode: %d\n", id, type, open_mode);
    
    // let's check if the user wants to effectively create a message queue (type == 1), otherwise let's call internal_openResource()
    if (type != DSOS_MQ) internal_openResource();
    // if the resource is opened in CREATE mode, create the message queue and return an error if the resource is already existing
    MessageQueue* mq = (MessageQueue*) MessageQueueList_byId((MessageQueueList*) &resources_list, id);
    if (open_mode == DSOS_CREATE){
        if (mq != 0){
            running->syscall_retvalue=DSOS_ERESOURCECREATE;
            return;
        }
        mq = MessageQueue_alloc(id, DSOS_MQ);
        List_insert(&resources_list, resources_list.last, (ListItem*)mq);
    }
    // check if something went wrong while creating our message queue
    if (! mq || mq->res.type!=type) {
        running->syscall_retvalue=DSOS_ERESOURCEOPEN;
        return;
    }
    if (open_mode&DSOS_EXCL && mq->res.descriptors_ptrs.size){
        running->syscall_retvalue=DSOS_ERESOURCENOEXCL;
        return;
    }

    // let's create our descriptor now and add it to the process descriptors list
    Descriptor* des=Descriptor_alloc(running->last_fd, (Resource*)mq, running);
    if (! des){
        running->syscall_retvalue=DSOS_ERESOURCENOFD;
        return;
    }

    running->last_fd++; // we increment the fd value for the next call
    DescriptorPtr* desptr=DescriptorPtr_alloc(des);
    List_insert(&running->descriptors, running->descriptors.last, (ListItem*) des);

    des->ptr = desptr;
    List_insert(&mq->res.descriptors_ptrs, mq->res.descriptors_ptrs.last, (ListItem*) desptr);

    running->syscall_retvalue = des->fd;
}
