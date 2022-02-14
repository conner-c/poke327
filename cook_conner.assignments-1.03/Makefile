all: heap terrain poke 

poke: poke.c
	gcc poke.c -Wall -Werror -c
	gcc -o poke poke.o terrain.o heap.o

terrain: terrain.c
	gcc terrain.c -Wall -Werror -c

heap: heap.c
	gcc heap.c -Wall -Werror -c

distanceMap: distanceMap.c
	gcc distanceMap.c -Wall -Werror -c

link:
	gcc -ggdb -o run *.o

clean:
	rm -f *.exe *.stackdump core *.o