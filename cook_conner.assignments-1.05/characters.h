#ifndef CHARACTERS_h
# define CHARACTERS_h
#include "terrain.h"
#include "heap.h"

typedef struct map map_t;

enum symbols{
	hiker = 'h',
	rival = 'r',
	pacer = 'p',
	wanderer = 'w',
	stationary = 's',
	random_walker = 'n',
	player = '@'
};

enum direction{
	west,
	east,
	north,
	south,
	NaN
};

typedef struct characters
{
	char symbol;
	int movement_cost;
	int direction;
	int x;
	int y;
	int new_x;
	int new_y;
	int spawned_in;
	int defeated;
}character_t;

void populate_character_map(map_t* map, int n);
character_t* generate_player(map_t* map);
void move_character(character_t* c, map_t* map, int new_x, int new_y);
void determine_next_move(character_t* c, map_t* tmap, character_t* player);
int character_compare(const void *key, const void *with);
int move_pc(character_t* c, map_t* map);
void insert_characters(map_t* map, heap_t* h);
void set_movement_0(map_t* map);

#endif