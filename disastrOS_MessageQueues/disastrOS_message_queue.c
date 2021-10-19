#include <assert.h>
#include <stdio.h>
#include "disastrOS_resource.h"
#include "disastrOS_descriptor.h"
#include "pool_allocator.h"
#include "linked_list.h"
#include "disastrOS_message_queue.h"

#define MESSAGEQUEUE_SIZE sizeof(MessageQueue)
#define MESSAGEQUEUE_MEMSIZE (sizeof(MessageQueue)+sizeof(int))
#define MESSAGEQUEUE_BUFFER_SIZE MAX_NUM_RESOURCES*MESSAGEQUEUE_MEMSIZE

static char _messagequeues_buffer[MESSAGEQUEUE_BUFFER_SIZE];
static PoolAllocator _messagequeues_allocator;

#define MESSAGE_SIZE sizeof(Message)
#define MESSAGE_MEMSIZE (sizeof(Message)+sizeof(int))
#define MESSAGE_BUFFER_SIZE MAX_NUM_RESOURCES*MESSAGE_MEMSIZE
static char _messages_buffer[MESSAGE_BUFFER_SIZE];
static PoolAllocator _messages_allocator;


void MessageQueue_init(){
    int result=PoolAllocator_init(& _messagequeues_allocator,
                MESSAGEQUEUE_SIZE,
                MAX_NUM_RESOURCES,
                _messagequeues_buffer,
                MESSAGEQUEUE_BUFFER_SIZE);
    assert(! result);

    result=PoolAllocator_init(& _messages_allocator,
                MESSAGE_SIZE,
                MAX_NUM_RESOURCES,
                _messages_buffer,
                MESSAGE_BUFFER_SIZE);
    assert(! result);
}

MessageQueue* MessageQueue_alloc(int id, int type){
    MessageQueue_init();
    MessageQueue* mq = (MessageQueue*) PoolAllocator_getBlock(&_messagequeues_allocator);
    if (!mq) return 0;
    mq->res = (Resource) *Resource_alloc(id, type);
    List_init((ListHead*)&mq->messages.head);
    return mq;
}

Message* Message_alloc(char* text){
    Message* msg = (Message*) PoolAllocator_getBlock(&_messages_allocator);
    msg->msg = text;
    return msg;
}

int MessageQueue_free(MessageQueue* mq){
    Resource_free(&mq->res);
    Message* aux = (Message*) mq->messages.head.first;
    Message* temp;
    while (aux){
        temp = aux;
        aux = (Message*)aux->list.next;
        List_detach((ListHead*)&mq->messages,(ListItem*) temp);
        PoolAllocator_releaseBlock(&_messages_allocator, temp);        
    }
    PoolAllocator_releaseBlock(&_messages_allocator, &mq->messages);
    return PoolAllocator_releaseBlock(&_messagequeues_allocator, mq);
}

void MessageQueue_print(MessageQueue* mq) {
    Resource_print(&mq->res);
    printf("I'm going to print every message in the MessageQueue from the first to the last:\n");
    Message* aux = (Message*) mq->messages.head.first;
    printf("[");
    while (aux){
        printf("- %s - ", aux->msg);
        aux = (Message*) aux->list.next;
    }
    printf("]\n");
}

MessageQueue* MessageQueueList_byId(MessageQueueList* l, int id) {
    return (MessageQueue*)ResourceList_byId((ResourceList*) l, id);
}

void MessageQueueList_print(MessagesHead* mh){
        
}












