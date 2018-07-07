#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<time.h>
#include<sys/time.h>



#define CHAR_BEGIN "["
#define CHAR_END "]"
#define PORT 696
#define MAX_MSG 128



typedef struct list_t{
	void * data;
	struct list_t * next;
} list;

typedef struct msg{
	int fd;
	int len;
	char str[MAX_MSG + 1];
} msg_t;

typedef struct server_t{
	int * fds;
	int fd_count;
	int status;
	list * recycled;
	msg_t ** buffers;
} server;



int list_len(list_t * head){
	int len = 0;
	list * curr;
	for(curr = head; curr; len++, curr = curr->next);
	return len;
}

list * list_prepend(list * head, void * data){
	list * curr = malloc(sizeof(list));
	curr->next = head;
	curr->data = data;
	return curr;
}

list * list_delete(list * head){
	
	list * curr = head->next;
	free(head);
	return curr;
}

list * list_index(list * head, int num){
	int i = 0;
	list * curr;
	for(curr = head; i < num; i++, curr = curr->next);
	return curr;
}

list * list_prepend_index(list * duff, void * data, int num){
	if(num == 0)
		return list_prepend(duff, data);
	list * temp = list_index(duff, num);
	if(!temp)
		return duff;
	temp->next = list_prepend(temp->next, data);
	return duff;
}

list * list_delete_index(list * duff, int num){
	if(num == 0)
		return list_delete(duff);
	list * temp = list_index(duff, num);
	if(!temp)
		return duff;
	list * curr = temp->next;
	if(!curr)
		return duff;
	temp->next = curr->next;
	free(curr);
	return duff;
}

void list_delete_all(list * head){
	while(head) {
		list* t = head->next;
		free(head);
		head = t;
	}	
}

int set_nonblock(int fd){
	int flags;
#if define(O_NONBLOCK)
	if(-1 == (flags = fcntl(fd, FD_GETFL, 0)))
		flags = 0;
	return fcntl(fd, FD_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIONBIO, &flags);
#endif
}
msg_t * mesg_empty() {
	msg_t * new_msg = malloc(sizeof(msg_t));
	new_msg->len = 0;
	new_msg->fd = -1;
	return new_msg;
}

msg_t * mesg_parse(int fd, const char * Recv, int len, msg_t * buf, list ** list_ref){
	int i = 0;
	for(i = 0; i < len; i++){
		switch (Recv[i]) {
			case CHAR_BEGIN:
				buf = mesg_empty();
			break;
			case CHAR_END:
				*list_ref = list_prepend(*list_ref, buf);
			break;
			default:
				if(buf != NULL)
					buf->str[buf->len++] = Recv[i];
			break;
		}
	}
	return buf;
}

server* server_create(uint16_t port){
	int fd = socket(AF_INET, 
			SOCK_STREAM, 
			IPPROTO_TCP);
	if(fd < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(fd, (struct sockaddr *)&name, sizeof(name)) < 0){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	set_nonblock(fd);
	if(listen(fd, SOMAXCONN)){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	server_t * new_server = malloc(sizeof(server));
	new_server->fds = calloc(MAX_CONNECTIONS, sizeof(int));
	new_server->fds[0] = fd;
	new_server->status = 0;
	return new_server;
}

list * server_read(server * new_server) {
	list * curr = NULL;
	fd_set active_fd_set, read_fd_set;
	FD_ZERO(&active_fd_set);
	struct sockaddr_in clientname;
	size_t size;
	int i;
	read_fd_set = active_fd_set;
	for(i = 0; i < FD_SETSIZE; i++){
		FD_SET(new_server->fds[i], &active_fd_set);
	}

	if(select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0){
		perror("select");
		exit(EXIT_FAILURE);
	}
	for(i = 1; i < FD_SETSIZE, i++){
		if(FD_ISSET(new_server->fds[0], &active_fd_set) && fd_count){
			size = sizeof(clientname);
			int new = accept(new_server->fds[0], 
					(struct sockaddr *)&clientname, &size);
			if(new < 0){
				perror("accept");
				exit(EXIT_FAILURE);	
				printf("Server: connect from host %s, port %d\n", 
					inet_ntoa(clientname.sin_addr), ntohs(clientname.sin_port));
				if(new_server->recycled){
					i = new_server->recycled->data;
					new_server->recycled = list_delete(new_server->recycled);
				}	
				else{
					i = new_server->fd_count++;
				}
				new_server->fds[i] = new;
			if(new_server->buffers[i]) 
				free(new_server->buffers[i];
			new_server->buffers[i] = NULL;
			}
		}
	}
	for(i = 1; i < FD_SETSIZE; i++){
		int RecvSize = recv(new_server->fds[0], buffer, MAX_MSG, MAX_MSG);
		if((RecvSize <= 0) && (errno != EAGAIN)){
			new_server->recycled = list_prepend(new_server->recycled, i);
			shutdown(new_server->fds[i], SHUT_RDWR);
			close(new_server->fds[i]);
			new_server->status = i;
		}
		else {
			new_server->buffers[i] = mesg_parse(new_server->fds[i], buffer, RecvSize,
				 																	new_server->buffers[i], &curr);
		}
	}
		return curr;
}

int server_broadcast(server * new_server, msg_t * mess){
	int i = 0;
	int j = 0;
	for(i = 0; i < new_server->fd_count; i++){
		int nbytes = send(new_server->fds[i], mess->str, mess->len, MAX_MSG);
		if(nbytes >= 0)
			j++;
	}
	return j;
}

void server_delete(server * new_server){
	int i = 0;
	for(i = 0; i < new_server->fd_count; i++){
		shutdown(new_server->fds[i], SHUT_RDWR);
		close(new_server->fds[i]);
		if(new_server->buffers[i])
			free(new_server->buffers[i]);
	}
	list_delete_all(new_server->recycled);
}



int main(){
	int servachock = server_create(PORT);
	if(servachock < 0)
		return 0;
	while(1){
		list * mesg = server_read(servachock);
		server_broadcast(servachock, mesg);
	}
	server_delete(servachock);
	return 0;
}
