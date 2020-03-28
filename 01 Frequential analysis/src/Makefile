#NOM    : Makefile
#OBJET  : Compile project
#HOWTO	: make; make clean

Synchro: Wait4.c 
	#gcc Wait4.c -o Wait4 -lm
	gcc -std=c99 -Wall -pedantic-errors -D_BSD_SOURCE Wait4.c -o Wait4 -lm

clean:
	rm build/Vigenere