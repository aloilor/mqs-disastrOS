#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>


#include "disastrOS.h"
#include "disastrOS_message_queue.h"
#include "linked_list.h"
#include "disastrOS_globals.h"

#define SLEEP 20

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}


void childFunction(void* args){
  int fd; int ret; int i;

  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 

  printf("[Child%d]: Hello, I am the child function %d\n",disastrOS_getpid(), disastrOS_getpid());
  int M = 5; // number of messages per queue

  int type=DSOS_MQ;
  int mode=0;
  printf("[Child%d]: Opening message queue id %d\n", disastrOS_getpid(), disastrOS_getpid());
  fd = disastrOS_mq_open(disastrOS_getpid(), type, mode);
  if (fd < 0) 
    printf("[Child%d]: error in opening message queue: %d\n", disastrOS_getpid(), fd);
  disastrOS_printStatus(); printf("******************************************************************\n\n");

  printf("[Child%d]: I'm gonna receive %d messages from the queue using file descriptor %d\n",disastrOS_getpid(), M, fd);
  char* msg_read = 0;
  for(i = 0; i < M; i++){
    ret = disastrOS_mq_receive(fd, msg_read);
    if (ret < 0)
      printf("[Child%d]: Error in receiving message: %d\n", disastrOS_getpid(), ret);
    else 
      printf("[Child%d]: this is the message I received and I removed from the queue: %s\n", disastrOS_getpid(),(char*) running->syscall_args[1]);
  } disastrOS_printStatus(); printf("******************************************************************\n\n");

  printf("[Child%d]: I'm gonna close file descriptor %d for the message queue\n",disastrOS_getpid(), fd);
  ret = disastrOS_mq_close(fd);
  if (ret < 0) 
    printf("[Child%d]: Error in closing file descriptor %d: error number %d", disastrOS_getpid(), fd, ret);

  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 

  printf("[Child%d]: terminating\n", disastrOS_getpid());
  disastrOS_printStatus();
  for (i=0; i<(disastrOS_getpid()+1); ++i){
    printf("[Child%d]: iterate %d\n", disastrOS_getpid(), i);
    disastrOS_sleep(4);
  }
  disastrOS_exit(disastrOS_getpid()+1);
}


void childFunctionRecv(void* args){
  int fd; int ret; int i;

  printf("[Child%d]: Hello, I am the child function %d\n",disastrOS_getpid(), disastrOS_getpid());
  int M = 13; // number of messages per queue

  // ------- MESSAGE QUEUES SYSCALLS BLOCKING TEST -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS BLOCKING TEST -- EVERYTHING WORKS FINE ------- 

  int type=DSOS_MQ;
  int mode=0;
  printf("[Child%d]: Opening message queue id %d\n", disastrOS_getpid(), 0);
  fd = disastrOS_mq_open(0, type, mode);
  if (fd < 0) 
    printf("[Child%d]: error in opening message queue: %d\n", disastrOS_getpid(), fd);
  disastrOS_printStatus(); printf("******************************************************************\n\n");

  disastrOS_printStatus();
  printf("[Child%d]: I'm gonna receive %d messages from the queue using file descriptor %d\n",disastrOS_getpid(), M, fd);
  char* msg_read = 0;
  for(i = 0; i < M; i++){
    ret = disastrOS_mq_receive(fd, msg_read);
    if (ret < 0){
      printf("[Child%d]: Error in receiving message: %d\n", disastrOS_getpid(), ret);
      if (ret == DSOS_MQ_EMPTY){
        printf("[Child%d]: Let's sleep for a bit, so that another process can fill the queue :)\n", disastrOS_getpid());
        disastrOS_sleep(SLEEP);
      } 
    }else 
      printf("[Child%d]: this is the message I received and I removed from the queue: %s\n", disastrOS_getpid(),(char*) running->syscall_args[1]);
  } disastrOS_printStatus(); printf("******************************************************************\n\n");

  printf("[Child%d]: Closing file descriptor %d for the queue.\n", disastrOS_getpid(), fd);
  ret = disastrOS_mq_close(fd);
  if (ret < 0) 
    printf("[Main process]: Error in closing file descriptor %d: error number %d", fd, ret);
  disastrOS_printStatus(); printf("******************************************************************\n\n");

  printf("[Child%d]: terminating\n", disastrOS_getpid());
  disastrOS_printStatus();
  for (i=0; i<(disastrOS_getpid()+1); ++i){
    printf("[Child%d]: iterate %d\n", disastrOS_getpid(), i);
    disastrOS_sleep(4);
  }
  disastrOS_exit(disastrOS_getpid()+1);


}

