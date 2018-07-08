#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "datastruct.h"

#define MAX_RECV 256

#define NET_CHAR_BEGIN '['
#define NET_CHAR_TERMINATE ']'

#define NET_MSG_TYPE_OPENED 'o'
#define NET_MSG_TYPE_CLOSED 'c'
#define NET_MSG_TYPE_TEXT 't'

enum STATUS {
	EMPTY = 0,
	RUNNING,
	CLOSED,
	FAILED
};

int socket_send(int fd, char* str, int len);
int socket_sendv(int fd, const char* str, va_list args);
int socket_sendf(int fd, const char* str, ...);

typedef struct {
	int fd;
	int len;
	char str[MAX_RECV + 1];
} msg_t;

msg_t* mesg_create(int fd, const char* str, int len);
msg_t* mesg_build(int fd, const char* str, ...);
msg_t* mesg_empty(int fd);

// sends msg_t into msg_t.fd
int mesg_send(msg_t* what);
int mesg_redirect(int fd, msg_t* what);

// Also frees the msg_t passed
int mesg_throw(msg_t* what);
int mesg_rethrow(int fd, msg_t* what);

msg_t* mesg_parse(int fd, const char* recieved, int len, msg_t* buffer_msg, list** list_ref);
