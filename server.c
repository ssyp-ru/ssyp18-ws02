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

#define PORT 5555
#define MAXMSG 512

int read_from_client(int filedes){
	char buffer[MAXMSG];
	int nbytes;
	nbytes = read(filedes, buffer, MAXMSG);
	if(nbytes < 0){
		perror("read");
		exit(EXIT_FAILURE);
	}
	else if(nbytes == 0)
		return -1;
	else{
		printf("Server: got message:  '%s'\n", buffer);
		return 0;
	}
}

int main(){
	int flags = MSG_NOSIGNAL;
	int domain = AF_INET;
	int type = SOCK_STREAM;
	int protocol = IPPROTO_TCP;
	int sock = socket(domain, type, protocol);
	fd_set active_fd_set, read_fd_set;
	int i;
	struct sockaddr_in clientname;
	size_t size;
	if(listen(sock, 1) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	FD_ZERO(&active_fd_set);
	FD_SET(sock, &active_fd_set);
	while(1){
		read_fd_set = active_fd_set;
		if(select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0){
			perror("select");
			exit(EXIT_FAILURE);
		}
		for(i = 0; i < FD_SETSIZE; i++)
			if(FD_ISSET(i, &read_fd_set)){
				if(i == sock){
					int new;
					size = sizeof(clientname);
					new = accept(sock, (struct sockaddr *)&clientname, &size);
					if(new < 0){
						perror("accept");
						exit(EXIT_FAILURE);
					}
					printf("Server: connect from host %s, port %hd.\n",
							inet_ntoa(clientname.sin_addr), ntohs(clientname.sin_port));
					FD_SET(new, &active_fd_set);
				}
				else{
					if(read_from_client(i) < 0){
						close(i);
						FD_CLR(i, &active_fd_set);
					}
				}
			}
		}
	return 0;
}
