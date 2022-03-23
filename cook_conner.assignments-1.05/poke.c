#include "terrain.h"
#include "distanceMap.h"
#include "characters.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "heap.h"
#include <string.h>
#include <ncurses.h>

int main(int argc, char const *argv[])
{
	// Have Curses take over the screen
	initscr();
	// Remove wait buffer of input
	raw();
	cbreak();
	// Removes echo of input
	noecho();
	// Makes cursor visibility 0
	curs_set(0);

	// Determines the number of characters, default is 10 if not specified by the user
	int numc = 10;

	if(argc > 1){
		for(int i = 1; i < argc; i++){
			if(!strcmp("--numtrainers", argv[i])){
				numc = atoi(argv[i+1]);
				break;
			}
		}
	}

	// Setting up the random seed
	srand(time(NULL));
	int x, y;
	character_t* player;

	// Creates the overall map Array
	map_t *Overallmap[399][399];

	// Initializes all the cells to NULL
	for(int col = 0; col < 399; col++){
		for(int row = 0; row < 399; row++){
			Overallmap[row][col] = NULL;
		}
	}

	// Setting coordinates of the position in the overall map
	x = y = 199;
	map_t* map = malloc(sizeof(map_t));
	allocate_map(map, x, y, Overallmap);
	// Gets the current map
	map_t *curr_map = Overallmap[y][x];

	// Creating character queue
	heap_t c_heap;
	heap_init(&c_heap, character_compare, free);
	populate_character_map(curr_map, numc);
	player = generate_player(curr_map);

	insert_characters(curr_map, &c_heap);

	printMap(Overallmap[y][x]);
	// printf("(%d, %d)\n", x - 199, 199 - y);
	character_t* c;

	int quit = 1;
	while(quit){
		c = (character_t*) heap_remove_min(&c_heap);
		if(c->symbol == '@'){
			printMap(curr_map);
			refresh();
			quit = move_pc(c, curr_map);

			// Moving to the west map
			if(player->x == 0){
				x = x - 1;
				// If the west map is not allocated yet allocate it
				if(Overallmap[y][x] == NULL){
					map = malloc(sizeof(map_t));
					allocate_map(map, x, y, Overallmap);
				}
				// Reset all character movements to 0
				set_movement_0(curr_map);
				// Swap the current map
				curr_map = Overallmap[y][x];
				// Populate the new current map
				populate_character_map(curr_map, numc);
				// Remove all characters from the heap
				while(c_heap.size){
					heap_remove_min(&c_heap);
				}
				curr_map->cmap[curr_map->east_exit][78] = player;
				// Insert all the new characters into the heap
				insert_characters(curr_map, &c_heap);
				printMap(curr_map);
				refresh();
			}
			// Moving to the east map
			else if(player->x == 79){
				x = x + 1;
				// If the west map is not allocated yet allocate it
				if(Overallmap[y][x] == NULL){
					map = malloc(sizeof(map_t));
					allocate_map(map, x, y, Overallmap);
				}
				// Reset all character movements to 0
				set_movement_0(curr_map);
				// Swap the current map
				curr_map = Overallmap[y][x];
				// Populate the new current map
				populate_character_map(curr_map, numc);
				// Remove all characters from the heap
				while(c_heap.size){
					heap_remove_min(&c_heap);
				}
				curr_map->cmap[curr_map->west_exit][1] = player;
				// Insert all the new characters into the heap
				insert_characters(curr_map, &c_heap);
				printMap(curr_map);
				refresh();
			}
			// Moving to the north map
			else if(player->y == 0){
				y = y - 1;
				// If the west map is not allocated yet allocate it
				if(Overallmap[y][x] == NULL){
					map = malloc(sizeof(map_t));
					allocate_map(map, x, y, Overallmap);
				}
				// Reset all character movements to 0
				set_movement_0(curr_map);
				// Swap the current map
				curr_map = Overallmap[y][x];
				// Populate the new current map
				populate_character_map(curr_map, numc);
				// Remove all characters from the heap
				while(c_heap.size){
					heap_remove_min(&c_heap);
				}
				curr_map->cmap[19][curr_map->south_exit] = player;
				// Insert all the new characters into the heap
				insert_characters(curr_map, &c_heap);
				printMap(curr_map);
				refresh();
			}
			// Moving to the south map
			else if(player->y == 20){
				y = y + 1;
				// If the west map is not allocated yet allocate it
				if(Overallmap[y][x] == NULL){
					map = malloc(sizeof(map_t));
					allocate_map(map, x, y, Overallmap);
				}
				// Reset all character movements to 0
				set_movement_0(curr_map);
				// Swap the current map
				curr_map = Overallmap[y][x];
				// Populate the new current map
				populate_character_map(curr_map, numc);
				// Remove all characters from the heap
				while(c_heap.size){
					heap_remove_min(&c_heap);
				}
				curr_map->cmap[1][curr_map->north_exit] = player;
				// Insert all the new characters into the heap
				insert_characters(curr_map, &c_heap);
				printMap(curr_map);
				refresh();
			}
		}
		else{
			determine_next_move(c, curr_map, player);
			move_character(c, curr_map, c->new_x, c->new_y);
		}
		if(!c->defeated){
				heap_insert(&c_heap, (void*) c);
		}
	}
	return 0;
}



/**	
if(player->x == 0){
	// If the west map is not allocated yet allocate it
	if(Overallmap[y][x - 1] == NULL){
		allocate_map(Overallmap, y, x - 1);
	}
	// Reset all character movements to 0
	set_movement_0(curr_map);
	// Swap the current map
	curr_map = Overallmap[y][x - 1];
	// Populate the new current map
	populate_character_map(curr_map, numc);
	// Remove all characters from the heap
	while(c_heap.size){
		heap_remove_min(&c_heap);
	}
	// Insert all the new characters into the heap
	insert_characters(curr_map, &c_heap);
}
else if(player->y == 0){
	// If the west map is not allocated yet allocate it
	if(Overallmap[y - 1][x] == NULL){
		allocate_map(Overallmap, y - 1, x);
	}
	// Reset all character movements to 0
	set_movement_0(curr_map);
	// Swap the current map
	curr_map = Overallmap[y - 1][x];
	// Populate the new current map
	populate_character_map(curr_map, numc);
	// Remove all characters from the heap
	while(c_heap.size){
		heap_remove_min(&c_heap);
	}
	// Insert all the new characters into the heap
	insert_characters(curr_map, &c_heap);
}
else if(player->x == 79){
	// If the west map is not allocated yet allocate it
	if(Overallmap[y][x + 1] == NULL){
		allocate_map(Overallmap, y, x + 1);
	}
	// Reset all character movements to 0
	set_movement_0(curr_map);
	// Swap the current map
	curr_map = Overallmap[y][x + 1];
	// Populate the new current map
	populate_character_map(curr_map, numc);
	// Remove all characters from the heap
	while(c_heap.size){
		heap_remove_min(&c_heap);
	}
	// Insert all the new characters into the heap
	insert_characters(curr_map, &c_heap);
}
else if(player->y == 20){
	// If the west map is not allocated yet allocate it
	if(Overallmap[y + 1][x] == NULL){
		allocate_map(Overallmap, y + 1, x);
	}
	// Reset all character movements to 0
	set_movement_0(curr_map);
	// Swap the current map
	curr_map = Overallmap[y + 1][x];
	// Populate the new current map
	populate_character_map(curr_map, numc);
	// Remove all characters from the heap
	while(c_heap.size){
		heap_remove_min(&c_heap);
	}
	**/
	// Insert all the new characters into the heap