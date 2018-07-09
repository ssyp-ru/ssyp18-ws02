#include <ncurses.h>
#include "actor.h"
#include <stdbool.h>
#include "common.h"
#include "levelvector.h"
#include "level.h"
#include "net_server.h"
#include "log.h"
#include "datastruct.h"
server_t *serv;

int it;

void init(int delay);
void mainLoop(int (*main)(void), int delay);
void deinit();

typedef struct {
	int fd;
	actor_t *hero;
} connection_t;

list_t *clients;

typedef struct gamestruct {
	levels_vt *levels;
	actors_vt *actors;
	actors_vt **queue;
} game_t;

game_t *game;

void onMsg(packet_t *msg) {
	actor_t *new_player;
	connection_t * client;
	list_t *clients_list;
	int n;
	switch (msg->str[0])
	{
	case NET_PACKET_TYPE_OPENED:
		new_player = init_player(lvector_get(game->levels, 0));
		client = (connection_t *)malloc(sizeof(connection_t));
		client->hero = new_player;
		client->fd = msg->fd;
		clients = list_append(clients, (actor_t*)client);

		add_vector_elem(game->actors, new_player);
		add_vector_elem(game->queue[(it) % 100], new_player);
		break;
	case NET_PACKET_TYPE_CLOSED:
		n = 0;
		clients_list = clients;
		client = (connection_t*) clients_list->val;
		while (client->fd != msg->fd)
		{
			clients_list = clients_list->next;
			client = (connection_t*) clients_list->val;
			n++;
		}

		client->hero->flags |= FLAG_DEAD;
		clients = list_remove(clients, n);
		break;
	case NET_PACKET_TYPE_TEXT:
		break;
	}
}

void destroy_the_world(size_t qlen) {
	lvector_free(game->levels);
	free_actors(game->actors, true);
	for (int i = 0; i < qlen; i++)
		free_actors(game->queue[i], false);
	free(game->queue);
}

int Main() {
	{
		list_t *l = server_read(serv);
		list_map(l, onMsg);
		list_delete(l);
	}

	size_t qlen = 100;
	render(actor_get(game->actors, 0), game->actors, NULL);
	//draw_level(lvector_get(game->levels, 0));
	//draw_actors(game->actors);
	//refresh();
	for (int i = 0; i < game->queue[it]->length; i++) {
		actor_t *current = actor_get(game->queue[it], i);
		if ((current->flags & FLAG_DEAD) != 0)
			continue;
		int new_index = (it + current->behave(current)) % qlen;
		if (new_index == it)
			return 1;
		add_vector_elem(game->queue[new_index], current);
	}
	game->queue[it]->length = 0;
	it++;
	it %= qlen;
	return 0;
}

int main() {
	serv = server_create(30000);
	if (serv != NULL) {
		time_t t;
		srand((unsigned)time(&t));
		it = 0;
		game = (game_t *)malloc(sizeof(game_t));
		game->levels = lvector_init(1);
		lvector_add(game->levels, init_level(60, 60));
		game->actors = init_actors(lvector_get(game->levels, 0), 0);
		game->queue = calloc(100, sizeof(actors_vt *));
		for (int i = 0; i < 100; i++)
			game->queue[i] = create_new_vector(1);

		clients = (list_t *)malloc(sizeof(list_t));
		clients->val = (actor_t*)actor_get(game->actors, 0);
		clients->next = NULL;

		for (int i = 0; i < game->actors->length; i++)
			add_vector_elem(game->queue[0], actor_get(game->actors, i));

		log_msg("SERVER ONLINE");
		init(0);
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
		
		mainLoop(Main, 0);
		destroy_the_world(100);
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
	endwin();
}

void mainLoop(int (*main)(void), int delay) {
	while (!main())
		;
}
