#include "terrain.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	// Setting up the random seed
	srand(time(NULL));
	char i;
	int x, y, mapx, mapy, r;
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
	allocate_map(Overallmap, x, y);
	// Print the map and coordinates
	printMap(Overallmap[y][x]);
	printf("(%d, %d)\n", x - 199, 199 - y);

	bool entered;
	// Until a q is press, allow user to change Overall map position
	while(1){
		entered = false;
		printf("\n");
		// Scans user input for direction to go or to fly to
		r = scanf("%c, %4d, %4d", &i, &mapx, &mapy);

		// If only a direction was specified
		if(r == 1){
			// If e was entered allocate map to the east and increment the coordinates
			if(i == 'e' && x < 398){
				if(Overallmap[y][x + 1] == NULL){
					allocate_map(Overallmap, x + 1, y);
				}
				x++;
				entered = true;
			}
			// If w was entered allocate map to the west and decrement the coordinates
			else if(i == 'w' && x > 0){
				if(Overallmap[y][x - 1] == NULL){
					allocate_map(Overallmap, x - 1, y);
				}
				x--;
				entered = true;
			}
			// If n was entered allocate map to the north and decrement the coordinates
			else if(i == 'n' && y > 0){
				if(Overallmap[y - 1][x] == NULL){
					allocate_map(Overallmap, x, y - 1);
				}
				y--;
				entered = true;
			}
			// If s was entered allocate map to the south and increment the coordinates
			else if(i == 's' && y < 398){
				if(Overallmap[y + 1][x] == NULL){
					allocate_map(Overallmap, x, y + 1);
				}
				y++;
				entered = true;
			}
			// If q was entered break out of the loop
			else if(i == 'q'){
				break;
			}
		}
		// If all three fields were specified and the the given coordinates were in bounds then fly to that location and update coordinates
		if(r == 3 && i == 'f' && (mapx < 200 && mapx > -200) && (mapy < 200 && mapy > -200)){
			x = mapx + 199;
			y = 199 - mapy;
			entered = true;
			if(Overallmap[y][x] == NULL){
				allocate_map(Overallmap, x, y);
			}
		}

		// If a valid char was entered then print map and coordinates
		if(entered){
			// Print map at coordinates
			printMap(Overallmap[y][x]);
			// Print the external coordinates
			printf("(%d, %d)\n", x - 199, 199 - y);
		}
	}

	// Freeing up memory from Malloc
	for (int i = 0; i < 399; i++){
		for(int j = 0; j < 399; j++){
			free(Overallmap[i][j]);
		}
	}

	return 0;
}