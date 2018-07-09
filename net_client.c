#include "net_client.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#include "log.h"

client_t* client_create(int PORT, const char* HOST) {
	int m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == m_sock)
		return NULL;
	int on;
	while(1) {
		if(-1 == setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on))) {
			log_err("Could not create server socket(%d). errno(%x)", m_sock, errno);
			break;
		}
		struct sockaddr_in m_addr;
		memset(&m_addr, 0, sizeof(m_addr));
		m_addr.sin_family = AF_INET;
		m_addr.sin_port = htons(PORT);
		inet_pton(AF_INET, HOST, &m_addr.sin_addr);
		if(errno == EAFNOSUPPORT) {
			log_err("Could not bind socket(%d) to port. errno(%x)", m_sock, errno);
			break;
		}
		if(-1 == connect(m_sock, (struct sockaddr*) & m_addr, sizeof(m_addr))) {
			log_err("Could not connect to socket(%d). errno(%x)", m_sock, errno);
			break;
		}
		int opts = fcntl(m_sock, F_GETFL);
		if(opts < 0) {
			log_err("Could not get socket(%d) opts. errno(%x)", m_sock, errno);
			break;
		}
		opts |= O_NONBLOCK;
		if(-1 == fcntl(m_sock, F_SETFL, opts)) {
			log_err("Could not set socket(%d) to non-blocking. errno(%x)", m_sock, errno);
			break;
		}
		client_t* nc = malloc(sizeof(client_t));
		memset(nc, 0, sizeof(client_t));
		nc->p.fd = m_sock;
		nc->p.events = POLLIN;
		nc->status = RUNNING;
		return nc;
	}
	close(m_sock);
	return NULL;
}

int client_send(client_t* which, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int r = socket_sendv(which->p.fd, fmt, args);
	va_end(args);
	return r;
}

list_t* client_read(client_t* which) {
	if(which->status != RUNNING) return NULL;
	int r = poll(&(which->p), 1, 0);
	if(r < 0) {
		log_err("Client thread crashed. errno(%x)", errno);
		which->status = FAILED;
		return NULL;
	}
	if(r == 0) return NULL;
	if(which->p.fd == -1) {
		log_err("No socket is opened for server. errno(%x)", errno);
		which->status = FAILED;
		return NULL;
	}
	if(which->p.revents == 0) return NULL;
	if(!(which->p.revents & POLLIN)) {
		log_err("Unknown events was returned from server socket: %x. errno(%x)", which->p.revents, errno);
	} else {
		char lineptr[MAX_RECV + 1];
		r = recv(which->p.fd, lineptr, MAX_RECV, 0);
		if(r < 0) {
			if(errno != EWOULDBLOCK) {
				log_err("Couldn't recv from server. errno(%x)", errno);
				which->status = FAILED;
			}
		} else if(r == 0) {
			log_msg("Connection was closed by server.");
			which->status = CLOSED;
			return list_queue(NULL, mesg_build(which->p.fd, "%c", NET_PACKET_TYPE_CLOSED));
		} else {
			log_msg("Recieved %d bytes: '%.*s'.", r, r, lineptr);
			list_t* rlist = NULL;
			which->buffer = mesg_parse(which->p.fd, lineptr, r, which->buffer, &rlist);
			return rlist;
		}
	}
	return NULL;
}

void client_delete(client_t* which) {
	close(which->p.fd);
	if(which->buffer) free(which->buffer);
	free(which);
}