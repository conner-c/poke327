#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

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
	debug,
	exitMap,
	path,
	center,
	mart
};

enum region_combination
{
	c2t2s0m1,
	c2t2s0m2,
	c2t2s1m1,
	c2t2s1m2
};

enum directions{
	Left = 0,
	Right = 1,
	Up = 2,
	Down = 3
};

typedef struct map
{
	enum terrain_type map_grid[Y][X];
	int west_exit;
	int east_exit;
	int north_exit;
	int south_exit;
}map_t;


int my_rand(int lower_bound, int upper_bound)
{
	static int counter;
	srand(time(NULL) + counter);
	counter++;
	return rand() % (upper_bound - lower_bound) + lower_bound;
}

void printMap(map_t *map)
{
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
				case sand:
					printf("*");
					break;
				case mixed:
					if(my_rand(0,10) < 7){
						printf(".");
					}
					else{
						printf(",");
					}
					break;
				case exitMap:
					printf("#");
					break;
				case path:
					printf("#");
					break;
				case center:
					printf("C");
					break;
				case mart:
					printf("M");
					break;
				case tree:
					printf("^");
					break;
				case rock:
					printf("-");
					break;
				default:
					printf(" ");
					break;
			}
		}
		printf("\n");
	}
}



void place_seed(map_t *map, int region_number, enum terrain_type tt, int seed_local[7][2])
{
	int x, y;
	// Determining location to plant the seed
	if(region_number == 1){
		x = my_rand(1, 26);
		y = my_rand(1, 9);
	}
	else if(region_number == 2){
		x = my_rand(1, 26);
		y = my_rand(11, 19);
	}
	else if(region_number == 3){
		x = my_rand(52, 78);
		y = my_rand(1, 9);
	}
	else if(region_number == 4){
		x = my_rand(52, 78);
		y = my_rand(11, 19);
	}
	else if(region_number == 5){
		x = my_rand(27, 51);
		y = my_rand(6, 14);
	}
	else if(region_number == 6){
		x = my_rand(27, 51);
		y = my_rand(1, 9);
	}
	else if(region_number == 7){
		x = my_rand(27, 51);
		y = my_rand(11, 19);
	}

	// Locations of the seeds
	seed_local[region_number - 1][0] = x;
	seed_local[region_number - 1][1] = y;

	// Planting the seed
	map->map_grid[y][x] = tt;
}


void grow(int pos[2], map_t *map, int dir[4], enum terrain_type tt, bool was_visited[Y][X])
{
	static bool hasGrown;
	hasGrown = false;
	int i;
	// Returns if it is searching on top of another tile type
	if(map->map_grid[pos[1]][pos[0]] != tt && map->map_grid[pos[1]][pos[0]] != debug){
		return;
	}
	// Returns if the position index it out of bounds
	else if(pos[0] < 1 || pos[0] > 78 || pos[1] < 1 || pos[1] > 19){
		return;
	}

	// Returns if it has already visited the tile
	else if(was_visited[pos[1]][pos[0]]) {		return;
	}

	// If the tile is empty, then plant the tt there, change hasGrown to true and return.
	if(map->map_grid[pos[1]][pos[0]] == debug){
		map->map_grid[pos[1]][pos[0]] = tt;
		hasGrown = true;
		return;
	}
	// If the tile is not empty, for each direction (up, down, left, right), try to grown in that direction
	for(i = 0; i < 4; i++){
		was_visited[pos[1]][pos[0]] = true;
		if(dir[i] == Left && !hasGrown){
			pos[0] = pos[0] - 1;
			grow(pos, map, dir, tt, was_visited);
		}
		else if(dir[i] == Right && !hasGrown){
			pos[0] = pos[0] + 1;
			grow(pos, map, dir, tt, was_visited);
		}
		else if(dir[i] == Up && !hasGrown){
			pos[1] = pos[1] - 1;
			grow(pos, map, dir, tt, was_visited);
		}
		else if(dir[i] == Down && !hasGrown){
			pos[1] = pos[1] + 1;
			grow(pos, map, dir, tt, was_visited);
		}
	}
	return;
}

