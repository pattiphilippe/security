#NOM    : Makefile
#OBJET  : Compile project
#HOWTO	: make; make clean; make run; make clean run

run : main
	./build/vigenere -c ./exemple/bigPlain.txt ./exemple/bigCiphered.txt MYKEY
	./build/vigenere -d ./exemple/bigCiphered.txt ./exemple/bigDeciphered.txt MYKEY
	./build/vigenere -s ./exemple/bigCiphered.txt ./exemple/bigSolved.txt

main : chi.o IC.o main.o vigenere.o
	gcc ./build/*.o -o ./build/vigenere

chi.o : ./scr/chi.hpp
	gcc ./src/chi.hpp -o ./build/chi.o

IC.o : ./scr/IC.hpp
	gcc ./src/IC.hpp -o ./build/IC.o

main.o : ./scr/main.cpp
	gcc ./src/main.cpp -o ./build/main.o

vigenere.o : ./scr/vigenere.h
	gcc ./src/vigenere.h -o ./build/vigenere.o

clean:
	rm -f ./build/* 
