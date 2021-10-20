#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include "disastrOS.h"
#include "disastrOS_message_queue.h"
#include "linked_list.h"
#include "disastrOS_globals.h"

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
  printf("I'm gonna try to open (not create) already existent message queues, let's see if it works out\n");
  int type=DSOS_MQ;
  int mode=0;

  int N = 5;
  for (int i = 0; i < N; i++){
    printf("Trying to open message queue id %d\n",i);
    int fd=disastrOS_mq_open(i,type,mode);
    if (fd < 0)
      printf("Error in opening message queue (id%d): %d\n",disastrOS_getpid(), fd);
    else printf("fd=%d\n", fd);
  }
  disastrOS_printStatus();


  for (int i = 0; i < N; i++){
    printf("Trying to close message queue with fd id %d\n",i);
    int ret=disastrOS_mq_close(i);
    if (ret == 0)
      printf("Succesfully closed fd %d\n", i);

    else if (ret < 0) printf("Error in closing fd %d: %d\n", i,ret); 
  }
  disastrOS_printStatus();

  printf("PID: %d, terminating\n", disastrOS_getpid());
  disastrOS_printStatus();
  for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iterate %d\n", disastrOS_getpid(), i);
    disastrOS_sleep(4);
  }
  disastrOS_exit(disastrOS_getpid()+1);
}

void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);
  
  /*
  // ------- MESSAGE QUEUES PRIMITIVES TESTING -- EVERYTHING WORKS FINE -------
  printf("Testing message queues primitives: \n");
  printf("-------------------------------------------------------------------\n");

  printf("Testing init and alloc: \n");
  MessageQueue_init();
  MessageQueue* mq = MessageQueue_alloc(1, DSOS_MQ);
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

  printf("Trying again to write smth in the queue: \n");
  msg = Message_alloc("I'm fine wbu");
  List_insert((ListHead*) &mq->messages, (ListItem*)mq->messages.head.first, (ListItem*)msg);
  MessageQueue_print(mq);
  printf("-------------------------------------------------------------------\n");

  printf("Trying again to write smth in the queue: \n");
  msg = Message_alloc("Everything's good thanks mate");
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
  printf("-------------------------------------------------------------------\n");
  // ------- MESSAGE QUEUES PRIMITIVES TESTING -- EVERYTHING WORKS FINE ------- 
  */

  // ------- MESSAGE QUEUES SYSCALLS BASIC TESTING -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS BASIC TESTING -- EVERYTHING WORKS FINE ------- 
  int fd; int ret;
  printf("Testing mq_open: creating message queue with 0 as id\n");
  fd = disastrOS_mq_open(0, DSOS_MQ, DSOS_CREATE);
  if (fd < 0)
    printf("Error in creating message queue: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_send: trying to send a message to the message queue with 0 as fd\n");
  ret = disastrOS_mq_send(fd, "Hello");
  if (ret < 0)
    printf("Error in sending message: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_send: trying to send a message to the message queue with 0 as fd\n");
  ret = disastrOS_mq_send(fd, "How are you");
  if (ret < 0)
    printf("Error in sending message: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_send: trying to send a message to the message queue with 0 as fd\n");
  ret = disastrOS_mq_send(fd, "How are you");
  if (ret < 0)
    printf("Error in sending message: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_close: closing message queue file descriptor with 0 as id\n");
  ret = disastrOS_mq_close(fd);
  if (ret == 0)
    printf("Succesfully closed message queue file descript %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  /*
  printf("Testing mq_open: trying to create an already existent message queue with 0 as id\n");
  fd = disastrOS_mq_open(0, DSOS_MQ, DSOS_CREATE);
  if (fd < 0)
    printf("Error in creating message queue: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_open: trying to create an already existent message queue with 0 as id \n");
  fd = disastrOS_mq_open(0, DSOS_MQ, DSOS_CREATE);
  if (fd < 0)
    printf("Error in creating message queue: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_open: trying to open (not create) a non existent message queue with 1 as id\n");
  fd = disastrOS_mq_open(1, DSOS_MQ, 0);
  if (fd < 0)
    printf("Error in creating message queue: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");
  */

  printf("Testing mq_open: trying to create another message queue with 1 as id\n");
  fd = disastrOS_mq_open(1, DSOS_MQ, DSOS_CREATE);
  if (fd < 0)
    printf("Error in creating message queue: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_close: closing message queue file descriptor with 1 as id\n");
  ret = disastrOS_mq_close(fd);
  if (ret == 0)
    printf("Succesfully closed message queue file descript %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");
  // ------- MESSAGE QUEUES SYSCALLS BASIC TESTING -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS BASIC TESTING -- EVERYTHING WORKS FINE ------- 

  /*
  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 
  printf("******************************************************************\n");
  printf("Testing creation of multiple message queues by the same process (thread in our case)\n");
  printf("We should get an error trying to create message queues number 0 and number 1, since they have already been created\n");
  printf("We will create N message queues and spawn N threads which are going to open (not create) message queues\n");
  printf("******************************************************************\n");
  int N = 3;
  int alive_children=0;
  for (int i=0; i<N; i++) {
    int type=DSOS_MQ;
    int mode=DSOS_CREATE;
    printf("Creating message queue id:%d (and launching an error if the message queue already exists)\n", i);
    int fd=disastrOS_mq_open(i,type,mode);
    if (fd < 0)
      printf("Error in creating message queue: %d\n", fd);
    else printf("fd=%d\n", fd);
  }

  for(int i = 0; i < N; i++){
    printf("Thread #%d is gonna spawn \n", last_pid);
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  }

  disastrOS_printStatus(); 
  
  printf("let's wait for our children to finish their job\n");
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  } 
  disastrOS_printStatus();

  for (int i = 0; i < 6; i++){
    printf("Trying to unlink message queue with id %d before closing its file descriptor from the main process \n",i);
    int ret=disastrOS_mq_unlink(i);
    if (ret == 0)
      printf("Succesfully closed id %d\n", i);
    else if (ret < 0) printf("Error in closing fd %d: %d\n", i,ret); 
  }
  disastrOS_printStatus();

  for (int i = 0; i < N; i++){
    printf("Trying to close message queue with fd id %d\n",i);
    int ret=disastrOS_mq_close(i);
    if (ret == 0)
      printf("Succesfully closed fd %d\n", i);
    else if (ret < 0) printf("Error in closing fd %d: %d\n", i,ret); 
  }
  disastrOS_printStatus();

  for (int i = 0; i < 6; i++){
    printf("Trying to unlink message queue with id %d\n",i);
    int ret=disastrOS_mq_unlink(i);
    if (ret == 0)
      printf("Succesfully closed id %d\n", i);
    else if (ret < 0) printf("Error in closing fd %d: %d\n", i,ret); 
  }
  disastrOS_printStatus();
  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 
  */


  printf("shutdown!\n");
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
