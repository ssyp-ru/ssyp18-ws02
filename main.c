/*#include <ncurses.h>

#include "net_server.h"
#include "log.h"

server* serv;

void init(int delay);
void mainLoop(int(*main)(int), int delay);
void deinit();

void onMsg(msg_t* msg) {
	log_msg("recv: %*s", msg->len, msg->str);
	server_broadcast(serv, msg);
	mesg_throw(msg);
}

int Main(int ev) {
	{
		list* l = server_read(serv);
		list_map(l, onMsg);
		list_delete(l);
	}
	switch(ev) {
		case KEY_F(2):
			return 1;
		break;
	}
	return 0;
}

int main() {
	serv = server_create(30000);
	if(serv != NULL) {
		log_msg("SERVER ONLINE");
		init(10);
		mainLoop(Main, 10);
		deinit();
	}
	log_msg("exit.");
	return 0;
}

void init(int delay) {
	initscr();
	cbreak();
	raw();
	noecho();
	curs_set(false);
	intrflush(stdscr, FALSE);
	keypad(stdscr, true);
	timeout(delay);
}

void deinit() {
	endwin();
}

void mainLoop(int(*main)(int), int delay) {
	while(!main(getch()));
}*/
