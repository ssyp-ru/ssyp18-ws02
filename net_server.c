#include "net_server.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#include "log.h"

server* server_create(int PORT) {
	int m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int on = 1;
	if(-1 == m_sock)
		return NULL;
	while(1) {
		if(-1 == setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on))) {
			log_err("Could not create listening socket(%d). errno(%x)", m_sock, errno);
			break;
		}
		struct sockaddr_in m_addr;
		memset(&m_addr, 0, sizeof(m_addr));
		m_addr.sin_family = AF_INET;
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		m_addr.sin_port = htons(PORT);
		if(-1 == bind(m_sock, (struct sockaddr*)&m_addr, sizeof(m_addr))) {
			log_err("Could not bind socket(%d) to port. errno(%x)", m_sock, errno);
			break;
		}
		if(-1 == listen(m_sock, MAX_CONNECTIONS)) {
			log_err("Could not listen socket(%d) to socket. errno(%x)", m_sock, errno);
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
		server* ns = malloc(sizeof(server));
		ns->fds = calloc(MAX_CONNECTIONS + 1, sizeof(struct pollfd));
		ns->buffers = calloc(MAX_CONNECTIONS + 1, sizeof(msg_t*));
		ns->recycled = NULL;
		ns->fd_count = 1;
		ns->fds[0].fd = m_sock;
		ns->fds[0].events = POLLIN;
		ns->status = RUNNING;
		return ns;
	}
	close(m_sock);
	return NULL;
}

int server_broadcast(server* which, msg_t* what) {
	if(which->status != RUNNING || what == NULL) return -1;
	int c = 0;
	for(int i = 1; i < which->fd_count; i++) if(which->fds[i].fd != -1 && which->fds[i].fd != what->fd) {
		if(-1 == mesg_redirect(which->fds[i].fd, what)) {
			log_err("Couldn't send '%s' to connection(%d) from (%d). errno(%x)", what->str, which->fds[i].fd, what->fd, errno);
		} else c++;
	}
	return c;
}

list* server_read(server* which) {
	if(which->status != RUNNING) return NULL;
	int r = poll(which->fds, which->fd_count, 0);
	if(r < 0) {
		log_err("Server poll error(%d). errno(%x)", r, errno);
		which->status = FAILED;
		return NULL;
	}
	if(r == 0) {
		return NULL;
	}
	list* rlist = NULL;
	struct pollfd* j = which->fds;
	if(j->fd != -1 && j->revents != 0) {
		if(!(j->revents & POLLIN)) {
			log_err("Unknown events was returned from Server socket(%d): %x. errno(%x)", j->fd, j->revents, errno);
		} else while(1) {
			struct sockaddr_in m_addr;
			int addr_len = sizeof(m_addr);
			int sock = accept(j->fd, (struct sockaddr*)&m_addr, (socklen_t*)&addr_len);
			if(sock < 0) {
				if(errno != EWOULDBLOCK) {
					log_err("Couldn't accept connection. errno(%x)", errno);
				}
				break;
			}
			int nIx;
			if(which->recycled != NULL) {
				nIx = (int)which->recycled->val;
				which->recycled = list_deque(which->recycled);
			} else {
				nIx = which->fd_count++;
			}
			log_msg("New connection(%d) was accepted with id(%d).", sock, nIx);
			which->fds[nIx].fd = sock;
			which->fds[nIx].events = POLLIN;
			rlist = list_append(rlist, mesg_build(sock, "%c", NET_MSG_TYPE_OPENED));
			mesg_throw(mesg_create(sock,"ping",4));
		}
	}

	for(int i = 1; i < which->fd_count; i++) {
		struct pollfd* j = &(which->fds[i]);
		if(j->fd == -1 || j->revents == 0) continue;
		int doClose = 0;
		if(!(j->revents & POLLIN)) {
			log_err("Unknown events was returned from connection(%d) socket(%d): %x. errno(%x)", i, j->fd, j->revents, errno);
			doClose = 1;
		} else {
			char buf[MAX_RECV + 3];
			memset(buf, 0, sizeof(buf));
			int r = recv(j->fd, buf, MAX_RECV, 0);
			if(r < 0) {
				if(errno != EWOULDBLOCK) {
					log_err("Couldn't recv from connection(%d). errno(%x)", j->fd, errno);
				}
			} else if(r == 0) {
				log_msg("Connection(%d) was closed by client.", j->fd);
				doClose = 1;
			} else {
				log_msg("Recieved %d bytes from (%d): '%*s'.", r, j->fd, r, buf);
				which->buffers[i] = mesg_parse(j->fd, buf, r, which->buffers[i], &rlist);
			}
		}
		if(doClose) {
			close(j->fd);
			rlist = list_append(rlist, mesg_build(j->fd, "%c", NET_MSG_TYPE_CLOSED));
			j->fd = -1;
			which->recycled = list_queue(which->recycled, (void*)i);
		}
	}
	return rlist;
}

void server_delete(server* which) {
	list_delete(which->recycled);
	for(int i = 0; i < which->fd_count; i++)
			if(which->fds[i].fd != -1)
				close(which->fds[i].fd);
	free(which->fds);
	free(which);
}