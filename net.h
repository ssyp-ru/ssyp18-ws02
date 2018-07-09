#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "datastruct.h"

#define MAX_RECV 2048

#define NET_CHAR_BEGIN '['
#define NET_CHAR_TERMINATE ']'

#define NET_PACKET_TYPE_OPENED 'o'
#define NET_PACKET_TYPE_CLOSED 'c'
#define NET_PACKET_TYPE_TEXT 't'

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
} packet_t;

packet_t* mesg_create(int fd, const char* str, int len);
packet_t* mesg_build(int fd, const char* str, ...);
packet_t* mesg_empty(int fd);

packet_t* mesg_serialize(int fd, void* what, int size);
void* mesg_deserialize(packet_t* msg, int size);

// sends packet_t into packet_t.fd
int mesg_send(packet_t* what);
int mesg_redirect(int fd, packet_t* what);

// Also frees the packet_t passed
int mesg_throw(packet_t* what);
int mesg_rethrow(int fd, packet_t* what);

packet_t* mesg_parse(int fd, const char* recieved, int len, packet_t* buffer_msg, list_t** list_ref);
