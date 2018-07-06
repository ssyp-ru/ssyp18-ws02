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
#define MAXMSG 512



typedef struct list_t{
	int data;
	struct list_t * next;
} list;

typedef struct msg{
	int fd;//socket
	int len;//len message
	char str[MAX_MSG + 1];//message
} msg_t;

typedef struct server_t{
	int * fds;//new socket
	int fd_count;//sizeof socket
	int status;//status socket
	list * resycled;//size closed sockets
	msg_t ** buffer;//all messages
} server;



int list_len(list * head){
	int len = 0;
	list * curr;
	for(curr = head; curr; len++, curr = curr->next);
	return len;
}

list * list_prepend(list * head, int data){
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

list * list_prepend_index(list * duff, int data, int num){
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
	list * curr;
	for(curr = head; curr; curr = curr->next)
		free(curr);
}+

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

server* server_create(uint16_t port){
	server fd = socket(AF_INET, 
			SOCK_STREAM, 
			IPPROTO_TCP);
	if(fd < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	/*if(setsockopt(fd, 
				SOL_SOCKET, 
				SO_REUSEADDR, 
				&yes, 
				sizeof(yes)) == -1){
		perror("setsockopt");
		exit(1);
	}*/
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
	server * new_server = malloc(sizeof(server));
	new_server->fds = calloc(MAX_CONNECTIONS, sizeof(int));
	new_server->fds[0] = fd;
	new_server->status = 0;
	return new_server;
}

list * server_read(server * sock){
	server * new_server;
	fd_set active_fd_set, read_fd_set;
	FD_ZERO(&active_fd_set);
	FD_SET(new_server->fds[0], &active_fd_set);
	struct sockaddr_in clientname;
	size_t size;
	int i;
	read_fd_set = active_fd_set;
	for(i = 1; i < FD_SETSIZE; i++){
		FD_SET(new_server->fds[i], &active_fd_set)
	}
	/*	if(FD_ISSET(i, &read_fd_set)){
			if(i == new_server->fds[i]){
				size = sizeof(clientname);
				new_server->fds[i] = accept(new_server[0], 
						(struct sockaddr *)&clientname, &size);
				if(new_server->fds < 0){
					perror("accept");
					exit(EXIT_FAILURE);
				
				printf("Server: connect from host %s, port %d\n", 
						inet_ntoa(clientname.sin_addr), ntohs(clientname.sin_port));
				i = recv(new_server->fds[0], buffer, MAX_MSG, MAX_MSG);
				if(i < 0){*/

	if(select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0){
		perror("select");
		exit(EXIT_FAILURE);
	}
	for(i = 1; i < FD_SETSIZE, i++){
		if(FD_ISSET(new_server->fds[i], &active_fd_set)){
			if(i == new_server->fds[i]){
				size = sizeof(clientname);
				new_server->fds[i] = accept(new_server[0], 
						(struct sockaddr *)&clientname, &size);
				if(new_server->fds < 0){
					perror("accept");
					exit(EXIT_FAILURE);	
				printf("Server: connect from host %s, port %d\n", 
						inet_ntoa(clientname.sin_addr), ntohs(clientname.sin_port));
			msg_t buffer = ;
			int RecvSize = recv(new_server->fds[0], buffer, MAX_MSG, MAX_MSG);
			if((RecvSize == 0) && (errno != EAGAIN)){
				shutdown(new_server->fds[i], SHUT_RDWR);<F9><F9><F9><F9><F9>
				close(new_server->fds[i]);
				new_server->status = i;
			}
			else if(RecvSize != 0){
				printf("Server: connect from host")
				send(new_server->fds[i], buffer, RecvSize, MSG_MAX);
			}
		}	
	}
	if(FD_ISSET(new_server->fds[0], &active_fd_set)){
		  = accept(new_server->fds[0], 0, 0);
	}
}


