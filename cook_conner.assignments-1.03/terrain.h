#ifndef TERRAIN_H
# define TERRAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <windows.h>

enum terrain_type 
{
	tall_grass,
	boulders,
	clearing,
	sand,
	mixed,
	tree,
	rock,
	debug,
	exitMap,
	path,
	center,
	mart,
	player,
	kiker,
	rival
};


// Map structure containing exits and map terrain
typedef struct map
{
	enum terrain_type map_grid[21][80];
	int west_exit;
	int east_exit;
	int north_exit;
	int south_exit;
	int playerx;
	int playery;
}map_t;
enum directions;
enum region_combination;


typedef struct map map_t;
void printMap(map_t *map);
void init_map(map_t *map);
void generate_remaining_exits_rand(map_t *map);
void generate_paths(map_t *map);
void generate_poke_centers(map_t *map, int prob);
void allocate_map(map_t *Overallmap[399][399], int x, int y);
void repalce_mixed(map_t *map);
void place_extra_terrain(map_t *map);
void seed_map(map_t *map);
void grow_all(int seed_local[7][2], map_t *map, int num_of_regions);
void finish_map(map_t *map);
bool check_if_Full(map_t *map);
void grow(int pos[2], map_t *map, int dir[4], enum terrain_type tt, bool was_visited[21][80]);
void place_seed(map_t *map, int region_number, enum terrain_type tt, int seed_local[7][2]);
int my_rand(int lower_bound, int upper_bound);
void random_pc(map_t *map);

#endif
