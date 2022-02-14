#ifndef DISTANCEMAP_H
# define DISTANCEMAP_H
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "terrain.h"
#include <stdbool.h>
#include <time.h>
#define inf 10000



enum trainers{
	pc = 0,
	rival_cost = 0,
	h = 5
};

void set_neighbors_arr(int arr[8][2], int x, int y);
bool is_in_bounds(int x, int y);


void shortest_path(int row, int col, struct heap_node *dis_map[21][80], map_t *m, enum trainers trainer, int dis[21][80]);
void create_dis_map(int playerx, int playery, int arr[21][80], enum trainers trainer, map_t *map);
void print_dis_map(int map[21][80]);
void print_dis_map_5by5(int map[21][80], int x, int y);


#endif