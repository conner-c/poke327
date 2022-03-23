#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "terrain.h"
#include <stdbool.h>
#include <time.h>
#include "distanceMap.h"

// inf value used to avoid overflow arithmetic with INT_MAX
#define inf 2000


// Compare function to compare the key arr{cost, x, y} by getting the difference in cost
int32_t compare(const void *key, const void *with)
{

	int k = ((int *)key)[0];
	int w = ((int *)with)[0];

 	return k - w;
}


// Returns the tile cost of a given position
int get_tile_cost(int x, int y, map_t *m, enum trainers trainer){
	// Gets the tile from map
	enum terrain_type tile = m->t_map_grid[y][x];
	// If the tile is path (path or exitMap) or a clearing return cost of 10
	if(tile == path || tile == exitMap || tile == clearing){
		return 10;
	}
	// If the tile is tall grass return cost of 20
	else if(tile == tall_grass)
	{
		return 20;
	}
	// If the tile is sand return cost of 15
	else if(tile == sand){
		return 15;
	}
	// If the tile is a rock grass return cost of 25
	else if(tile == rock){
		return 25;
	}
	// If the tile is a center or mart and the trainer is the player than return cost of 10
	if(tile == center || tile == mart){
		if(trainer == pc){
			return 10;
		}
	}
	// Else return inf value
	return inf;
}

// Updates the cost of a cell (key) if it is cheap to move from the current tile than a previous one
void update_cost(heap_t *h, map_t *m, heap_node_t *dis_map[21][80], int neighbors_x, int neighbors_y, int current_cost,
									int current_x, int current_y, enum trainers trainer){

	// Getting the terrain cost of the neighbor's tile
	heap_node_t *n;
	n = dis_map[neighbors_y][neighbors_x];
	int neighbors_cost = ((int*)(n->datum))[0];

	// Getting the from the current tile plus the movement cost
	int from_current_cost = current_cost + get_tile_cost(current_x, current_y, m, trainer);

	// Compares the cost of the neighbor and the current tile, and updates if from current tile is cheaper
	if(neighbors_cost > from_current_cost){
		int* cost;
		cost = malloc(3*sizeof(int));
		cost[0] = from_current_cost;
		cost[1] = neighbors_x;
		cost[2] = neighbors_y;
		heap_decrease_key(h, n, cost);
	}

}

// Returns an array containing all possible neighbor positions. DOES NOT check if the neighbor is in bounds
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

// Checks if the given coordinates are within map bounds
bool is_in_bounds(int x, int y){
	if(x > 79 || x < 0){
		return false;
	}
	if(y > 20 || y < 0){
		return false;
	}
	return true;
}

// Finds the shortest path from the given position, row, col, for a given trainer
void shortest_path(int row, int col, heap_node_t *dis_map[21][80], map_t *m, enum trainers trainer, int dis[21][80]){
	// Sets up cost int array and heap
	int *cost;
	heap_t h;
	heap_init(&h, compare, free);

	// Initializing distance map to infinity and inserting into heap
	for(int i = 0; i < 80; i++){
		for(int j = 0; j < 21; j++){
			cost = malloc(3*sizeof(int));
			cost[0] = 1000;
			cost[1] = i;
			cost[2] = j;
			dis_map[j][i] = heap_insert(&h, cost);
			dis[j][i] = -1;
		}
	}

	// Updates the starting node to a cost of 0
	cost = malloc(3*sizeof(int));
	cost[0] = 0;
	cost[1] = col;
	cost[2] = row;
	heap_decrease_key(&h, dis_map[row][col], cost);

	// Creates a void pointer to take in the return value of the heap
	void *n;
	// Values of the returned nodes position and cost
	int current_cost, current_row, current_col;
	// Array to store neighbor positions
	int arr[8][2];
	// While the heap is not empty, update all neighbor costs, of the current node (lowest distance node)
	while(h.size > 0){
		// Getting the current nodes information
		n = heap_peek_min(&h);
		current_cost = ((int*)n)[0];
		current_row = ((int*)n)[1];
		current_col = ((int*)n)[2];
	
		// Getting all the possible neighbors of current node
		set_neighbors_arr(arr, current_row, current_col);

		// For each neighbor in bounds and not NULL, update the cost
		for (int neighbor = 0; neighbor < 8; neighbor++) {
			if(is_in_bounds(arr[neighbor][0], arr[neighbor][1]) && dis_map[arr[neighbor][1]][arr[neighbor][0]] != NULL){
				update_cost(&h, m, dis_map, arr[neighbor][0], arr[neighbor][1], current_cost, current_row, current_col, trainer);
			}
		}
		// Remove the smallest node from the heap
		heap_remove_min(&h);
		// Move the cost from the heap node array to the final distance cost array map
		dis[current_col][current_row] = current_cost;
		// Sets the current heap node in heap node array to NULL to not recheck during future cost updates
		dis_map[current_col][current_row] = NULL;
	}
}

// Replaces cells on the distance map containing non-valid locations to INT_MAX
void replace_with_inf(map_t *map, int m[21][80]){
	for(int i = 0; i < 21; i++){
		for(int j = 0; j < 80; j++){
			if(map->t_map_grid[i][j] == boulders || map->t_map_grid[i][j] == exitMap || map->t_map_grid[i][j] == tree || map->t_map_grid[i][j] == center || map->t_map_grid[i][j] == mart){
				m[i][j] = INT_MAX;
			}
		}
	}
}

// Prints the given integer distance map
void print_dis_map(int map[21][80]){
	// Prints distance map with room for a 2 digit number and spaces for invalid locations
	for(int i = 0; i < 21; i++){
		for(int j = 0; j < 80; j++){
			if(map[i][j] == 10000){
				printf("     ");
			}
			else {
				printf("%04d ", map[i][j]);
			}	
		}
		printf("\n");
	}
}

// Prints a 5 by 5 area of the distance map around the given coordinates. DOES NOT check for in bounds of 5 by 5 area
void print_dis_map_5by5(int map[21][80], int x, int y){
	for (int i = y - 2; i < y + 3; i++)
	{
		for (int j = x - 2; j < x + 3; j++)
		{
			if(i < 0 || j < 0 || i > 19 || j > 79){
				continue;
			}
			if(map[i][j] == INT_MAX){
				printf("     ");
			}
			else {
				printf("%04d ", map[i][j]);
			}		
		}
		printf("\n");
	}
}

// Main function to update a given distance map for the given trainer
void create_dis_map(int playerx, int playery, int dmap[21][80], enum trainers trainer, map_t *map){	
	// Creates a heap node array used in shortest path
	heap_node_t *node_distance[21][80];

	// Calculates the shortest path
	shortest_path(playery, playerx, node_distance, map, trainer, dmap);

	// Replaces the invalid locations with INT_MAX values
	replace_with_inf(map, dmap);
}