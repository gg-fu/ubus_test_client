#ifndef _UBUS_CLIENT_H
#define _UBUS_CLIEN_H

#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "libubus.h"
#include "blobmsg_json.h"

int Send_user_message();
void Bru_create_pthread(void *(*func)(void *),void *arg);
void init_ubus(void);


#endif


