// Always in use
#include "terrain.h"
#include "characters.h"
#include "heap.h"
#include <curses.h>
#include "distanceMap.h"
#include <string.h>
#include "battle.h"

#define atoa(x) #x

// Character compare
int character_compare(const void *key, const void *with){
	return ((character_t *)key)->movement_cost - ((character_t *)with)->movement_cost;
}

int int_compare (const void * a, const void * b) {
   return (*(int*)a - *(int*)b);
}

void determine_next_move(character_t* c, map_t* map, character_t* player){
	// Int array used for distance map
	int dmap[21][80];
	int nw = INT_MAX;
	int n = INT_MAX;
	int ne = INT_MAX;
	int w = INT_MAX;
	int e = INT_MAX;
	int sw = INT_MAX;
	int s = INT_MAX;
	int se = INT_MAX;
	int x = c->x;
	int y = c->y;
	if(c->symbol == 'h'){
		if(c->defeated){
			return;
		}

		if((map->cmap[y - 1][x - 1] == player || map->cmap[y - 1][x] == player || map->cmap[y - 1][x + 1] == player || map->cmap[y][x - 1] == player ||
				 	map->cmap[y][x + 1] == player || map->cmap[y + 1][x - 1] == player || map->cmap[y + 1][x] == player || map->cmap[y + 1][x + 1] == player) &&
		 	map->t_map_grid[player->y][player->x] != mart && map->t_map_grid[player->y][player->x] != center){
			battle(player, c);
			return;
		}

		create_dis_map(player->x, player->y, dmap, h, map);
		// North West
		if(c->x > 0 && c->y > 0 && map->cmap[c->y - 1][c->x - 1] == NULL){
			nw = dmap[c->y - 1][c->x - 1];
		}
		// North
		if(c->y > 0 && map->cmap[c->y - 1][c->x] == NULL){
			n = dmap[c->y - 1][c->x];
		}

		// North East
		if(c->x < 80 && c->y > 0 && map->cmap[c->y - 1][c->x + 1] == NULL){
			ne = dmap[c->y - 1][c->x + 1];
		}

		// West
		if(c->x > 0 && map->cmap[c->y][c->x - 1] == NULL){
			w = dmap[c->y][c->x - 1];
		}

		// East
		if(c->x < 80 && map->cmap[c->y][c->x + 1] == NULL){
			e = dmap[c->y][c->x + 1];
		}

		// South West
		if(c->x > 0 && c->y < 21 && map->cmap[c->y + 1][c->x - 1] == NULL){
			sw = dmap[c->y + 1][c->x - 1];
		}

		// South
		if(c->y < 21 && map->cmap[c->y + 1][c->x] == NULL){
			s = dmap[c->y + 1][c->x];
		}

		// South East
		if(c->x < 80 && c->y < 21 && map->cmap[c->y + 1][c->x + 1] == NULL){
			se = dmap[c->y + 1][c->x + 1];
		}

		if(nw == n && nw == ne && nw == w && nw == e && nw == sw && nw == s && nw == se && nw == INT_MAX){
			c->new_x = c->x;
			c->new_y = c->y;
		}
		// If the North West is the quickest
		if(e <= nw && e <= n && e <= ne && e <= w && e <= sw && e <= s && e <= se && map->cmap[c->y][c->x + 1] == NULL && e != INT_MAX){
			c->new_x = c->x + 1;
			c->new_y = c->y;
		}
		else if(w <= nw && w <= n && w <= ne && w <= sw && w <= s && w <= se && map->cmap[c->y][c->x - 1] == NULL && w != INT_MAX){
			c->new_x = c->x - 1;
			c->new_y = c->y;
		}
		else if(n <= nw && n <= ne && n <= sw && n <= s && n <= se && map->cmap[c->y - 1][c->x] == NULL && n != INT_MAX){
			c->new_x = c->x;
			c->new_y = c->y - 1;
		}
		else if(s <= nw && s <= ne && s <= sw && s <= se && map->cmap[c->y + 1][c->x] == NULL && s != INT_MAX){
			c->new_x = c->x;
			c->new_y = c->y + 1;
		}
		else if(nw <= ne && nw <= sw && nw <= se && map->cmap[c->y - 1][c->x - 1] == NULL && nw != INT_MAX){
			c->new_x = c->x - 1;
			c->new_y = c->y - 1;
		}
		else if(ne <= sw && ne <= se && map->cmap[c->y - 1][c->x + 1] == NULL && ne != INT_MAX){
			c->new_x = c->x + 1;
			c->new_y = c->y - 1;
		}
		else if(sw <= se && map->cmap[c->y + 1][c->x - 1] == NULL && sw != INT_MAX){
			c->new_x = c->x - 1;
			c->new_y = c->y + 1;
		}
		else if(map->cmap[c->y + 1][c->x + 1] == NULL && se != INT_MAX){
			c->new_x = c->x + 1;
			c->new_y = c->y + 1;
		}
	}
	else if(c->symbol == 'r'){
		if(c->defeated){
			return;
		}

		if((map->cmap[y - 1][x - 1] == player || map->cmap[y - 1][x] == player || map->cmap[y - 1][x + 1] == player || map->cmap[y][x - 1] == player ||
				 	map->cmap[y][x + 1] == player || map->cmap[y + 1][x - 1] == player || map->cmap[y + 1][x] == player || map->cmap[y + 1][x + 1] == player) &&
		 	map->t_map_grid[player->y][player->x] != mart && map->t_map_grid[player->y][player->x] != center){
			battle(player, c);
			return;
		}

		// enum trainers norm = normal;	
		create_dis_map(player->x, player->y, dmap, rival, map);
		// Grabs all the available movement locations and stores them in quickest array (Checks in bounds and no character is already moving there)
		// North West

		if(c->x > 0 && c->y > 0 && map->cmap[c->y - 1][c->x - 1] == NULL){
			nw = dmap[c->y - 1][c->x - 1];
		}
		// North
		if(c->y > 0 && map->cmap[c->y - 1][c->x] == NULL){
			n = dmap[c->y - 1][c->x];
		}

		// North East
		if(c->x < 80 && c->y > 0 && map->cmap[c->y - 1][c->x + 1] == NULL){
			ne = dmap[c->y - 1][c->x + 1];
		}

		// West
		if(c->x > 0 && map->cmap[c->y][c->x - 1] == NULL){
			w = dmap[c->y][c->x - 1];
		}

		// East
		if(c->x < 80 && map->cmap[c->y][c->x + 1] == NULL){
			e = dmap[c->y][c->x + 1];
		}

		// South West
		if(c->x > 0 && c->y < 21 && map->cmap[c->y + 1][c->x - 1] == NULL){
			sw = dmap[c->y + 1][c->x - 1];
		}

		// South
		if(c->y < 21 && map->cmap[c->y + 1][c->x] == NULL){
			s = dmap[c->y + 1][c->x];
		}

		// South East
		if(c->x < 80 && c->y < 21 && map->cmap[c->y + 1][c->x + 1] == NULL){
			se = dmap[c->y + 1][c->x + 1];
		}
		if(nw == n && nw == ne && nw == w && nw == e && nw == sw && nw == s && nw == se && nw == INT_MAX){
			c->new_x = c->x;
			c->new_y = c->y;
		}
		// If the North West is the quickest
		if(e <= nw && e <= n && e <= ne && e <= w && e <= sw && e <= s && e <= se && map->cmap[c->y][c->x + 1] == NULL && e != INT_MAX){
			c->new_x = c->x + 1;
			c->new_y = c->y;
		}
		else if(w <= nw && w <= n && w <= ne && w <= sw && w <= s && w <= se && map->cmap[c->y][c->x - 1] == NULL && w != INT_MAX){
			c->new_x = c->x - 1;
			c->new_y = c->y;
		}
		else if(n <= nw && n <= ne && n <= sw && n <= s && n <= se && map->cmap[c->y - 1][c->x] == NULL && n != INT_MAX){
			c->new_x = c->x;
			c->new_y = c->y - 1;
		}
		else if(s <= nw && s <= ne && s <= sw && s <= se && map->cmap[c->y + 1][c->x] == NULL && s != INT_MAX){
			c->new_x = c->x;
			c->new_y = c->y + 1;
		}
		else if(nw <= ne && nw <= sw && nw <= se && map->cmap[c->y - 1][c->x - 1] == NULL && nw != INT_MAX){
			c->new_x = c->x - 1;
			c->new_y = c->y - 1;
		}
		else if(ne <= sw && ne <= se && map->cmap[c->y - 1][c->x + 1] == NULL && ne != INT_MAX){
			c->new_x = c->x + 1;
			c->new_y = c->y - 1;
		}
		else if(sw <= se && map->cmap[c->y + 1][c->x - 1] == NULL && sw != INT_MAX){
			c->new_x = c->x - 1;
			c->new_y = c->y + 1;
		}
		else if(map->cmap[c->y + 1][c->x + 1] == NULL && se != INT_MAX){
			c->new_x = c->x + 1;
			c->new_y = c->y + 1;
		}
	}
	else if(c->symbol == 'p'){
		if(c->direction == west){
			if(map->cmap[y][x - 1] == player && !c->defeated && map->t_map_grid[player->y][player->x] != mart &&
				map->t_map_grid[player->y][player->x] != center){
				battle(player, c);
				return;
			}
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move
			if(c->x > 1 && map->cmap[c->y][c->x - 1] == NULL && map->t_map_grid[c->y][c->x - 1] > 5){
				c->new_x = c->x - 1;
				c->new_y = c->y;
			}
			// Else turn around
			else{
				c->direction = east;
			}
		}
		else if(c->direction == east){
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move
			if(map->cmap[y][x + 1] == player && !c->defeated && map->t_map_grid[player->y][player->x] != mart &&
				map->t_map_grid[player->y][player->x] != center){
				battle(player, c);
				return;
			}
			if(c->x < 78 && map->cmap[c->y][c->x + 1] == NULL && map->t_map_grid[c->y][c->x + 1] > 5){
				c->new_x = c->x + 1;
				c->new_y = c->y;
			}
			// Else turn around
			else{
				c->direction = west;
			}
		}
		else if(c->direction == north){
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move
			if(map->cmap[y - 1][x] == player && !c->defeated && map->t_map_grid[player->y][player->x] != mart &&
				map->t_map_grid[player->y][player->x] != center){
				battle(player, c);
				return;
			}
			if(c->y > 1 && map->cmap[c->y - 1][c->x] == NULL && map->t_map_grid[c->y - 1][c->x] > 5){
				c->new_x = c->x;
				c->new_y = c->y - 1;
			}
			// Else turn around
			else{
				c->direction = south;
			}
		}
		else if(c->direction == south){
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move
			if(map->cmap[y + 1][x] == player && !c->defeated && map->t_map_grid[player->y][player->x] != mart &&
				map->t_map_grid[player->y][player->x] != center){
				battle(player, c);
				return;
			}
			if(c->x > 1 && map->cmap[c->y + 1][c->x] == NULL && map->t_map_grid[c->y + 1][c->x] > 5){
				c->new_x = c->x;
				c->new_y = c->y + 1;
			}
			// Else turn around
			else{
				c->direction = north;
			}
		}
	}
	else if(c->symbol == 'w'){
		if(c->direction == west){
			if(map->cmap[y][x - 1] == player && !c->defeated && map->t_map_grid[player->y][player->x] != mart &&
				map->t_map_grid[player->y][player->x] != center){
				battle(player, c);
				return;
			}
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move
			if(c->x > 1 && map->cmap[c->y][c->x - 1] == NULL &&
			 (map->t_map_grid[c->y][c->x - 1] == c->spawned_in || map->t_map_grid[c->y][c->x - 1] == rock ||
			 (c->spawned_in == mixed && (map->t_map_grid[c->y][c->x - 1] == tall_grass || map->t_map_grid[c->y][c->x - 1] == clearing)))){
				c->new_x = c->x - 1;
				c->new_y = c->y;
			}
			// Else turn around
			else{
				c->direction = my_rand(0, 4);
			}
		}
		else if(c->direction == east){
			if(map->cmap[y][x + 1] == player && !c->defeated && map->t_map_grid[player->y][player->x] != mart &&
				map->t_map_grid[player->y][player->x] != center){
				battle(player, c);
				return;
			}
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move
			if(c->x < 78 && map->cmap[c->y][c->x + 1] == NULL &&
			 (map->t_map_grid[c->y][c->x + 1] == c->spawned_in || map->t_map_grid[c->y][c->x + 1] == rock ||
			 (c->spawned_in == mixed && (map->t_map_grid[c->y][c->x + 1] == tall_grass || map->t_map_grid[c->y][c->x + 1] == clearing)))){
				c->new_x = c->x + 1;
				c->new_y = c->y;
			}
			// Else turn around
			else{
				c->direction = my_rand(0, 4);
			}
		}
		else if(c->direction == north){
			if(map->cmap[y - 1][x] == player && !c->defeated && map->t_map_grid[player->y][player->x] != mart &&
				map->t_map_grid[player->y][player->x] != center){
				battle(player, c);
				return;
			}
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move
			if(c->y > 1 && map->cmap[c->y - 1][c->x] == NULL &&
			 (map->t_map_grid[c->y - 1][c->x] == c->spawned_in || map->t_map_grid[c->y - 1][c->x] == rock ||
			 (c->spawned_in == mixed && (map->t_map_grid[c->y - 1][c->x] == tall_grass || map->t_map_grid[c->y - 1][c->x] == clearing)))){
				c->new_x = c->x;
				c->new_y = c->y - 1;
			}
			// Else turn around
			else{
				c->direction = my_rand(0, 4);
			}
		}
		else if(c->direction == south){
			if(map->cmap[y + 1][x] == player && !c->defeated && map->t_map_grid[player->y][player->x] != mart &&
				map->t_map_grid[player->y][player->x] != center){
				battle(player, c);
				return;
			}
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move
			if(c->x > 1 && map->cmap[c->y + 1][c->x] == NULL &&
			 (map->t_map_grid[c->y + 1][c->x] == c->spawned_in || map->t_map_grid[c->y + 1][c->x] == rock ||
			 (c->spawned_in == mixed && (map->t_map_grid[c->y + 1][c->x] == tall_grass || map->t_map_grid[c->y + 1][c->x] == clearing)))){
				c->new_x = c->x;
				c->new_y = c->y + 1;
			}
			// Else turn around
			else{
				c->direction = my_rand(0, 4);
			}
		}
	}
	else if(c->symbol == 'n'){
		if(c->direction == west){
			if(map->cmap[y][x - 1] == player && !c->defeated && map->t_map_grid[player->y][player->x] != mart &&
				map->t_map_grid[player->y][player->x] != center){
				battle(player, c);
				return;
			}
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move, then pick a new random direction
			if(c->x > 1 && map->cmap[c->y][c->x - 1] == NULL && map->t_map_grid[c->y][c->x - 1] > 5){
				c->new_x = c->x - 1;
				c->new_y = c->y;
				c->direction = my_rand(0, 4);
			}
		}
		else if(c->direction == east){
			if(map->cmap[y][x + 1] == player && !c->defeated){
				battle(player, c);
				return;
			}
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move, then pick a new random direction
			if(c->x < 78 && map->cmap[c->y][c->x + 1] == NULL && map->t_map_grid[c->y][c->x + 1] > 5){
				c->new_x = c->x + 1;
				c->new_y = c->y;
				c->direction = my_rand(0, 4);
			}
		}
		else if(c->direction == north){
			if(map->cmap[y - 1][x] == player && !c->defeated){
				battle(player, c);
				return;
			}
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move, then pick a new random direction
			if(c->y > 1 && map->cmap[c->y - 1][c->x] == NULL && map->t_map_grid[c->y - 1][c->x] > 5){
				c->new_x = c->x;
				c->new_y = c->y - 1;
				c->direction = my_rand(0, 4);
			}
		}
		else if(c->direction == south){
			if(map->cmap[y + 1][x] == player && !c->defeated){
				battle(player, c);
				return;
			}
			// If they are not hitting the boundary and the position is not occupied, and it is moveable terrain move, then pick a new random direction
			if(c->x > 1 && map->cmap[c->y + 1][c->x] == NULL && map->t_map_grid[c->y + 1][c->x] > 5){
				c->new_x = c->x;
				c->new_y = c->y + 1;
				c->direction = my_rand(0, 4);
			}
		}
	}
	if(c->symbol == hiker){
		c->movement_cost = c->movement_cost + get_tile_cost(c->new_x, c->new_y, map, h);
	}
	else{
		c->movement_cost = c->movement_cost + get_tile_cost(c->new_x, c->new_y, map, normal);
	}
}