bool check_if_Full(map_t *map)
{
	int row, col;
	for(row = 0; row < Y; row++){
		for(col = 0; col < X; col++){
			if(map->map_grid[row][col] == debug){
				return false;
			}
		}
	}
	return true;
}


void finish_map(map_t *map)
{
	int col, row;
	for(row = 0; row < Y; row++){
		for(col = 0; col < X; col++){
			if(map->map_grid[row][col] == debug){
				if(col != 1 && map->map_grid[row][col - 1] != debug){
					map->map_grid[row][col] = map->map_grid[row][col - 1];
				}
				else if(row != 1 && map->map_grid[row - 1][col] != debug){
					map->map_grid[row][col] = map->map_grid[row - 1][col];
				}
			}
				
		}
	}
	if(!check_if_Full(map)){
		for(row = Y -1; row > 0; row--){
			for(col = X - 1; col > 0; col--){
				if(map->map_grid[row][col] == debug){
					if(col != (X - 2) && map->map_grid[row][col + 1] != debug){
						map->map_grid[row][col] = map->map_grid[row][col + 1];
					}
					else if(row != (Y - 2) && map->map_grid[row + 1][col] != debug){
						map->map_grid[row][col] = map->map_grid[row + 1][col];
					}
				}
					
			}
		}
	}
}


void grow_all(int seed_local[7][2], map_t *map, int num_of_regions)
{
	int i = 0;
	// Directions 0 = left, 1 = right, 2 = up, and 3 = down
	int dir[4] = {Left, Right, Up, Down};
	int j, index_a, index_b, tmp;
	bool map_tile_check[Y][X];


	// Growing the map  
	for(i = 0; i < 1500; i++){
		for(int k = 0; k < Y; k++){
			for(int l = 0; l < X; l++){
				map_tile_check[k][l] = false;
			}
		}
		for(j = 0; j < my_rand(1,11); j++){
			index_a = my_rand(0,4);
			index_b = my_rand(0,4);
			tmp = dir[index_a];
			dir[index_a] = dir[index_b];
			dir[index_b] = tmp;
		}
		grow(seed_local[i % num_of_regions], map, dir, map->map_grid[seed_local[i % num_of_regions][1]][seed_local[i % num_of_regions][0]], map_tile_check);
	}

	if(!check_if_Full(map)){
		finish_map(map);
	}
}


void seed_map(map_t *map)
{
	// Determining style of map to create
	int region_comb;
	int num_sand, num_mix;
	
	// Getting random value
	region_comb = my_rand(0, 100);

	// Determining map style
	if(region_comb >= 0 && region_comb <= 27){
		num_mix = 1;
		num_sand = 0;
	}
	else if(region_comb >= 28 && region_comb <= 49){
		num_mix = 2;
		num_sand = 0;
	}
	else if(region_comb >= 50 && region_comb <= 77){
		num_mix = 1;
		num_sand = 1;
	}
	else{
		num_sand = 1;
		num_mix = 2;
	}

	// Used to check if all regions have been seeded
	int clearing_region, tall_grass_region, sand_region, mixed_region, type_of_region, counter;
	clearing_region = tall_grass_region = sand_region = mixed_region = 0;
	counter = 1;
	int seed_local[7][2];

	// Placing seeds for regions
	while(clearing_region != 2 || tall_grass_region != 2 || sand_region != num_sand || mixed_region != num_mix){
		// Determines which type of region to seed
		type_of_region = my_rand(0, 4);

		// Determines which region to place
		if((type_of_region == 0) && (clearing_region != 2)){
			place_seed(map, counter, clearing, seed_local);
			clearing_region++;
			counter++;
		}
		else if((type_of_region == 1) && (tall_grass_region != 2)){
			place_seed(map, counter, tall_grass, seed_local);
			tall_grass_region++;
			counter++;
		}
		else if((type_of_region == 2) && (sand_region != num_sand)){
			place_seed(map, counter, sand, seed_local);
			sand_region++;
			counter++;
		}
		else if((type_of_region == 3) && (mixed_region != num_mix)){
			place_seed(map, counter, mixed, seed_local);
			mixed_region++;
			counter++;
		}
	}
	// Growing seeds
	grow_all(seed_local, map, counter -1);
}

