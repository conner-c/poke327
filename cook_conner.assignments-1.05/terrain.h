#ifndef TERRAIN_H
# define TERRAIN_H
#include <stdio.h>
#include "characters.h"	
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "heap.h"
#include <unistd.h>


typedef struct characters character_t;
// Different terrain types. Debug used to initialize map
// Values are used in the character movement 2 indicates valid spawn and movement location, 1 indicates not valid spawn
// but valid movement, and 0 indicates not valid spawn or movement
enum terrain_type 
{
	tall_grass = 11,
	boulders = 1,
	clearing = 8,
	sand = 9,
	mixed = 10,
	tree = 2,
	rock = 7,
	debug = 0,
	exitMap = 3,
	path = 6,
	center = 4,
	mart = 5,
};


// Map structure containing exits and map terrain
typedef struct map
{
	enum terrain_type t_map_grid[21][80];
	character_t* cmap[21][80];
	int west_exit;
	int east_exit;
	int north_exit;
	int south_exit;
}map_t;
enum directions;
enum region_combination;



typedef struct map map_t;
void printMap(map_t *map);
void allocate_map(map_t* map, int x, int y, map_t* Overallmap[399][399]);
int my_rand(int lower_bound, int upper_bound);

#endif