void move_character(character_t* c, map_t* map, int new_x, int new_y){
	map->cmap[new_y][new_x] = map->cmap[c->y][c->x];
	if(c->y != c->new_y || c->x != c->new_x){
		map->cmap[c->y][c->x] = NULL;
	}
	c->x = new_x;
	c->y = new_y;
}


character_t* generate_character(char r){
	// Creating character pointer
	character_t *c = malloc(sizeof(character_t));
	// Setting movement_cost to 0
	c->movement_cost = 0;
	// Defeated status
	c->defeated = 0;
	// Generating character based on given symbol
	int i;
	switch(r){
		case 'h':
			c->symbol = hiker;
			c->direction = NaN;
			return c;
		case 'r':
			c->symbol = rival;
			c->direction = NaN;
			return c;
		case 'p':
			c->symbol = pacer;
			c->direction = my_rand(0,4);
			return c;
		case 'w':
			c->symbol = wanderer;
			c->direction = my_rand(0,4);
			return c;
		case 's':
			c->symbol = stationary;
			c->direction = NaN;
			return c;
		case 'n':
			c->symbol = random_walker;
			c->direction = my_rand(0,4);
			return c;
		case 'u':
			c->symbol = player;
			c->direction = NaN;
			return c;
		// If symbol not of character generate random number 
		default:
			i = my_rand(0, 6);
	}
	// Based on random number generate a character 
	switch(i){
		case 0:
			c->symbol = hiker;
			c->direction = NaN;
			return c;
		case 1:
			c->symbol = rival;
			c->direction = NaN;
			return c;
		case 2:
			c->symbol = pacer;
			c->direction = my_rand(0,4);
			return c;
		case 3:
			c->symbol = wanderer;
			c->direction = my_rand(0,4);
			return c;
		case 4:
			c->symbol = stationary;
			c->direction = NaN;
			return c;
		case 5:
			c->symbol = random_walker;
			c->direction = my_rand(0,4);
			return c;
	}
	return c;
}

