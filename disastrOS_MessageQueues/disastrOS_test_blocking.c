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

  printf("[Child%d]: I'm gonna receive %d messages (if possible ;) )from the queue using file descriptor %d\n",disastrOS_getpid(), M, fd);
  char* msg_read = 0;
  for(i = 0; i < M; i++){
    ret = disastrOS_mq_receive(fd, msg_read);
    if (ret < 0)
      printf("[Child%d]: Error in receiving message: %d\n", disastrOS_getpid(), ret);
    // else printf("[Child%d]: this is the message I received and I removed from the queue: %s\n", disastrOS_getpid(),(char*) running->syscall_args[1]);
  } disastrOS_printStatus(); printf("******************************************************************\n\n");

  printf("[Child%d]: Closing file descriptor %d for the queue.\n", disastrOS_getpid(), fd);
  ret = disastrOS_mq_close(fd);
  if (ret < 0) 
    printf("[Main process]: Error in closing file descriptor %d: error number %d", fd, ret);
  disastrOS_printStatus(); printf("******************************************************************\n\n");

  printf("[Child%d]: terminating\n", disastrOS_getpid());
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

  int fd; int ret;
  int M = 13; // number of messages to send per message queues
  int type=DSOS_MQ;
  int mode=DSOS_CREATE;
  char buf[20][20][30];
  int alive_children = 0;

  // populating our matrix of strings
  int i; int j;
  for (i = 0; i < 19; i++){
    for(j = 0; j < 19; j++){
      sprintf(buf[i][j], "msg%d,%d", i, j);
      //printf("%s\n",buf[i][j]);
    }
  }

  // ------- MESSAGE QUEUES SYSCALLS BLOCKING TEST -- KINDA WORKS ------- 
  // ------- MESSAGE QUEUES SYSCALLS BLOCKING TEST -- KINDA WORKS ------- 
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

  printf("[Main process]: I'm now gonna populate message queue id %d with %d messages (by sending them through the message queue) (if possibile ;) ).\n", 0, M);
  for (i = 0; i < M; i++){
    ret = disastrOS_mq_send(fd, buf[0][i]);
    if (ret < 0){
      printf("[Main process]: Error in sending message: %d.\n", ret);
    }
    //else printf("[Main process]: this is the message that I just sent through the queue: %s\n", buf[0][i]);
  }disastrOS_printStatus(); 
  printf("******************************************************************\n\n");

  int retval;
  int pid;
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
  printf("the function pointer is: %p", childFunctionRecv);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
