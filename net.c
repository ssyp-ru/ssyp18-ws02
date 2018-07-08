#include "net.h"

#include <stdio.h>
#include <arpa/inet.h>

#include "log.h"

int socket_send(int fd, char* str, int len) {
	log_msg("Sending %d bytes to %d: '%.*s'.", len, fd, len, str);
	char temp = NET_CHAR_BEGIN;
	str[len++] = NET_CHAR_TERMINATE;
	if(send(fd, &temp, 1, MSG_NOSIGNAL) == -1) return -1;
	if(send(fd, str, len, MSG_NOSIGNAL) == -1) return -1;
	return 0;
}

int socket_sendv(int fd, const char* str, va_list args) {
	char buf[MAX_RECV + 3];
	int len = vsnprintf(buf, MAX_RECV, str, args);
	return socket_send(fd, buf, len);
}

int socket_sendf(int fd, const char* str, ...) {
	va_list args;
	va_start(args, str);
	int r = socket_sendv(fd, str, args);
	va_end(args);
	return r;
}

msg_t* mesg_create(int fd, const char* str, int len) {
	msg_t* new_msg = malloc(sizeof(msg_t));
	memset(new_msg->str + len, 0, (MAX_RECV - len)*sizeof(char));
	strncpy(new_msg->str, str, len);
	new_msg->fd = fd;
	new_msg->len = len;
	return new_msg;
}

msg_t* mesg_build(int fd, const char* str, ...) {
	msg_t* new_msg = malloc(sizeof(msg_t));
    va_list args;
	va_start(args, str);
	new_msg->len = vsnprintf(new_msg->str, MAX_RECV, str, args);
	va_end(args);
	new_msg->fd = fd;
	return new_msg;
}

msg_t* mesg_empty(int fd) {
	msg_t* new_msg = malloc(sizeof(msg_t));
	memset(new_msg, 0, sizeof(msg_t));
	new_msg->fd = fd;
	return new_msg;
}

int mesg_send(msg_t* what) {
	return socket_send(what->fd, what->str, what->len);
}

int mesg_throw(msg_t* what) {
	int r = socket_send(what->fd, what->str, what->len);
	free(what);
	return r;
}

int mesg_redirect(int fd, msg_t* what) {
	return socket_send(fd, what->str, what->len);
}

int mesg_rethrow(int fd, msg_t* what) {
	int r = socket_send(fd, what->str, what->len);
	free(what);
	return r;
}

msg_t* mesg_parse(int fd, const char* recieved, int len, msg_t* buffer_msg, list** list_ref) {
	for(int t = 0; t < len; t++) {
		switch(recieved[t]) {
			case NET_CHAR_BEGIN:
				if(buffer_msg != NULL) {
					log_err("recv#%d - There's a recieving message already!", t);
					free(buffer_msg);
				}
				buffer_msg = mesg_empty(fd);
			break;
			case NET_CHAR_TERMINATE:
				if(buffer_msg == NULL) {
					log_err("recv#%d - There's no recieving message yet!", t);
				} else {
					(*list_ref) = list_append(*list_ref, buffer_msg);
					buffer_msg = NULL;
				}
			break;
			default:
				if(buffer_msg == NULL)
					log_err("recv#%d - There's no recieving message yet!", t);
				else
					if(buffer_msg->len < MAX_RECV)
						buffer_msg->str[buffer_msg->len++] = recieved[t];
					else
						log_err("recv#%d - The recieving message is too big!", t);
			break;
		}
	}
	return buffer_msg;
}