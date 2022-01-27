#include <stdio.h>
#include <stdlib.h>

#define X 80
#define Y 21

enum terrain_type 
{
	tall_grass,
	boulders,
	clearing
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
	
	for(int row = 0; row < Y; row++){
		for(int col = 0; col < X; col++){
			printf("I am working before switch statement");
			switch (map->map_grid[col][row]){
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
					printf("");
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
			map->map_grid[row][col] = clearing;
		}
	}
} 

int main(int argc, char const *argv[])
{
	map_t map;
	init_map(&map);
	printMap(&map);
	return 0;
}