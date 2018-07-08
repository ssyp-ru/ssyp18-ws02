#pragma once
#include "net.h"

#define MAX_CONNECTIONS 32

typedef struct {
	struct pollfd* fds;
	int fd_count;
	list* recycled;
	int status;
	msg_t** buffers;
} server;

server* server_create(int PORT);
list* server_read(server* which);
int server_broadcast(server* which, msg_t* what);

void server_delete(server* which);