// Populates the character map with the NPC and returns the player character
void populate_character_map(map_t* map, int n){

	// Empties the character map to NULL
	for(int i = 0; i < 80; i++){
		for(int j = 0; j < 21; j++){
			map->cmap[j][i] = NULL;
		}
	}

	// Create and place n characters in the map
	character_t* c;
	int x, y;
	for(int i = 0; i < n; i++){
		// First character generated is hiker
		if(i == 0){
			c = generate_character('h');
		}
		// Second character generated is rival
		else if(i == 1){
			c = generate_character('r');
		}
		// All other characters are random
		else{
			c = generate_character('z');
		}

		// Places the character in a valid location
		while(1){
			// Generates random location
			x = my_rand(1, 79);
			y = my_rand(1, 20);
			// Checks if it is a valid spawn location
			if(map->t_map_grid[y][x] > 7 && map->cmap[y][x] == NULL){
				// Setting Character Position
				c->x = x;
				c->y = y;
				// Setting first move to the same location
				c->new_y = y;
				c->new_x = x;
				c->spawned_in = map->t_map_grid[y][x];

				// Determines roughly if the character spawned in a mixed terrain region
				if(map->t_map_grid[y][x] == clearing || map->t_map_grid[y][x] == tall_grass){
					int different_neighbors = 0;
					for(int i = -1; i < 2; i++){
						for(int j = -1; j < 2; j++){
							// If neighbors are different and the differing neighbor is clearing, path, or tall grass increase different neighbors count
							if(map->t_map_grid[y][x] != map->t_map_grid[y + j][x + i] && (map->t_map_grid[y + j][x + i] == clearing || map->t_map_grid[y + j][x + i] == tall_grass || map->t_map_grid[y + j][x + i] == path)){
								different_neighbors++;
							}
						}
					}
					if(different_neighbors > 3){
						c->spawned_in = mixed;
					}
				}

				// Places character into the heap and onto the character map 
				map->cmap[y][x] = c;
				// Inserting the character into the heap
				break;
			}
		}
	}
}

