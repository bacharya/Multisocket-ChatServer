#ifndef __UTILS_H__
#define __UTILS_H__
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "inttypes.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "errno.h"
#include "string.h"
#include "sys/types.h"
#include "time.h"
#include "pthread.h"
#include "sys/epoll.h"
#include "fcntl.h"

#define SERVER_CONN_MAX 10
#define SERVER_BUFF_MAX 1024
#define SERVER_MAX_EVENTS 10

#define CLIENT_BUFF_MAX 1024

typedef enum return_code_ {
    RET_CODE_SUCCESS = 0,
    RET_CODE_GRACEFUL_EXIT,
    RET_CODE_FAILURE,
    RET_CODE_ERROR,
    RET_CODE_INVALID,
    RET_CODE_MAX,
} ret_code;

#endif
