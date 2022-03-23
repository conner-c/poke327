#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include "characters.h"
#include "terrain.h"
#include "heap.h"
#include <ncurses.h>

#define X 80
#define Y 21
#define atoa(x) #x



// Different combination of region types, c2t2s1m1 = two clearings, two tall grass, 1 sand, and 1 mixed
enum region_combination
{
	c2t2s0m1,
	c2t2s0m2,
	c2t2s1m1,
	c2t2s1m2
};

// Different directions used to help create maps
enum directions{
	Left = 0,
	Right = 1,
	Up = 2,
	Down = 3
};


// Random generator between lower bound and upper bound
int my_rand(int lower_bound, int upper_bound)
{
	return rand() % (upper_bound - lower_bound) + lower_bound;
}

// Prints the given map
void printMap(map_t *map)
{
	// Creating variables
	int y, x;
	clear();

	//Printing the map to terminal by going through each cell within t_map_grid and printing the corresponding character to stored enum
	for(y = 0; y < Y; y++){
		for(x = 0; x < X; x++){
			enum terrain_type cell = map->t_map_grid[y][x];
			// If there is a character at this position then print the character symbol
			if(map->cmap[y][x] != NULL){
				mvaddch(y + 1, x, (map->cmap[y][x])->symbol);
				continue;
			}
			switch (cell){
				case tall_grass:
					mvaddch((int) (y + 1), x, ':');
					break;
				case boulders:
					mvaddch(y + 1, x, '%');
					break;
				case clearing:
					mvaddch(y + 1, x, '.');
					break;
				case sand:
					mvaddch(y + 1, x, '*');
					break;
				case exitMap:
					mvaddch(y + 1, x, '#');
					break;
				case path:
					mvaddch(y + 1, x, '#');
					break;
				case center:
					mvaddch(y + 1, x, 'C');
					break;
				case mart:
					mvaddch(y + 1, x, 'M');
					break;
				case tree:
					mvaddch(y + 1, x, '^');
					break;
				case rock:
					mvaddch(y + 1, x, '-');
					break;
				default:
					mvaddch(y + 1, x, ' ');
					break;
			}
		}
	}
	refresh();
}


// Places the starting seed on a map to grow from
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
	map->t_map_grid[y][x] = tt;
}

