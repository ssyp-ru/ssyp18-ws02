#include "common.h"
#include "GUI.h"
#include "net_client.h"
#include <stdio.h>
#include <ncurses.h>
#include "net.h"
#include "game.h"
#include "single.h"
#include "networking.h"

int doExit = FALSE;
map_t * _map;
void foo(packet_t* msg) {
	
	switch(msg->str[0]) {
		case 'u':
			int overhead[4] = mesg_deserialize(msg, sizeof(int)*4);
			int size = overhead[2]*overhead[3]*sizeof(tile_t) + sizeof(int)*4;
			update_map_t* chunk = mesg_deserialize(msg, size);
			client_map(_map, chunk);
			break;
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
	_map = create_map(50, 50);
	initGUI();
	int port = 30000;
	client_t * client = client_create(port, argv[1]);
	if(client == NULL) {
		printf("Could not connect to %s!\n", argv[1]);
		return 0;
	}
	UNPACK(map, _map);
	initscr();
	noecho();
	int input;
	packet_t * chunk;
	curs_set(0);
	timeout(20);
	keypad(stdscr, true);
	levels_vt* levels = lvector_init(1);
	lvector_add(levels, level);
	//start(client);
	socket_send(client->p.fd, "Misha loh 1", 11);
	socket_send(client->p.fd, "Misha loh 2", 11);
	socket_send(client->p.fd, "Misha lox 3", 11);
	printf("Reading from server...\n");
	while(!doExit){
		for(int i = 0; i < 50; i++){
			for(int k = 0; k < 50; k++){
				mvaddch(i, k, map[i][k].symbol);
			}
		}
		input = getch();
		if(input == KEY_F(2))
			break;
		if(input > 0)
			socket_sendf(client->p.fd,"%c", input);
		render(actors->data[0], actors, msgs);
		list_t * list = client_read(client);
		list_map(list, foo);
		list_delete(list);
	}
	client_delete(client);
	printw("Done!\n");
	endwin();
	return 0;
}
