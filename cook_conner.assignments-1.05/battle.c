#include "battle.h"

void battle(character_t* palyer, character_t* opponent){
	clear();
	mvaddstr(10, 35, "Battle screen, please press esc to defeat the trainer and return to the map");
	refresh();
	while(1){
		opponent->defeated = 1;
		switch(getch()){
			case 27:
				return;
		}
	}
}