character_t* generate_player(map_t* map){
	character_t* c;
	// Used to create and place the player onto the character map
	c = malloc(sizeof(character_t));
	c->symbol = '@';
	c->direction = NaN;
	// Sets the player to a random position on a path
	int d = my_rand(0, 5);
	int i = 1;
	int placed = 0;
	// Places it on the East path
	if(d == 1){
		while(map->t_map_grid[map->west_exit][i] == path){
			if(my_rand(0,11) > 7){
				c->x = i;
				c->y = map->west_exit;
				placed = 1;
				break;
			}
			i++;
		}
	}
	// Places it on the North path
	else if(d == 2){
		while(map->t_map_grid[i][map->north_exit] == path){
			if(my_rand(0,11) > 7){
				c->x = map->north_exit;
				c->y = i;
				placed = 1;
				break;
			}
			i++;
		}
	}

	// Places it on the East path
	else if(d == 3){
		i = 78;
		while(map->t_map_grid[map->east_exit][i] == path){
			if(my_rand(0,11) > 7){
				c->x = i;
				c->y = map->east_exit;
				placed = 1;
				break;
			}
			i--;
		}
	}

	// Places it on the south path
	else if(d == 3){
		i = 19;
		while(map->t_map_grid[i][map->south_exit] == path){
			if(my_rand(0,11) > 7){
				c->x = map->south_exit;
				c->y = i;
				placed = 1;
				break;
			}
			i--;
		}
	}

	// If it failed to place the player, place it on a guaranteed path spot
	if(placed == 0){
		c->x = 5;
		c->y = map->west_exit;
	}
	c->new_y = c->y;
	c->new_x = c->x;
	map->cmap[c->y][c->x] = c;
	return c;
}



