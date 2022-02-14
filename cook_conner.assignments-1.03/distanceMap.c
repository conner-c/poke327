#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "terrain.h"
#include <stdbool.h>
#include <time.h>

#define inf 10000


typedef struct heap_node {
  heap_node_t *next;
  heap_node_t *prev;
  heap_node_t *parent;
  heap_node_t *child;
  void *datum;
  uint32_t degree;
  uint32_t mark;
}heap_node_t;

int32_t compare(const void *key, const void *with)
{

	int k = ((int *)key)[0];
	int w = ((int *)with)[0];

 	return k - w;
}


enum trainers{
	pc = 0,
	r = 0,
	h = 5
};


int get_tile_cost(int x, int y, map_t *m, enum trainers trainer){
	enum terrain_type tile = m->map_grid[y][x];
	if(tile == path || tile == exitMap){
		return 10;
	}
	else if(tile == clearing){
		return 10;
	}
	else if(tile == tall_grass)
	{
		return 20 - trainer;
	}
	else if(tile == sand){
		return 15;
	}
	else if(tile == rock){
		return 25;
	}
	if(tile == center || tile == mart){
		if(trainer == pc){
			return 10;
		}
	}
	return inf;

}

void update_cost(heap_t *h, map_t *m, heap_node_t *dis_map[21][80], int neighbors_x, int neighbors_y, int current_cost,
									int current_x, int current_y, enum trainers trainer){

	// Getting the terrain cost of the neighbor's tile
	heap_node_t *n;
	n = dis_map[neighbors_y][neighbors_x];
	int neighbors_cost = ((int*)(n->datum))[0];

	// Getting the from the current tile plus the movement cost
	int from_current_cost = current_cost + get_tile_cost(current_x, current_y, m, trainer);


	if(neighbors_cost > from_current_cost){
		int* cost;
		cost = malloc(3*sizeof(int));
		cost[0] = from_current_cost;
		cost[1] = neighbors_x;
		cost[2] = neighbors_y;
		heap_decrease_key(h, n, cost);
	}

}

void set_neighbors_arr(int arr[8][2], int x, int y){
	// North West Neighbor
	arr[0][0] = x - 1; 
	arr[0][1] = y - 1;
	// North Neighbor
	arr[1][0] = x;
	arr[1][1] = y - 1;
	// North East Neighbor
	arr[2][0] = x + 1;
	arr[2][1] = y - 1;
	// West Neighbor
	arr[3][0] = x - 1; 
	arr[3][1] = y;
	// East Neighbor
	arr[4][0] = x + 1;
	arr[4][1] = y;
	// South West Neighbor
	arr[5][0] = x - 1;
	arr[5][1] = y + 1;
	// South Neighbor
	arr[6][0] = x;
	arr[6][1] = y + 1;
	// South West Neighbor
	arr[7][0] = x + 1;
	arr[7][1] = y + 1;
}


bool is_in_bounds(int x, int y){
	if(x > 79 || x < 0){
		return false;
	}
	if(y > 20 || y < 0){
		return false;
	}
	return true;
}

void shortest_path(int col, int row, heap_node_t *dis_map[21][80], map_t *m, enum trainers trainer, int dis[21][80]){
	int *cost;
	heap_t h;
	heap_init(&h, compare, free);
	// Initializing distance map to infinity and inserting into heap
	for(int i = 0; i < 80; i++){
		for(int j = 0; j < 21; j++){
			cost = malloc(3*sizeof(int));
			cost[0] = INT_MAX;
			cost[1] = i;
			cost[2] = j;
			dis_map[j][i] = heap_insert(&h, cost);
		}
	}

	cost = malloc(3*sizeof(int));
	cost[0] = 0;
	cost[1] = row;
	cost[2] = col;
	heap_decrease_key(&h, dis_map[col][row], cost);



	void *n;
	int current_cost, current_row, current_col;
	int arr[8][2];
	while(h.size > 0){
		// Getting the current nodes information
		n = heap_peek_min(&h);
		current_cost = ((int*)n)[0];
		current_row = ((int*)n)[1];
		current_col = ((int*)n)[2];

		// Getting all the possible neighbors of current node
		set_neighbors_arr(arr, current_row, current_col);

		// For each neighbor in bounds, update the cost
		for (int neighbor = 0; neighbor < 8; neighbor++) {
			if(is_in_bounds(arr[neighbor][0], arr[neighbor][1]) && dis_map[arr[neighbor][1]][arr[neighbor][0]] != NULL){
				update_cost(&h, m, dis_map, arr[neighbor][0], arr[neighbor][1], current_cost, current_row, current_col, trainer);
			}
		}
		heap_remove_min(&h);
		dis[current_col][current_row] = current_cost;
		dis_map[current_col][current_row] = NULL;
	}

}


void print_dis_map(int map[21][80]){
	// Prints distance map with room for a four digit number
	for(int i = 0; i < 21; i++){
		for(int j = 0; j < 80; j++){
			printf("%d, ", map[i][j]%100);
		}
		printf("\n");
	}
}

void print_dis_map_5by5(int map[21][80], int x, int y){
	for (int i = y - 2; i < y + 2; i++)
	{
		for (int j = x - 2; j < x + 2; j++)
		{
			printf("%d, ", map[i][j]);		
		}
		printf("\n");
	}
}





int main(int argc, char const *argv[])
{	
	heap_node_t *hiker_dis[21][80];
	int h_dis[21][80];
	map_t map;
	init_map(&map);
	generate_remaining_exits_rand(&map);
	generate_paths(&map);
	generate_poke_centers(&map, 100);

	map.playerx = 15;
	map.playery = 15;
	printMap(&map);

	shortest_path(15, 15, hiker_dis, &map, h, h_dis);
	// print_dis_map(h_dis);
	print_dis_map_5by5(h_dis, 15, 15);

	return 0;
}