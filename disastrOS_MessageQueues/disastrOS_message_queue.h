#pragma once
#include "linked_list.h"
#include "disastrOS_pcb.h"
#include "disastrOS_resource.h"

typedef struct Message {
  ListItem list;
  char* msg;
} Message;

typedef struct MessagesHead{
  ListHead head;
} MessagesHead;

typedef struct {
  Resource res; // inheritance
  MessagesHead messages; // head of linked list which contains all the messages
  int num_msg;
} MessageQueue;

void MessageQueue_init();

MessageQueue* MessageQueue_alloc(int id, int type);

Message* Message_alloc(char* text);

int MessageQueue_free(MessageQueue* mq);

typedef ListHead MessageQueueList;

void MessageQueue_print(MessageQueue* mq);

MessageQueue* MessageQueueList_byId(MessageQueueList* l, int id);
