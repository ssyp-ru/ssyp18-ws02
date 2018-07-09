#pragma once
#include "net.h"

#define MAX_CONNECTIONS 32

typedef struct {
	struct pollfd* fds;
	int fd_count;
	list_t* recycled;
	int status;
	packet_t** buffers;
} server_t;

server_t* server_create(int PORT);
list_t* server_read(server_t* which);
int server_broadcast(server_t* which, packet_t* what);

void server_delete(server_t* which);