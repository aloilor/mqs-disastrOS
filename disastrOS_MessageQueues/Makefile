CC=gcc
CCOPTS=--std=gnu99 -Wall -g
AR=ar

HEADERS=disastrOS.h\
	disastrOS_constants.h\
	disastrOS_descriptor.h\
	disastrOS_globals.h\
	disastrOS_pcb.h\
	disastrOS_resource.h\
	disastrOS_syscalls.h\
	disastrOS_timer.h\
	linked_list.h\
	pool_allocator.h\
     disastrOS_message_queue.h\

OBJS=pool_allocator.o\
     linked_list.o\
     disastrOS_timer.o\
     disastrOS_pcb.o\
     disastrOS_resource.o\
     disastrOS_descriptor.o\
     disastrOS.o\
     disastrOS_fork.o\
     disastrOS_wait.o\
     disastrOS_spawn.o\
     disastrOS_exit.o\
     disastrOS_shutdown.o\
     disastrOS_schedule.o\
     disastrOS_preempt.o\
     disastrOS_sleep.o\
     disastrOS_open_resource.o\
     disastrOS_close_resource.o\
     disastrOS_destroy_resource.o\
     disastrOS_message_queue.o\
     disastrOS_mq_open.o\
     disastrOS_mq_close.o\
     disastrOS_mq_unlink.o\
     disastrOS_mq_send.o\
     disastrOS_mq_receive.o\

LIBS=libdisastrOS.a

BINS=disastrOS_test disastrOS_test_blocking

#disastros_test

.phony: clean all


all:	$(LIBS) $(BINS)

%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

libdisastrOS.a: $(OBJS) $(HEADERS) 
	$(AR) -rcs $@ $^
	$(RM) $(OBJS)

disastrOS_test:		disastrOS_test.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^

disastrOS_test_blocking:		disastrOS_test_blocking.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^

clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