// Grows terrain type from the starting seed
void grow(int pos[2], map_t *map, int dir[4], enum terrain_type tt, bool was_visited[Y][X])
{
	static bool hasGrown;
	hasGrown = false;
	int i;
	// Returns if it is searching on top of another tile type
	if(map->t_map_grid[pos[1]][pos[0]] != tt && map->t_map_grid[pos[1]][pos[0]] != debug){
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
	if(map->t_map_grid[pos[1]][pos[0]] == debug){
		map->t_map_grid[pos[1]][pos[0]] = tt;
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

// Checks if the map has been filled out
bool check_if_Full(map_t *map)
{
	int row, col;
	for(row = 0; row < Y; row++){
		for(col = 0; col < X; col++){
			if(map->t_map_grid[row][col] == debug){
				return false;
			}
		}
	}
	return true;
}

// Fills in holes in the map from initialization
void finish_map(map_t *map)
{
	int col, row;
	for(row = 0; row < Y; row++){
		for(col = 0; col < X; col++){
			if(map->t_map_grid[row][col] == debug){
				if(col != 1 && map->t_map_grid[row][col - 1] != debug){
					map->t_map_grid[row][col] = map->t_map_grid[row][col - 1];
				}
				else if(row != 1 && map->t_map_grid[row - 1][col] != debug){
					map->t_map_grid[row][col] = map->t_map_grid[row - 1][col];
				}
			}

		}
	}
	if(!check_if_Full(map)){
		for(row = Y -1; row > 0; row--){
			for(col = X - 1; col > 0; col--){
				if(map->t_map_grid[row][col] == debug){
					if(col != (X - 2) && map->t_map_grid[row][col + 1] != debug){
						map->t_map_grid[row][col] = map->t_map_grid[row][col + 1];
					}
					else if(row != (Y - 2) && map->t_map_grid[row + 1][col] != debug){
						map->t_map_grid[row][col] = map->t_map_grid[row + 1][col];
					}
				}

			}
		}
	}
}

// Grows each seed on the map until theoretically the map should be filled
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
		grow(seed_local[i % num_of_regions], map, dir, map->t_map_grid[seed_local[i % num_of_regions][1]][seed_local[i % num_of_regions][0]], map_tile_check);
	}

	if(!check_if_Full(map)){
		finish_map(map);
	}
}

// Determines the starting seed locations
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

// Generates exits yet to be placed
void generate_remaining_exits_rand(map_t *map)
{	
	if(map->west_exit == -1){
		map->west_exit = my_rand(5, 15);
		map->t_map_grid[map->west_exit][0] = exitMap;
	}
	if(map->east_exit == -1){
		map->east_exit = my_rand(5, 15);
		map->t_map_grid[map->east_exit][X - 1] = exitMap;
	}
	if(map->north_exit == -1){
		map->north_exit = my_rand(11, 75);
		map->t_map_grid[0][map->north_exit] = exitMap;
	}
	if(map->south_exit == -1){
		map->south_exit = my_rand(11, 75);
		map->t_map_grid[Y - 1][map->south_exit] = exitMap;
	}
}


// Creates paths between exits
void generate_paths(map_t *map)
{	
	//Section for all four exits spawned
	int east_west_path, i, diff_west_east_exits, counter, current;
	// If an both exits spawn, exit > 0, then build a road between the exits

	if(map->west_exit > 0 && map->east_exit > 0){
		east_west_path = my_rand(20, 60);
		diff_west_east_exits = map->west_exit - map->east_exit;
		counter = 0;

		// Generating EastWest Path
		for(i = 1; i <= east_west_path; i++){
			map->t_map_grid[map->west_exit][i] = path;
		}
		for(i = map->west_exit; i <= (abs(diff_west_east_exits) + map->west_exit); i++){
			map->t_map_grid[map->west_exit - counter * (diff_west_east_exits / abs(diff_west_east_exits))][east_west_path] = path;
			counter++;
		}
		for(i = east_west_path; i < X; i++){
			map->t_map_grid[map->east_exit][i] = path;
		}

	}


	// If an exit was not spawned (Edge of overall map), exit < 0, then do not try to build a road to -1 to nonexistent exit
	if(map->north_exit > 0 && map->south_exit > 0){
		int north_south_path, diff_north_south;
		north_south_path = my_rand(3, 18);
		diff_north_south = map->north_exit - map->south_exit;
		counter = 0;

		for(i = 1; i <= north_south_path; i++){
			map->t_map_grid[i][map->north_exit] = path;
		}
		for(i = map->north_exit; i <= (abs(diff_north_south) + map->north_exit); i++){
			map->t_map_grid[north_south_path][map->north_exit - counter * (diff_north_south / abs(diff_north_south))] = path;
			counter++;
		}
		for(i = north_south_path; i < Y; i++){
			map->t_map_grid[i][map->south_exit] = path;
		}


	}

	// Section for three exits spawned

	// If the west exit does not spawn and both the north and south exits spawn, create a path from east exit till path from north to south
	if(map->west_exit < 0 && (map->north_exit > 0 && map->south_exit > 0)){

		current = 78;
		while(map->t_map_grid[map->east_exit][current] != path){
			map->t_map_grid[map->east_exit][current] = path;
			current--;
		}
	}

	// If the east exit does not spawn and both the north and south exits spawn, create a path from west exit till path from north to south
	else if(map->east_exit < 0 && (map->north_exit > 0 && map->south_exit > 0)){

		current = 1;
		while(map->t_map_grid[map->west_exit][current] != path){
			map->t_map_grid[map->west_exit][current] = path;
			current++;
		}
	}

	// If the north exit does not spawn and both the east and west exits spawn, create a path from south exit till path from east to west
	else if(map->north_exit < 0 && (map->west_exit > 0 && map->east_exit > 0)){

		current = 19;
		while(map->t_map_grid[current][map->south_exit] != path){
			map->t_map_grid[current][map->south_exit] = path;
			current--;
		}
	}
	// If the south exit does not spawn and both the east and west exits spawn, create a path from north exit till path from east to west
	else if(map->south_exit < 0 && (map->east_exit > 0 && map->west_exit > 0)){

		current = 1;
		while(map->t_map_grid[current][map->north_exit] != path){
			map->t_map_grid[current][map->north_exit] = path;
			current++;
		}
	}

	// Section for two exits spawned
	// If the West and North
	if(map->east_exit == -2 && map->south_exit == -2){

		for(i = 1; i <= map->north_exit; i++){
			map->t_map_grid[map->west_exit][i] = path;
		}
		for(i = 1; i <= map->west_exit; i++){
			map->t_map_grid[i][map->north_exit] = path;
		}
	}

	// If the East and North
	else if(map->west_exit == -2 && map->south_exit == -2){

		for(i = 78; i >= map->north_exit; i--){
			map->t_map_grid[map->east_exit][i] = path;
		}
		for(i = 1; i <= map->east_exit; i++){
			map->t_map_grid[i][map->north_exit] = path;
		}
	}

	// If the West and South
	else if(map->east_exit == -2 && map->north_exit == -2){

		for(i = 1; i <= map->south_exit; i++){
			map->t_map_grid[map->west_exit][i] = path;
		}
		for(i = 19; i >= map->west_exit; i--){
			map->t_map_grid[i][map->south_exit] = path;
		}
	}
	//If the East and South
	else if(map->west_exit == -2 && map->north_exit == -2){

		for(i = 78; i >= map->south_exit; i--){
			map->t_map_grid[map->east_exit][i] = path;
		}
		for(i = 19; i >= map->east_exit; i--){
			map->t_map_grid[i][map->south_exit] = path;
		}
	}
}

// Generates poke centers and marts on the map
void generate_poke_centers(map_t *map, int prob){
	bool placed = false;
	int pos;
	// If the random number is less than the given probability of spawning a center, then spawn a center
	//Placing Poke Center
	if(my_rand(0, 100) < prob){
		// Goes until it places a mart or center
		while(!placed){
			// Chooses if it should place the center/mart on a north-south path or an east-west path
			if(my_rand(0,2) < 1){
				// Randomly choose a coordinate to try and place a mart
				pos = my_rand(5, 74);
				// Checks four tiles to try and place the center/mart 
				if(map->t_map_grid[map->west_exit][pos] == path && map->t_map_grid[map->west_exit - 1][pos] != path &&
				 map->t_map_grid[map->west_exit - 2][pos] != path && map->t_map_grid[map->west_exit - 1][pos + 1] != path &&
				 map->t_map_grid[map->west_exit - 2][pos + 1] != path){
					map->t_map_grid[map->west_exit - 1][pos] = center;
					map->t_map_grid[map->west_exit - 1][pos + 1] = center;
					map->t_map_grid[map->west_exit - 2][pos] = center;
					map->t_map_grid[map->west_exit - 2][pos + 1] = center;
					break;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[map->west_exit][pos] == path && map->t_map_grid[map->west_exit + 1][pos] != path &&
				 map->t_map_grid[map->west_exit + 2][pos] != path && map->t_map_grid[map->west_exit + 1][pos + 1] != path &&
				 map->t_map_grid[map->west_exit + 2][pos + 1] != path){
					map->t_map_grid[map->west_exit + 1][pos] = center;
					map->t_map_grid[map->west_exit + 1][pos + 1] = center;
					map->t_map_grid[map->west_exit + 2][pos] = center;
					map->t_map_grid[map->west_exit + 2][pos + 1] = center;
					break;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[map->east_exit][pos] == path && map->t_map_grid[map->east_exit - 1][pos] != path &&
				 map->t_map_grid[map->east_exit - 2][pos] != path && map->t_map_grid[map->east_exit - 1][pos + 1] != path &&
				 map->t_map_grid[map->east_exit - 2][pos + 1] != path){
					map->t_map_grid[map->east_exit - 1][pos] = center;
					map->t_map_grid[map->east_exit - 1][pos + 1] = center;
					map->t_map_grid[map->east_exit - 2][pos] = center;
					map->t_map_grid[map->east_exit - 2][pos + 1] = center;
					break;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[map->east_exit][pos] == path && map->t_map_grid[map->east_exit + 1][pos] != path &&
				 map->t_map_grid[map->east_exit + 2][pos] != path && map->t_map_grid[map->east_exit + 1][pos + 1] != path &&
				 map->t_map_grid[map->east_exit + 2][pos + 1] != path){
					map->t_map_grid[map->east_exit + 1][pos] = center;
					map->t_map_grid[map->east_exit + 1][pos + 1] = center;
					map->t_map_grid[map->east_exit + 2][pos] = center;
					map->t_map_grid[map->east_exit + 2][pos + 1] = center;
					break;
				}
			}
			else{
				pos = my_rand(5, 15);
				// Checks four tiles to try and place the center/mart 
				if(map->t_map_grid[pos][map->north_exit] == path && map->t_map_grid[pos][map->north_exit - 1] != path &&
				 map->t_map_grid[pos][map->north_exit - 2] != path && map->t_map_grid[pos - 1][map->north_exit - 1] != path &&
				 map->t_map_grid[pos - 1][map->north_exit - 2]){
					map->t_map_grid[pos - 1][map->north_exit - 1] = center;
					map->t_map_grid[pos - 2][map->north_exit - 1] = center;
					map->t_map_grid[pos - 1][map->north_exit - 2] = center;
					map->t_map_grid[pos - 2][map->north_exit - 2] = center;
					placed = true;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[pos][map->north_exit] == path && map->t_map_grid[pos][map->north_exit + 1] != path &&
				 map->t_map_grid[pos][map->north_exit + 2] != path && map->t_map_grid[pos - 1][map->north_exit + 1] != path &&
				 map->t_map_grid[pos - 1][map->north_exit + 2]){
					map->t_map_grid[pos - 1][map->north_exit + 1] = center;
					map->t_map_grid[pos - 2][map->north_exit + 1] = center;
					map->t_map_grid[pos - 1][map->north_exit + 2] = center;
					map->t_map_grid[pos - 2][map->north_exit + 2] = center;
					break;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[pos][map->south_exit] == path && map->t_map_grid[pos][map->south_exit - 1] != path &&
				 map->t_map_grid[pos][map->south_exit - 2] != path && map->t_map_grid[pos - 1][map->south_exit - 1] != path &&
				 map->t_map_grid[pos - 1][map->south_exit - 2]){
					map->t_map_grid[pos - 1][map->south_exit - 1] = center;
					map->t_map_grid[pos - 2][map->south_exit - 1] = center;
					map->t_map_grid[pos - 1][map->south_exit - 2] = center;
					map->t_map_grid[pos - 2][map->south_exit - 2] = center;
					break;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[pos][map->south_exit] == path && map->t_map_grid[pos][map->south_exit + 1] != path &&
				 map->t_map_grid[pos][map->south_exit + 2] != path && map->t_map_grid[pos - 1][map->south_exit + 1] != path &&
				 map->t_map_grid[pos - 1][map->south_exit + 2]){
					map->t_map_grid[pos - 1][map->south_exit + 1] = center;
					map->t_map_grid[pos - 2][map->south_exit + 1] = center;
					map->t_map_grid[pos - 1][map->south_exit + 2] = center;
					map->t_map_grid[pos - 2][map->south_exit + 2] = center;
					break;
				}
			}
		}
	}
	
	// If the random number is less than the given probability of spawning a mart, then spawn a mart
	if(my_rand(0, 100) < prob){
		// Until the mart is mart
		while(!placed){
			if(my_rand(0,2) < 1){
				pos = my_rand(5, 74);
				// Checks if there is a center there already
				if(map->t_map_grid[map->west_exit - 1][pos] == center || map->t_map_grid[map->east_exit - 1][pos] == center ||
				   map->t_map_grid[map->west_exit + 1][pos] == center || map->t_map_grid[map->east_exit + 1][pos] == center){
					continue;
				}
				// Checks four tiles to try and place the center/mart 
				if(map->t_map_grid[map->west_exit][pos] == path && map->t_map_grid[map->west_exit - 1][pos] != path &&
				 map->t_map_grid[map->west_exit - 2][pos] != path && map->t_map_grid[map->west_exit - 1][pos + 1] != path &&
				 map->t_map_grid[map->west_exit - 2][pos + 1] != path){
					map->t_map_grid[map->west_exit - 1][pos] = mart;
					map->t_map_grid[map->west_exit - 1][pos + 1] = mart;
					map->t_map_grid[map->west_exit - 2][pos] = mart;
					map->t_map_grid[map->west_exit - 2][pos + 1] = mart;
					break;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[map->west_exit][pos] == path && map->t_map_grid[map->west_exit + 1][pos] != path &&
				 map->t_map_grid[map->west_exit + 2][pos] != path && map->t_map_grid[map->west_exit + 1][pos + 1] != path &&
				 map->t_map_grid[map->west_exit + 2][pos + 1] != path){
					map->t_map_grid[map->west_exit + 1][pos] = mart;
					map->t_map_grid[map->west_exit + 1][pos + 1] = mart;
					map->t_map_grid[map->west_exit + 2][pos] = mart;
					map->t_map_grid[map->west_exit + 2][pos + 1] = mart;
					break;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[map->east_exit][pos] == path && map->t_map_grid[map->east_exit - 1][pos] != path &&
				 map->t_map_grid[map->east_exit - 2][pos] != path && map->t_map_grid[map->east_exit - 1][pos + 1] != path &&
				 map->t_map_grid[map->east_exit - 2][pos + 1] != path){
					map->t_map_grid[map->east_exit - 1][pos] = mart;
					map->t_map_grid[map->east_exit - 1][pos + 1] = mart;
					map->t_map_grid[map->east_exit - 2][pos] = mart;
					map->t_map_grid[map->east_exit - 2][pos + 1] = mart;
					break;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[map->east_exit][pos] == path && map->t_map_grid[map->east_exit + 1][pos] != path &&
				 map->t_map_grid[map->east_exit + 2][pos] != path && map->t_map_grid[map->east_exit + 1][pos + 1] != path &&
				 map->t_map_grid[map->east_exit + 2][pos + 1] != path){
					map->t_map_grid[map->east_exit + 1][pos] = mart;
					map->t_map_grid[map->east_exit + 1][pos + 1] = mart;
					map->t_map_grid[map->east_exit + 2][pos] = mart;
					map->t_map_grid[map->east_exit + 2][pos + 1] = mart;
					break;
				}
			}
			// Tries to place the mart on the north-south path
			else{
				pos = my_rand(5, 15);
				// Checks if there is already a center there
				if(map->t_map_grid[pos][map->north_exit - 1] == center || map->t_map_grid[pos][map->south_exit - 1] == center ||
				   map->t_map_grid[pos][map->north_exit + 1] == center || map->t_map_grid[pos][map->south_exit + 1] == center){
					continue;
				}
				// Checks four tiles to try and place the center/mart 
				if(map->t_map_grid[pos][map->north_exit] == path && map->t_map_grid[pos][map->north_exit - 1] != path &&
				 map->t_map_grid[pos][map->north_exit - 2] != path && map->t_map_grid[pos - 1][map->north_exit - 1] != path &&
				 map->t_map_grid[pos - 1][map->north_exit - 2]){
					map->t_map_grid[pos][map->north_exit - 1] = mart;
					map->t_map_grid[pos - 1][map->north_exit - 1] = mart;
					map->t_map_grid[pos][map->north_exit - 2] = mart;
					map->t_map_grid[pos - 1][map->north_exit - 2] = mart;
					placed = true;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[pos][map->north_exit] == path && map->t_map_grid[pos][map->north_exit + 1] != path &&
				 map->t_map_grid[pos][map->north_exit + 2] != path && map->t_map_grid[pos - 1][map->north_exit + 1] != path &&
				 map->t_map_grid[pos - 1][map->north_exit + 2]){
					map->t_map_grid[pos][map->north_exit + 1] = mart;
					map->t_map_grid[pos - 1][map->north_exit + 1] = mart;
					map->t_map_grid[pos][map->north_exit + 2] = mart;
					map->t_map_grid[pos - 1][map->north_exit + 2] = mart;
					break;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[pos][map->south_exit] == path && map->t_map_grid[pos][map->south_exit - 1] != path &&
				 map->t_map_grid[pos][map->south_exit - 2] != path && map->t_map_grid[pos - 1][map->south_exit - 1] != path &&
				 map->t_map_grid[pos - 1][map->south_exit - 2]){
					map->t_map_grid[pos][map->south_exit - 1] = mart;
					map->t_map_grid[pos - 1][map->south_exit - 1] = mart;
					map->t_map_grid[pos][map->south_exit - 2] = mart;
					map->t_map_grid[pos - 1][map->south_exit - 2] = mart;
					break;
				}
				// Checks four tiles to try and place the center/mart 
				else if(map->t_map_grid[pos][map->south_exit] == path && map->t_map_grid[pos][map->south_exit + 1] != path &&
				 map->t_map_grid[pos][map->south_exit + 2] != path && map->t_map_grid[pos - 1][map->south_exit + 1] != path &&
				 map->t_map_grid[pos - 1][map->south_exit + 2]){
					map->t_map_grid[pos][map->south_exit + 1] = mart;
					map->t_map_grid[pos - 1][map->south_exit + 1] = mart;
					map->t_map_grid[pos][map->south_exit + 2] = mart;
					map->t_map_grid[pos - 1][map->south_exit + 2] = mart;
					break;
				}
			}
		}
	}
}

// Places extra terrain pieces such as rocks and trees
void place_extra_terrain(map_t *map){
	int row, col;
	for(row = 1; row < Y - 1; row++){
		for(col = 1; col < X - 1; col++){
			switch(map->t_map_grid[row][col]){
				case clearing:
				if(my_rand(0, 100) < 7){
					map->t_map_grid[row][col] = tree;
				}
				if(my_rand(0, 100) < 3){
					map->t_map_grid[row][col] = rock;
				}
				break;
				case tall_grass:
				if(my_rand(0, 100) < 3){
					map->t_map_grid[row][col] = tree;
				}
				if(my_rand(0, 100) < 7){
					map->t_map_grid[row][col] = rock;
				}
				break;
				case sand:
				if(my_rand(0, 100) < 15){
					map->t_map_grid[row][col] = rock;
				}
				break;
				case mixed:
				if(my_rand(0, 100) < 3){
					map->t_map_grid[row][col] = tree;
				}
				if(my_rand(0, 100) < 3){
					map->t_map_grid[row][col] = rock;
				}
				break;
				default:
				break;
			}
		}
	}
}

// Replaces the mixed value type to either tall grass or clearing to keep map between moving to and from
void repalce_mixed(map_t *map){
	int row, col;
	for(row = 0; row < Y - 1; row++){
		for(col = 0; col < X - 1; col++){
			if(map->t_map_grid[row][col] == mixed){
				if(my_rand(0,10) < 7){
					map->t_map_grid[row][col] = clearing;
				}
				else{
					map->t_map_grid[row][col] = tall_grass;
				}
			}
		}
	}
}

// Initializes map with a border and an empty center (debug)
void init_map(map_t *map){
	int col, row;
	// Initializing Border to Boulders
	for(row = 0; row < Y; row++){
		map->t_map_grid[row][0] = boulders;
		map->t_map_grid[row][X - 1] = boulders;
	}
	for(col = 0; col < X; col++){
		map->t_map_grid[0][col] = boulders;
		map->t_map_grid[Y - 1][col] = boulders;
	}
	for(row = 1; row < Y - 1; row++){
		for(col = 1; col < X - 1; col++){
			map->t_map_grid[row][col] = debug;
		}
	}
	// Setting exits to still needing to be randomly spawned
	map->east_exit = -1;
	map->west_exit = -1;
	map->south_exit = -1;
	map->north_exit = -1;

	// Seeds and grows the map
	seed_map(map);
	// Places extra terrain types
	place_extra_terrain(map);
	// Replaces the mixed type with either clearing or tall grass
	repalce_mixed(map);
} 


// Allocates a new map at the given Overall map position
void allocate_map(map_t* map, int x, int y, map_t* Overallmap[399][399]){
	int prob, d;
	// Allocating and initializing a map
	init_map(map);

	// Checking for non-NULL neighbors and for boundary check
	// Checking West Map if not null and not out of bounds
	if(x - 1 > -1 && (Overallmap[y][x - 1]) != NULL){
		map->west_exit = (Overallmap[y][x - 1])->east_exit;
	}
	// Checking if West is out of bounds
	else if(x - 1 == -1){
		map->west_exit = -2;
	}

	// Checking East Map if not null and not out of bounds
	if(x + 1 < 399 && (Overallmap[y][x + 1]) != NULL){
		map->east_exit = (Overallmap[y][x + 1])->west_exit;
	}
	// Checking if East is out of bounds
	else if(x + 1 == 399){
		map->east_exit = -2;
	}

	// Checking North Map if not null and not out of bounds
	if(y - 1 > -1 && (Overallmap[y - 1][x]) != NULL){
		map->north_exit = (Overallmap[y - 1][x])->south_exit;
	}
	// Checking if North is out of bounds
	else if(y - 1 < 0){
		map->north_exit = -2;
	}
	
	// Checking South Map if not null and not out of bounds
	if(y + 1 < 399 && (Overallmap[y + 1][x]) != NULL){
		map->south_exit = Overallmap[y + 1][x]->north_exit;
	}
	// Checking if South is out of bounds
	else if(y + 1 == 399){
		map->south_exit = -2;
	}
	
	// Generates remaining exits
	generate_remaining_exits_rand(map);

	//Generates paths
	generate_paths(map);

	//Generates Poke marts and centers
	d = abs(x - 199) + abs(y - 199);
	if(d < 201){
		prob = (-45*d/200 + 50);
	}
	else{
		prob = 5;
	}
	// Generate centers and marts
	if(d == 0){
		prob = 100;
	}
	generate_poke_centers(map, prob);
	
	// Place the map in the overall map Array
	if(map->north_exit > 0){
		map->t_map_grid[0][map->north_exit] = exitMap;
	}
	if(map->south_exit > 0){
		map->t_map_grid[20][map->south_exit] = exitMap;
	}
	if(map->east_exit > 0){
		map->t_map_grid[map->east_exit][0] = exitMap;
	}
	if(map->west_exit > 0){
		map->t_map_grid[map->west_exit][79] = exitMap;
	}

	Overallmap[y][x] = map;
}



