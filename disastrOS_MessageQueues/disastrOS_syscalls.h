#pragma once
#include <assert.h>
#include "disastrOS.h"
#include "disastrOS_globals.h"

void internal_preempt();

void internal_fork();

void internal_exit();

void internal_wait();

void internal_spawn();

void internal_shutdown();

void internal_schedule();

void internal_sleep();

void internal_openResource();

void internal_closeResource();

void internal_destroyResource();

void internal_mqOpen();

void internal_mqClose();

void internal_mqUnlink();

void internal_mqSend();

void internal_mqReceive();