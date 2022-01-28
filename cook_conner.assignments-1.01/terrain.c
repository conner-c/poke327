#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define X 80
#define Y 21

enum terrain_type 
{
	tall_grass,
	boulders,
	clearing,
	sand,
	mixed,
	tree,
	rock,
	debug
};

enum region_combination{
	c2t2s0m1,
	c2t2s0m2,
	c2t2s1m1,
	c2t2s1m2
};

typedef struct map
{
	enum terrain_type map_grid[Y][X];
	int west_exit[2];
	int east_exit[2];
	int north_exit[2];
	int south_exit[2];
}map_t;

void printMap(map_t *map){
	// Creating variables
	int row, col;

	//Printing the map to terminal by going through each cell within map_grid and printing the corresponding character to stored enum
	for(row = 0; row < Y; row++){
		for(col = 0; col < X; col++){
			enum terrain_type cell = map->map_grid[row][col];
			switch (cell){
				case tall_grass:
					printf(",");
					break;
				case boulders:
					printf("%%");
					break;
				case clearing:
					printf(".");
					break;
				default:
					printf(" ");
					break;
			}
		}
		printf("\n");
	}
}

void init_map(map_t *map){
	int col, row;
	// Initializing Border to Boulders
	for(row = 0; row < Y; row++){
		map->map_grid[row][0] = boulders;
		map->map_grid[row][X - 1] = boulders;
	}
	for(col = 0; col < X; col++){
		map->map_grid[0][col] = boulders;
		map->map_grid[Y - 1][col] = boulders;
	}

	for(row = 1; row < Y - 1; row++){
		for(col = 1; col < X - 1; col++){
			map->map_grid[row][col] = debug;
		}
	}
} 


int my_rand(int lower_bound, int upper_bound){
	srand(time(NULL));
	return rand() % upper_bound + lower_bound;
}

void place_seed(enum region_combination, map_t *map){
	// Used to check if all regions have been seeded
	bool clearing_region, tall_grass_region, sand_region, mixed_region;
	int type_of_region;
	clearing_region = tall_grass_region = sand_region = mixed_region = false;

	// Placing seeds for regions
	while(!clearing_region && !tall_grass_region && !sand_region && !mixed_region){
		//
		type_of_region = my_rand(0, 4);
	}
}


void seed_map(map_t *map){
	// Determining style of map to create
	int number_of_regions, region_comb;
	enum region_combination region_combi;


	// Getting random value
	region_comb = my_rand(0, 100);

	// Determining map style
	if(region_comb >= 0 && region_comb <= 27){
		number_of_regions = 5;
		region_combi = c2t2s0m1;
	}
	else if(region_comb >= 28 && region_comb <= 49){
		number_of_regions = 6;
		region_combi = c2t2s0m2;
	}
	else if(region_comb >= 50 && region_comb <= 77){
		number_of_regions = 6;
		region_combi = c2t2s1m1;
	}
	else{
		number_of_regions = 7;
		region_combi = c2t2s1m2;
	}


	place_seed(region_combi, map);




}



int main(int argc, char const *argv[])
{
	map_t map;
	init_map(&map);
	printMap(&map);
	return 0;
}