// Use to list all trainers on screen
void write_list(map_t* map, character_t** arr_c, int start, int end, int player_x, int player_y, int max){
	// Informs user of controls
	mvaddstr(0, 0, "Press esc to exit menu. Press up or down keys to scroll the list if necessary.");
	// Counts up the number of characters on screen
	for(int i = 16; i < 64; i++){
		mvaddch(1, i, ACS_HLINE);
		mvaddch(20, i, ACS_HLINE);
	}
	for(int i = 1; i < 21; i++){
		mvaddch(i, 16, ACS_VLINE);
		mvaddch(i, 40, ACS_VLINE);
		mvaddch(i, 64, ACS_VLINE);
	}
	mvaddch(1, 16, ACS_ULCORNER);
	mvaddch(20, 16, ACS_LLCORNER);
	mvaddch(1, 64, ACS_URCORNER);
	mvaddch(20, 64, ACS_LRCORNER);
	mvaddch(20, 40, ACS_BTEE);
	mvaddch(1, 40, ACS_TTEE);

	int counter = 0;
	for(int i = start; i < end && i < max; i += 2){
		char* num = malloc(2);
		mvaddch(2 + counter, 17, arr_c[i]->symbol);
		// In north east
		if(arr_c[i]->x - player_x <= 0 && arr_c[i]->y - player_y <= 0){
			addstr(", north ");
			sprintf(num, "%d", player_y - arr_c[i]->y);
			addstr(num);
			addstr(", west ");
			sprintf(num, "%d", player_x - arr_c[i]->x);
			addstr(num);
		}
		// In north west
		else if(arr_c[i]->x - player_x >= 0 && arr_c[i]->y - player_y <= 0){
			addstr(", north ");
			sprintf(num, "%d", player_y - arr_c[i]->y);
			addstr(num);
			addstr(", east ");
			sprintf(num, "%d", arr_c[i]->x - player_x);
			addstr(num);
		}
		// In south east
		else if(arr_c[i]->x - player_x <= 0 && arr_c[i]->y - player_y >= 0){
			addstr(", south ");
			sprintf(num, "%d", arr_c[i]->y - player_y);
			addstr(num);
			addstr(", west ");
			sprintf(num, "%d", player_x - arr_c[i]->x);
			addstr(num);
		}
		// In south west
		else if(arr_c[i]->x - player_x >= 0 && arr_c[i]->y - player_y >= 0){
			addstr(", south ");
			sprintf(num, "%d", arr_c[i]->y - player_y);
			addstr(num);
			addstr(", east ");
			sprintf(num, "%d", arr_c[i]->x - player_x);
			addstr(num);
		}

		if(i + 1 >= end && i + 1 >= max){
			break;
		}
		mvaddch(2 + counter, 41, arr_c[i + 1]->symbol);
		// In north east
		if(arr_c[i + 1]->x - player_x <= 0 && arr_c[i + 1]->y - player_y <= 0){
			addstr(", north ");
			sprintf(num, "%2d", player_y - arr_c[i + 1]->y);
			addstr(num);
			addstr(", west ");
			sprintf(num, "%2d", player_x - arr_c[i + 1]->x);
			addstr(num);
		}
		// In north west
		else if(arr_c[i + 1]->x - player_x >= 0 && arr_c[i + 1]->y - player_y <= 0){
			addstr(", north ");
			sprintf(num, "%2d", player_y - arr_c[i + 1]->y);
			addstr(num);
			addstr(", east ");
			sprintf(num, "%2d", arr_c[i + 1]->x - player_x);
			addstr(num);
		}
		// In south east
		else if(arr_c[i + 1]->x - player_x <= 0 && arr_c[i + 1]->y - player_y >= 0){
			addstr(", south ");
			sprintf(num, "%2d", arr_c[i]->y - player_y);
			addstr(num);
			addstr(", west ");
			sprintf(num, "%2d", player_x - arr_c[i + 1]->x);
			addstr(num);
		}
		// In south west
		else if(arr_c[i + 1]->x - player_x >= 0 && arr_c[i + 1]->y - player_y >= 0){
			addstr(", south ");
			sprintf(num, "%2d", arr_c[i + 1]->y - player_y);
			addstr(num);
			addstr(", east ");
			sprintf(num, "%2d", arr_c[i + 1]->x - player_x);
			addstr(num);
		}
		counter++;
	}
}


