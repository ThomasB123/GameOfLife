all: gameoflife.o libgol.so
	gcc -Wall -Wextra -pedantic -std=c11 -o gameoflife -L. -lgol gameoflife.o

gameoflife.o: gameoflife.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -c gameoflife.c

gol.o: gol.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -fPIC -c gol.c

libgol.so: gol.o
	gcc -Wall -Wextra -pedantic -std=c11 -shared -lm gol.o -o libgol.so

clean:
	rm -rf gameoflife gameoflife.o libgol.so gol.o
