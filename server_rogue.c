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

#define PORT 5555
#define MAXMSG 512

int make_socket(uint16_t port){
	int server_sock;
	struct sockaddr_in name;
	int yes = 1;
	server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(server_sock < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1){
		perror("setsockopt");
		exit(1);
	}
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(server_sock, (struct sockaddr *)&name, sizeof(name)) < 0){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	return server_sock;
}

int read_from_client(int filedes){
	char buffer[MAXMSG];
	int nbytes;
	nbytes = recv(filedes, buffer, 512, MAXMSG);
	if(nbytes < 0){
		perror("read");
		exit(EXIT_FAILURE);
	}
	else if(nbytes == 0)
		return -1;
	else{
		printf("Server: got message: %s\n", buffer);
		return 0;
	}
}

int main(void){
	int server_sock;
	fd_set active_fd_set, read_fd_set;
	int fd;
	struct sockaddr_in clientname;
	size_t size;
	server_sock = make_socket(PORT);
	if(listen(server_sock, SOMAXCONN) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	FD_ZERO(&active_fd_set);
	FD_SET(server_sock, &active_fd_set);
	while(1){
		read_fd_set = active_fd_set;
		if(select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0){
			perror("select");
			exit(EXIT_FAILURE);
		}
		for(fd = 0; fd < FD_SETSIZE; fd++){
			if(FD_ISSET(fd, &read_fd_set)){
				if(fd == server_sock){
					int new;
					size = sizeof(clientname);
					new = accept(server_sock, (struct sockaddr *)&clientname, &size);
					if(new < 0){
						perror("accept");
						exit(EXIT_FAILURE);
					}
					printf("Server: connect from host %s, port %d\n",
												inet_ntoa(clientname.sin_addr), ntohs(clientname.sin_port));
					FD_SET(new, &active_fd_set);
				}
				else{
					if(read_from_client(fd) < 0){
						FD_CLR(fd, &active_fd_set);
						shutdown(fd, SHUT_RDWR);
						close(fd);
					}
				}
			}
		}
	}
}