void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);
  
  /* ------- MESSAGE QUEUES PRIMITIVES TESTING -- EVERYTHING WORKS FINE -------
  // ------- MESSAGE QUEUES PRIMITIVES TESTING -- EVERYTHING WORKS FINE -------
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
  // ------- MESSAGE QUEUES PRIMITIVES TESTING -- EVERYTHING WORKS FINE ------- 
  */



  /* ------- MESSAGE QUEUES SYSCALLS BASIC TESTING -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS BASIC TESTING -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS BASIC TESTING -- EVERYTHING WORKS FINE ------- 
  int fd; int ret; char* msg_read;
  printf("Testing mq_open: creating message queue with 0 as id\n");
  fd = disastrOS_mq_open(0, DSOS_MQ, DSOS_CREATE);
  if (fd < 0)
    printf("Error in creating message queue: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_open: creating message queue with 1 as id\n");
  fd = disastrOS_mq_open(1, DSOS_MQ, DSOS_CREATE);
  if (fd < 0)
    printf("Error in creating message queue: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");


  printf("Testing mq_send: trying to send a message to the message queue with 0 as fd\n");
  ret = disastrOS_mq_send(0, "Hello");
  if (ret < 0)
    printf("Error in sending message: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_send: trying to send a message to the message queue with 0 as fd\n");
  ret = disastrOS_mq_send(0, "How are you");
  if (ret < 0)
    printf("Error in sending message: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_send: trying to send a message to the message queue with 0 as fd\n");
  ret = disastrOS_mq_send(0, "I'm fine thanks");
  if (ret < 0)
    printf("Error in sending message: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");


  printf("Testing mq_send: trying to send a message to the message queue with 1 as fd\n");
  ret = disastrOS_mq_send(1, "Hello");
  if (ret < 0)
    printf("Error in sending message: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_send: trying to send a message to the message queue with 1 as fd\n");
  ret = disastrOS_mq_send(1, "How are you");
  if (ret < 0)
    printf("Error in sending message: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_send: trying to send a message to the message queue with 1 as fd\n");
  ret = disastrOS_mq_send(1, "I'm fine thanks");
  if (ret < 0)
    printf("Error in sending message: %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");


  printf("Testing mq_receive: trying to receive a message from the message queue with 0 as fd\n");
  ret = disastrOS_mq_receive(fd, msg_read);
  if (ret < 0)
    printf("Error in receiving message: %d\n", fd);
  else {
    msg_read = (char*) running->syscall_args[1];
    printf("I received this message: %s\n", msg_read);
  }
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_receive: trying to receive a message from the message queue with 0 as fd\n");
  msg_read = 0;
  ret = disastrOS_mq_receive(fd, msg_read);
  if (ret < 0)
    printf("Error in receiving message: %d\n", fd);
  else{
    msg_read = (char*) running->syscall_args[1];
    printf("I received this message: %s\n", msg_read);
  } 
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_close: closing message queue file descriptor with 0 as id\n");
  ret = disastrOS_mq_close(fd);
  if (ret == 0)
    printf("Succesfully closed message queue file descript %d\n", fd);
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");

  printf("Testing mq_unlink: unlinking message queue with 0 as id\n");
  ret = disastrOS_mq_unlink(0);
  if (ret == 0)
    printf("Succesfully unlinked message queue with id 0\n");
  disastrOS_printStatus();
  printf("-------------------------------------------------------------------\n");
  
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
  */



  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 
  int P = 5; // number of processes to spawn
  int N = 7; // number of message queues to create
  int M = 5; // number of messages to send per message queues
  int fd; int ret; int i; int j;
  printf("******************************************************************\n");
  printf("Testing creation of multiple message queues by the same process (thread in our case):\n");
  printf("we will create %d message queues and spawn %d processes which are going to open 1 message queue each;\n",N,P);
  printf("the id of that message queue will be the same as the child pid (we will handle errors in case there's no message queue with such id.\n");
  printf("The main process will send %d messages through each of the %d message queues;\n", M, N);
  printf("it will then close every file descriptor for the message queues it opened.\n");
  printf("Children will then receive the %d messages; they will close every file descriptor for the message queues they opened.\n", N);
  printf("Last but not least, the main process, after waiting for every child, will terminate every message queue by unlinking them.\n");
  printf("******************************************************************\n\n");
  disastrOS_printStatus();

  int type=DSOS_MQ;
  int mode=DSOS_CREATE;
  char buf[20][20][30];

  // populating our matrix of strings
  for (i = 0; i < 19; i++){
    for(j = 0; j < 19; j++){
      sprintf(buf[i][j], "msg%d,%d", i, j);
      //printf("%s\n",buf[i][j]);
    }
  }

  printf("[Main process]: I'm gonna create %d message queues.\n", N);
  for (i = 0; i < N; i++) {
    printf("[Main process]: Creating message queue id:%d\n", i);
    fd=disastrOS_mq_open(i,type,mode);
    if (fd < 0)
      printf("[Main process]: Error in creating message queue: %d\n", fd);

    printf("[Main process]: I'm now gonna populate message queue id %d with %d messages (by sending them through the message queue).\n", i, M);
    for (j = 0; j < M; j++){
      ret = disastrOS_mq_send(i, buf[i][j]);
      if (ret < 0)
        printf("[Main process]: Error in sending message: %d.\n", ret);
    } 
  } disastrOS_printStatus();
  printf("******************************************************************\n\n");


  printf("[Main process]: I'm gonna close every fd for the message queues I openend.\n");
  for (i = 0; i < N; i++){
    printf("[Main process]: Closing file descriptor %d.\n", i);
    ret = disastrOS_mq_close(i);
    if (ret < 0) 
      printf("[Main process]: Error in closing file descriptor %d: error number %d", i, ret);
  } disastrOS_printStatus();
  printf("******************************************************************\n\n");


  printf("[Main process]: I'm gonna create %d new processes\n", P);
  int alive_children=0;
  for(i = 0; i < P; i++){
    printf("[Main process]: Process #%d is gonna spawn \n", last_pid);
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  } disastrOS_printStatus(); 
  printf("******************************************************************\n\n");

  
  printf("[Main process]: I'm gonna wait for my children to finish their job\n");
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("[Main process]: child %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  } disastrOS_printStatus();
  printf("******************************************************************\n\n");


  printf("[Main process]: I'm gonna unlink every message queue\n");
  for (i = 0; i < N; i++){
    printf("[Main process]: unlinking message queue id %d\n", i);
    int ret=disastrOS_mq_unlink(i);
    if (ret < 0) 
      printf("[Main process]: Error in unlinking message queue id %d; error number:%d\n", i,ret); 
  } disastrOS_printStatus();
  printf("******************************************************************\n\n");
  printf("******************************************************************\n\n");
  printf("******************************************************************\n\n\n");
  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 
  // ------- MESSAGE QUEUES SYSCALLS MORE ADVANCED TESTING -- EVERYTHING WORKS FINE ------- 
  

  // ------- MESSAGE QUEUES SYSCALLS BLOCKING TEST -- KINDA WORKS ------- 
  // ------- MESSAGE QUEUES SYSCALLS BLOCKING TEST -- KINDA WORKS ------- 
  int M1 = 13; // number of messages to send
  printf("******************************************************************\n");
  printf("In this section we will test the blocking aspect of the message queue: \n");
  printf("The main process is going to create 1 message queue;\n");
  printf("it will then spawn 1 process which will open that message queue.\n");
  printf("The main process will populate the message queue by using mq_send and will exceed the maximum number of messages allowed per queue\n");
  printf("to test the block when we send the messages\n");
  printf("The child process will then receive every single one of those messages using the mq_receive syscall, so this means that \n");
  printf("the child process will block too, waiting for the main process to resume its send \n");
  printf("******************************************************************\n\n");

  printf("[Main process]: Creating message queue id:%d\n", 0);
  fd=disastrOS_mq_open(0,type,mode);
  if (fd < 0)
    printf("[Main process]: Error in creating message queue: %d\n", fd);
  disastrOS_printStatus(); printf("******************************************************************\n\n");

  alive_children=0;
  printf("[Main process]: Process #%d is gonna spawn (this process is going to receive the messages from the queue)\n", last_pid);
  disastrOS_spawn(childFunctionRecv, 0);
  alive_children++;
  disastrOS_printStatus(); printf("******************************************************************\n\n");

  printf("[Main process]: I'm now gonna populate message queue id %d with %d messages (by sending them through the message queue).\n", 0, M1);
  for (i = 0; i < M1; i++){
    ret = disastrOS_mq_send(fd, buf[0][i]);
    if (ret < 0){
      printf("[Main process]: Error in sending message: %d.\n", ret);
      if (ret == DSOS_MQ_FULL){
        printf("[Main process]: let's sleep for a bit so that another process can empty the queue :)\n");
        disastrOS_sleep(SLEEP);
      }
    }
    else printf("[Main process]: this is the message that I just sent throught the queue: %s\n", buf[0][i]);
  }disastrOS_printStatus(); 
  printf("******************************************************************\n\n");

  printf("[Main process]: I'm gonna wait for my children to finish their job\n");
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    printf("[Main process]: child %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  } disastrOS_printStatus();
  printf("******************************************************************\n\n");

  printf("[Main process]: Closing file descriptor %d for the queue.\n", fd);
  ret = disastrOS_mq_close(fd);
  if (ret < 0) 
    printf("[Main process]: Error in closing file descriptor %d: error number %d", fd, ret);
  disastrOS_printStatus(); printf("******************************************************************\n\n");

  printf("[Main process]: unlinking message queue id %d\n", 0);
  ret=disastrOS_mq_unlink(0);
  if (ret < 0) 
    printf("[Main process]: Error in unlinking message queue id %d; error number:%d\n", 0, ret); 
  disastrOS_printStatus(); printf("******************************************************************\n\n");
                           printf("******************************************************************\n\n");


  // ------- MESSAGE QUEUES SYSCALLS BLOCKING TEST -- KINDA WORKS ------- 
  // ------- MESSAGE QUEUES SYSCALLS BLOCKING TEST -- KINDA WORKS ------- 

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
