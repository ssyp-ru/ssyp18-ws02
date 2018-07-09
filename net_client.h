#pragma once
#include "net.h"

typedef struct {
	struct pollfd p;
	int status;
	packet_t* buffer;
} client_t;


client_t* client_create(int PORT, const char * HOST);
int client_send(client_t* which, const char* fmt, ...);
list_t* client_read(client_t* which);

void client_delete(client_t* which);
