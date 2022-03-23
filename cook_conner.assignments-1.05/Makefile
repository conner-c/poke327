all: 
	gcc *.c -Wall -Werror -c

allNoW:
	gcc *.c -c

poke: poke.c
	gcc poke.c -Wall -Werror -c

terrain: terrain.c
	gcc terrain.c -Wall -Werror -c

heap: heap.c
	gcc heap.c -Wall -Werror -c

distanceMap: distanceMap.c
	gcc distanceMap.c -Wall -Werror -c

characters: characters.c
	gcc characters.c -Wall -Werror -c

linkDebug:
	gcc -ggdb -o run *.o

link:
	gcc -o run *.o -lncurses
clean:
	rm -f *.exe *.stackdump core *.o