void generate_exits(map_t *map)
{	
	map->west_exit = my_rand(5, 15);
	map->map_grid[map->west_exit][0] = exitMap;
	map->east_exit = my_rand(5, 15);
	map->map_grid[map->east_exit][X - 1] = exitMap;
	map->north_exit = my_rand(11, 75);
	map->map_grid[0][map->north_exit] = exitMap;
	map->south_exit = my_rand(11, 75);
	map->map_grid[Y - 1][map->south_exit] = exitMap;
}

void generate_paths(map_t *map)
{
	int east_west_path, i, diff_west_east_exits, counter;
	east_west_path = my_rand(20, 60);
	diff_west_east_exits = map->west_exit - map->east_exit;
	counter = 0;
	printf("%d\n", abs(diff_west_east_exits));

	// Generating EastWest Path
	for(i = 1; i <= east_west_path; i++){
		map->map_grid[map->west_exit][i] = path;
	}
	for(i = map->west_exit; i <= (abs(diff_west_east_exits) + map->west_exit); i++){
		map->map_grid[map->west_exit - counter * (diff_west_east_exits / abs(diff_west_east_exits))][east_west_path] = path;
		counter++;
	}
	for(i = east_west_path; i < X; i++){
		map->map_grid[map->east_exit][i] = path;
	}

	int north_south_path, diff_north_south;
	north_south_path = my_rand(3, 18);
	diff_north_south = map->north_exit - map->south_exit;
	counter = 0;

	for(i = 1; i <= north_south_path; i++){
		map->map_grid[i][map->north_exit] = path;
	}
	for(i = map->north_exit; i <= (abs(diff_north_south) + map->north_exit); i++){
		map->map_grid[north_south_path][map->north_exit - counter * (diff_north_south / abs(diff_north_south))] = path;
		counter++;
	}
	for(i = north_south_path; i < Y; i++){
		map->map_grid[i][map->south_exit] = path;
	}

}

void generate_poke_centers(map_t *map){
	map->map_grid[map->west_exit - 1][5] = center;
	map->map_grid[map->west_exit - 1][6] = center;
	map->map_grid[map->west_exit - 2][5] = center;
	map->map_grid[map->west_exit - 2][6] = center;

	map->map_grid[map->west_exit - 1][8] = mart;
	map->map_grid[map->west_exit - 1][9] = mart;
	map->map_grid[map->west_exit - 2][8] = mart;
	map->map_grid[map->west_exit - 2][9] = mart;
}



void place_extra_terrain(map_t *map){
	int row, col;
	for(row = 1; row < Y - 1; row++){
		for(col = 1; col < X - 1; col++){
			switch(map->map_grid[row][col]){
				case clearing:
					if(my_rand(0, 100) < 7){
						map->map_grid[row][col] = tree;
					}
					if(my_rand(0, 100) < 3){
						map->map_grid[row][col] = rock;
					}
					break;
				case tall_grass:
					if(my_rand(0, 100) < 3){
						map->map_grid[row][col] = tree;
					}
					if(my_rand(0, 100) < 7){
						map->map_grid[row][col] = rock;
					}
					break;
				case sand:
					if(my_rand(0, 100) < 15){
						map->map_grid[row][col] = rock;
					}
					break;
				case mixed:
					if(my_rand(0, 100) < 3){
						map->map_grid[row][col] = tree;
					}
					if(my_rand(0, 100) < 3){
						map->map_grid[row][col] = rock;
					}
					break;
				default:
					break;
			}
		}
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
	seed_map(map);
	generate_exits(map);
	generate_paths(map);
	generate_poke_centers(map);
	place_extra_terrain(map);
} 

int main(int argc, char const *argv[])
{
	map_t map;
	init_map(&map);
	printMap(&map);
	return 0;
}