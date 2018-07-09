#include "common.h"
#include "GUI.h"
#include "net_client.h"
#include <stdio.h>
#include <ncurses.h>
#include "net.h"
#include "game.h"
#include "single.h"

int doExit = FALSE;

void foo(packet_t* msg) {
	switch(msg->str[0]) {
		case 'c':
			doExit = TRUE;
			break;
		default:
			printw("recv %d: %*s", msg->len, msg->len, msg->str);
			break;
	}
	free(msg);
}

int main(int argc, char * argv[]){
	if(argc < 2) {
		printf("No IP was passed!\n");
		return 0;
	}
	int port = 30000;
	client_t * client = client_create(port, argv[1]);
	if(client == NULL) {
		printf("Could not connect to %s!\n", argv[1]);
		return 0;
	}
	initscr();
	noecho();
	int input;
	curs_set(0);
	timeout(20);
	keypad(stdscr, true);
	//start(client);
	socket_send(client->p.fd, "Misha loh 1", 11);
	socket_send(client->p.fd, "Misha loh 2", 11);
	socket_send(client->p.fd, "Misha lox 3", 11);
	printf("Reading from server...\n");
	while(!doExit){
		input = getch();
		if(input == KEY_F(2))
			break;
		if(input > 0)
			socket_sendf(client->p.fd,"%c", input);
		list_t * list = client_read(client);
		list_map(list, foo);
		list_delete(list);
	}
	client_delete(client);
	printw("Done!\n");
	endwin();
	return 0;
}
