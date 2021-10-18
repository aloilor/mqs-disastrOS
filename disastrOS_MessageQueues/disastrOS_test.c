#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include "disastrOS.h"
#include "disastrOS_message_queue.h"
#include "linked_list.h"

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void childFunction(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  printf("PID: %d, terminating\n", disastrOS_getpid());

  for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iterate %d\n", disastrOS_getpid(), i);
    disastrOS_sleep(6);
  }
  disastrOS_exit(disastrOS_getpid()+1);
}


void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);
  
  /*
  printf("I feel like to spawn 3 nice threads\n");
  int alive_children=0;
  for (int i=0; i<3; ++i) {
    int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource (and creating if necessary)\n");
    int fd=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd);
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  }
  disastrOS_printStatus(); 


  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  } 
  disastrOS_printStatus();
  */


  
  printf("Testing message queues primitives: \n");
  printf("-------------------------------------------------------------------\n");

  printf("Testing init and alloc: \n");
  MessageQueue_init();
  MessageQueue* mq = MessageQueue_alloc(1, 1);
  MessageQueue_print(mq);
  printf("-------------------------------------------------------------------\n");

  printf("Trying to write smth in the queue: \n");
  Message* msg = Message_alloc("Hello");
  printf("%s \n", msg->msg);
  List_insert((ListHead*) &mq->messages, (ListItem*)mq->messages.head.first, (ListItem*)msg);
  MessageQueue_print(mq);
  printf("-------------------------------------------------------------------\n");

  printf("Trying again to write smth in the queue: \n");
  msg = Message_alloc("How you doing");
  List_insert((ListHead*) &mq->messages, (ListItem*)mq->messages.head.first, (ListItem*)msg);
  MessageQueue_print(mq);
  printf("-------------------------------------------------------------------\n");

  printf("Trying to remove smth from the queue\n");
  List_detach((ListHead*) &mq->messages, (ListItem*)mq->messages.head.last);
  MessageQueue_print(mq);
  printf("-------------------------------------------------------------------\n");

  printf("Testing free of the message queue\n");
  MessageQueue_free(mq);  
  MessageQueue_print(mq);
  printf("-------------------------------------------------------------------\n");
  
  printf("shutdown!");
  disastrOS_shutdown();
}


int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