int move_pc(character_t* c, map_t* map){
	int popup = 0;
	// Counting the characters on the map -1 (not counting player)
	int counter = -1;
	for(int i = 0; i < 80; i++){
		for(int j = 0; j < 21; j++){
			if(map->cmap[j][i] != NULL){
				map->cmap[j][i]->x = i;
				map->cmap[j][i]->y = j;
				counter++;
			}
		}
	}
	int n = counter;
	int start = 0;
	int end = 0;
	// Creating character pointer array size of NPC count
	character_t** arr_c = malloc(counter * sizeof(character_t*));
	for(int i = 0; i < 80; i++){
		for(int j = 0; j < 21; j++){
			if(map->cmap[j][i] != NULL && map->cmap[j][i]->symbol != '@'){
				arr_c[counter - 1] = map->cmap[j][i];
				counter--;
			}
		}
	}
	if(n < 37){
		end = n;
	}
	else{
		end = 36;
	}

	// Player position
	int x, y;
	x = c->x;
	y = c->y;

	// Move cursor to the start of the widow
	move(0, 0);
	// Clear the line
	clrtoeol();
	mvaddstr(0, 0, "Your turn! Input your movement.");
	refresh();

	while(1){
		char key = getch();
		switch ((unsigned int)key){
			// Moving up and left
				case '7':
					if(popup){
						continue;
					}
					// If desired movement is not valid, non-moveable or defeated trainer 
					if(map->t_map_grid[y - 1][x - 1] < 3 || (map->cmap[y - 1][x - 1] != NULL && map->cmap[y - 1][x - 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					// Move to new terrain
					else if(map->cmap[y - 1][x - 1] == NULL){
						map->cmap[y - 1][x - 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y - 1;
						c->x = x - 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x - 1, y - 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y - 1][x - 1]);
						printMap(map);
					}
					break;
				case 'y':
					if(popup){
						continue;
					}
					// If desired movement is not valid, non-moveable or defeated trainer 
					if(map->t_map_grid[y - 1][x - 1] < 3 || (map->cmap[y - 1][x - 1] != NULL && map->cmap[y - 1][x - 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					// Move to new terrain
					else if(map->cmap[y - 1][x - 1] == NULL){
						map->cmap[y - 1][x - 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y - 1;
						c->x = x - 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x - 1, y - 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y - 1][x - 1]);
						printMap(map);
					}
					break;

				// Moving up
				case '8':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y - 1][x] < 3 || (map->cmap[y - 1][x] != NULL && map->cmap[y - 1][x]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y - 1][x] == NULL) {
						map->cmap[y - 1][x] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y - 1;
						c->x = x;
						c->movement_cost = c->movement_cost + get_tile_cost(x, y - 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y - 1][x]);
						printMap(map);
					}
					break;
				case 'k':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y - 1][x] < 3 || (map->cmap[y - 1][x] != NULL && map->cmap[y - 1][x]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y - 1][x] == NULL) {
						map->cmap[y - 1][x] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y - 1;
						c->x = x;
						c->movement_cost = c->movement_cost + get_tile_cost(x, y - 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y - 1][x]);
						printMap(map);	
					}
					break;

				// Moving up and right
				case '9':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y - 1][x + 1] < 3 || (map->cmap[y - 1][x + 1] != NULL && map->cmap[y - 1][x + 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					// Moving onto new terrain
					else if(map->cmap[y - 1][x + 1] == NULL){
						map->cmap[y - 1][x + 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y - 1;
						c->x = x + 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x + 1, y - 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y - 1][x + 1]);
						printMap(map);
					}
					break;
				case 'u':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y - 1][x + 1] < 3 || (map->cmap[y - 1][x + 1] != NULL && map->cmap[y - 1][x + 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					// Moving onto new terrain
					else if(map->cmap[y - 1][x + 1] == NULL){
						map->cmap[y - 1][x + 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y - 1;
						c->x = x + 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x + 1, y - 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y - 1][x + 1]);
						printMap(map);
					}
					break;

				// Moving right
				case '6':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y][x + 1] < 3 || (map->cmap[y][x + 1] != NULL && map->cmap[y][x + 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y][x + 1] == NULL) {
						map->cmap[y][x + 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y;
						c->x = x + 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x + 1, y, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y][x + 1]);
						printMap(map);
						}
					break;
				case 'l':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y][x + 1] < 3 || (map->cmap[y][x + 1] != NULL && map->cmap[y][x + 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y][x + 1] == NULL) {
						map->cmap[y][x + 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y;
						c->x = x + 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x + 1, y, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y][x + 1]);
						printMap(map);
					}
					break;

				// Moving down and right 
				case '3':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y + 1][x + 1] < 3 || (map->cmap[y + 1][x + 1] != NULL && map->cmap[y + 1][x + 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y + 1][x + 1] == NULL) {
						map->cmap[y + 1][x + 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y + 1;
						c->x = x + 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x + 1, y + 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y + 1][x + 1]);
						printMap(map);
					}
					break;
				case 'n':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y + 1][x + 1] < 3 || (map->cmap[y + 1][x + 1] != NULL && map->cmap[y + 1][x + 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y + 1][x + 1] == NULL) {
						map->cmap[y + 1][x + 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y + 1;
						c->x = x + 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x + 1, y + 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y + 1][x + 1]);
						printMap(map);
					}
					break;

				// Moving down
				case '2':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y + 1][x] < 3 || (map->cmap[y + 1][x] != NULL && map->cmap[y + 1][x]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y + 1][x] == NULL) {
						map->cmap[y + 1][x] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y + 1;
						c->x = x;
						c->movement_cost = c->movement_cost + get_tile_cost(x, y + 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y + 1][x]);
						printMap(map);
					}
					break;
				case 'j':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y + 1][x] < 3 || (map->cmap[y + 1][x] != NULL && map->cmap[y + 1][x]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y + 1][x] == NULL) {
						map->cmap[y + 1][x] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y + 1;
						c->x = x;
						c->movement_cost = c->movement_cost + get_tile_cost(x, y + 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y + 1][x]);
						printMap(map);
					}
					break;

				// Moving down and left
				case '1':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y + 1][x - 1] < 3 || (map->cmap[y + 1][x - 1] != NULL && map->cmap[y + 1][x - 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y + 1][x - 1] == NULL){
						map->cmap[y + 1][x - 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y + 1;
						c->x = x - 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x - 1, y + 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y + 1][x - 1]);
						printMap(map);
					}
					break;
				case 'b':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y + 1][x - 1] < 3 || (map->cmap[y + 1][x - 1] != NULL && map->cmap[y + 1][x - 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y + 1][x - 1] == NULL){
						map->cmap[y + 1][x - 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y + 1;
						c->x = x - 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x - 1, y + 1, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y + 1][x - 1]);
						printMap(map);
					}
					break;

				// Moving left
				case '4':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y][x - 1] < 3 || (map->cmap[y][x - 1] != NULL && map->cmap[y][x - 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y][x - 1] == NULL){
						map->cmap[y][x - 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y;
						c->x = x - 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x - 1, y, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y][x - 1]);
						printMap(map);
					}
					break;
				case 'h':
					if(popup){
						continue;
					}
					// If desired movement is not valid ()
					if(map->t_map_grid[y][x - 1] < 3 || (map->cmap[y][x - 1] != NULL && map->cmap[y][x - 1]->defeated)){
						// Move cursor to the start of the widow
						move(0, 0);
						// Clear the line
						clrtoeol();
						mvaddstr(0, 0, "You cant move onto that terrain");
						refresh();
					}
					else if(map->cmap[y][x - 1] == NULL){
						map->cmap[y][x - 1] = map->cmap[y][x];
						map->cmap[y][x] = NULL;
						c->y = y;
						c->x = x - 1;
						c->movement_cost = c->movement_cost + get_tile_cost(x - 1, y, map, pc);
						return 1;
					}
					else{
						battle(c, map->cmap[y][x - 1]);
						printMap(map);			
					}
					break;

					// Entering a building
					case '>':
						if(map->t_map_grid[y][x] != 4 && map->t_map_grid[y][x] != 5){
						// Move cursor to the start of the widow
							move(0, 0);
							// Clear the line
							clrtoeol();
							mvaddstr(0, 0, "You can only enter a building when on a Mart or Center");
							refresh();
						}
						else{
							clear();
							mvaddstr(10,35, "You have entered a building press esc to exit");
							refresh();
							while(1){
								if(getch() == '<'){
									break;
								}
							}
						}
					return 1;
					break;

					// Rest

				// Wait
				case '5':
					if(popup){
						continue;
					}
					c->movement_cost = c->movement_cost + get_tile_cost(x, y, map, pc);
					return 1;
					break;
				case ' ':
					if(popup){
						continue;
					}
					c->movement_cost = c->movement_cost + get_tile_cost(x, y, map, pc);
					return 1;
					break;
				case '.':
					if(popup){
						continue;
					}
					c->movement_cost = c->movement_cost + get_tile_cost(x, y, map, pc);	
					return 1;
					break;

			case 't':
				popup = 1;
				// List the trainers
				// Clear the screen
				clear();
					if(n < 37){
						end = n;
					}
					else{
						end = 36;
					}
				write_list(map, arr_c, start, end, x, y, n);
				continue;
				break;

			case KEY_DOWN:				
				// Shifts list of trainers
				if(start >= 36 && popup){
					end = start;
					start -= 36;
					clear();
					write_list(map, arr_c, start, end, x, y, n);
				}
				continue;
			
			case KEY_UP:
				if(end < n && popup){
					start = end;
					end += 36;
					clear();
					write_list(map, arr_c, start, end, x, y, n);
				}
				continue;
			case 'q':
				curs_set(1);
				endwin();
				return 0;
				break;
			case 27:
				if(!popup){
					continue;
				}
				popup = 0;
				return 1;
		}
	}
}

void insert_characters(map_t* map, heap_t* h){
	for(int i = 0; i < 80; i++){
		for(int j = 0; j < 21; j++){
			if(map->cmap[j][i] != NULL){
				heap_insert(h, (void *) map->cmap[j][i]);
			}
		}
	}
}

void set_movement_0(map_t* map){
	for(int i = 0; i < 80; i++){
		for(int j = 0; j < 21; j++){
			if(map->cmap[j][i] != NULL){
				map->cmap[j][i]->movement_cost = 0;
			}
		}
	}
}
