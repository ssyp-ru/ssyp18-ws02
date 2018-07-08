#pragma once
#include "net.h"

typedef struct {
	struct pollfd p;
	int status;
	msg_t* buffer;
} client;


client* client_create(int PORT, const char* HOST);
int client_send(client* which, const char* fmt, ...);
list* client_read(client* which);

void client_delete(client* which);
